#pragma once
#include "ElevateEngine/Renderer/Camera.h"

namespace Elevate {
	class EditorCamera : public Camera
	{
	public:
		EditorCamera(float fov = 60.0f); // TODO CHANGER, ON VEUT QUELQUE CHOSE DE CUSTUM

	protected:
		void Update() override;
		void OnNotify(Event& event) override;

		void ClampRotation();
	private:
		float m_lastX, m_lastY;
		bool m_followCursor = false;
		float m_sensitivity = 0.5f;
	};
};

