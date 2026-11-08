module;

#include <ElevateEngine/Core/Reflection/Reflection.h>

export module Elevate.Core.Assets.Asset;

import Elevate.Core.Objects;
import Elevate.Core.Types.TypeLayout;

export namespace Elevate
{
	class Asset : public EEObject
	{
		BEGIN_OBJECT(Asset);
		using Super = Asset;

	public:
		Asset() = default;
		virtual ~Asset() = default;

		virtual std::string GetName() const override;

		virtual void OnLoad() {}
		virtual void OnUnbind() {}

		END_OBJECT_CUSTOM();
		DECLARE_AUTO_OBJECT_LAYOUT();
	};
}
