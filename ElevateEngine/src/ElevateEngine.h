#pragma once

// For use by ElevateEngine applications
#include "ElevateEngine/Core/Application.h"
#include "ElevateEngine/Core/Layers/Layer.h"

// === Core ==========================
#include "ElevateEngine/Core/Log.h"
#include "ElevateEngine/Core/Assert.h"
#include "ElevateEngine/Core/Application.h"
#include "ElevateEngine/Core/GameObject.h"
#include "ElevateEngine/Core/Time.h"
// ===================================

// === Inputs ========================
#include "ElevateEngine/Inputs/Input.h"
#include "ElevateEngine/Inputs/KeyCodes.h"
#include "ElevateEngine/Inputs/MouseButtonCodes.h"
#include "ElevateEngine/Inputs/Joystick.h"
#include "ElevateEngine/Inputs/Gamepad.h"
// ===================================

// === Rendering =====================
#include "ElevateEngine/Renderer/Renderer.h"
#include "ElevateEngine/Renderer/VertexArray.h"
#include "ElevateEngine/Renderer/Buffer.h"
#include "ElevateEngine/ImGui/ImGuiLayer.h"

#include <ElevateEngine/Renderer/Texture/Texture.h>
#include "ElevateEngine/Renderer/Material.h"
#include "ElevateEngine/Renderer/Mesh.h"
#include "ElevateEngine/Renderer/Camera.h"
#include "ElevateEngine/Renderer/Model.h"
// --- Lighting ----------------------
#include <ElevateEngine/Renderer/Light/PointLight.h>
#include <ElevateEngine/Renderer/Light/DirectionalLight.h>
// ===================================

// === Physics =====================
#include <ElevateEngine/Physics.h>
// ===================================

// === Entry Point ===================
#include "ElevateEngine/Core/EntryPoint.h"
// ===================================