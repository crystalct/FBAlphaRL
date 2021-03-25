// =======================================================================
// DISPLAY SECTIONS (FRAME)

#include <unistd.h>
#include "capp.h"
#include <rsx/rsx.h>
#include <ppu-lv2.h>
#include <lv2/memory.h>
#include "rsxutil.h"
#include <bdffnt.h>

extern smeminfo meminfo;

#define NSHADOWXYPOS2   0.00150f

//COLORS R,G,B,A
#define SHADOWCOLOR		0.02f, 0.02f, 0.02f, 1.0f
#define YELLOWCOLOR		1.0f, 1.0f, 0.0f, 1.0f
#define WHITECOLOR		1.0f, 1.0f, 1.0f, 1.0f
#define LIGHTBLUECOLOR  0.0f, 0.8f, 1.0f, 1.8f
#define REDCOLOR		1.0f, 0.0f, 0.0f, 1.0f
#define GRAYCOLOR		0.87f, 0.87f, 0.87f , 1.0f
#define GRAYCOLOR2		0.7f, 0.7f, 0.7f , 1.0f

#define NGAMELISTMAX    27
#define DRVMAP          ((hashmap_map *) app.drvMap)
#define GAMESMAP        ((hashmap_map *) app.gamesMap)
#define FBADRV          ((FBA_DRV *)DRVMAP->data[hashmap_position].data)
#define FBAGAMES        ((FBA_GAMES *)GAMESMAP->data[hashmap_position].data)

extern u32* texture_buffer;
extern u32 texture_offset;
extern u32 myLabelValue;
extern vu32* mLabel;

static uint32_t game_ord_id;
static float xPos;
static float yPos;
static float yPosDiff;

static char txt[288];
extern char ipaddress[32];
extern fnt_class alpha;

void c_fbaRL::DisplayFrame()
{
	if(nSection == SECTION_FILEBROWSER)	{
		FileBrowser_Frame();
	}
	if(nSection == SECTION_OPTIONS && !bProcessingGames) {
		Options_Frame();
	}
	if(nSection == SECTION_GAMELIST && !bProcessingGames) {
		GameList_Frame();
	}
	if(nSection == SECTION_ZIPINFO)	{
		ZipInfo_Frame();
	}
	if(nSection == SECTION_ROMINFO)	{
		RomInfo_Frame();
	}
	if(nSection == SECTION_MAIN) {
		MainMenu_Frame();
	}
}

void c_fbaRL::MainMenu_Frame()
{
	xPos		= 0.2600f;
	yPos		= 0.5885f;
	yPosDiff	= 0.0400f;
	
	switch (app.state.displayMode.resolution)
	{
		case VIDEO_RESOLUTION_1080:
			SetCurrentFont_fnt(&alpha, 1);
			setDimension_fnt(&alpha, alpha.fnt[1].maxwidth, alpha.fnt[1].height);
			break;
		case VIDEO_RESOLUTION_720:
			SetCurrentFont_fnt(&alpha, 2);
			setDimension_fnt(&alpha, alpha.fnt[2].maxwidth, alpha.fnt[2].height);
			break;
		case VIDEO_RESOLUTION_576:
		case VIDEO_RESOLUTION_480:
			SetCurrentFont_fnt(&alpha, 3);
			setDimension_fnt(&alpha, alpha.fnt[3].maxwidth, alpha.fnt[3].height);
			break;
	}

	if (nFrameStep == 0) 
	{
			setColor_fnt(&alpha, SHADOWCOLOR);
			setPosition_fnt(&alpha, (int)(0.216 * display_width) + 2, (int)(0.135 * display_height) + 2);
	}
	else
	{
			setColor_fnt(&alpha, WHITECOLOR);
			setPosition_fnt(&alpha, (int)(0.216 * display_width), (int)(0.135 * display_height));
	}

	snprintf(txt,sizeof(txt),"ver: %.2f (ip: %s)", _APP_VER, ipaddress);
	printf_fnt(&alpha, txt);
	
	yPos = 0.2050f;
	xPos = 0.0600f;
	yPosDiff = 0.0804f;
	
	int nMenuItem = 0;
	
	switch (app.state.displayMode.resolution)
	{
	case VIDEO_RESOLUTION_1080:
		SetCurrentFont_fnt(&alpha, 0);
		setDimension_fnt(&alpha, alpha.fnt[0].maxwidth, alpha.fnt[0].height);
		break;
	case VIDEO_RESOLUTION_720:
		SetCurrentFont_fnt(&alpha, 1);
		setDimension_fnt(&alpha, alpha.fnt[1].maxwidth, alpha.fnt[1].height);
		break;
	case VIDEO_RESOLUTION_576:
	case VIDEO_RESOLUTION_480:
		SetCurrentFont_fnt(&alpha, 2);
		setDimension_fnt(&alpha, alpha.fnt[2].maxwidth, alpha.fnt[2].height);
		break;
	}
	
	while(nMenuItem < main_menu->nTotalItem)
	{
		if (nFrameStep == 0) {
			setColor_fnt(&alpha, SHADOWCOLOR);
			setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(int)(yPos * display_height) + 2);
		}
		else
		{
			if (nMenuItem == main_menu->nSelectedItem) {  // selected
				setColor_fnt(&alpha, YELLOWCOLOR);
			}
			else
			{
				setColor_fnt(&alpha, WHITECOLOR);
			}
			setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(int)(yPos * display_height));
		}

		printf_fnt(&alpha, main_menu->item[nMenuItem]->szMenuLabel);
		
		yPos += yPosDiff;

		nMenuItem++;
	}
	
}

