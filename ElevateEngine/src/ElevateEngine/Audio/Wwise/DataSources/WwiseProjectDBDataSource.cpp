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
	unsigned int count = GetSoundBankCount();
	void* arr = (void*) GetAllSoundBanksRef();
	for (int i = 0; i < count; i++)
	{
		const ::WwiseCRefSoundBank* ref = GetSoundBankRefIndex(arr, i);
		if (ref)
		{
			WwiseSoundbankPtr item = WwiseSoundbank::Create();
			item->IsInitBank = ref->bIsInitBank;
			item->IsUserBank = ref->bIsUserBank;
			item->IsValid = ref->bIsValid;
			item->GUID = ref->guid->ToString().String;
			item->ShortID = ref->shortId;
			item->Language = ref->language;
			item->Name = ref->name;
			item->Path = ref->path;
			item->ObjectPath = ref->objectPath;

			PopulateEvents(ref);
		}
	}
	//DeleteSoundBanksArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateEvents(const WwiseCRefSoundBank* bnkRef)
{
	unsigned int count = bnkRef->eventsCount;
	void* arr = (void*) bnkRef->events;
	for (int i = 0; i < count; i++)
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
			item->ShortID = ref->shortId;

			// Linking with the soundbank ref
			item->SoundbankShortId = bnkRef->shortId;

			m_treeRoot->AddChildren(item);
		}
	}
	//DeleteEventArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateSwitchGroups()
{
	unsigned int count = GetSwitchGroupCount();
	void* arr = (void*) GetAllSwitchGroupRef();
	for (int i = 0; i < count; i++)
	{
		const WwiseCRefGroup* ref = GetSwitchGroup(arr, i);
		if (ref)
		{ 
			WwiseSwitchGroupPtr item = WwiseSwitchGroup::Create();
			item->GUID = ref->groupGuid->ToString().String;
			item->Name = ref->groupName;
			item->Path = ref->groupPath;
			item->ShortID = ref->groupShortId;
		}
	}
	//DeleteSwitchGroupArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateSwitches()
{
	unsigned int count = GetSwitchCount();
	void* arr = (void*)GetAllSwitchRef();
	for (int i = 0; i < count; i++)
	{
		const WwiseCRefSwitch* ref = GetSwitch(arr, i);
		if (ref)
		{
			WwiseSwitchPtr item = WwiseSwitch::Create();
			item->GUID = ref->switchGuid->ToString().String;
			item->Name = ref->switchName;
			item->Path = ref->switchPath;
			item->ShortID = ref->switchShortId;
			item->GroupShortId = ref->groupShortId;
		}
	}
	//DeleteSwitchArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateStateGroups()
{
	unsigned int count = GetStateGroupCount();
	void* arr = (void*)GetAllStateGroupRef();
	for (int i = 0; i < count; i++)
	{
		const WwiseCRefGroup* ref = GetSwitchGroup(arr, i);
		if (ref)
		{
			WwiseStateGroupPtr item = WwiseStateGroup::Create();
			item->GUID = ref->groupGuid->ToString().String;
			item->Name = ref->groupName;
			item->Path = ref->groupPath;
			item->ShortID = ref->groupShortId;
		}
	}
	//DeleteStateGroupArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateStates()
{
	unsigned int count = GetStateCount();
	void* arr = (void*)GetAllStateRef();
	for (int i = 0; i < count; i++)
	{
		const WwiseCRefState* ref = GetState(arr, i);
		if (ref)
		{
			WwiseStatePtr item = WwiseState::Create();
			item->GUID = ref->stateGuid->ToString().String;
			item->Name = ref->stateName;
			item->Path = ref->statePath;
			item->ShortID = ref->stateShortId;
			item->GroupShortId = ref->groupShortId;
		}
	}
	//DeleteSwitchArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateGameParameters()
{
	unsigned int count = GetGameParameterCount();
	void* arr = (void*)GetAllGameParameterRef();
	for (int i = 0; i < count; i++)
	{
		const WwiseCRefGameParameter* ref = GetGameParameter(arr, i);
		if (ref)
		{
			WwiseStateGroupPtr item = WwiseStateGroup::Create();
			item->GUID = ref->gameParameterGuid->ToString().String;
			item->Name = ref->gameParameterName;
			item->Path = ref->gameParameterPath;
			item->ShortID = ref->gameParameterShortId;
			DeleteGameParameterRef((void*)ref);
		}
	}
	//DeleteGameParameterArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateBusses()
{
	unsigned int count = GetBusCount();
	void* arr = (void*)GetAllBusRef();
	for (int i = 0; i < count; i++)
	{
		const WwiseCRefBus* ref = GetBus(arr, i);
		if (ref)
		{
			WwiseBusPtr item = WwiseBus::Create();
			item->GUID = ref->busGuid->ToString().String;
			item->Name = ref->busName;
			item->Path = ref->busPath;
			item->ShortID = ref->busShortId;
			DeleteBusRef((void*)ref);
		}
	}
	//DeleteBusArrayRef(arr);
}

void Elevate::WwiseProjectDBDataSource::PopulateAuxBusses()
{
	unsigned int count = GetAuxBusCount();
	void* arr = (void*)GetAllAuxBusRef();
	for (int i = 0; i < count; i++)
	{
		const WwiseCRefAuxBus* ref = GetAuxBus(arr, i);
		if (ref)
		{
			WwiseAuxBusPtr item = WwiseAuxBus::Create();
			item->GUID = ref->auxBusGuid->ToString().String;
			item->Name = ref->auxBusName;
			item->Path = ref->auxBusPath;
			item->ShortID = ref->auxBusShortId;
		}
	}
	//DeleteAuxBusArrayRef(arr);
}
