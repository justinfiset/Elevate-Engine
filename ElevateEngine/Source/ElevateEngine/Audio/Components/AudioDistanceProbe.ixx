module;

#include <memory>
#include <ElevateEngine/Core/Reflection/Reflection.h>

export module Elevate.Audio.AudioDistanceProbe;

import Elevate.Core.Scene.GameObject;
import Elevate.Core.Scene.Component;

export namespace Elevate
{
	class AudioDistanceProbe : public Component
	{
	public:
		BEGIN_COMPONENT(AudioDistanceProbe);
		EECATEGORY("Audio");

		AudioDistanceProbe() = default;

		void Init() override;
		void Destroy() override;

		END_COMPONENT();
	};
}