#pragma once

#include <memory>
#include <ElevateEngine/Core/Reflection.h>
#include <ElevateEngine/Core/TypeField.h>
#include <ElevateEngine/Core/Component.h>

namespace Elevate
{
	class AudioDistanceProbe : public Component
	{
	public:
		BEGIN_COMPONENT(AudioDistanceProbe)
		EECATEGORY("Audio")

		AudioDistanceProbe() = default;

		void Init() override;
		void Destroy() override;

		END_COMPONENT()
	};
}