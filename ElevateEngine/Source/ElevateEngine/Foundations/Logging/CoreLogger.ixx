module;

#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>

export module Elevate.Foundations.CoreLogger;

export import Elevate.Foundations.Logger.Facade;
import Elevate.Foundations.Logger.Internal;

export namespace Elevate
{
	class CoreLogger : public Internal::LoggerFacade<CoreLogger>
	{
	public:
		static Internal::LogImpl* GetImpl();
	};
}
