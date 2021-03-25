// =======================================================================
// FILE BROWSER
#include <lv2/sysfs.h>
#include <sys/dirent.h>
#include <sys/stat.h>
#include "capp.h"
#include "misc.h"




int c_Filebrowser::UpdateTopItem()
{
	if(nSelectedItem > nListMax || nTopItem > 0)
	{
		if(nTopItem < (nSelectedItem - nListMax)) {
			nTopItem = nSelectedItem - nListMax;
		}
		if(nTopItem > 0 && nSelectedItem < nTopItem) {
				nTopItem = nSelectedItem;
		} else {
			nTopItem = nSelectedItem - nListMax;
		}
	} else {
		nTopItem = nSelectedItem - nListMax;
	}
	if(nTopItem < 0) {
		nTopItem = 0;
	}
	return nTopItem;
}

void c_fbaRL::InitFileBrowser()
{
	filebrowser = new c_Filebrowser(23);

	if(fbaRL->nFileBrowserType == 1) {
		strcpy(filebrowser->pszCurrentDir, "/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/"); // <-- left the last slash to make it start in this directory
	} else {
		strcpy(filebrowser->pszCurrentDir, "/dev_hdd0/game/FBNE00123/USRDIR/cores/roms/");
	}

	filebrowser->NavigateDirUp();
}

void c_fbaRL::EndFileBrowser()
{
	SAFE_DELETE(filebrowser);
}

c_Filebrowser::c_Filebrowser(int nListMaximum)
{
	nSelectedItem	= 0;
	nTopItem		= 0;
	nTotalItem		= 0;
	nListMax		= nListMaximum;

	pszCurrentDir = (char*)malloc(2048);
	memset(pszCurrentDir, 0, 2048);
}

c_Filebrowser::~c_Filebrowser()
{
	for(int n = 0; n < nTotalItem; n++)
	{
		SAFE_DELETE(item[n])
	}
	////*&nSelectedItem = NULL;
	////*&nTopItem		= NULL;
	////*&nTotalItem	= NULL;
	////*&nListMax		= NULL;
}

void c_Filebrowser::AddItem(char* szItemLabel, char* szPath, uint8_t _nType)
{
	item[nTotalItem] =  new c_FilebrowserItem(nTotalItem);

	strcpy(item[nTotalItem]->szMenuLabel, szItemLabel);
	strcpy(item[nTotalItem]->szPath, szPath);
	memcpy(&item[nTotalItem]->nType, &_nType, sizeof(uint8_t));

	nTotalItem++;
}

void c_Filebrowser::NavigateSelectedDir()
{
	if(nTotalItem < 1) return;

	////if(item[nSelectedItem]->nType != CELL_FS_TYPE_DIRECTORY) return;
    if(item[nSelectedItem]->nType != DT_DIR) return;


	strcpy(pszCurrentDir, item[nSelectedItem]->szPath);

	CleanUpItems();
	ScanContents();
}

void c_Filebrowser::NavigateDirUp()
{
	if(strlen(pszCurrentDir) == 0)
	{
		if(nTotalItem >= 1) {
			CleanUpItems();
		}

		if(dirExist((char*)"/dev_hdd0")) {
			AddItem((char*)"/dev_hdd0", (char*)"/dev_hdd0", DT_DIR);
		}

		for(int n = 0; n < 30; n++)
		{
			char *szUsb = NULL;
			szUsb = (char*)malloc(256);
			memset(szUsb, 0, 256);

			sprintf(szUsb, "/dev_usb00%d", n);
			if(dirExist(szUsb)) {
				AddItem(szUsb, szUsb, DT_DIR);
			}
			SAFE_FREE(szUsb)
		}

		if(dirExist((char*)"/dev_cf")) {
			AddItem((char*)"/dev_cf", (char*)"/dev_cf", DT_DIR);
		}

		if(dirExist((char*)"/dev_sd")) {
			AddItem((char*)"/dev_sd", (char*)"/dev_sd", DT_DIR);
		}

		if(dirExist((char*)"/dev_ms")) {
			AddItem((char*)"/dev_ms", (char*)"/dev_ms", DT_DIR);
		}

	} else {

		char* pch = strrchr(pszCurrentDir, '/');
		pszCurrentDir[pch-pszCurrentDir] = 0;

		if(strlen(pszCurrentDir) == 0) {
			return NavigateDirUp();
		}

		CleanUpItems();
		ScanContents();
	}
}


