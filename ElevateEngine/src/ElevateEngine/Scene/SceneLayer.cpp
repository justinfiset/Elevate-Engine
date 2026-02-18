#include "eepch.h"
#include "SceneLayer.h"

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Scene/SceneManager.h>

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
		Renderer::BeginFrame(*cam);
		m_scene->RenderScene(cam);
	}
	else
	{
		EE_CORE_ERROR("Cannot render a scene with a nullptr camera.");
	}
}

void Elevate::SceneLayer::OnEvent(Event& event)
{
	m_scene->Notify(event);
}