#ifndef FBA_RL_H
#define FBA_RL_H
// ---------------------------------------------------------------------
// FB Neo Retro Loader Plus (CaptainCPS-X, 2013) - (CrystalCT, 2020)
// ---------------------------------------------------------------------


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <pngdec/pngdec.h>


//#include <algorithm>
#include <rsx/rsx.h>

// --------------------------------------------------------------------
//#define _APP_VER "2.13"
#define _APP_TITLE "\tFB NEO PLUS RETRO LOADER " _APP_VER " - by CaptainCPS-X [2013] CrystalCT [2020]"
// --------------------------------------------------------------------

#define MAX_WIDTH	1920
#define MAX_HEIGHT	1080
#define PITCH		(MAX_WIDTH * 4)
#define MAXQUADXTEX 9


/*struct FBA_DRV
{
	uint32_t nDrv;
	char szName[128];
	char szParent[32];
	char szBoardROM[32];
	char szTitle[256];
	char szYear[8];
	char szCompany[256];
	char szSystem[256];
	uint32_t nMaxPlayers;
	uint32_t nWidth;
	uint32_t nHeight;
	uint32_t nAspectX;
	uint32_t nAspectY;
	char szSystemFilter[32];
	bool isClone;
};*/

//extern FBA_DRV fba_drv[3183];

extern unsigned char	example_cfg[20570];		// For creating basic input presets
void _ScanROMs(uint64_t);

#define STATUS_NORMAL			0
#define STATUS_SHOW_ZIPINFO		1
#define STATUS_UPDATEPREVIEW	2
#define STATUS_RESETPREVIEW		3
#define STATUS_EXIT				4
#define STATUS_EXIT_FBARL_RA	5
#define STATUS_EXIT_RA			6
#define STATUS_EXIT_MMAN		7
#define STATUS_EXIT_IMANAGER	8

#define STATUS_ROMSCAN_DLG		20
#define STATUS_MISSING_GAME_DLG 21
#define STATUS_CORRUPT_APP_DLG	22

#define STATUS_ROMSCAN_START	30
#define STATUS_ROMSCAN_WORKING	33
#define STATUS_ROMSCAN_END		31
#define STATUS_ROMPATH_CHANGED	32

#define STATUS_MISSING_CORE_2   40
#define STATUS_MISSING_CORE_4   41
#define STATUS_MISSING_CORE_5   42

#define SECTION_MAIN			TEX_MAIN_MENU		
#define SECTION_GAMELIST		TEX_GAME_LIST		
#define SECTION_ZIPINFO			TEX_ZIP_INFO		
#define SECTION_ROMINFO			TEX_ROM_INFO		
#define SECTION_OPTIONS			TEX_OPTIONS			
#define SECTION_FILEBROWSER		TEX_FILEBROWSER		
#define SECTION_HELP			6			

#define MENU_MAIN_GAMELIST		0
#define MENU_MAIN_OPTIONS		1
#define MENU_MAIN_FBARL_RA		2
#define MENU_MAIN_IMANAGER		3
#define MENU_MAIN_MMAN			4
#define MENU_MAIN_EXIT			5

// Filter system
#define MASKCAPMISC		0
#define MASKCAVE		1
#define MASKCPS			2
#define	MASKCPS2		3
#define MASKCPS3		4
#define MASKDATAEAST	5
#define MASKGALAXIAN	6
#define MASKIREM		7
#define MASKKANEKO		8
#define MASKKONAMI		9
#define MASKNEOGEO		10
#define MASKPACMAN		11
#define MASKPGM			12
#define MASKPSIKYO		13
#define MASKSEGA		14
#define MASKSEG32		15
#define MASKSETA		16
#define MASKTAITO		17
#define MASKTECHNOS		18
#define MASKTOAPLAN		19
#define MASKMISCPRE90S	20
#define MASKMISCPOST90S 21
#define MASKMIDWAY		22
#define MASKSNES		23
#define MASKMEGADRIVE	24
#define MASKAMIGA       25
#define MASKCOLECO		26
#define MASKTG16		27
#define MASKPCE			28
#define MASKSGX			29
#define MASKFAVORITE	30


#define MASKALL			31
#define MASKCUSTOM		32


#define MENU_OPT_AUTO_CFG		0
#define MENU_OPT_MUSIC  		1
#define MENU_OPT_RETROARCH_MENU 2
#define MENU_OPT_DISP_CLONES	3
#define MENU_OPT_USE_UNIBIOS	4
#define MENU_OPT_DISP_MISS_GMS	5
#define MENU_OPT_MD_DEF_CORE	6
#define MENU_OPT_FILTER_START	(MENU_OPT_MD_DEF_CORE+1)

