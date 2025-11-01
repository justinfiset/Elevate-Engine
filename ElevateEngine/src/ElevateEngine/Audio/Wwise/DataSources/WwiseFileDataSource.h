#pragma once

#include <string>

#include <filesystem>

#include <ElevateEngine/Audio/Wwise/DataSources/WwiseDataSource.h>
#include <ElevateEngine/Audio/Wwise/WwiseItem.h>

namespace rapidxml
{
	template<class Ch>
	class xml_node;
}

namespace Elevate
{
	class WwiseFileDataSource : public WwiseDataSource
	{
	public:
		WwiseFileDataSource(const std::string projectPath)
			: m_projectPath(projectPath) { }
		~WwiseFileDataSource() = default;

		virtual void InitializeSource() override;
		virtual void RefreshSource() override;

	protected:
		void ProcessRootFolder(const std::filesystem::path& rootFolder);
		void ProcessWorkUnit(WwiseItemPtr parent, const std::filesystem::path& workUnitPath);
		void ProcessNode(WwiseItemPtr parent, rapidxml::xml_node<char>* node);
		bool IsValidDirectory(std::string directoryName);
	private:
		WwiseType GetTypeFromName(std::string name);

	private:
		std::string m_projectPath;
	};
}