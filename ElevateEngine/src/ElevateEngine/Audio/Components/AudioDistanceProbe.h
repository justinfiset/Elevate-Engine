#pragma once

#include <memory>
#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/ComponentRegistry.h>
#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>

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