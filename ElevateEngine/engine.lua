local function values(t)
    local res = {}
    for _, v in pairs(t) do table.insert(res, v) end
    return res
end

local function orderedModuleFiles()
	local moduleFiles = os.matchfiles(_MAIN_SCRIPT_DIR .. "/ElevateEngine/Source/**/*.ixx")
	local modules = {}
	local dependencies = {}

	for _, filename in ipairs(moduleFiles) do
		local file = io.open(filename, "r")
		if file then
			local content = file:read("*all")
			file:close()

			local moduleName = content:match("export%s+module%s+([%w%.]+)%s*;")
			if moduleName then
				modules[moduleName] = filename
				dependencies[moduleName] = {}
				for importedModule in content:gmatch("import%s+([%w%.]+)%s*;") do
					table.insert(dependencies[moduleName], importedModule)
				end
			end
		end
	end

	local ordered = {}
	local visiting = {}
	local visited = {}
	local function visit(moduleName)
		if visited[moduleName] or visiting[moduleName] then
			return
		end

		visiting[moduleName] = true
		for _, dependency in ipairs(dependencies[moduleName] or {}) do
			if modules[dependency] then
				visit(dependency)
			end
		end
		visiting[moduleName] = nil
		visited[moduleName] = true
		table.insert(ordered, modules[moduleName])
	end

	for moduleName in pairs(modules) do
		visit(moduleName)
	end

	return ordered
end

local function moduleBuildCommands()
	local commands = {}
	for _, filename in ipairs(orderedModuleFiles()) do
		local relativePath = filename:gsub("^" .. _MAIN_SCRIPT_DIR .. "/", "../")
		table.insert(commands, "$(CXX) $(ALL_CXXFLAGS) -fmodules-ts -fmodule-only -x c++ -c " .. relativePath)
	end
	return commands
end

local function webModuleBuildCommands()
	local commands = {}
	local moduleDirectory = "%{cfg.objdir}/modules"
	local includeFlags = "-I../Source -I../Vendor -I../Vendor/glm -I../Vendor/spdlog/include "
		.. "-I../Vendor/rapidjson/include -I../Vendor/entt/include -I../Vendor/stduuid/include "
		.. "-I../Vendor/assimp/include -I../Vendor/JoltPhysics"

	table.insert(commands, "cmd /c if not exist " .. moduleDirectory .. " mkdir " .. moduleDirectory)
	for _, filename in ipairs(orderedModuleFiles()) do
		local file = io.open(filename, "r")
		local moduleName = nil
		if file then
			moduleName = file:read("*all"):match("export%s+module%s+([%w%.]+)%s*;")
			file:close()
		end

		if moduleName then
			local relativePath = filename:gsub("^" .. _MAIN_SCRIPT_DIR .. "/", "../")
			local moduleOutput = moduleDirectory .. "/" .. moduleName .. ".pcm"
			table.insert(commands,
				"em++ -std=c++20 -DEE_ENGINE_BUILD -DEE_PLATFORM_WEB -DEE_EDITOR_BUILD "
				.. includeFlags .. " -x c++ -fmodule-output=" .. moduleOutput
				.. " -fprebuilt-module-path=" .. moduleDirectory .. " -c " .. relativePath)
		end
	end
	return commands
end

project "ElevateEngine"
	location "./Build"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	systemversion "latest"
	
	targetdir ("%{wks.location}/Build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Build/bin-int/" .. outputdir .. "/%{prj.name}")

	IncludeDir = {}
	IncludeDir["Vendors"] = "Vendor"
	IncludeDir["GLFW"] = "Vendor/GLFW/include" 
	IncludeDir["Glad"] = "Vendor/Glad/include"
	IncludeDir["ImGui"] = "Vendor/ImGui/"
	IncludeDir["glm"] = "Vendor/glm/"
	IncludeDir["rapidjson"] = "Vendor/rapidjson/include"
	IncludeDir["rapidxml"] = "Vendor/rapidxml/include"
	IncludeDir["stb"] = "Vendor/stb/"
	IncludeDir["spdlog"] = "Vendor/spdlog/include"
	IncludeDir["assimp"] = "Vendor/assimp/include"
	IncludeDir["tinyfiledialogs"] = "Vendor/tinyfiledialogs"
	IncludeDir["ImGuizmo"] = "Vendor/ImGuizmo"
	IncludeDir["entt"] = "Vendor/entt/include"
	IncludeDir["stduuid"] = "Vendor/stduuid/include"
	IncludeDir["Jolt"] = "Vendor/JoltPhysics"

	files
	{
		"Source/**.ixx",
		"Source/**.cpp",
		"Source/**.h",
		"Source/**.inl",

		"Vendor/ImGuizmo/ImGuizmo.cpp",
		"Vendor/ImGuiFileDialog/**.cpp",
		"Vendor/tinyfiledialogs/tinyfiledialogs.c",
	}

	defines 
	{
		"EE_RESOURCE_DIR=\""..path.getabsolute("./Resources/").."\"",
		"EE_ENGINE_BUILD",
		"_CRT_SECURE_NO_WARNINGS",
		"IMGUI_DEFINE_MATH_OPERATORS",
		"GLM_ENABLE_EXPERIMENTAL",
	}

	includedirs { "Source", values(IncludeDir) }

	links { "ImGui", "assimp", "Jolt" }

	Wwise.SetupEngine()
	filter "not system:emscripten"
		links { "GLFW" }
        files { "Vendor/Glad/src/glad.c" }
    filter {}

	BuildPlatform.SetPlatformDefines()

	filter "system:windows"
		links { "opengl32.lib" }
		
	filter { "system:linux", "language:C++" }
		links { "GL", "X11", "pthread", "dl" }
		buildoptions { "-fmodules-ts" }

	filter "system:linux"
		prebuildcommands(moduleBuildCommands())

	filter "system:emscripten"
		prebuildcommands(webModuleBuildCommands())

	filter { "system:emscripten", "files:**.ixx" }
		buildoptions
		{
			"-x", "c++",
			"-fprebuilt-module-path=%{cfg.objdir}/modules"
		}

	filter "configurations:Editor_Debug"
		defines { "EE_DEBUG", "EE_EDITOR_BUILD" }
		runtime "Debug"
		symbols "on"

	 filter "configurations:Editor_Release"
		defines { "EE_RELEASE", "EE_EDITOR_BUILD" }
		runtime "Release"
		optimize "on"

	filter "configurations:Debug"
		defines "EE_DEBUG"
		runtime "Debug"
		symbols "on"
		links { "ws2_32" }

	filter "configurations:Release"
		defines "EE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EE_DIST"
		runtime "Release"
		optimize "on"

	filter {}
	
print("Finished Generating Engine Solution.\n")
