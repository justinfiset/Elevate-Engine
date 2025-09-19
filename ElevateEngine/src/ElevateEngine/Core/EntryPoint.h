#pragma once

extern Elevate::Application* Elevate::CreateApplication();

int main(int argc, char** argv) {
    // Init sequence - Later to be put in another method
    Elevate::Log::Init();
    EE_CORE_INFO("Initializing ElevateEngine...");

    auto app = Elevate::CreateApplication();
    EE_CORE_TRACE("Application Initialized.");
    app->Run();
    delete app;
}
