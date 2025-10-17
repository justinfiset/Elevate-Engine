#pragma once

#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/ComponentRegistry.h> // TODO PREVENT FROM HAVING TO INCLUDE IN PROJECTS

namespace Elevate
{
	class AudioDistanceProbe : public Component
	{
	public:
		BEGIN_COMPONENT(AudioDistanceProbe)
		EECATEGORY("Audio")

		void Init() override;
		void Destroy() override;

		END_COMPONENT()
	};
}