#include "eepch.h"
#include "EditorCamera.h"

#include <ElevateEngine/Inputs/Input.h>
#include <ElevateEngine/Core/Time.h>
#include <ElevateEngine/Core/GameObject.h>

namespace Elevate
{
    EditorCamera::EditorCamera(float fov)
        : Camera(fov, false) { }

    void EditorCamera::Update()
    {
        // sprint
        float baseCamSpeed = 0.5f;
        if (Input::IsKeyPressed(EE_KEY_LEFT_SHIFT))
        {
            baseCamSpeed = 2.5f;
        }
        float cameraSpeed = baseCamSpeed * Time::GetDeltaTime();

        glm::vec3 offset = { 0.0f, 0.0f, 0.0f };
        if (Input::IsKeyPressed(EE_KEY_W))
            offset += cameraSpeed * GetFrontVec();
        if (Input::IsKeyPressed(EE_KEY_S))
            offset -= cameraSpeed * GetFrontVec();
        if (Input::IsKeyPressed(EE_KEY_D))
            offset -= cameraSpeed * GetRightVec();
        if (Input::IsKeyPressed(EE_KEY_A))
            offset += cameraSpeed * GetRightVec();
        gameObject->SetPosition(gameObject->GetPosition() + offset);

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
