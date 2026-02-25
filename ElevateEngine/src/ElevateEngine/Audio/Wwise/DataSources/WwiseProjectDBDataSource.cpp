#include "WwiseProjectDBDataSource.h"
#include <ElevateEngine/Core/Log.h>

#define WWISE_DB_STANDARD_TYPES
#include "WwiseProjectDatabase/WwiseProjectDatabase.h"

Elevate::WwiseProjectDBDataSource::WwiseProjectDBDataSource(const std::string& rootOutputPath, const std::string& platform)
{
	Init(rootOutputPath.c_str(), platform.c_str());
}

void Elevate::WwiseProjectDBDataSource::InitializeSource()
{
	RefreshSource();
}

void Elevate::WwiseProjectDBDataSource::RefreshSource()
{
	m_treeRoot.reset(new WwiseItem());
	m_treeRoot->Type = WwiseType::TreeRoot;
	m_treeRoot->IsInBank = true;

	PopulateSoundbanks(); // This also populates the events

	PopulateSwitchGroups();
	PopulateSwitches();

	PopulateStateGroups();
	PopulateStates();

	PopulateGameParameters();
	PopulateBusses();
	PopulateAuxBusses();
}

void Elevate::WwiseProjectDBDataSource::PopulateSoundbanks()
{
	uint32_t count = (uint32_t) GetSoundBankCount();
	void* arr = (void*) GetAllSoundBanksRef();
	for (uint32_t i = 0; i < count; i++)
	{
		const ::WwiseCRefSoundBank* ref = GetSoundBankRefIndex(arr, i);
		if (ref)
		{
			WwiseSoundbankPtr item = WwiseSoundbank::Create();
			item->IsInitBank = ref->bIsInitBank;
			item->IsUserBank = ref->bIsUserBank;
			item->IsValid = ref->bIsValid;
			item->GUID = ref->guid->ToString().String;
			item->ShortID = (uint32_t) ref->shortId;
			item->Language = ref->language;
			item->Name = ref->name;
			item->Path = ref->path;
			item->ObjectPath = ref->objectPath;
			m_treeRoot->AddChildren(item);
			PopulateEvents(ref);
		}
	}
	DeleteSoundBanksArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateEvents(const WwiseCRefSoundBank* bnkRef)
{
	uint32_t count = (uint32_t) bnkRef->eventsCount;
	void* arr = (void*) bnkRef->events;
	for (uint32_t i = 0; i < count; i++)
	{
		const ::WwiseCRefEvent* ref = GetEvent(arr, i);
		if (ref)
		{
			WwiseEventPtr item = WwiseEvent::Create();
			item->GUID = ref->guid->ToString().String;
			item->MaxAttenuation = ref->maxAttenuation;
			item->MaxDuration = ref->maxDuration;
			item->MinDuration = ref->minDuration;
			item->Name = ref->name;
			item->Path = ref->path;
			item->ShortID = (uint32_t) ref->shortId;
			item->SoundbankShortId = (uint32_t) bnkRef->shortId;
			m_treeRoot->AddChildren(item);
		}
	}
}

void Elevate::WwiseProjectDBDataSource::PopulateSwitchGroups()
{
	uint32_t count = (uint32_t) GetSwitchGroupCount();
	void* arr = (void*) GetAllSwitchGroupRef();
	for (uint32_t i = 0; i < count; i++)
	{
		const WwiseCRefGroup* ref = GetSwitchGroup(arr, i);
		if (ref)
		{ 
			WwiseSwitchGroupPtr item = WwiseSwitchGroup::Create();
			item->GUID = ref->groupGuid->ToString().String;
			item->Name = ref->groupName;
			item->Path = ref->groupPath;
			item->ShortID = (uint32_t) ref->groupShortId;
			m_treeRoot->AddChildren(item);
		}
	}
	DeleteSwitchGroupArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateSwitches()
{
	uint32_t count = (uint32_t) GetSwitchCount();
	void* arr = (void*)GetAllSwitchRef();
	for (uint32_t i = 0; i < count; i++)
	{
		const WwiseCRefSwitch* ref = GetSwitch(arr, i);
		if (ref)
		{
			WwiseSwitchPtr item = WwiseSwitch::Create();
			item->GUID = ref->switchGuid->ToString().String;
			item->Name = ref->switchName;
			item->Path = ref->switchPath;
			item->ShortID = (uint32_t) ref->switchShortId;
			item->GroupShortId = (uint32_t) ref->groupShortId;
			m_treeRoot->AddChildren(item);
		}
	}
	DeleteSwitchArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateStateGroups()
{
	uint32_t count = (uint32_t) GetStateGroupCount();
	void* arr = (void*)GetAllStateGroupRef();
	for (uint32_t i = 0; i < count; i++)
	{
		const WwiseCRefGroup* ref = GetSwitchGroup(arr, i);
		if (ref)
		{
			WwiseStateGroupPtr item = WwiseStateGroup::Create();
			item->GUID = ref->groupGuid->ToString().String;
			item->Name = ref->groupName;
			item->Path = ref->groupPath;
			item->ShortID = (uint32_t) ref->groupShortId;
			m_treeRoot->AddChildren(item);
		}
	}
	DeleteStateGroupArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateStates()
{
	uint32_t count = (uint32_t) GetStateCount();
	void* arr = (void*)GetAllStateRef();
	for (uint32_t i = 0; i < count; i++)
	{
		const WwiseCRefState* ref = GetState(arr, i);
		if (ref)
		{
			WwiseStatePtr item = WwiseState::Create();
			item->GUID = ref->stateGuid->ToString().String;
			item->Name = ref->stateName;
			item->Path = ref->statePath;
			item->ShortID = (uint32_t) ref->stateShortId;
			item->GroupShortId = (uint32_t) ref->groupShortId;
			m_treeRoot->AddChildren(item);
		}
	}
	DeleteSwitchArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateGameParameters()
{
	uint32_t count = (uint32_t) GetGameParameterCount();
	void* arr = (void*)GetAllGameParameterRef();
	for (uint32_t i = 0; i < count; i++)
	{
		const WwiseCRefGameParameter* ref = GetGameParameter(arr, i);
		if (ref)
		{
			WwiseGameParameterPtr item = WwiseGameParameter::Create();
			item->Name = std::string(GetGameParameterName((void*) ref));
			item->Path = GetGameParameterPath((void*) ref);
			item->ShortID = (uint32_t) GetGameParameterShortId((void*) ref);
			item->GUID = GetGameParameterGuid((void*) ref)->Guid.ToString();
			m_treeRoot->AddChildren(item);
		}
	}
	DeleteGameParameterArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateBusses()
{
	uint32_t count = (uint32_t) GetBusCount();
	void* arr = (void*)GetAllBusRef();
	for (uint32_t i = 0; i < count; i++)
	{
		const WwiseCRefBus* ref = GetBus(arr, i);
		if (ref)
		{
			WwiseBusPtr item = WwiseBus::Create();
			item->GUID = ref->busGuid->ToString().String;
			item->Name = ref->busName;
			item->Path = ref->busPath;
			item->ShortID = (uint32_t) ref->busShortId;
			m_treeRoot->AddChildren(item);
		}
	}
	DeleteBusArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateAuxBusses()
{
	uint32_t count = (uint32_t) GetAuxBusCount();
	void* arr = (void*)GetAllAuxBusRef();
	for (uint32_t i = 0; i < count; i++)
	{
		const WwiseCRefAuxBus* ref = GetAuxBus(arr, i);
		if (ref)
		{
			WwiseAuxBusPtr item = WwiseAuxBus::Create();
			item->GUID = ref->auxBusGuid->ToString().String;
			item->Name = ref->auxBusName;
			item->Path = ref->auxBusPath;
			item->ShortID = (uint32_t) ref->auxBusShortId;
			m_treeRoot->AddChildren(item);
		}
	}
	DeleteAuxBusArrayRef(arr);
}
