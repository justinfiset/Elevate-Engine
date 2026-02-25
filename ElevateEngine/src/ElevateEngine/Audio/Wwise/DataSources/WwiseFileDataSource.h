#pragma once

#include <string>

#include <filesystem>

#include <ElevateEngine/Audio/Wwise/DataSources/WwiseDataSource.h>
#include <ElevateEngine/Audio/Wwise/WwiseItem.h>

#include <rapidxml.hpp>

namespace Elevate
{
	class WwiseFileDataSource : public WwiseDataSource
	{
	public:
		WwiseFileDataSource(const std::string projectPath)
			: m_projectPath(projectPath) {
		}
		~WwiseFileDataSource() = default;

		virtual void InitializeSource() override;
		virtual void RefreshSource() override;

	protected:
		void ProcessRootFolder(const std::filesystem::path& rootFolder);
		void ProcessWorkUnit(WwiseItemPtr parent, const std::filesystem::path& workUnitPath);
		void ProcessNode(WwiseItemPtr parent, rapidxml::xml_node<>* node);

		// Process each type of node in Wwise
		//void P
		WwiseItemPtr ProcessBus(WwiseItemPtr parent, rapidxml::xml_node<>* node);
		WwiseItemPtr ProcessAuxBus(WwiseItemPtr parent, rapidxml::xml_node<>* node);
		WwiseItemPtr ProcessEvent(WwiseItemPtr parent, rapidxml::xml_node<>* node);
		WwiseItemPtr ProcessSoundBank(WwiseItemPtr parent, rapidxml::xml_node<>* node);
		WwiseItemPtr ProcessStateGroup(WwiseItemPtr parent, rapidxml::xml_node<>* node);
		WwiseItemPtr ProcessState(WwiseItemPtr parent, rapidxml::xml_node<>* node);
		WwiseItemPtr ProcessSwitchGroup(WwiseItemPtr parent, rapidxml::xml_node<>* node);
		WwiseItemPtr ProcessSwitch(WwiseItemPtr parent, rapidxml::xml_node<>* node);

		bool IsValidDirectory(std::string directoryName);
	private:
		WwiseType GetTypeFromName(std::string name);

	private:
		std::string m_projectPath;
	};
}