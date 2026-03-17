#pragma once

#include <ElevateEngine/Audio/Wwise/DataSources/WwiseDataSource.h>
#include <ElevateEngine/Audio/Wwise/WwiseItem.h>

namespace flxml
{
	template<class Ch> class xml_node;
}

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
		void ProcessNode(WwiseItemPtr parent, flxml::xml_node<char>* node);

		// Process each type of node in Wwise
		//void P
		WwiseItemPtr ProcessBus(WwiseItemPtr parent, flxml::xml_node<char>* node);
		WwiseItemPtr ProcessAuxBus(WwiseItemPtr parent, flxml::xml_node<char>* node);
		WwiseItemPtr ProcessEvent(WwiseItemPtr parent, flxml::xml_node<char>* node);
		WwiseItemPtr ProcessSoundBank(WwiseItemPtr parent, flxml::xml_node<char>* node);
		WwiseItemPtr ProcessStateGroup(WwiseItemPtr parent, flxml::xml_node<char>* node);
		WwiseItemPtr ProcessState(WwiseItemPtr parent, flxml::xml_node<char>* node);
		WwiseItemPtr ProcessSwitchGroup(WwiseItemPtr parent, flxml::xml_node<char>* node);
		WwiseItemPtr ProcessSwitch(WwiseItemPtr parent, flxml::xml_node<char>* node);

		bool IsValidDirectory(std::string directoryName);
	private:
		WwiseType GetTypeFromName(std::string name);

	private:
		std::string m_projectPath;
	};
}