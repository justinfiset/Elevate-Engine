#include "eepch.h"
#include "EditorCamera.h"

#include <ElevateEngine/Inputs/Input.h>
#include <ElevateEngine/Core/Time.h>
#include <ElevateEngine/Core/GameObject.h>

namespace Elevate
{
    REGISTER_COMPONENT(EditorCamera);

    EditorCamera::EditorCamera(float fov)
        : Camera(fov) { }

    void EditorCamera::Update()
    {
        // sprint
        float baseCamSpeed = 0.5f;
        if (Input::IsKeyPressed(EE_KEY_LEFT_SHIFT))
        {
            baseCamSpeed = 2.5f;
        }
        float cameraSpeed = baseCamSpeed * Time::GetDeltaTime();

        if (Input::IsKeyPressed(EE_KEY_W))
            gameObject->GetPosition() += cameraSpeed * GetFrontVec();
        if (Input::IsKeyPressed(EE_KEY_S))
            gameObject->GetPosition() -= cameraSpeed * GetFrontVec();
        if (Input::IsKeyPressed(EE_KEY_D))
            gameObject->GetPosition() -= cameraSpeed * GetRightVec();
        if (Input::IsKeyPressed(EE_KEY_A))
            gameObject->GetPosition() += cameraSpeed * GetRightVec();

        if (m_followCursor)
        {
            // Calcul du décalage de souris avec sensibilité configurable
            float xpos = Input::GetMouseX();
            float ypos = Input::GetMouseY();

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

    void EditorCamera::OnNotify(Event& event)
    {
        if (event.IsInCategory(EventCategoryMouseButton))
        {
            if (Input::IsMouseButtonDown(EE_MOUSE_BUTTON_RIGHT))
            {
                m_followCursor = true;
                m_lastX = Input::GetMouseX();
                m_lastY = Input::GetMouseY();
            }
            else if (Input::IsMouseButtonUp(EE_MOUSE_BUTTON_RIGHT))
            {
                m_followCursor = false;
            }
        }
    }

    void EditorCamera::Init()
    {
        gameObject->GetRotation().y = -90.0f;
        Camera::Init();
    }

    void EditorCamera::ClampRotation()
    {
        if (gameObject->GetRotation().x > 89.0f)
            gameObject->GetRotation().x = 89.0f;
        if (gameObject->GetRotation().x < -89.0f)
            gameObject->GetRotation().x = -89.0f;
    }
}
