#pragma once

import Elevate.Core.Application;

namespace Elevate
{
	Application* CreateApplication();
}

int main(int argc, char** argv)
{
	Elevate::Application::Start(argc, argv, &Elevate::CreateApplication);
}
