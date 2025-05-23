/*
 Derived from source code of TrueCrypt 7.1a, which is
 Copyright (c) 2008-2012 TrueCrypt Developers Association and which is governed
 by the TrueCrypt License 3.0.

 Modifications and additions to the original source code (contained in this file)
 and all other portions of this file are Copyright (c) 2013-2025 AM Crypto
 and are governed by the Apache License 2.0 the full text of which is
 contained in the file License.txt included in VeraCrypt binary and source
 code distribution packages.
*/

#include "System.h"
#include "Main/GraphicUserInterface.h"
#include "VolumeFormatOptionsWizardPage.h"
#include <wx/platinfo.h>

namespace VeraCrypt
{
	VolumeFormatOptionsWizardPage::VolumeFormatOptionsWizardPage (wxPanel* parent, uint64 filesystemSize, uint32 sectorSize, bool enableQuickFormatButton, bool disableNoneFilesystem, bool disable32bitFilesystems)
		: VolumeFormatOptionsWizardPageBase (parent)
	{
		InfoStaticText->SetLabel ( LangString["QUICK_FORMAT_HELP"] );
		if (!disableNoneFilesystem)
			FilesystemTypeChoice->Append (LangString["NONE"],	(void *) VolumeCreationOptions::FilesystemType::None);

		if (!disable32bitFilesystems && filesystemSize <= TC_MAX_FAT_SECTOR_COUNT * sectorSize)
			FilesystemTypeChoice->Append (L"FAT",			(void *) VolumeCreationOptions::FilesystemType::FAT);

#ifdef TC_WINDOWS
		FilesystemTypeChoice->Append (L"NTFS",				(void *) VolumeCreationOptions::FilesystemType::NTFS);
		FilesystemTypeChoice->Append (L"exFAT",				(void *) VolumeCreationOptions::FilesystemType::exFAT);
#elif defined (TC_LINUX)
		FilesystemTypeChoice->Append (L"Linux Ext2",		(void *) VolumeCreationOptions::FilesystemType::Ext2);
		FilesystemTypeChoice->Append (L"Linux Ext3",		(void *) VolumeCreationOptions::FilesystemType::Ext3);
		if (VolumeCreationOptions::FilesystemType::IsFsFormatterPresent (VolumeCreationOptions::FilesystemType::Ext4))
			FilesystemTypeChoice->Append (L"Linux Ext4",		(void *) VolumeCreationOptions::FilesystemType::Ext4);		
		if (VolumeCreationOptions::FilesystemType::IsFsFormatterPresent (VolumeCreationOptions::FilesystemType::NTFS))
			FilesystemTypeChoice->Append (L"NTFS",				(void *) VolumeCreationOptions::FilesystemType::NTFS);
		if (VolumeCreationOptions::FilesystemType::IsFsFormatterPresent (VolumeCreationOptions::FilesystemType::exFAT))
			FilesystemTypeChoice->Append (L"exFAT",				(void *) VolumeCreationOptions::FilesystemType::exFAT);
		if (VolumeCreationOptions::FilesystemType::IsFsFormatterPresent (VolumeCreationOptions::FilesystemType::Btrfs))
		{
			// minimum size to be able to format as Btrfs is 16777216 bytes
			if (filesystemSize >= VC_MIN_SMALL_BTRFS_VOLUME_SIZE)
				FilesystemTypeChoice->Append (L"Btrfs",				(void *) VolumeCreationOptions::FilesystemType::Btrfs);
		}
#elif defined (TC_MACOSX)
		FilesystemTypeChoice->Append (L"Mac OS Extended",	(void *) VolumeCreationOptions::FilesystemType::MacOsExt);
		FilesystemTypeChoice->Append (L"exFAT",				(void *) VolumeCreationOptions::FilesystemType::exFAT);
		if (wxPlatformInfo::Get().CheckOSVersion (10, 13))
			FilesystemTypeChoice->Append (L"APFS",			(void *) VolumeCreationOptions::FilesystemType::APFS);
#elif defined (TC_FREEBSD) || defined (TC_SOLARIS)
		FilesystemTypeChoice->Append (L"UFS",				(void *) VolumeCreationOptions::FilesystemType::UFS);
#endif

		if (!disable32bitFilesystems && filesystemSize <= TC_MAX_FAT_SECTOR_COUNT * sectorSize)
			SetFilesystemType (VolumeCreationOptions::FilesystemType::FAT);
		else
			SetFilesystemType (VolumeCreationOptions::FilesystemType::GetPlatformNative());

		QuickFormatCheckBox->Enable (enableQuickFormatButton);
	}

	VolumeCreationOptions::FilesystemType::Enum VolumeFormatOptionsWizardPage::GetFilesystemType () const
	{
		return (VolumeCreationOptions::FilesystemType::Enum) reinterpret_cast <unsigned long long> (Gui->GetSelectedData <void> (FilesystemTypeChoice));
	}

	void VolumeFormatOptionsWizardPage::OnFilesystemTypeSelected (wxCommandEvent& event)
	{
	}

	void VolumeFormatOptionsWizardPage::OnQuickFormatCheckBoxClick (wxCommandEvent& event)
	{
		if (event.IsChecked())
		{
			QuickFormatCheckBox->SetValue (Gui->AskYesNo (LangString["WARN_QUICK_FORMAT"], false, true));
		}
	}

	void VolumeFormatOptionsWizardPage::SetFilesystemType (VolumeCreationOptions::FilesystemType::Enum type)
	{
		switch (type)
		{
		case VolumeCreationOptions::FilesystemType::None:		FilesystemTypeChoice->SetStringSelection (LangString["NONE"]); break;
		case VolumeCreationOptions::FilesystemType::FAT:		FilesystemTypeChoice->SetStringSelection (L"FAT"); break;
		case VolumeCreationOptions::FilesystemType::exFAT:		FilesystemTypeChoice->SetStringSelection (L"exFAT"); break;
		case VolumeCreationOptions::FilesystemType::NTFS:		FilesystemTypeChoice->SetStringSelection (L"NTFS"); break;
		case VolumeCreationOptions::FilesystemType::Ext2:		FilesystemTypeChoice->SetStringSelection (L"Linux Ext2"); break;
		case VolumeCreationOptions::FilesystemType::Ext3:		FilesystemTypeChoice->SetStringSelection (L"Linux Ext3"); break;
		case VolumeCreationOptions::FilesystemType::Ext4:		FilesystemTypeChoice->SetStringSelection (L"Linux Ext4"); break;
		case VolumeCreationOptions::FilesystemType::Btrfs:		FilesystemTypeChoice->SetStringSelection (L"Btrfs"); break;
		case VolumeCreationOptions::FilesystemType::MacOsExt:	FilesystemTypeChoice->SetStringSelection (L"Mac OS Extended"); break;
		case VolumeCreationOptions::FilesystemType::APFS:		FilesystemTypeChoice->SetStringSelection (L"APFS"); break;
		case VolumeCreationOptions::FilesystemType::UFS:		FilesystemTypeChoice->SetStringSelection (L"UFS"); break;

		default:
			throw ParameterIncorrect (SRC_POS);
		}
	}
}