void c_fbaRL::GameList_Frame()
{

	u32 box_title_len = 0, box_games_len = 0, box_filters_len = 0;
	xPos = 0.3260;
	yPos = 0.2010;
	yPosDiff	= 0.025f;

	switch (app.state.displayMode.resolution)
	{
	case VIDEO_RESOLUTION_1080:
		SetCurrentFont_fnt(&alpha, 1);
		setDimension_fnt(&alpha, alpha.fnt[1].maxwidth, alpha.fnt[1].height);
		box_title_len = 66;
		box_games_len = 98;
		box_filters_len = 80;
		break;
	case VIDEO_RESOLUTION_720:
		SetCurrentFont_fnt(&alpha, 2);
		setDimension_fnt(&alpha, alpha.fnt[2].maxwidth, alpha.fnt[2].height);
		box_title_len = 64;
		box_games_len = 95;
		box_filters_len = 73;
		break;
	case VIDEO_RESOLUTION_576:
	case VIDEO_RESOLUTION_480:
		box_title_len = 46;
		box_games_len = 69;
		box_filters_len = 55;
		SetCurrentFont_fnt(&alpha, 3);
		setDimension_fnt(&alpha, alpha.fnt[3].maxwidth, alpha.fnt[3].height);
		break;
	}
	
    game_ord_id = 0;

    if(nFilteredGames >= 1)
	{
		game_ord_id = fgames[nSelectedGame]->GameID;
		if(nSelectedGame > NGAMELISTMAX || nGameListTop > 0)
		{
			if(nGameListTop < (nSelectedGame - NGAMELISTMAX)){
				nGameListTop = nSelectedGame - NGAMELISTMAX;
			}
			if((nGameListTop > 0) && (nSelectedGame < nGameListTop)) {
				nGameListTop = nSelectedGame;
			} else {
				nGameListTop = nSelectedGame - NGAMELISTMAX;
			}
		} else {
			nGameListTop = nSelectedGame - NGAMELISTMAX;
		}
		if(nGameListTop < 0) nGameListTop = 0;

		int nGame = nGameListTop;

		
		while(nGame <= (nGameListTop + NGAMELISTMAX))
		{
			if(nFilteredGames < 1) break;
			if(nGame == nFilteredGames) break;

			

			if (nFrameStep == 0) {
				setColor_fnt(&alpha, SHADOWCOLOR);
				setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(int)(yPos * display_height) + 2);
			}
			else
			{
				if (nGame == nSelectedGame) {  // selected
					setColor_fnt(&alpha, YELLOWCOLOR);
				}
				else
				{	// missing
					if (!fgames[nGame]->bAvailable) {
						setColor_fnt(&alpha, REDCOLOR);
					}
					else
						if (fgames[nGame]->isFavorite) // favorite
						{
							setColor_fnt(&alpha, LIGHTBLUECOLOR);
						}
						else
						{
							setColor_fnt(&alpha, WHITECOLOR);
						}
				}
				
				setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(int)(yPos * display_height));
			}
			
			//if(nFrameStep == 0) { nColor = NSHADOWCOLOR; } // Shadow color

            hashmap_position = games[fgames[nGame]->GameID]->nSize;
			printn_fnt(&alpha, FBADRV->szTitle, box_games_len);

			yPos += yPosDiff;

			nGame++;
		}


	}

	xPos = 0.3050f;
	yPos = 0.9184f;
	yPosDiff = 0.0199f;
	if (nFrameStep == 0)
	{
		setColor_fnt(&alpha, SHADOWCOLOR);
		setPosition_fnt(&alpha, (int)(xPos* display_width) + 2, (int)(int)(yPos* display_height) + 2);
	}
	else
	{
		setColor_fnt(&alpha, GRAYCOLOR2);
		setPosition_fnt(&alpha, (int)(xPos* display_width), (int)(int)(yPos* display_height));
	}

	if (app.ftrack != NULL) {
		snprintf(txt, sizeof(txt), "TRACK: %s", (*app.trackID).c_str());
		printn_fnt(&alpha, txt, box_filters_len);
	}

    yPos += yPosDiff;
	if (nFrameStep == 0)
	{
		setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
	}
	else
	{
		setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
	}
 	snprintf(txt,sizeof(txt),"FILTER: %s - FILTERED: %d",
                GetSystemFilter(g_opt_nActiveSysFilter),
                nFilteredGames);
	printf_fnt(&alpha, txt);

    yPos += yPosDiff;
	if (nFrameStep == 0)
	{
		setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
	}
	else
	{
		setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
	}
	snprintf(txt, sizeof(txt), "GAMES AVAILABLE: %d - MISSING: %d",
		nTotalGames,
		app.nMissingGames);
	printf_fnt(&alpha, txt);

	xPos = 0.5422f;
	yPos = 0.0361f;
	
	if (nFrameStep == 0)
	{
		setColor_fnt(&alpha, SHADOWCOLOR);
		setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
	}
	else
	{
		setColor_fnt(&alpha, GRAYCOLOR);
		setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
	}

	if(nFilteredGames >= 1) //if(nBurnSelected >= 0)
	{

		hashmap_position = games[game_ord_id]->nSize;

		snprintf(txt,sizeof(txt)," TITLE: %s", FBADRV->szTitle);

		if (nFrameStep == 0)
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
		}
		else
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
		}
		printn_fnt(&alpha, txt, box_title_len);
		yPos += yPosDiff;

		snprintf(txt,sizeof(txt)," ROMSET:      %s  -  PARENT:      %s",
                    games[game_ord_id]->name,games[nBurnSelected]->parent_name);
		if (nFrameStep == 0)
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
		}
		else
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
		}
		printf_fnt(&alpha, txt);

		yPos += yPosDiff;


		snprintf(txt,sizeof(txt)," COMPANY:     %s", games[game_ord_id]->company);
		if (nFrameStep == 0)
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
		}
		else
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
		}
		printf_fnt(&alpha, txt);

		yPos += yPosDiff;

		snprintf(txt,sizeof(txt)," YEAR:        %s  -  SYSTEM:      %s",
           games[game_ord_id]->year, games[game_ord_id]->subsystem);
		if (nFrameStep == 0)
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
		}
		else
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
		}
		printf_fnt(&alpha, txt);

		yPos += yPosDiff;

		snprintf(txt,sizeof(txt)," MAX PLAYERS: %d  -  RESOLUTION:  %s (%s)",
                    games[game_ord_id]->players,games[nBurnSelected]->resolution,games[game_ord_id]->aspectratio);
		if (nFrameStep == 0)
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
		}
		else
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
		}
		printf_fnt(&alpha, txt);

		yPos += yPosDiff;


		if (games[game_ord_id]->bAvailable) {
                hashmap_position = fgames[nSelectedGame]->nSize;

				if (strlen(FBAGAMES->szPath) + 7 > box_title_len)
					snprintf(txt, sizeof(txt), " PATH: %s", &FBAGAMES->szPath[strlen(FBAGAMES->szPath) + 7 - box_title_len]);
				else
					snprintf(txt,sizeof(txt)," PATH: %s",FBAGAMES->szPath);
				if (nFrameStep == 0)
				{
					setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
				}
				else
				{
					setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
				}
				printn_fnt(&alpha, txt, box_title_len);

        }
        yPos += yPosDiff + 0.015;

		switch (app.state.displayMode.resolution)
		{
		case VIDEO_RESOLUTION_1080:
			xPos = xPos + 0.15;
			break;
		case VIDEO_RESOLUTION_720:
			xPos = xPos + 0.15;
			break;
		case VIDEO_RESOLUTION_576:
		case VIDEO_RESOLUTION_480:
			xPos = xPos + 0.1;
			break;
		}
		
        lv2syscall1(352, (uint64_t) &meminfo);
        snprintf(txt,sizeof(txt),"MEMORY TOTAL: %d - AVAIL: %d", meminfo.total / 1024, meminfo.avail / 1024);
		if (nFrameStep == 0)
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
		}
		else
		{
			setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
		}
		printf_fnt(&alpha, txt);
		yPos += yPosDiff;
	}


}

