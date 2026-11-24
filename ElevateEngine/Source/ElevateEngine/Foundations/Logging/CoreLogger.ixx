module;


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
