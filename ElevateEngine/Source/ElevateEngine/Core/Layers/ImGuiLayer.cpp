module;

#include <cstdio>
#include <string>

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "ImGuizmo.h"

#include <GLFW/glfw3.h>

module Elevate.Core.Layers.ImGuiLayer;

import Elevate.Foundations.CoreLogger;
import Elevate.Core.Application;
import Elevate.Renderer.Shaders;

namespace
{
	constexpr ImGuiKey FPSOverlayToggleKey = ImGuiKey_F3;
	constexpr ImGuiKey FPSOverlayAlternateToggleKey = ImGuiKey_GraveAccent;
}

Elevate::ImGuiLayer::ImGuiLayer()
	: Layer("ImGuiLayer")
{
}

Elevate::ImGuiLayer::~ImGuiLayer()
{
}

void Elevate::ImGuiLayer::OnAttach()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	CoreLogger::Info("ImGui {} (version num: {})", IMGUI_VERSION, IMGUI_VERSION_NUM);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	  // Enable Multi-Viewport / Platform Windows

	// io.ConfigViewportsNoAutoMerge = true;
	// io.ConfigViewportsNoTaskBarIcon = true;

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

	// Setup Platform/Renderer bindings#
	ImGui_ImplGlfw_InitForOpenGL(window, true);

#ifdef EE_PLATFORM_WEB
	ImGui_ImplOpenGL3_Init("#version 300 es");
#else
	ImGui_ImplOpenGL3_Init("#version 410 core");
#endif
}

void Elevate::ImGuiLayer::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Elevate::ImGuiLayer::OnImGuiRender()
{
}

void Elevate::ImGuiLayer::OnImGuiRenderBegin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void Elevate::ImGuiLayer::OnImGuiRenderEnd()
{
#ifndef EE_DIST
	if (ImGui::IsKeyPressed(FPSOverlayToggleKey, false) ||
		ImGui::IsKeyPressed(FPSOverlayAlternateToggleKey, false))
	{
		m_showFpsOverlay = !m_showFpsOverlay;
	}

	if (m_showFpsOverlay)
	{
		const ImGuiIO& io = ImGui::GetIO();
		const float frameTimeMs = io.Framerate > 0.0f ? 1000.0f / io.Framerate : 0.0f;

		char label[64];
		std::snprintf(label, sizeof(label), "%.1f FPS (%.2f ms)", io.Framerate, frameTimeMs);

		const ImVec2 textSize = ImGui::CalcTextSize(label);
		const ImVec2 padding(6.0f, 4.0f);
		const ImVec2 viewportPos = ImGui::GetMainViewport()->Pos;
		const ImVec2 min(viewportPos.x + 10.0f, viewportPos.y + 10.0f);
		const ImVec2 max(min.x + textSize.x + 2.0f * padding.x, min.y + textSize.y + 2.0f * padding.y);

		ImDrawList* drawList = ImGui::GetForegroundDrawList(ImGui::GetMainViewport());
		drawList->AddRectFilled(min, max, IM_COL32(0, 0, 0, 160), 4.0f);
		drawList->AddText(ImVec2(min.x + padding.x, min.y + padding.y), IM_COL32(255, 255, 255, 255), label);
	}
#endif

	ImGui::Render();

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(
		(float)Application::Get().GetWindow().GetWidth(),
		(float)Application::Get().GetWindow().GetHeight());

	// Rendering
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void Elevate::ImGuiLayer::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