void c_fbaRL::Options_Frame()
{
	xPos		= 0.0600f;
	yPos		= 0.0460f;
	yPosDiff	= 0.0410f;
	int X, Y;

	yPos += (yPosDiff * 4);
	yPosDiff = 0.0800f;

	int nMenuItem = options_menu->UpdateTopItem();
	switch (app.state.displayMode.resolution)
	{
	case VIDEO_RESOLUTION_1080:
		SetCurrentFont_fnt(&alpha, 1);
		setDimension_fnt(&alpha, alpha.fnt[1].maxwidth, alpha.fnt[1].height);
		break;
	case VIDEO_RESOLUTION_720:
		SetCurrentFont_fnt(&alpha, 2);
		setDimension_fnt(&alpha, alpha.fnt[2].maxwidth, alpha.fnt[2].height);
		break;
	case VIDEO_RESOLUTION_576:
	case VIDEO_RESOLUTION_480:
		SetCurrentFont_fnt(&alpha, 3);
		setDimension_fnt(&alpha, alpha.fnt[3].maxwidth, alpha.fnt[3].height);
		break;
	}

	while(nMenuItem <= (options_menu->nTopItem + options_menu->nListMax))
	{
		if(nMenuItem == options_menu->nTotalItem) break;

		setColor_fnt(&alpha, WHITECOLOR);
		
		if(nMenuItem == options_menu->nSelectedItem)
		{

			switch (app.state.displayMode.resolution)
			{
				case VIDEO_RESOLUTION_1080:
					SetCurrentFont_fnt(&alpha, 1);
					setDimension_fnt(&alpha, alpha.fnt[1].maxwidth, alpha.fnt[1].height);
					break;
				case VIDEO_RESOLUTION_720:
					SetCurrentFont_fnt(&alpha, 2);
					setDimension_fnt(&alpha, alpha.fnt[2].maxwidth, alpha.fnt[2].height);
					break;
				case VIDEO_RESOLUTION_576:
				case VIDEO_RESOLUTION_480:
					SetCurrentFont_fnt(&alpha, 3);
					setDimension_fnt(&alpha, alpha.fnt[3].maxwidth, alpha.fnt[3].height);
					break;
			}

			if (nFrameStep == 0)
			{
				X = 0.0600f * display_width + 2;
				Y = 0.8450f * display_height + 2;
				setColor_fnt(&alpha, SHADOWCOLOR);
			}
			else
			{
				X = 0.0600f * display_width;
				Y = 0.8450f * display_height;
			}

			setPosition_fnt(&alpha, X,Y);

			if(nMenuItem == MENU_OPT_AUTO_CFG) {
				snprintf(txt,sizeof(txt),"Auto create basic Input Preset CFG files for all systems.");
                printf_fnt(&alpha, txt);
			}

			if(nMenuItem == MENU_OPT_MUSIC) {
				snprintf(txt,sizeof(txt),"Enable / Disable background MP3 music.");
                printf_fnt(&alpha, txt);
			}

			if(nMenuItem == MENU_OPT_RETROARCH_MENU) {
				snprintf(txt,sizeof(txt),"Change Retro Arch menu driver.");
                printf_fnt(&alpha, txt);
			}

			if(nMenuItem == MENU_OPT_DISP_CLONES) {
				snprintf(txt,sizeof(txt),"Enable / Disable display of clone games.");
                printf_fnt(&alpha, txt);
			}

			if(nMenuItem == MENU_OPT_USE_UNIBIOS) {
				snprintf(txt,sizeof(txt),"Use UNI-BIOS when playing Neo-Geo games.");
                printf_fnt(&alpha, txt);
			}

			if(nMenuItem == MENU_OPT_DISP_MISS_GMS) {
				snprintf(txt,sizeof(txt),"Enable / Disable display of missing games.");
                printf_fnt(&alpha, txt);
			}

			if (nMenuItem == MENU_OPT_MD_DEF_CORE) {
				snprintf(txt, sizeof(txt), "Select default core for MegaDrive games.");
				printf_fnt(&alpha, txt);
			}

			if(nMenuItem >= MENU_OPT_FILTER_START && nMenuItem <=  MASKFAVORITE+MENU_OPT_FILTER_START) {
				snprintf(txt,sizeof(txt),"Choose the emulated systems you (don't) want to be displayed.");
				printf_fnt(&alpha, txt);
			}

			// Rom Paths (directories)
			if(nMenuItem > MASKFAVORITE+MENU_OPT_FILTER_START && nMenuItem <= MASKFAVORITE+MENU_OPT_FILTER_START+NDIRPATH) {
				int nRomPath = nMenuItem-(MASKFAVORITE+MENU_OPT_FILTER_START+1);
				snprintf(txt,sizeof(txt),"Current: %s", g_opt_szROMPaths[nRomPath]);
				printf_fnt(&alpha, txt);
			}

			// Input Preset Paths (CFG)
			if(nMenuItem > MASKFAVORITE+MENU_OPT_FILTER_START+12 && nMenuItem <= MASKFAVORITE+MENU_OPT_FILTER_START+NDIRPATH+MASKFAVORITE) {
				int nCfgPath = nMenuItem-(MASKFAVORITE+MENU_OPT_FILTER_START+12+1);
				snprintf(txt,sizeof(txt),"Current: %s", g_opt_szInputCFG[nCfgPath]);
				printf_fnt(&alpha, txt);
			}

			setColor_fnt(&alpha, YELLOWCOLOR);

		}

		switch (app.state.displayMode.resolution)
		{
			case VIDEO_RESOLUTION_1080:
				SetCurrentFont_fnt(&alpha, 0);
				setDimension_fnt(&alpha, alpha.fnt[0].maxwidth, alpha.fnt[0].height);
				break;
			case VIDEO_RESOLUTION_720:
				SetCurrentFont_fnt(&alpha, 1);
				setDimension_fnt(&alpha, alpha.fnt[1].maxwidth, alpha.fnt[1].height);
				break;
			case VIDEO_RESOLUTION_576:
			case VIDEO_RESOLUTION_480:
				SetCurrentFont_fnt(&alpha, 2);
				setDimension_fnt(&alpha, alpha.fnt[2].maxwidth, alpha.fnt[2].height);
				break;
		}

		if (nFrameStep == 0)
		{
			X = xPos * display_width + 2;
			Y = yPos * display_height + 2;
			setColor_fnt(&alpha, SHADOWCOLOR);
		}
		else
		{
			X = xPos * display_width;
			Y = yPos * display_height;
		}
		
		setPosition_fnt(&alpha, X,Y);

		if(nMenuItem == MENU_OPT_AUTO_CFG) {
                snprintf(txt,sizeof(txt),"%s: [%s]", options_menu->item[nMenuItem]->szMenuLabel,
                    g_opt_bAutoInputCfgCreate ? "ON" : "OFF");
				printf_fnt(&alpha, txt);
		}

		if(nMenuItem == MENU_OPT_MUSIC) {
                snprintf(txt,sizeof(txt),"%s: [%s]", options_menu->item[nMenuItem]->szMenuLabel,
                    g_opt_bMusic ? "ON" : "OFF");
				printf_fnt(&alpha, txt);
		}

		if(nMenuItem == MENU_OPT_RETROARCH_MENU) {
                snprintf(txt,sizeof(txt),"%s: [%s]", options_menu->item[nMenuItem]->szMenuLabel,
                    g_opt_sRetroArchMenu[g_opt_nRetroArchMenu]);
				printf_fnt(&alpha, txt);
		}

		if(nMenuItem == MENU_OPT_DISP_CLONES) { //CRYSTAL
                snprintf(txt,sizeof(txt),"%s: [%s]", options_menu->item[nMenuItem]->szMenuLabel,
                    g_opt_bDisplayCloneGames ? "ON" : "OFF");
				printf_fnt(&alpha, txt);
		}

		if(nMenuItem == MENU_OPT_USE_UNIBIOS) {
                snprintf(txt,sizeof(txt),"%s: [%s]", options_menu->item[nMenuItem]->szMenuLabel,
                    g_opt_bUseUNIBIOS ? "ON" : "OFF");
				printf_fnt(&alpha, txt);
		}

		if(nMenuItem == MENU_OPT_DISP_MISS_GMS) {
                snprintf(txt,sizeof(txt),"%s: [%s]", options_menu->item[nMenuItem]->szMenuLabel,
                    g_opt_bDisplayMissingGames ? "ON" : "OFF");
				printf_fnt(&alpha, txt);
		}

		if (nMenuItem == MENU_OPT_MD_DEF_CORE) {
			snprintf(txt, sizeof(txt), "%s: [%s]", options_menu->item[nMenuItem]->szMenuLabel,
				g_opt_sMegaDriveCores[g_opt_nMegaDriveDefaultCore]);
			printf_fnt(&alpha, txt);
		}

		// Custom System Filters
		if(nMenuItem >= MENU_OPT_FILTER_START && nMenuItem <=  MASKFAVORITE+MENU_OPT_FILTER_START)
		{
			int nSysFilter = nMenuItem-MENU_OPT_FILTER_START;

                snprintf(txt,sizeof(txt),"%s: [%s]", options_menu->item[nMenuItem]->szMenuLabel,
                    g_opt_bCustomSysFilter[nSysFilter] ? "ON" : "OFF");
				printf_fnt(&alpha, txt);
		}

		// Rom Paths (directories)
		if(nMenuItem > MASKFAVORITE+MENU_OPT_FILTER_START && nMenuItem <= MASKFAVORITE+MENU_OPT_FILTER_START+NDIRPATH)
		{
                snprintf(txt,sizeof(txt),"%s: [...]", options_menu->item[nMenuItem]->szMenuLabel);
				printf_fnt(&alpha, txt);
		}

		// Input Preset Paths (CFG)
		if(nMenuItem > MASKFAVORITE+MENU_OPT_FILTER_START+12 && nMenuItem <= MASKFAVORITE+MENU_OPT_FILTER_START+NDIRPATH+MASKFAVORITE)
		{
                snprintf(txt,sizeof(txt),"%s: [...]", options_menu->item[nMenuItem]->szMenuLabel);
				printf_fnt(&alpha, txt);
		}

		yPos += yPosDiff;

		nMenuItem++;
	}
}


