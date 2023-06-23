#pragma once

#include "ElevateEngine/Core/Window.h"

#include "ElevateEngine/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Elevate
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps&);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline bool GetFocus() const override { return m_Data.Focused; }

		// Widnow attributess
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; };
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		// Informations that may be requested by GLFW callbacks
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool Focused;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

