#pragma once

#include <string>
#include <functional>

namespace Elevate
{
	namespace UI
	{
		void InputField(const char* label, std::string& data);
		void InputField(const char* label, const std::string& data);

		bool EECollapsingHeader(const char* label, bool canOpen = true, const void* icon = nullptr, std::function<float()> headerCustomContent = nullptr);
	}
}