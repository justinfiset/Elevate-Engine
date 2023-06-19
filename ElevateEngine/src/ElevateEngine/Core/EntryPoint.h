#pragma once

#ifdef EE_PLATFORM_WINDOWS

extern Hammer::Application* Hammer::CreateApplication();

int main(int argc, char** argv) {
    // Init sequence - Later to be put in another method
    Hammer::Log::Init();
    EE_CORE_INFO("Initializing ElevateEngine...");
    EE_CORE_TRACE("Logger Initialized.");

    auto app = Hammer::CreateApplication();
    app->Run();
    delete app;
}

#endif
