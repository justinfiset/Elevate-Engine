module;

#include <glm/glm.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

module Elevate.Editor.Camera;

// Foundations
import Elevate.Foundations.Time;
// Core
import Elevate.Core.Scene;
// Renderer
import Elevate.Renderer.Camera;
// Input
import Elevate.Input;

namespace Elevate
{
	EditorCamera::EditorCamera(float fov)
		: Camera(fov, false) {
	}

	void EditorCamera::Update()
	{
		float baseCamSpeed = 0.5f;

		if (Input::IsKeyPressed(KeyCode::LeftShift))
			baseCamSpeed = 2.5f;

		float cameraSpeed = baseCamSpeed * Time::GetDeltaTime();

		glm::vec3 offset(0.0f);

		if (Input::IsKeyPressed(KeyCode::W))
			offset += cameraSpeed * GetFrontVec();

		if (Input::IsKeyPressed(KeyCode::S))
			offset -= cameraSpeed * GetFrontVec();

		if (Input::IsKeyPressed(KeyCode::D))
			offset += cameraSpeed * GetRightVec();

		if (Input::IsKeyPressed(KeyCode::A))
			offset -= cameraSpeed * GetRightVec();

		gameObject->SetPosition(gameObject->GetPosition() + offset);

		if (!m_followCursor)
			return;

		float xpos = Input::GetMouseX();
		float ypos = Input::GetMouseY();

		float xoffset = (xpos - m_lastX) * m_sensitivity;
		float yoffset = (m_lastY - ypos) * m_sensitivity;

		m_lastX = xpos;
		m_lastY = ypos;

		m_yaw -= xoffset;
		m_pitch += yoffset;

		m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

		glm::quat yawRotation =
			glm::angleAxis(
				glm::radians(m_yaw),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);

		glm::quat pitchRotation =
			glm::angleAxis(
				glm::radians(m_pitch),
				glm::vec3(1.0f, 0.0f, 0.0f)
			);

		gameObject->GetTransform().SetRotationQuaternion(
			yawRotation * pitchRotation
		);
	}

	void EditorCamera::OnNotify(Event& event)
	{
		if (event.IsInCategory(EventCategoryMouseButton))
		{
			if (Input::IsMouseButtonDown(MouseButton::Right))
			{
				m_followCursor = true;
				m_lastX = Input::GetMouseX();
				m_lastY = Input::GetMouseY();
			}
			else if (Input::IsMouseButtonUp(MouseButton::Right))
			{
				m_followCursor = false;
			}
		}
	}

	void EditorCamera::Init()
	{
		m_yaw = -90.0f;
		m_pitch = 0.0f;

		gameObject->GetTransform().SetRotationQuaternion(
			glm::angleAxis(
				glm::radians(m_yaw),
				glm::vec3(0.0f, 1.0f, 0.0f)
			)
		);

		Camera::Init();
	}
}
