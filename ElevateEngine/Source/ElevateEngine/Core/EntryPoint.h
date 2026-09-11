#pragma once

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Application.h>

extern Elevate::Application* Elevate::CreateApplication();

int main(int argc, char** argv)
{
	Elevate::Application::Start(argc, argv);
}