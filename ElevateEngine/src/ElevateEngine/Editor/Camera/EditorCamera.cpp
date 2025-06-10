#include "eepch.h"
#include "EditorCamera.h"

#include <ElevateEngine/Inputs/Input.h>
#include <ElevateEngine/Core/Time.h>
#include <ElevateEngine/Core/GameObject.h>

Elevate::EditorCamera::EditorCamera(float fov)
    : Camera(fov) { }

void Elevate::EditorCamera::Update()
{
    // sprint
    float baseCamSpeed = 0.5f;
    if (Elevate::Input::IsKeyPressed(EE_KEY_LEFT_SHIFT))
    {
        baseCamSpeed = 2.5f;
    }
    float cameraSpeed = baseCamSpeed * Elevate::Time::GetDeltaTime();

    if (Elevate::Input::IsKeyPressed(EE_KEY_W))
        gameObject->GetPosition() += cameraSpeed * GetFrontVec();
    if (Elevate::Input::IsKeyPressed(EE_KEY_S))
        gameObject->GetPosition() -= cameraSpeed * GetFrontVec();
    if (Elevate::Input::IsKeyPressed(EE_KEY_D))
        gameObject->GetPosition() -= cameraSpeed * GetRightVec();
    if (Elevate::Input::IsKeyPressed(EE_KEY_A))
        gameObject->GetPosition() += cameraSpeed * GetRightVec();

    if (m_followCursor)
    {
        // Calcul du décalage de souris avec sensibilité configurable
        float xpos = Elevate::Input::GetMouseX();
        float ypos = Elevate::Input::GetMouseY();

        float xoffset = (xpos - m_lastX) * m_sensitivity;
        float yoffset = (m_lastY - ypos) * m_sensitivity;

        m_lastX = xpos;
        m_lastY = ypos;

        gameObject->GetRotation().y += xoffset;
        gameObject->GetRotation().x += yoffset;

        ClampRotation();  // Utilisation de la méthode helper
        UpdateCameraVectors();
    }
}

void Elevate::EditorCamera::OnNotify(Event& event)
{
    if (event.IsInCategory(Elevate::EventCategoryMouseButton))
    {
        if (Elevate::Input::IsMouseButtonDown(EE_MOUSE_BUTTON_RIGHT))
        {
            m_followCursor = true;
            m_lastX = Elevate::Input::GetMouseX();
            m_lastY = Elevate::Input::GetMouseY();
        }
        else if (Elevate::Input::IsMouseButtonUp(EE_MOUSE_BUTTON_RIGHT))
        {
            m_followCursor = false;
        }
    }
}

void Elevate::EditorCamera::Init()
{
    gameObject->GetRotation().y = -90.0f;
    Camera::Init();
}

void Elevate::EditorCamera::ClampRotation()
{
    if (gameObject->GetRotation().x > 89.0f)
        gameObject->GetRotation().x = 89.0f;
    if (gameObject->GetRotation().x < -89.0f)
        gameObject->GetRotation().x = -89.0f;
}
