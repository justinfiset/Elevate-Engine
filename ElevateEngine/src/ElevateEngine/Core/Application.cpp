#include "eepch.h"
#include "Application.h"

#include "ElevateEngine/Core/Time.h"
#include "ElevateEngine/Core/Log.h"
#include "ElevateEngine/Core/Assert.h"
#include "ElevateEngine/Core/Layers/LayerStack.h"

#include "ElevateEngine/Inputs/Input.h"
#include "ElevateEngine/Files/FileUtility.h"

#include "ElevateEngine/Renderer/Renderer.h"
#include "ElevateEngine/Renderer/Texture/TextureManager.h"

#include "ElevateEngine/Events/ApplicationEvent.h"

#include "ElevateEngine/ImGui/ImGuiLayer.h"

#include "ElevateEngine/Editor/EditorLayer.h"

#include <ElevateEngine/Renderer/Debug/DebugRenderer.h>

// Wwise Integration
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h>

#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <ElevateEngine/Audio/Ak/Common/AkFilePackageLowLevelIODeferred.h>

// Sound Engine
#include <AK/SoundEngine/Common/AkSoundEngine.h>
// Spatial audio
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>
// Wwise Communication
#ifndef AK_OPTIMIZED // Keep the following header out in release mode
#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

#include <AK/Plugin/AkVorbisDecoderFactory.h> // This is necessary to be able to play vorbis files
#include <AK/Plugin/AkOpusDecoderFactory.h> // This is necessary to be able to play opus files

// TODO REMOVE AND ABSTRACT IN A DEDICATED CLASS
// For Wwise integration testing
#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_CAR L"Car.bnk"
#define BANKNAME_HUMAN L"Human.bnk"
#define BANKNAME_MARKERTEST L"MarkerTest.bnk"

#include <filesystem>

namespace Elevate {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		EE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		EE_CORE_TRACE("Current working directory : {}", std::filesystem::current_path().string());

		InitSoundEngine();

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		DebugRenderer::Init();

		FrameBuffer.reset(Framebuffer::Create(m_Window->GetWidth(), m_Window->GetHeight())); 
		FrameBuffer->SetClearColor({ 0.8f, 0.4f, 0.7f, 1.0f }); // Pink / purple for debug purposes

		#ifdef EE_EDITOR_BUILD
				PushOverlay(new Elevate::Editor::EditorLayer());
				SetState(GameContextState::EditorMode);
		#endif

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()	
	{
		TermSoundEngine();
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	// Event Dispatcher
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		// Keyboard
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleasedEvent));

		// Mousse
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(OnMouseButtonReleasedEvent));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() // Each frame
	{
		float lastTime = 0.0f;
		while (m_Running)
		{
			try
			{
				// TIME UPDATE //////////////////
				Time::currentTime_ = (float) m_Window->GetTime();
				Time::deltaTime_ = Time::currentTime_ - lastTime;
				lastTime = Time::currentTime_;
				/////////////////////////////////

				ProcessAudio();

				TextureManager::UpdateLoadingTextures();

				FrameBuffer->Bind(); // Rendering the screen in a single texture
				FrameBuffer->Clear();

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate();

				// Draw Layers and Scenes
				for (Layer* layer : m_LayerStack)
					layer->OnRender();

				DebugRenderer::Render();

				FrameBuffer->Unbind(); // Back to normal

				//imgui
				m_ImGuiLayer->PreRender();
				m_ImGuiLayer->Begin();

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				
				m_ImGuiLayer->Render(); // render imgui
				m_ImGuiLayer->End(); // finish the imgui rendering

				Input::ManageMidStates(); // Manage Key/Button up and down state

				Renderer::FlushBuffers();	
				// Poll events and swap buffers
				m_Window->OnUpdate();
			}
			catch (const std::exception& exc)
			{
				EE_CORE_ERROR(exc.what());
			}
		}
		Exit();
	}

	void Application::Exit()
	{
		m_ImGuiLayer->Cleanup();
	}

