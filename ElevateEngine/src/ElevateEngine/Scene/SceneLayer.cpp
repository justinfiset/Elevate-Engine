#include "eepch.h"
#include "SceneLayer.h"

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
	m_scene->RenderScene(cam);
}

void Elevate::SceneLayer::OnEvent(Event& event)
{
	m_scene->Notify(event);
}