#pragma once

#include <ElevateEngine/Events/Event.h>
#include <ElevateEngine/Core/GameContext.h>
#include <sstream>

namespace Elevate {
	class GameContextEvent : public Event
	{
	public:
		GameContextEvent() = default;
		GameContextEvent(GameContextState oldState, GameContextState newState)
			: m_oldState(oldState), m_newState(newState) { }

		inline GameContextState GetOldState() const { return m_oldState; }
		inline GameContextState GetNewState() const { return m_newState; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "GameContextEvent: old:" << m_oldState << ", new:" << m_newState;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GameContextChanged)
		EVENT_CLASS_CATEGORY(EventCategoryGameContext)
	private:
		GameContextState m_newState;
		GameContextState m_oldState;
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent(bool isFocused)
			: m_IsFocused(isFocused) {
		}

		bool GetFocusState() const { return m_IsFocused; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowFocusChange: " << m_IsFocused;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		bool m_IsFocused;
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}