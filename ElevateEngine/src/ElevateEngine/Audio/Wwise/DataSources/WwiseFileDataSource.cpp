#include "WwiseFileDataSource.h"
#include <ElevateEngine/Core/Log.h>

#include <array>
#include <filesystem>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#define WWISE_WORKUNIT_EXTENSION ".wwu"
#define GET_FIRST_NODE_CHECKED(x) x; if(!x) return

namespace fs = std::filesystem;
using namespace rapidxml;

namespace Elevate
{
	void WwiseFileDataSource::RefreshSource()
	{
		EE_CORE_INFO("Getting Wwise Project Info from Files...");

		m_treeRoot.reset(new WwiseItem());
		m_treeRoot->Type = WwiseType::TreeRoot;
		m_treeRoot->IsOnDisk = true;

		fs::path basePath = m_projectPath;
		if (!fs::is_directory(basePath))
		{
			EE_CORE_ERROR("ERROR (WwiseFileDataSource::RefreshSource() : The provided project path is not a valid directory : {}", basePath.string());
		}

		for (const auto& entry : fs::directory_iterator(basePath))
		{
			ProcessRootFolder(entry);
		}

		EE_CORE_TRACE("Wwise Project File Analysed.");
	}

	void WwiseFileDataSource::ProcessRootFolder(const std::filesystem::path& rootFolder)
	{
		// todo avoid checking useless folders like cache and others
		if (!fs::is_directory(rootFolder) || !IsValidDirectory(rootFolder.stem().string()))
		{
			return; // We do not have a directory
		}

		WwiseItemPtr folderItem = WwiseItem::Create();
		folderItem->Name = rootFolder.filename().string();
		folderItem->Type = WwiseType::RootDirectory;
		folderItem->IsOnDisk = true;
		m_treeRoot->AddChildren(folderItem);

		for (const auto& entry : fs::directory_iterator(rootFolder))
		{
			ProcessWorkUnit(folderItem, entry);
		}
	}

	void WwiseFileDataSource::ProcessWorkUnit(WwiseItemPtr parent, const std::filesystem::path& workUnitPath)
	{
		if (workUnitPath.extension() != WWISE_WORKUNIT_EXTENSION)
		{
			return;
		}

		file<> workUnitFile(workUnitPath.string().c_str());
		xml_document<> doc;
		doc.parse<0>(workUnitFile.data());

		// Get the second node as the first one is the xml
		// todo check the document type to check if really workunit
		xml_node<>* docNode = GET_FIRST_NODE_CHECKED(doc.first_node());
		xml_node<>* typeNode = GET_FIRST_NODE_CHECKED(docNode->first_node());
		xml_node<>* workUnitNode = GET_FIRST_NODE_CHECKED(typeNode->first_node());

		WwiseItemPtr workUnitItem = WwiseItem::Create();
		workUnitItem->Name = workUnitPath.stem().string();
		workUnitItem->Type = WwiseType::WorkUnit;
		workUnitItem->GUID = workUnitNode->first_attribute("ID")->value();
		workUnitItem->IsOnDisk = true;
		parent->AddChildren(workUnitItem);

		xml_node<>* childrenListNode = GET_FIRST_NODE_CHECKED(workUnitNode->first_node());
		for (xml_node<>* child = childrenListNode->first_node(); child; child = child->next_sibling())
		{
			ProcessNode(workUnitItem, child);
		}

		doc.clear();
	}

	void Elevate::WwiseFileDataSource::ProcessNode(WwiseItemPtr parent, rapidxml::xml_node<>* node)
	{
		xml_attribute<>* name = node->first_attribute("Name");
		xml_attribute<>* id = node->first_attribute("ID");
		xml_attribute<>* shortId = node->first_attribute("ShortID");
		WwiseType type = GetTypeFromName(node->name());

		if (type != WwiseType::Unknown)
		{
			WwiseItemPtr childItem = WwiseItem::Create();
			childItem->Type = type;
			if (name)
			{
				childItem->Name = name->value();
			}
			if (id)
			{
				childItem->GUID = id->value();
			}
			if (shortId)
			{
				childItem->ShortID = std::stoi(shortId->value());
			}
			parent->AddChildren(childItem);

			if (childItem->IsDirectory())
			{
				xml_node<>* childrenListNode = GET_FIRST_NODE_CHECKED(node->first_node("ChildrenList"));
				for (xml_node<>* child = childrenListNode->first_node(); child; child = child->next_sibling())
				{
					ProcessNode(childItem, child);
				}
			}
		}
	}

	bool WwiseFileDataSource::IsValidDirectory(std::string directoryName)
	{
		constexpr std::array<std::string_view, 6> validDirectories = {
			"Busses", "Devices", "Events", "SoundBanks", "States", "Switches"
		};

		for (const auto& name : validDirectories)
		{
			if (name == directoryName)
			{
				return true;
			}
		}
		return false;
	}

	WwiseType WwiseFileDataSource::GetTypeFromName(std::string name)
	{
		if (name == "Bus")					return WwiseType::Bus;
		else if (name == "WorkUnit")		return WwiseType::WorkUnit;
		else if (name == "AudioDevice")		return WwiseType::AudioDevice;
		else if (name == "Event")			return WwiseType::Event;
		else if (name == "SwitchGroup")		return WwiseType::SwitchGroup;
		else if (name == "Switch")			return WwiseType::Switch;
		else if (name == "StateGroup")		return WwiseType::StateGroup;
		else if (name == "State")			return WwiseType::State;
		else if (name == "GameParameter")	return WwiseType::GameParameter;
		else if (name == "Effect")			return WwiseType::Effect;
		else if (name == "Trigger")			return WwiseType::Trigger;
		else if (name == "Folder")			return WwiseType::VirtualFolder;
		else return WwiseType::Unknown;
	}

	void WwiseFileDataSource::InitializeSource()
	{
		RefreshSource();
	}
}