module;

#include <memory>
#include <algorithm>
#include <vector>
#include <string>

module Elevate.Core.SceneLayer;

import Elevate.Foundations.CoreLogger;
import Elevate.Renderer.Renderer;

// todo : remove
import Elevate.Serialization.JsonSerializer;
import Elevate.Core.Events;
import Elevate.Input;


namespace Elevate
{
	void SceneLayer::OnAttach()
	{
		m_sceneManager->LoadScene(m_scene);
	}

	void SceneLayer::OnDetach()
	{
		m_sceneManager->UnloadScene(m_scene);
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
			m_scene->RenderScene();
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
			if (keyEvent.GetKeyCode() == KeyCode::E)
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
