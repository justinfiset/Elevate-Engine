#include "Window.h"

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Events/ApplicationEvent.h>

#include <rapidjson/document.h>
#include <rapidjson/allocators.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/encodings.h>
#include <rapidjson/rapidjson.h>

#if defined(EE_PLATFORM_WINDOWS)
	#include <Platform/Windows/WindowsWindow.h>
#elif defined(EE_PLATFORM_LINUX)
	#include <Platform/Linux/LinuxWindow.h>
#elif defined(EE_PLATFORM_WEB)
	#include <Platform/Web/WebWindow.h>
#else
	#error "Unknown platform! Cannot create window on this platform."
#endif

namespace Elevate
{
#if defined(EE_PLATFORM_WINDOWS)
	using PlatformWindow = WindowsWindow;
#elif defined(EE_PLATFORM_LINUX)
	using PlatformWindow = LinuxWindow;
#elif defined(EE_PLATFORM_WEB)
	using PlatformWindow = WebWindow;
#else
	#error "Unknown platform! Cannot create window on this platform."
#endif

	WindowProps::WindowProps(const std::string appConfigFilePath)
	{
		// TODO put this part in a util somehwerre to get the document
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
			EE_CORE_TRACE("ERROR PARSING The WindowProps JSON");
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
		EE_CORE_TRACE("Creating window: {} ({}x{})", props.Title.c_str(), props.Width, props.Height);
	}

	void Window::SetWindowSize(unsigned int width, unsigned int height)
	{ 
		m_Data.Width = width;
		m_Data.Height = height;
		WindowResizeEvent event(width, height);
		EventCallback(event);
	}
}