#include <string>
#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/TypeRegistry.h>
#include <ElevateEngine/Editor/Serialization/TypeLayout.h>

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