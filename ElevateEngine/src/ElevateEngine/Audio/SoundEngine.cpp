#include "eepch.h"
#include "SoundEngine.h"

#include <ElevateEngine/Audio/Wwise/WwiseSoundEngine.h>

#include <ElevateEngine/Audio/Components/AudioDistanceProbe.h>

// todo ajouter une condition, on pourrait vouloir le laisser à null et non tout le tmeps à ceci, même si les header ne sont pas included
Elevate::SoundEngine* Elevate::SoundEngine::Impl = new Elevate::WwiseSoundEngine();