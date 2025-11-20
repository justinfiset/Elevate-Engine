#pragma once

#include <string>
#include <vector>
#include <memory>

enum class WwiseType : uint8_t
{
	Unknown,
	None,

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

#define CREATE_STRUCT_PTR_TYPES(x) \
struct x; \
typedef std::shared_ptr<##x> x##Ptr; \
typedef std::weak_ptr<##x> x##WeakPtr; \

CREATE_STRUCT_PTR_TYPES(WwiseItem);
struct WwiseItem
{
	std::string Name;
	std::string Path;
	std::string GUID;
	uint32_t ShortID;
	WwiseType Type;
	std::vector<WwiseItemPtr> Children;

	bool IsOnDisk = false;  // true if this item existsa in one of the xml files.
	bool IsInBank = false;  // true if the item exists is one of the bank (.bnk)
	bool IsInWwise = false; // true if the item exits in the Wwise project (availible via WAAPI)

	static WwiseItemPtr Create();

	bool HasValidShortID();
	bool IsAnyOfType(std::initializer_list<WwiseType> types) const;
	bool IsDirectory() const;
	void AddChildren(std::shared_ptr<WwiseItem> newChild);
	bool HasChildren() const;

	bool IsEvent() const;

	WwiseType GetContainerType() const;

	std::string GetTypeName() const;
};

CREATE_STRUCT_PTR_TYPES(WwiseSoundbank);
struct WwiseSoundbank : public WwiseItem
{
	std::string Language;
	std::string ObjectPath;
	bool IsUserBank;
	bool IsInitBank;
	bool IsValid;

	static WwiseSoundbankPtr Create()
	{
		return std::make_shared<WwiseSoundbank>();
	}
};

CREATE_STRUCT_PTR_TYPES(WwiseEvent);
struct WwiseEvent : public WwiseItem
{
	float MaxAttenuation;
	float MinDuration;
	float MaxDuration;

	uint32_t SoundbankShortId;

	static WwiseEventPtr Create()
	{
		return std::make_shared<WwiseEvent>();
	}
};

CREATE_STRUCT_PTR_TYPES(WwiseSwitchGroup);
struct WwiseSwitchGroup : public WwiseItem
{
	static WwiseSwitchGroupPtr Create()
	{
		return std::make_shared<WwiseSwitchGroup>();
	}
};

CREATE_STRUCT_PTR_TYPES(WwiseSwitch);
struct WwiseSwitch : public WwiseItem
{
	uint32_t GroupShortId;

	static WwiseSwitchPtr Create()
	{
		return std::make_shared<WwiseSwitch>();
	}
};

CREATE_STRUCT_PTR_TYPES(WwiseStateGroup);
struct WwiseStateGroup : public WwiseItem
{
	static WwiseStateGroupPtr Create()
	{
		return std::make_shared<WwiseStateGroup>();
	}
};

CREATE_STRUCT_PTR_TYPES(WwiseState);
struct WwiseState : public WwiseItem
{
	uint32_t GroupShortId;

	static WwiseStatePtr Create()
	{
		return std::make_shared<WwiseState>();
	}
};

CREATE_STRUCT_PTR_TYPES(WwiseGameParameter);
struct WwiseGameParameter : public WwiseItem
{
	double InitialValue;
	double Min;
	double Max;

	static WwiseGameParameterPtr Create()
	{
		return std::make_shared<WwiseGameParameter>();
	}
};

CREATE_STRUCT_PTR_TYPES(WwiseBus);
struct WwiseBus : public WwiseItem
{
	static WwiseBusPtr Create()
	{
		return std::make_shared<WwiseBus>();
	}
};

CREATE_STRUCT_PTR_TYPES(WwiseAuxBus);
struct WwiseAuxBus : public WwiseItem
{
	static WwiseAuxBusPtr Create()
	{
		return std::make_shared<WwiseAuxBus>();
	}
};