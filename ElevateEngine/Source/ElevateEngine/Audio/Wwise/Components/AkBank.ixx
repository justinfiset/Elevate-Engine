module;

#include "Reflection.h"
#include <string>

export module Elevate.Audio.Wwise.AkBank;

import Elevate.Core.Scene;

export namespace Elevate
{
	class AkBank : public Component
	{
		BEGIN_COMPONENT(AkBank);

	public:
		//WwiseSoundbankPtr m_soundBank;
		//PROPERTY(m_soundBank);

		std::string m_soundBankName;
		PROPERTY(m_soundBankName);

		void Init() override;

		END_COMPONENT();
	};
}