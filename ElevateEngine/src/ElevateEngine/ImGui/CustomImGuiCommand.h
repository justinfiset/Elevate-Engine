#pragma once
#include <string>

namespace Elevate
{
	namespace UI
	{
		void InputField(const char* label, std::string& data);

		bool EECollapsingHeader(const char* label, bool canOpen = true, std::function<float()> headerCustomContent = nullptr);
	}
}