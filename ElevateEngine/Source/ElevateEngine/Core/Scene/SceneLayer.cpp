#include "SceneLayer.h"

import Elevate.Foundations;

#include <ElevateEngine/Scene/Scene.h>
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Scene/SceneManager.h>
#include <ElevateEngine/Events/ApplicationEvent.h>

void Elevate::SceneLayer::OnDetach()
{
	SceneManager::UnloadScene(m_scene);
}

void Elevate::SceneLayer::OnUpdate()
{
	m_scene->UpdateScene();
}

void Elevate::SceneLayer::OnRender()
{
	OnRender(nullptr);
}

void Elevate::SceneLayer::OnRender(Camera* cam)
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

#include <ElevateEngine/Inputs/KeyCodes.h>
#include <ElevateEngine/Events/KeyEvent.h>
#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Serialization/JsonSerializer.h>
void Elevate::SceneLayer::OnEvent(Event& event)
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
			Elevate::ByteBuffer buffer;
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