#define RETROARCH_MENU_GLUI_S "GLUI"
#define RETROARCH_MENU_GLUI_N 0
#define RETROARCH_MENU_OZONE_S "OZONE"
#define RETROARCH_MENU_OZONE_N 1
#define RETROARCH_MENU_RGUI_S "RGUI"
#define RETROARCH_MENU_RGUI_N 2
#define RETROARCH_MENU_XMB_S "XMB"
#define RETROARCH_MENU_XMB_N 3


#define SAFE_FREE(x)	if(x) {	free(x); *&x = NULL; }
#define SAFE_DELETE(x)	if(x) { delete x; *&x = NULL; }

#define MAX_GAMES	20000 // <-- should be enough xD
#define TOTAL_DRV_GAMES 16919

class c_game
{
public:

	//char*		path;		// ROM Path
	char*		zipname;	// ZIP name
	char*		title;		// Title, if found
	char*       name;
	char*       parent_name;
	char*		sysmask;	// System Mask / Filter
	char*       company;
	char*       year;
	char*       system;
	char*       subsystem;
	char*       resolution;
	char*       aspectratio;
    uint32_t    players;
	uint32_t	nGame;
//	uint32_t	parent_id;
	uint32_t	GameID;
	uint32_t	nSize;		// size in bytes
	bool		bAvailable; //
	bool 		isClone;
	bool        isFavorite;
	uint32_t    def_core_id; //have a default core if > 0
	uint16_t    core_id;

	c_game(uint32_t _nGame)
	{

//		path	= (char*)malloc(1024);
//		if (path == NULL)
//            printf("MEMORY ERROR\n");
		zipname = (char*)malloc(64);
		if (zipname == NULL)
            printf("MEMORY ERROR\n");
		title = NULL;
		//title	= (char*)malloc(256);
		//if (title == NULL)
          //  printf("MEMORY ERROR\n");
		name    = (char*)malloc(128);
		if (name == NULL)
            printf("MEMORY ERROR\n");

        parent_name  = (char*)malloc(128);
		if (parent_name == NULL)
            printf("MEMORY ERROR\n");
		sysmask = (char*)malloc(32);
		if (sysmask == NULL)
            printf("MEMORY ERROR\n");
        company = (char*)malloc(256);
		if (company == NULL)
            printf("MEMORY ERROR\n");
        year = (char*)malloc(8);
		if (year == NULL)
            printf("MEMORY ERROR\n");
        system = (char*)malloc(256);
		if (system == NULL)
            printf("MEMORY ERROR\n");
        subsystem = (char*)malloc(32);
		if (subsystem == NULL)
            printf("MEMORY ERROR\n");
        resolution = (char*)malloc(256);
		if (resolution == NULL)
            printf("MEMORY ERROR\n");
        aspectratio = (char*)malloc(8);
		if (aspectratio == NULL)
            printf("MEMORY ERROR\n");

		//memset(path, 0, 1024);
		memset(zipname, 0, 64);
		//memset(title, 0, 256);
		memset(name, 0, 128);
		memset(parent_name, 0, 128);
		memset(sysmask, 0, 32);
		memset(company, 0, 256);
		memset(year, 0, 5);
		memset(system, 0, 256);
		memset(subsystem, 0, 32);
		memset(resolution, 0, 10);
        memset(aspectratio, 0, 7);
		nGame		= _nGame;
		GameID      = _nGame;
		nSize		= 0;
		bAvailable	= false;
	}

	~c_game()
	{
		//SAFE_FREE(path)
		SAFE_FREE(zipname)
		//SAFE_FREE(title)
		SAFE_FREE(name)
		SAFE_FREE(parent_name)
		SAFE_FREE(sysmask)
		SAFE_FREE(company)
		SAFE_FREE(year)
		SAFE_FREE(system)
		SAFE_FREE(subsystem)
		SAFE_FREE(resolution)
		SAFE_FREE(aspectratio)
		////*&nGame = NULL;
		////*&nSize = NULL;
		////*&bAvailable = NULL;
	}
};
class c_MenuItem
{
public:
	int		nIndex;
	char*	szMenuLabel;

	c_MenuItem(int nId)
	{
		szMenuLabel = (char*)malloc(1024);
		memset(szMenuLabel, 0, 1024);

		nIndex		= nId;
	}

	~c_MenuItem()
	{
		SAFE_FREE(szMenuLabel)
		////*&nIndex = NULL;
	}
};

class c_Menu
{
public:
	int nSelectedItem;
	int nTopItem;
	int nTotalItem;
	int nListMax;
	c_MenuItem* item[2048];

