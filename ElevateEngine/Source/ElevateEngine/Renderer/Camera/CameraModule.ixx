module;

// EnTT iterator operators must be visible in the global module when inline
// Scene code is instantiated here; they otherwise stay attached to a module
// BMI and are not found by ADL.
#include <entt/entt.hpp>

export module Elevate.Renderer.Camera;

export import :Fwd;

export import :CameraProjectionMode;
export import :CameraProjection;
export import :PerspectiveProjection;
export import :OrthographicProjection;

export import :Camera;
export import :CameraManager;
