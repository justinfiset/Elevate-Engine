#pragma once

#include <string>

#include <ElevateEngine/Core/Reflection.h>
#include <ElevateEngine/Core/EEObject.h>

namespace Elevate
{
	class Asset : public EEObject
	{
		BEGIN_OBJECT(Asset)

		using Super = Asset;

	public:
		Asset() = default;
		virtual ~Asset() = default;

		virtual void OnLoad() {}
		virtual void OnUnbind() {}

		END_OBJECT()
	};
}