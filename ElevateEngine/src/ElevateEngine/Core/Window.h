#pragma once

#include "ElevateEngine/Core/Core.h"

namespace Elevate {
	class Event;

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool VSync;

		WindowProps () : Title("ElevateEngine Dev"), Width(1280), Height(720), VSync(false) { }

		WindowProps(const std::string& title, unsigned int width, unsigned int height, bool vsync)
			: Title(title), Width(width), Height(height), VSync(vsync) { }

		// To get all the revelant data from a .config file written in json
		WindowProps(const std::string appConfigFilePath);
	};

	class EE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() { }

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual bool GetFocus() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		// todo let the user create this file
		static Window* Create(const WindowProps& props = WindowProps("app.config"));

		virtual double GetTime() const = 0;

		virtual void* GetNativeWindow() const = 0; // Ex: Get the GLFW window on Windows
	};
}