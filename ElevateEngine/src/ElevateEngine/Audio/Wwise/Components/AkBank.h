#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/ComponentRegistry.h>

#include <ElevateEngine/Audio/Wwise/WwiseItem.h>

namespace Elevate
{
	class AkBank : public Component
	{
		BEGIN_COMPONENT(AkBank)

	public:
		//WwiseSoundbankPtr m_soundBank;
		//EXPOSE(m_soundBank);

		std::string m_soundBankName;
		EXPOSE(m_soundBankName)

	protected:
		void Init() override;
	};
}