#pragma region Events	
	void Application::OnStateChange(GameContextState oldState, GameContextState newState)
	{
		GameContextEvent e(oldState, newState);
		OnEvent(e);
	}

	// TODO: IMPLEMENT AN ABSTRACTION FOR THIS IMPLEMENTATION
	bool Application::InitSoundEngine()
	{
		EE_CORE_INFO("Initializing Wwise Sound Engine...");

		// Creating the memory manager
		AkMemSettings memorySettings;
		AK::MemoryMgr::GetDefaultSettings(memorySettings);
		if (AK::MemoryMgr::Init(&memorySettings) != AK_Success)
		{
			EE_CORE_ASSERT(false, "ERROR: Failed to initialize the Wwise memory manager.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise memory manager initialized!");
		}

		// Creating the stream manager
		AkStreamMgrSettings streamSettings;
		AK::StreamMgr::GetDefaultSettings(streamSettings);
		if (!AK::StreamMgr::Create(streamSettings))
		{
			EE_CORE_ASSERT(false, "ERROR: Failed to create the Wwise stream manager.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise stream manager initialized!");
		}

		// Creating a streaming device
		AkDeviceSettings deviceSettings;
		AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
		if (m_lowLevelIO.Init(deviceSettings) != AK_Success)
		{
			EE_CORE_ASSERT(false, "ERROR: Failed to create the Wwise streaming device and low-level I/O system.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise streaming device and low-level I/O initialized!");
		}

		// Initializing the sound engine itself
		AkInitSettings initSettings;
		AK::SoundEngine::GetDefaultInitSettings(initSettings);

		AkPlatformInitSettings platformInitSettings;
		AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

		if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
		{
			EE_CORE_ASSERT(false, "ERROR: Failded to initialize the Sound Engine.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise SoundEngine initialized!");
		}

		// Initializing the interactive music engine
		AkSpatialAudioInitSettings spatialAudioSettings;
		if (AK::SpatialAudio::Init(spatialAudioSettings) != AK_Success)
		{
			EE_CORE_ASSERT(false, "ERROR: Failded to initialize Spatial Audio.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise Spatial Audio initialized!");
		}

#ifndef AK_OPTIMIZED
		AkCommSettings communicationSettings;
		AK::Comm::GetDefaultInitSettings(communicationSettings);
		if (AK::Comm::Init(communicationSettings) != AK_Success)
		{
			EE_CORE_ASSERT(false, "ERROR: Failded to initialize Wwise communication.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise communication initialized!");
		}
#endif // AK_OPTIMIZED

		EE_CORE_INFO("Wwise Initialized!");

		PrepareAudio();

		return true;
	}

	void Application::PrepareAudio()
	{
		m_lowLevelIO.SetBasePath(AKTEXT("./WwiseProject/GeneratedSoundBanks/Windows/"));
		AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

		// TODO TRANSFORM IN A FUNCITON AND HAVE A BETTER ERROR LOGGING IN CASE OF ACCIDENT
		// + RETURN TRUE / FALSE IN LOADED OR NO
		// Load banks synchronously (from file name).
		AkBankID bankID; // Not used. These banks can be unloaded with their file name.
		AKRESULT eResult = AK::SoundEngine::LoadBank(BANKNAME_INIT, bankID);
		EE_CORE_CERROR(eResult == AK_Success, "ERROR: Failed to load bank.");
		eResult = AK::SoundEngine::LoadBank(BANKNAME_CAR, bankID);
		EE_CORE_CERROR(eResult == AK_Success, "ERROR: Failed to load bank.");
		eResult = AK::SoundEngine::LoadBank(BANKNAME_HUMAN, bankID);
		EE_CORE_CERROR(eResult == AK_Success, "ERROR: Failed to load bank.");
		eResult = AK::SoundEngine::LoadBank(BANKNAME_MARKERTEST, bankID);
		EE_CORE_CERROR(eResult == AK_Success, "ERROR: Failed to load bank.");
	}

	void Application::TermSoundEngine()
	{
#ifndef AK_OPTIMIZED
		AK::Comm::Term();
#endif // AK_OPTIMIZED
		//AK::SpatialAudio::Term(); // TODO FIND WHY NOT DEFINED
		AK::SoundEngine::Term();
		
		m_lowLevelIO.Term();
		if (AK::IAkStreamMgr::Get())
		{
			AK::IAkStreamMgr::Get()->Destroy();
		}

		AK::MemoryMgr::Term();
	}

	// TODO MOVE SOMEHWERE ELSE
	void Application::ProcessAudio()
	{
		AK::SoundEngine::RenderAudio();
	}

	bool Application::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		Input::OnKeyPressedEvent(e);
		return true;
	}

	bool Application::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		Input::OnKeyReleasedEvent(e);
		return true;
	}

	bool Application::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		Input::OnMouseButtonPressedEvent(e);
		return true;
	}

	bool Application::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		Input::OnMouseButtonReleasedEvent(e);
		return true;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}

#pragma endregion
}