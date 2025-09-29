#pragma once

#include <ElevateEngine/Core/Log.h>

extern Elevate::Application* Elevate::CreateApplication();

int main(int argc, char** argv) {
    // Init sequence - Later to be put in another method
    Elevate::Log::Init();
    EE_INFO("Initializing ElevateEngine...");

    auto app = Elevate::CreateApplication();
    EE_TRACE("Application Initialized.");
    app->Run();
    delete app;
}