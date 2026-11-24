module;

#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>

export module Elevate.Foundations.Cleanups.IShutdownable;

export namespace Elevate
{
	class IShutdownable
	{
	public:
		virtual ~IShutdownable() = default;

		virtual void Shutdown() = 0;
	};
}
