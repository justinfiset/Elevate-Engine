#pragma once

#ifdef EE_PLATFORM_WINDOWS

extern Elevate::Application* Elevate::CreateApplication();

int main(int argc, char** argv) {
    // Init sequence - Later to be put in another method
    Elevate::Log::Init();
    EE_CORE_INFO("Initializing ElevateEngine...");
    EE_CORE_TRACE("Logger Initialized.");

    auto app = Elevate::CreateApplication();
    app->Run();
    delete app;
}

#endif
