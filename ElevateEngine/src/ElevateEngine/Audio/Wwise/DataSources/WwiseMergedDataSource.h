#pragma once

#include "WwiseDataSource.h"
#include "WwiseProjectDBDataSource.h"
#include "WwiseFileDataSource.h"

#include <string>
#include <map>

namespace Elevate
{
	class WwiseMergedDataSource : public WwiseDataSource
	{
	public:
		WwiseMergedDataSource(std::string& wwiseProjectPath, std::string& rootOutputPath, std::string& platform);
		~WwiseMergedDataSource() = default;

		virtual void InitializeSource() override;
		virtual void RefreshSource() override;
		virtual std::weak_ptr<WwiseItem> GetItems() const override;

		void ProcessChildrenMerge(WwiseItemPtr parent, std::map<std::string, WwiseItemPtr>& projectDBRefs);

	protected:
		std::unique_ptr<WwiseFileDataSource> m_fileDataSource;
		// this is to complement the file datasource, fetching informations from the .json generated in parralel with banks.
		std::unique_ptr<WwiseProjectDBDataSource> m_projectDataSource; 
	};
}