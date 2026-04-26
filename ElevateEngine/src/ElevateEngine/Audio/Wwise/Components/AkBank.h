#include <string>
#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/ComponentRegistry.h>
#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>

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