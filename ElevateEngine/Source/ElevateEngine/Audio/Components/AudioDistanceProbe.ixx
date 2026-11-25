module;

#include <memory>
#include "Reflection.h"

export module Elevate.Audio.AudioDistanceProbe;

import Elevate.Core.Scene;

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