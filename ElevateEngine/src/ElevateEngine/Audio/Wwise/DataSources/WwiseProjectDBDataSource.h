#pragma once

#include <ElevateEngine/Audio/Wwise/DataSources/WwiseDataSource.h>
#include <ElevateEngine/Audio/Wwise/WwiseItem.h>

struct WwiseCRefSoundBank;

namespace Elevate
{
	class WwiseProjectDBDataSource : public WwiseDataSource
	{
	public:
		WwiseProjectDBDataSource(const std::string& rootOutputPath, const std::string& platform);
		~WwiseProjectDBDataSource() = default;

		virtual void InitializeSource() override;
		virtual void RefreshSource() override;

	protected:
		void PopulateSoundbanks();
		void PopulateEvents(const WwiseCRefSoundBank* bnkRef);

		void PopulateSwitchGroups();
		void PopulateSwitches();

		void PopulateStateGroups();
		void PopulateStates();

		void PopulateGameParameters();
		void PopulateBusses();
		void PopulateAuxBusses();
	};
}