module;

#define ELEVATE_GLFW_FORWARD_DECLARED
#include <stdio.h>
#include <format>
#include <functional>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/allocators.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/encodings.h>
#include <rapidjson/rapidjson.h>
#include <GLFW/glfw3.h>

module Elevate.Core.Window;

import Elevate.Foundations.CoreLogger;
import Elevate.Core.Events.ApplicationEvent;
import Elevate.Core.Events.Event;
import Elevate.Renderer.GraphicsContext;

#define ELEVATE_WINDOW_IMPLEMENTATION
// These implementation headers derive from types owned by this module, so they
// must stay in the module purview. MSVC warns about textual inclusion there.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 5244)
#endif
#if defined(EE_PLATFORM_WINDOWS)
#include <ElevateEngine/Platform/Windows/WindowsWindow.h>
#elif defined(EE_PLATFORM_WEB)
#include <ElevateEngine/Platform/Web/WebWindow.h>
#elif defined(EE_PLATFORM_LINUX)
#include <ElevateEngine/Platform/Linux/LinuxWindow.h>
#else
#error "Unknown platform! Cannot create window on this platform."
#endif
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace Elevate
{
#if defined(EE_PLATFORM_WINDOWS)
	using PlatformWindow = WindowsWindow;
#elif defined(EE_PLATFORM_WEB)
	using PlatformWindow = WebWindow;
#elif defined(EE_PLATFORM_LINUX)
	using PlatformWindow = LinuxWindow;
#else
#error "Unknown platform! Cannot create window on this platform."
#endif

	WindowProps::WindowProps(const std::string appConfigFilePath)
	{
		// TODO put this part in a util somehwerre to get the document
		// todo : use impl in foundations to read file content;
		///////////////////////////////////////////////////////
		FILE* fp = fopen(appConfigFilePath.c_str(), "r");
		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer,
			sizeof(readBuffer));
		rapidjson::Document doc;
		doc.ParseStream(is);
		fclose(fp);

		// Error handling
		if (doc.HasParseError())
		{
			CoreLogger::Trace("ERROR PARSING The WindowProps JSON");
		}
		///////////////////////////////////////////////////////

		if (doc.HasMember("title") && doc["title"].IsString())
		{
			this->Title = doc["title"].GetString();
		}

		if (doc.HasMember("size"))
		{
			const rapidjson::Value& size = doc["size"];
			if (size.HasMember("x") && size["x"].IsInt())
			{
				this->Width = size["x"].GetInt();
			}
			if (size.HasMember("y") && size["y"].IsInt())
			{
				this->Height = size["y"].GetInt();
			}
		}

		if (doc.HasMember("vsync") && doc["vsync"].IsBool())
		{
			this->VSync = doc["vsync"].GetBool();
		}
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new PlatformWindow(props);
	}

	void Window::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		CoreLogger::Trace("Creating window: {} ({}x{})", props.Title.c_str(), props.Width, props.Height);
	}

	void Window::SetWindowSize(unsigned int width, unsigned int height)
	{
		CoreLogger::Info("Setting window size to {}x{}", width, height);
		m_Data.Width = width;
		m_Data.Height = height;
		WindowResizeEvent event(width, height);
		EventCallback(event);
	}
}