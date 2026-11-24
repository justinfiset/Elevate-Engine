module;

#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>

export module Elevate.Foundations.Logger;

export import Elevate.Foundations.Logger.Facade;
import Elevate.Foundations.Logger.Internal;

export namespace Elevate
{
	class Logger : public Internal::LoggerFacade<Logger>
	{
	public:
		static Internal::LogImpl* GetImpl();
	};
}
