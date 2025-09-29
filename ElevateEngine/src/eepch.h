#pragma once

#ifndef EE_ENGINE_BUILD
	#error "eepch.h should only be included while building the engine!"
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>

#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <sstream>
#include <ostream>
#include <fstream>

#include <filesystem>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// RAPIDJSON
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

// VENDORS
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <assimp/scene.h>
#include <tinyfiledialogs.h>

#ifdef EE_PLATFORM_WINDOWS
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif