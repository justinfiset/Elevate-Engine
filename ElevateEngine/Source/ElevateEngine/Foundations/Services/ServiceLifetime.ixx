module;

#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>

export module Elevate.Foundations.Services:ServiceLifetime;

export namespace Elevate::Internal
{
    enum class ServiceLifetime
    {
        Singleton,
        Transient,
        Scoped
    };
}
