#pragma once
#include "ElevateEngine/Renderer/Camera.h"
#include "ElevateEngine/Core/TypeRegistry.h"

namespace Elevate {
	class Event;

	class EditorCamera : public Camera
	{
	public:
		BEGIN_COMPONENT(EditorCamera, EE_HideInInspector)
		DECLARE_BASE(Camera)

		EditorCamera(float fov = 60.0f); // TODO CHANGER, ON VEUT QUELQUE CHOSE DE CUSTUM

		void Init() override;
		void Update() override;
		void OnNotify(Event& event) override;

	private:
		float m_yaw = -90.0f;
		float m_pitch = 0.0f;
		float m_lastX, m_lastY;
		bool m_followCursor = false;
		float m_sensitivity = 0.5f;

		END_COMPONENT()
	};
};