void c_Filebrowser::ScanContents()
{
	s32 d;
	//*&d = NULL;

	if(sysFsOpendir(pszCurrentDir, &d) == 0)
	{
		sysFSDirent* dirEntry = NULL;
		dirEntry = (sysFSDirent*)malloc(sizeof(sysFSDirent));
		memset(dirEntry, 0, sizeof(sysFSDirent));

		uint64_t nread = 0;

		// start scanning for directories...
		uint64_t nFilesProcessed = 0;

		while(sysFsReaddir(d, dirEntry, &nread) == 0)
		{
			if(nread == 0) break;
			if(nFilesProcessed > 1000) break; // We are not trying to do magic here...

			if (fbaRL->nFileBrowserType == 0 && dirEntry->d_type == DT_DIR)
			{
				if( strcmp(dirEntry->d_name, ".\0") == 0 ||
					strcmp(dirEntry->d_name, "..\0") == 0
				  )
				{
					continue;
				}

				// DIR
				char* pszTmp = NULL;
				pszTmp = (char*)malloc(2048);
				memset(pszTmp, 0, 2048);

				sprintf(pszTmp, "%s/%s", pszCurrentDir, dirEntry->d_name);

				// ------------------------------
				AddItem(dirEntry->d_name, pszTmp, dirEntry->d_type);
				// ------------------------------

				SAFE_FREE(pszTmp)
			}

			if(fbaRL->nFileBrowserType == 1 && dirEntry->d_type == DT_DIR)
			{
				if( strcmp(dirEntry->d_name, ".\0") == 0 ||
					strcmp(dirEntry->d_name, "..\0") == 0
				  )
				{
					continue;
				}

				// DIR
				char* pszTmp = NULL;
				pszTmp = (char*)malloc(2048);
				memset(pszTmp, 0, 2048);

				sprintf(pszTmp, "%s/%s", pszCurrentDir, dirEntry->d_name);

				// ------------------------------
				AddItem(dirEntry->d_name, pszTmp, dirEntry->d_type);
				// ------------------------------

				SAFE_FREE(pszTmp)
			}

			if(fbaRL->nFileBrowserType == 1 && dirEntry->d_type == DT_REG)
			{
				// FILE

				int nNameLen = strlen(dirEntry->d_name);
				if (nNameLen < 5) continue;

				char* pszFilename = NULL;

				pszFilename = (char*)malloc(nNameLen+1);
				memcpy(pszFilename, dirEntry->d_name, nNameLen);
				pszFilename[nNameLen] = 0;

				// Check file for .CFG
				if(NULL != strstr(toLowerCase(pszFilename, strlen(pszFilename)), ".cfg"))
				{
					char* pszTmp = NULL;
					pszTmp = (char*)malloc(2048);
					memset(pszTmp, 0, 2048);

					sprintf(pszTmp, "%s/%s", pszCurrentDir, dirEntry->d_name);

					// -------------------------------
					AddItem(dirEntry->d_name, pszTmp, dirEntry->d_type);
					// -------------------------------

					SAFE_FREE(pszTmp)
				}

				if(pszFilename) {
					free(pszFilename);
					pszFilename = NULL;
				}
			}
			nFilesProcessed++;
		}
		////SAFE_FREE(dirEntry)

		sysFsClosedir(d);
		//*&d = NULL;
	}
}

void c_Filebrowser::CleanUpItems()
{
	for(int n = 0; n < nTotalItem; n++)
	{
		SAFE_DELETE(item[n])
	}
	nSelectedItem	= 0;
	nTopItem		= 0;
	nTotalItem		= 0;
}

