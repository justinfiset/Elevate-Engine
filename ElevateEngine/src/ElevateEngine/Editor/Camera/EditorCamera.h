#pragma once
#include "ElevateEngine/Renderer/Camera.h"
#include <ElevateEngine/Core/GameObject.h>
#include "ElevateEngine/Core/ComponentRegistry.h"

namespace Elevate {
	class EditorCamera : public Camera
	{
	public:
		BEGIN_COMPONENT(EditorCamera, HideInInspector)
		DECLARE_BASE(Camera)

		EditorCamera(float fov = 60.0f); // TODO CHANGER, ON VEUT QUELQUE CHOSE DE CUSTUM


		void Init() override;
		void Update() override;
		void OnNotify(Event& event) override;

	protected:
		void ClampRotation();

	private:
		float m_lastX, m_lastY;
		bool m_followCursor = false;
		float m_sensitivity = 0.5f;

		END_COMPONENT()
	};
};