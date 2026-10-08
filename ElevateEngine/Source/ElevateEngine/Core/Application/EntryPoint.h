#pragma once

import Elevate.Core.Application;

extern Elevate::Application* Elevate::CreateApplication();

int main(int argc, char** argv)
{
	Elevate::Application::Start(argc, argv);
}