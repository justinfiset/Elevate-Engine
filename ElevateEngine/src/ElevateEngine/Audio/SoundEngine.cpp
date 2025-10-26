#include "eepch.h"
#include "SoundEngine.h"

#include <ElevateEngine/Audio/Wwise/WwiseSoundEngine.h>

Elevate::SoundEngine* Elevate::SoundEngine::Impl = new Elevate::WwiseSoundEngine();