module;

#include <ElevateEngine/Renderer/Renderer/Renderer.h>

module Elevate.Core.Scene:SceneLayer;

import Elevate.Foundations.CoreLogger;

import Elevate.Core.Scene:Scene;
import Elevate.Core.Scene.SceneManager;

// todo : remove
import Elevate.Core.Events;
import Elevate.Core.Scene:GameObject;
import Elevate.Serialization.JsonSerializer;
import Elevate.Core.Events;
#include <ElevateEngine/Inputs/KeyCodes.h>

namespace Elevate
{
	void SceneLayer::OnDetach()
	{
		SceneManager::UnloadScene(m_scene);
	}

	void SceneLayer::OnUpdate()
	{
		m_scene->UpdateScene();
	}

	void SceneLayer::OnRender()
	{
		OnRender(nullptr);
	}

	void SceneLayer::OnRender(Camera* cam)
	{
		if (cam)
		{
			Renderer::BeginFrame(m_scene, *cam);
			m_scene->RenderScene(cam);
		}
		else
		{
			CoreLogger::Error("Cannot render a scene with a nullptr camera.");
		}
	}

	// todo remove:
	void SceneLayer::OnEvent(Event& event)
	{
		const auto type = event.GetEventType();
		if (type == EventType::GameContextChanged)
		{
			// If we are getting into runtime
			auto contextEvent = dynamic_cast<GameContextEvent&>(event);
			if (contextEvent.GetNewState() == GameContextState::Runtime)
			{
				m_scene->OnAwake();
				m_scene->OnStart();
			}
		}
		else if (type == EventType::KeyPressed)
		{
			auto keyEvent = dynamic_cast<KeyPressedEvent&>(event);
			if (keyEvent.GetKeyCode() == EE_KEY_E)
			{
				JsonSerializer serializer;
				ByteBuffer buffer;
				for (auto& obj : m_scene->GetRootObjects())
				{
					serializer.Serialize(obj->GetProperties(), buffer);
					std::string serialization = ByteUtils::ToString(buffer);
					CoreLogger::Info("{}", serialization);
				}
			}
		}
		m_scene->Notify(event);
	}
}