	c_Menu(int nListMaximum)
	{
		nSelectedItem	= 0;
		nTopItem		= 0;
		nTotalItem		= 0;
		nListMax		= nListMaximum;
	}

	~c_Menu()
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

	void AddItem(char* szItemLabel)
	{
		item[nTotalItem] =  new c_MenuItem(nTotalItem);
		strcpy(item[nTotalItem]->szMenuLabel, szItemLabel);
		nTotalItem++;
	}

	int UpdateTopItem()
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
};

class c_FilebrowserItem
{
public:
	int		nIndex;
	char*	szMenuLabel;
	char*	szPath;
	uint8_t	nType;

	c_FilebrowserItem(int nId)
	{
		szMenuLabel = (char*)malloc(2048);
		szPath = (char*)malloc(2048);

		memset(szMenuLabel, 0, 2048);
		memset(szPath, 0, 2048);

		nIndex		= nId;
		nType		= 0; ////CELL_FS_TYPE_UNKNOWN;
	}

	~c_FilebrowserItem()
	{
		SAFE_FREE(szPath);
		SAFE_FREE(szMenuLabel)
		////*&nIndex = NULL;
		////*&nType = NULL;
	}
};

class c_Filebrowser
{
public:
	int nSelectedItem;
	int nTopItem;
	int nTotalItem;
	int nListMax;
	c_FilebrowserItem* item[2048];

	char* pszCurrentDir;

	c_Filebrowser(int nListMaximum);

	~c_Filebrowser();

	void AddItem(char* szItemLabel, char* szPath, uint8_t _nType);

	void NavigateSelectedDir();

	void NavigateDirUp();

	void ScanContents();

	void CleanUpItems();

	int UpdateTopItem();
};


class c_fbaRL
{
public:

	c_fbaRL(bool StartWithGamesList);

	c_Menu*		main_menu;
	c_Menu*		zipinfo_menu;
	c_Menu*		options_menu;

	c_Filebrowser* filebrowser;
	int			nFileBrowserType;
	void		InitFileBrowser();
	void		EndFileBrowser();

	void		CreateAllInputCFG();

	//c_thread	*thread_ScanRoms;

	bool		bProcessingGames;

	int			nTotalGames;
	//int			nMissingGames;

	c_game**	games;

	int			nSelectedGame;
	int			nGameListTop;
	int			nFilteredGames;
	c_game**	fgames;

	int			nStatus;
	int			nSection;

	int			nBurnSelected;
	//rsxBuffer*  rsxbuffer;
    //pngData*    png;
    uint32_t    hashmap_position;

	void		LaunchFBACore(char* arg1, char* arg2, char* arg3, char* arg4, char* arg5, char* arg6 );  //CRYSTAL

	int			SaveGameListCache();
	int			ParseGameListCache();

	void		InitFilterList();
	void		EndFilterList();
	void		NextSysFilter();
	void		PrevSysFilter();
	bool		FilterGame(char* szMask);

	int			GetSystemMaskId(char* szMask);
	char*		GetSystemFilter(int nFilter);

	void		RenderBackground();
	void		ResetPreviewImage();
	void		UpdatePreviewImage();

	void		MakeRetroDirs(char* szMainPath);

	void		InitMainMenu();
	void		EndMainMenu();

	void		InitZipInfoMenu();
	void		EndZipInfoMenu();

	void		InitGameList();
	void		EndGameList();

	void		InitOptionsMenu();
	void		EndOptionsMenu();

	uint32_t	GetBurnDriverID(char* szRomzip);
	void		UpdateBurnSelected(char* szROM);
	bool		IsBurnDuplicate(char* szROM);

	void		RefreshGameList();

	int			ParseGameList();

	void		ParseDirectory();
	void		FileBrowserFrame();

	//void		Frame();
	int			nFrameStep;
	void		DisplayFrame();

	void		MainMenu_Frame();
	void		GameList_Frame();
	void		Options_Frame();
	void		FileBrowser_Frame();
	void		ZipInfo_Frame();

	c_Menu*		rominfo_menu;
	void		InitRomInfoMenu();
	void		EndRomInfoMenu();
	void		RomInfo_Frame();
	//void        RomScan2();

    void        MakeAmigaCFG(char *romPath, char *subsystem, char** argv);

	void		InputFrame();
	void		DlgDisplayFrame();
	void        DrawIMG(int x, int y, pngData *png);
	void        ScaleLine(u32 *Target, u32 *Source, u32 SrcWidth, u32 TgtWidth);
	int         ResizeImage(pngData *png_in, void *TgtTexture, u32 TgtWidth, u32 TgtHeight);


private:
	// ...
};

extern c_fbaRL* fbaRL;

#endif