void c_fbaRL::FileBrowser_Frame()
{
	xPos		= 0.0750f;
	yPos		= 0.0500f;
	yPosDiff	= 0.0265f;

	yPos = 0.1250L;

	float yHeadPos = 0.8450f;
	
	switch (app.state.displayMode.resolution)
	{
	case VIDEO_RESOLUTION_1080:
		SetCurrentFont_fnt(&alpha, 1);
		setDimension_fnt(&alpha, alpha.fnt[1].maxwidth, alpha.fnt[1].height);
		break;
	case VIDEO_RESOLUTION_720:
		SetCurrentFont_fnt(&alpha, 2);
		setDimension_fnt(&alpha, alpha.fnt[2].maxwidth, alpha.fnt[2].height);
		break;
	case VIDEO_RESOLUTION_576:
	case VIDEO_RESOLUTION_480:
		SetCurrentFont_fnt(&alpha, 3);
		setDimension_fnt(&alpha, alpha.fnt[3].maxwidth, alpha.fnt[3].height);
		break;
	}
	
	if (nFrameStep == 0)
	{
		setColor_fnt(&alpha, SHADOWCOLOR);
		setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yHeadPos * display_height) + 2);
	}
	else
	{
		setColor_fnt(&alpha, WHITECOLOR);
		setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yHeadPos * display_height));
	}
	
	snprintf(txt,sizeof(txt),"CURRENT DIR: %s", filebrowser->pszCurrentDir);
	printf_fnt(&alpha, txt);
    
	yPos += yPosDiff;

	switch (app.state.displayMode.resolution)
	{
	case VIDEO_RESOLUTION_1080:
		SetCurrentFont_fnt(&alpha, 0);
		setDimension_fnt(&alpha, alpha.fnt[0].maxwidth, alpha.fnt[0].height);
		break;
	case VIDEO_RESOLUTION_720:
		SetCurrentFont_fnt(&alpha, 1);
		setDimension_fnt(&alpha, alpha.fnt[1].maxwidth, alpha.fnt[1].height);
		break;
	case VIDEO_RESOLUTION_576:
	case VIDEO_RESOLUTION_480:
		SetCurrentFont_fnt(&alpha, 2);
		setDimension_fnt(&alpha, alpha.fnt[2].maxwidth, alpha.fnt[2].height);
		break;
	}

	if(filebrowser->nTotalItem >= 1)
	{
		yPos += yPosDiff;

		int nMenuItem = filebrowser->UpdateTopItem();

		while(nMenuItem <= (filebrowser->nTopItem + filebrowser->nListMax))
		{
			if(nMenuItem == filebrowser->nTotalItem) break;

			// normal
			setColor_fnt(&alpha, WHITECOLOR);
			// selected
			if(nMenuItem == filebrowser->nSelectedItem) {
				setColor_fnt(&alpha, YELLOWCOLOR);
			}

			if (nFrameStep == 0)
			{
				setColor_fnt(&alpha, SHADOWCOLOR);
				setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
			}
			else
			{
				setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
			}
			snprintf(txt, sizeof(txt), "%s", filebrowser->item[nMenuItem]->szMenuLabel);
			printf_fnt(&alpha, txt);
			yPos += yPosDiff;

			nMenuItem++;
		}
	}
}


