#include "eepch.h"
#include "SceneLayer.h"

#include <ElevateEngine/Core/Log.h>
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
		EE_CORE_ERROR("Cannot render a scene with a nullptr camera.");
	}
}

void Elevate::SceneLayer::OnEvent(Event& event)
{
	const auto type = event.GetEventType();
	if (type == EventType::GameContextChanged)
	{
		// If we are getting into runtime
		GameContextEvent& contextEvent = dynamic_cast<GameContextEvent&>(event);
		if (contextEvent.GetNewState() == GameContextState::Runtime)
		{
			m_scene->OnAwake();
			m_scene->OnStart();
		}
	}
	m_scene->Notify(event);
}