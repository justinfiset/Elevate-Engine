#include "WwiseItem.h"

#include <ElevateEngine/Core/Log.h>

WwiseItemPtr WwiseItem::Create()
{
	return std::make_shared<WwiseItem>();
}

bool WwiseItem::HasValidShortID()
{
	return ShortID != 0;
}

bool WwiseItem::IsAnyOfType(std::initializer_list<WwiseType> types) const
{
	for (const auto& type : types)
	{
		if (type == Type)
		{
			return true;
		}
	}
	return false;
}

std::string WwiseItem::GetTypeName() const
{
	switch (Type)
	{
	case WwiseType::TreeRoot:
		return "Tree Root";
		break;
	case WwiseType::RootDirectory:
		return "Root Directory";
		break;
	case WwiseType::WorkUnit:
		return "Work Unit";
		break;
	case WwiseType::VirtualFolder:
		return "Virtual Folder";
		break;
	case WwiseType::Event:
		return "Event";
		break;
	case WwiseType::Bus:
		return "Bus";
		break;
	case WwiseType::SwitchGroup:
		return "Switch Group";
		break;
	case WwiseType::Switch:
		return "Switch";
		break;
	case WwiseType::StateGroup:
		return "State Group";
		break;
	case WwiseType::State:
		return "State";
		break;
	case WwiseType::GameParameter:
		return "Game Parameter";
		break;
	case WwiseType::Effect:
		return "Effect ";
		break;
	case WwiseType::Trigger:
		return "Trigger";
		break;
	case WwiseType::AudioDevice:
		return "Audio Device";
		break;
	default:
		return "Unknown";
		break;
	}
}

bool WwiseItem::IsDirectory() const
{
	return IsAnyOfType({ WwiseType::WorkUnit, WwiseType::VirtualFolder, WwiseType::StateGroup, WwiseType::SwitchGroup, WwiseType::Bus, WwiseType::RootDirectory });
}

void WwiseItem::AddChildren(std::shared_ptr<WwiseItem> newChild)
{
	if (newChild)
	{
		Children.push_back(newChild);
	}
}

bool WwiseItem::HasChildren() const
{
	return !Children.empty();
}

bool WwiseItem::IsEvent() const
{
	return Type == WwiseType::Event;
}

WwiseType WwiseItem::GetContainerType() const
{
	// For root folders
	if (!IsDirectory()) return WwiseType::None;

	if (Name == "Busses")				return WwiseType::Bus;
	else if (Name == "Devices")			return WwiseType::AudioDevice;
	else if (Name == "Events")			return WwiseType::Event;
	else if (Name == "Switches")		return WwiseType::Switch;
	else if (Name == "StateGroup")		return WwiseType::StateGroup;
	else if (Name == "States")			return WwiseType::State;
	else if (Name == "GameParameter")	return WwiseType::GameParameter;
	else if (Name == "Effects")			return WwiseType::Effect;
	else if (Name == "Triggers")		return WwiseType::Trigger;
	else return WwiseType::Unknown;
}
