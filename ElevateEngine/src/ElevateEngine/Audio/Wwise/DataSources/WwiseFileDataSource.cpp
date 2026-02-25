#include "WwiseFileDataSource.h"
#include <ElevateEngine/Core/Log.h>

#include <array>
#include <filesystem>

#include <flxml/utils.h>

#define WWISE_WORKUNIT_EXTENSION ".wwu"

namespace fs = std::filesystem;
using namespace flxml;

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

		rapidxml::
			file<> workUnitFile(workUnitPath.string().c_str());
		xml_document<> doc;
		doc.parse<0>(workUnitFile.data());

		// Get the second node as the first one is the xml
		// todo check the document type to check if really workunit
		auto optDocNode = doc.first_node();
		if (!optDocNode) return;
		xml_node<char>* docNode = optDocNode.get();

		auto optTypeNode = docNode->first_node();
		if (!optTypeNode) return;
		xml_node<char>* typeNode = optTypeNode.get();

		auto optWorkUnitNode = typeNode->first_node();
		if (!optWorkUnitNode) return;
		xml_node<char>* workUnitNode = optWorkUnitNode.get();

		auto optIdAttr = workUnitNode->first_attribute("ID");

		WwiseItemPtr workUnitItem = WwiseItem::Create();
		workUnitItem->Name = workUnitPath.stem().string();
		workUnitItem->Type = WwiseType::WorkUnit;
		workUnitItem->IsOnDisk = true;

		if (optIdAttr)
		{
			workUnitItem->GUID = optIdAttr.get()->value();
		}

		parent->AddChildren(workUnitItem);

		auto optChildrenListNode = workUnitNode->first_node();
		if (optWorkUnitNode)
		{
			for (auto child = optWorkUnitNode->first_node(); child; child = child->next_sibling())
			{
				if (child)
				{
					ProcessNode(workUnitItem, child.get());
				}
			}
		}

		doc.clear();
	}

	void Elevate::WwiseFileDataSource::ProcessNode(WwiseItemPtr parent, rapidxml::xml_node<char>* node)
	{
		auto optName = node->first_attribute("Name");
		auto optId = node->first_attribute("ID");
		auto optShortId = node->first_attribute("ShortID");

		WwiseType type = GetTypeFromName(std::string(node->name()));
		if (type != WwiseType::Unknown)
		{
			WwiseItemPtr childItem = WwiseItem::Create();

			childItem->Type = type;
			if (optName)    childItem->Name = optName.get()->value();
			if (optId)      childItem->GUID = optId.get()->value();
			if (optShortId) childItem->ShortID = std::stoi(std::string(optShortId.get()->value()));
			parent->AddChildren(childItem);

			if (childItem->IsDirectory())
			{
				auto optChildrenList = node->first_node("ChildrenList");
				if (optChildrenList)
				{
					auto childrenListNode = optChildrenList.get();
					for (auto optChild = childrenListNode->first_node(); optChild; optChild = optChild.get()->next_sibling())
					{
						ProcessNode(childItem, optChild.get());
					}
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