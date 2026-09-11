#pragma once

#include <string>

#include <ElevateEngine/Core/Reflection.h>
#include <ElevateEngine/Core/TypeField.h>
#include <ElevateEngine/Core/Component.h>

namespace Elevate
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