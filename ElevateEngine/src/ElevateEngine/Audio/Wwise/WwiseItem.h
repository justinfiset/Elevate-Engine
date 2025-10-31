#pragma once

#include <string>
#include <vector>
#include <memory>

enum class WwiseType : uint8_t
{
	Unknown,

	TreeRoot,
	RootDirectory,
	WorkUnit,
	VirtualFolder,

	Event,
	Bus,
	SwitchGroup,
	Switch,
	StateGroup,
	State,
	GameParameter,
	Effect,
	Trigger,
	AudioDevice
};

struct WwiseItem;
typedef std::shared_ptr<WwiseItem> WwiseItemPtr;
typedef std::weak_ptr<WwiseItem> WwiseItemWeakPtr;

struct WwiseItem
{
	std::string Name;
	std::string GUID;
	uint16_t ShortID;
	WwiseType Type;
	std::vector<WwiseItemPtr> Children;

	bool IsOnDisk = false; // true if this item existsa in one of the xml files.

	static WwiseItemPtr Create();

	bool HasValidShortID();
	bool IsAnyOfType(std::initializer_list<WwiseType> types) const;
	bool IsDirectory() const;
	void AddChildren(std::shared_ptr<WwiseItem> newChild);
	bool HasChildren() const;

	std::string GetTypeName() const;
};

struct WwiseGameParameter : public WwiseItem
{
	double InitialValue;
	double Min;
	double Max;
};

struct WwiseEvent : public WwiseItem
{

};