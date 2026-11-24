module;

#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>

export module Elevate.Foundations.Cleanups.ICleanup;

export namespace Elevate
{
    class ICleanup
    {
        virtual ~ICleanup() = default;

        virtual void Cleanup() = 0;
    };
}