void c_fbaRL::ZipInfo_Frame()
{
	xPos		= 0.0400f;
	yPos		= 0.0400f;
	yPosDiff	= 0.0200f;
	u32 line_len = 0;
	
	switch (app.state.displayMode.resolution)
	{
	case VIDEO_RESOLUTION_1080:
		SetCurrentFont_fnt(&alpha, 1);
		setDimension_fnt(&alpha, alpha.fnt[1].maxwidth, alpha.fnt[1].height);
		line_len = 87;
		break;
	case VIDEO_RESOLUTION_720:
		SetCurrentFont_fnt(&alpha, 2);
		setDimension_fnt(&alpha, alpha.fnt[2].maxwidth, alpha.fnt[2].height);
		line_len = 83;
		break;
	case VIDEO_RESOLUTION_576:
	case VIDEO_RESOLUTION_480:
		SetCurrentFont_fnt(&alpha, 3);
		setDimension_fnt(&alpha, alpha.fnt[3].maxwidth, alpha.fnt[3].height);
		line_len = 60;
		break;
	}
	if (nFrameStep == 0)
	{
		setColor_fnt(&alpha, SHADOWCOLOR);
		setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
	}
	else
	{
		setColor_fnt(&alpha, WHITECOLOR);
		setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
	}

	if(nFilteredGames >= 1)
	{
		char* pszZipName = NULL;
		pszZipName = (char*)malloc(33);
		memset(pszZipName, 0, 33);
		memcpy(pszZipName, fgames[nSelectedGame]->zipname, 32);

		snprintf(txt,sizeof(txt),"ZIP: %s / TOTAL FILES FOUND: %d", pszZipName, zipinfo_menu->nTotalItem);
		printn_fnt(&alpha, txt, line_len);
		yPos += yPosDiff;

		SAFE_FREE(pszZipName)

		xPos = 0.0650f;
		
		yPos += (yPosDiff * 3);

		int nMenuItem = zipinfo_menu->UpdateTopItem();

		while(nMenuItem <= (zipinfo_menu->nTopItem + zipinfo_menu->nListMax))
		{
			if(nMenuItem == zipinfo_menu->nTotalItem) break;

			// normal
			setColor_fnt(&alpha, WHITECOLOR);
			// selected
			if(nMenuItem == zipinfo_menu->nSelectedItem) {
				setColor_fnt(&alpha, YELLOWCOLOR);
			}

			if (nFrameStep == 0)
			{
				setColor_fnt(&alpha, SHADOWCOLOR);
				setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
			}
			else
			{
				setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
			}
			setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(int)(yPos * display_height));
            snprintf(txt,sizeof(txt),"%s", zipinfo_menu->item[nMenuItem]->szMenuLabel);
			printn_fnt(&alpha, txt, line_len);
			yPos += yPosDiff;

			nMenuItem++;
		}
	}
}

