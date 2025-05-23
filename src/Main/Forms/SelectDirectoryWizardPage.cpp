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
#include "SelectDirectoryWizardPage.h"

namespace VeraCrypt
{
	bool SelectDirectoryWizardPage::IsValid ()
	{
		if (!DirectoryTextCtrl->IsEmpty())
		{
			return FilesystemPath (DirectoryTextCtrl->GetValue().wc_str()).IsDirectory();
		}

		return false;
	}

	void SelectDirectoryWizardPage::OnBrowseButtonClick (wxCommandEvent& event)
	{
		DirectoryPath dir = Gui->SelectDirectory (this);

		if (!dir.IsEmpty())
			DirectoryTextCtrl->SetValue (wstring (dir));
	}
}
