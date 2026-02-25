#include <ElevateEngine/Core/Log.h>

#include "WwiseMergedDataSource.h"

Elevate::WwiseMergedDataSource::WwiseMergedDataSource(std::string& wwiseProjectPath, std::string& rootOutputPath, std::string& platform)
{
	m_fileDataSource.reset(new WwiseFileDataSource(wwiseProjectPath));;
	m_projectDataSource.reset(new WwiseProjectDBDataSource(rootOutputPath, platform));
}

void Elevate::WwiseMergedDataSource::InitializeSource()
{
	m_fileDataSource->InitializeSource();
	m_projectDataSource->InitializeSource();
	RefreshSource();
}

void Elevate::WwiseMergedDataSource::RefreshSource()
{
	// This works as the project db data source keeps all of the items at the first le vel in the hierarchy
	// This lets us acess to the refs without having to loop over the array every time.
	std::map<std::string, WwiseItemPtr> projectDBRefs;
	for (const auto item : m_projectDataSource->GetItems().lock()->Children)
	{
		projectDBRefs.emplace(item->GUID, item);
	}
	ProcessChildrenMerge(m_fileDataSource->GetItems().lock(), projectDBRefs);

	// todo go get the remaining items in the refs and add them to the project
	EE_CORE_TRACE("Completed the Wwise Merged Data Source Refresh.");
}

std::weak_ptr<WwiseItem> Elevate::WwiseMergedDataSource::GetItems() const
{
	// we return this root as this is the map that is modified, for the moment there is not copy whatsoever
	return m_fileDataSource->GetItems();
}

void Elevate::WwiseMergedDataSource::ProcessChildrenMerge(WwiseItemPtr parent, std::map<std::string, WwiseItemPtr>& projectDBRefs)
{
	for (const auto item : parent->Children)
	{
		if (projectDBRefs.contains(item->GUID))
		{
			WwiseItemPtr projectDBItemRef = projectDBRefs[item->GUID];
			item->ShortID = projectDBItemRef->ShortID;
			item->Path = projectDBItemRef->Path;
			item->IsInBank = true;
			projectDBRefs.erase(item->GUID);
		}
		ProcessChildrenMerge(item, projectDBRefs);
	}
}