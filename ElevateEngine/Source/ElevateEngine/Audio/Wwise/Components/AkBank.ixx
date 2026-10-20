module;

#include "Reflection.h"
#include <string>

export module Elevate.Audio.Wwise.AkBank;

import Elevate.Core.Scene:GameObject;
import Elevate.Core.Scene:Component;

export namespace Elevate
{
	class AkBank : public Component
	{
		BEGIN_COMPONENT(AkBank)

	public:
		//WwiseSoundbankPtr m_soundBank;
		//PROPERTY(m_soundBank);

		std::string m_soundBankName;
		PROPERTY(m_soundBankName)

	protected:
		void Init() override;
	};
}