void c_fbaRL::RomInfo_Frame()
{
	xPos		= 0.0400f;
	yPos		= 0.0400f;
	yPosDiff	= 0.0220f;

	switch (app.state.displayMode.resolution)
	{
	case VIDEO_RESOLUTION_1080:
		SetCurrentFont_fnt(&alpha, 1);
		setDimension_fnt(&alpha, alpha.fnt[1].maxwidth, alpha.fnt[1].height);
		break;
	case VIDEO_RESOLUTION_720:
		SetCurrentFont_fnt(&alpha, 2);
		setDimension_fnt(&alpha, alpha.fnt[2].maxwidth, alpha.fnt[2].height);
		break;
	case VIDEO_RESOLUTION_576:
	case VIDEO_RESOLUTION_480:
		SetCurrentFont_fnt(&alpha, 3);
		setDimension_fnt(&alpha, alpha.fnt[3].maxwidth, alpha.fnt[3].height);
		break;
	}
	
	if(nFilteredGames >= 1)
	{
		char* pszZipName = NULL;
		pszZipName = (char*)malloc(33);
		memset(pszZipName, 0, 33);
		memcpy(pszZipName, fgames[nSelectedGame]->zipname, 32);

		snprintf(txt,sizeof(txt),"ROMSET: %s ", pszZipName);
		if (nFrameStep == 0)
		{
			setColor_fnt(&alpha, SHADOWCOLOR);
			setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
		}
		else
		{
			
			setColor_fnt(&alpha, WHITECOLOR);
			setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
		}
		printf_fnt(&alpha, txt);
		yPos += yPosDiff;

		SAFE_FREE(pszZipName)

		xPos = 0.0650f;
		yPos += (yPosDiff * 3);

		int nMenuItem = rominfo_menu->UpdateTopItem();

		while(nMenuItem <= (rominfo_menu->nTopItem + rominfo_menu->nListMax))
		{
			if(nMenuItem == rominfo_menu->nTotalItem) break;

			// normal
			setColor_fnt(&alpha, WHITECOLOR);
			// selected
			if(nMenuItem == rominfo_menu->nSelectedItem) {
				setColor_fnt(&alpha, YELLOWCOLOR);
			}

			if (nFrameStep == 0)
			{
				setColor_fnt(&alpha, SHADOWCOLOR);
				setPosition_fnt(&alpha, (int)(xPos * display_width) + 2, (int)(yPos * display_height) + 2);
			}
			else
			{
				setPosition_fnt(&alpha, (int)(xPos * display_width), (int)(yPos * display_height));
			}

			snprintf(txt,sizeof(txt),"%s", rominfo_menu->item[nMenuItem]->szMenuLabel);
			printf_fnt(&alpha, txt);
			yPos += yPosDiff;

			nMenuItem++;
		}
	}
}
