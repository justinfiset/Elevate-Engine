/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

  Copyright (c) 2025 Audiokinetic Inc.
*******************************************************************************/
//////////////////////////////////////////////////////////////////////
//
// AkMultipleFileLocation.h
//
// File location resolving: Supports multiple base paths for file access, searched in reverse order.
// For more details on resolving file location, refer to section "File Location" inside
// "Going Further > Overriding Managers > Streaming / Stream Manager > Low-Level I/O"
// of the SDK documentation. 
//
//////////////////////////////////////////////////////////////////////

#ifndef _AK_MULTI_FILE_LOCATION_H_
#define _AK_MULTI_FILE_LOCATION_H_

struct AkFileSystemFlags;

#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/Tools/Common/AkListBareLight.h>


// This file location class supports multiple base paths for Wwise file access.
// Each path will be searched the reverse order of the addition order until the file is found.
class CAkMultipleFileLocation
{
protected:

	// Internal user paths.
	struct FilePath
	{
		FilePath *pNextLightItem;
		AkOSChar szPath[1];	//Variable length
	};
public:
	CAkMultipleFileLocation();
	void Term();

	//
	// Global path functions.
	// ------------------------------------------------------

	// Base path is prepended to all file names.
	// Audio source path is appended to base path whenever uCompanyID is AK and uCodecID specifies an audio source.
	// Bank path is appended to base path whenever uCompanyID is AK and uCodecID specifies a sound bank.
	// Language specific dir name is appended to path whenever "bIsLanguageSpecific" is true.
	AKRESULT SetBasePath(const AkOSChar*   in_pszBasePath)
	{
		return AddBasePath(in_pszBasePath);
	}

	AKRESULT AddBasePath(const AkOSChar*   in_pszBasePath);

	void SetUseSubfoldering(bool bUseSubfoldering) { m_bUseSubfoldering = bUseSubfoldering; }

	AKRESULT Open(
		const AkFileOpenData& in_OpenData,		///< File open information (name, flags, etc)
		bool			in_bOverlapped,			// Overlapped IO open
		AkFileDesc& out_FileDesc				///< File descriptor to fill out when successfully opened
	);

	//
	// Path resolving services.
	// ------------------------------------------------------

	// Returns 
	// AK_Success if input flags are supported and the resulting path is not too long.
	// AK_InvalidParameter if the path doesn't end with a file separator or if pFlags is not specified when in_FileOpen.fileID is specified.
	// AK_FilePathTooLong if the path exceeds AK_MAX_PATH
	AKRESULT GetFullFilePath(
		const AkFileOpenData& in_FileOpen,		// File name or file ID (only one should be valid!), open flags, open mode
		AkOSChar *			out_pszFullFilePath, // Full file path.
		const AkOSChar*		in_pszBasePath = NULL	// Base path to use.  If null, the first suitable location will be given.		
		);  

	AKRESULT OutputSearchedPaths(
		const AkFileOpenData& in_FileOpen,		// File name or file ID (only one should be valid!), open flags, open mode
		AkOSChar* out_searchedPath,				///< String containing all searched paths
		AkInt32 in_pathSize						///< The maximum size of the string
	);

protected:
	void ClearLocations();

	// Overridable per-platform open file operation
	// By default, invokes CAkFileHelpers::Open
	virtual AKRESULT PlatformOpenFile(
		const AkOSChar* in_pszFullFilePath, // Full file path, as resolved by GetFullFilePath().
		AkOpenMode      in_eOpenMode,       // Open mode.
		bool			in_bOverlapped,		// Overlapped IO
		AkFileDesc &    out_fileDesc		// File descriptor
		);
	
	// Overridable per-platform path normalization
	// For platforms supporting multiple directory separators, this function converts mixed separators in the the OS-native one.
	// By default, invokes CAkFileHelpers::NormalizeDirectorySeparators
	virtual void PlatformNormalizeDirectorySeparators(AkOSChar* io_pszFullFilePath);

	// Overridable per-platform directory existence check
	// Returns:
	// - AK_Success if directory exists
	// - AK_PathNotFound if directory does not exist
	// - AK_NotImplemented if operation is not supported by the platform.
	// By default, invokes CAkFileHelpers::CheckDirectoryExists
	virtual AKRESULT PlatformCheckDirectoryExists(const AkOSChar* io_pszBasePath);

	AkListBareLight<FilePath> m_Locations;
	bool m_bUseSubfoldering; // If true, the file resolver will assume auto-generated banks and loose/streamed WEM files are organized in sub-folders
};

#endif //_AK_MULTI_FILE_LOCATION_H_
