// =======================================================================
// GAMELIST (FILTERS)

#include "capp.h"
static char pszFilter[64];

char szSysFilter[MASKALL][256] = {
	"MASKCAPMISC"		,"MASKCAVE"		,"MASKCPS"		,"MASKCPS2",
	"MASKCPS3"			,"MASKDATAEAST"	,"MASKGALAXIAN"	,"MASKIREM",
	"MASKKANEKO"		,"MASKKONAMI"	,"MASKNEOGEO"	,"MASKPACMAN",
	"MASKPGM"			,"MASKPSIKYO"	,"MASKSEGA"		,"MASKSEG32", "MASKSETA",
	"MASKTAITO"			,"MASKTECHNOS"	,"MASKTOAPLAN"	,"MASKMISCPRE90S",
	"MASKMISCPOST90S"	,"MASKMIDWAY"	,"MASKSNES"		,"MASKMEGADRIVE",
	"MASKAMIGA"			,"MASKCOLECO"	,"MASKTG16"		,"MASKPCE",
	"MASKSGX"			,"MASKFAVORITE"
};

bool bSysFilter[MASKCUSTOM+1] =
{
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true// <--- fillers
};

int c_fbaRL::GetSystemMaskId(char* szMask)
{
	for(uint32_t n = 0; n < sizeof(szSysFilter) / 256; n++)
	{
		if(strcmp(szMask, szSysFilter[n]) == 0)
		{
			return n;
		}
	}
	return -1;
}

char* c_fbaRL::GetSystemFilter(int nFilter)
{

//	char* pszFilter = NULL;
//	pszFilter = (char*)malloc(256);
//	memset(pszFilter, 0, 256);

	switch(nFilter)
	{
		case 0:  strcpy(pszFilter, "Capcom Misc"); break;
		case 1:  strcpy(pszFilter, "Cave"); break;
		case 2:  strcpy(pszFilter, "Capcom Play System I (CPS-I)"); break;
		case 3:  strcpy(pszFilter, "Capcom Play System II (CPS-II)"); break;
		case 4:  strcpy(pszFilter, "Capcom Play System III (CPS-III)"); break;
		case 5:  strcpy(pszFilter, "Data East"); break;
		case 6:  strcpy(pszFilter, "Galaxian"); break;
		case 7:  strcpy(pszFilter, "Irem"); break;
		case 8:  strcpy(pszFilter, "Kaneko"); break;
		case 9:  strcpy(pszFilter, "Konami"); break;
		case 10:  strcpy(pszFilter, "Neo Geo"); break;
		case 11:  strcpy(pszFilter, "Pac-Man"); break;
		case 12:  strcpy(pszFilter, "Poly Game Master (PGM)"); break;
		case 13:  strcpy(pszFilter, "Psikyo"); break;
		case 14:  strcpy(pszFilter, "Sega"); break;
		case 15:  strcpy(pszFilter, "Sega32"); break;
		case 16:  strcpy(pszFilter, "Seta"); break;
		case 17:  strcpy(pszFilter, "Taito"); break;
		case 18:  strcpy(pszFilter, "Technos"); break;
		case 19:  strcpy(pszFilter, "Toaplan"); break;
		case 20:  strcpy(pszFilter, "Misc Pre 90\'s"); break;
		case 21:  strcpy(pszFilter, "Misc Post 90\'s"); break;
		case 22:  strcpy(pszFilter, "Midway"); break;
		case 23:  strcpy(pszFilter, "SNES"); break;
		case 24:  strcpy(pszFilter, "Mega Drive"); break;
		case 25:  strcpy(pszFilter, "Amiga"); break;
		case 26:  strcpy(pszFilter, "Coleco"); break;
		case 27:  strcpy(pszFilter, "TurboGrafx-16"); break;
		case 28:  strcpy(pszFilter, "PC Engine"); break;
		case 29:  strcpy(pszFilter, "SuperGrafx"); break;
		case 30:  strcpy(pszFilter, "Favorite"); break;
		case 31:  strcpy(pszFilter, "All Games"); break;
		case 32:  strcpy(pszFilter, "Custom Filter"); break;
	}

	return pszFilter;
}

void c_fbaRL::NextSysFilter()
{
	bProcessingGames = true;
    // custom --> first single filter
	if(g_opt_nActiveSysFilter == MASKCUSTOM)
	{
		g_opt_nActiveSysFilter = 0;

		for(uint32_t n = 0; n < sizeof(bSysFilter)-2; n++){
			bSysFilter[n] = false;
		}

		bSysFilter[g_opt_nActiveSysFilter] = true;

		// refresh filters
		EndFilterList();
		InitFilterList();
		return;
	}

	// show all --> custom
	if(g_opt_nActiveSysFilter == MASKALL)
	{
		g_opt_nActiveSysFilter = MASKCUSTOM;

		// use values from FBA_RL.ini
		for(uint32_t n = 0; n < sizeof(bSysFilter)-2; n++){
			bSysFilter[n] = g_opt_bCustomSysFilter[n];
		}

		// refresh filters
		EndFilterList();
		InitFilterList();
		return;
	}

	// next single system filter --> ...
	g_opt_nActiveSysFilter++;

	for(uint32_t n = 0; n < sizeof(bSysFilter)-2; n++){
		bSysFilter[n] = false;
	}

	bSysFilter[g_opt_nActiveSysFilter] = true;

	// refresh filters
	EndFilterList();
	InitFilterList();

	bProcessingGames = false;
}

void c_fbaRL::PrevSysFilter()
{
	bProcessingGames = true;

	// custom --> show all
	if(g_opt_nActiveSysFilter == MASKCUSTOM)
	{
		g_opt_nActiveSysFilter = MASKALL;

		for(uint32_t n = 0; n < sizeof(bSysFilter)-2; n++){
			bSysFilter[n] = false;
		}

		bSysFilter[g_opt_nActiveSysFilter] = true;

		// refresh filters
		EndFilterList();
		InitFilterList();
		return;
	}

	// first single filter --> custom
	if(g_opt_nActiveSysFilter == 0)
	{
		g_opt_nActiveSysFilter = MASKCUSTOM;

		// use values from FBA_RL.ini
		for(uint32_t n = 0; n < sizeof(bSysFilter)-2; n++){
			bSysFilter[n] = g_opt_bCustomSysFilter[n];
		}

		// refresh filters
		EndFilterList();
		InitFilterList();
		return;
	}

	// previous single system filter --> ...
	g_opt_nActiveSysFilter--;

	for(uint32_t n = 0; n < sizeof(bSysFilter)-2; n++){
		bSysFilter[n] = false;
	}

	bSysFilter[g_opt_nActiveSysFilter] = true;

	// refresh filters
	EndFilterList();
	InitFilterList();

	bProcessingGames = false;
}

bool c_fbaRL::FilterGame(char* szSystemMask)
{
    // show all?
	if(g_opt_nActiveSysFilter == MASKALL) {
		return true;
	}

	if(g_opt_nActiveSysFilter == MASKCUSTOM)
	{
		// filter...
		for(uint32_t n=0; n < sizeof(bSysFilter)-2; n++)
		{
			if(strcmp(szSystemMask, szSysFilter[n]) == 0)
			{
				if(g_opt_bCustomSysFilter[n] == true) {
					return true;
				} else {
					return false;
				}
			}
		}

	}


	if(strcmp(szSystemMask, szSysFilter[g_opt_nActiveSysFilter]) == 0)
	{
		return true;
	}

	return false;
}

void c_fbaRL::InitFilterList()
{
	bProcessingGames = true;
    hashmap_map *drvmap, *gamesmap;
    FBA_DRV *fba_drv;
    FBA_GAMES *fba_games;
    drvmap = (hashmap_map *) app.drvMap;
    gamesmap =  (hashmap_map *) app.gamesMap;
    char key[KEY_MAX_LENGTH];
    int ret = 0;

	nBurnSelected = -1;

	nSelectedGame = 0;
	nGameListTop = 0;
	nFilteredGames = 0;
	fgames = (c_game**)malloc(sizeof(c_game) * MAX_GAMES);
	if (fgames == NULL)
		printf("Memory error fgames\n");
//    else
//        printf("Malloc Fgames OK\n");
	//memset(fgames, 0, sizeof(c_game) * MAX_GAMES);

	// process...
	if (nTotalGames == 0) {
		bProcessingGames = false;
		return;
	}

	for(int n=0; n < (int) app.nTotalGamesWithDup; n++)
	{
		if(!g_opt_bDisplayMissingGames && !games[n]->bAvailable)
		{
			continue;
		}
		//printf("1 %s\n", games[n]->name);
		//favorite?
        if(g_opt_nActiveSysFilter == MASKFAVORITE) {
            if (!games[n]->isFavorite)
                    continue;
        }
		else {
			//printf("ELSE %s\n", games[n]->sysmask);
			if(FilterGame(games[n]->sysmask) == false)
            {
                continue;
            }
		}
		 
		if (!g_opt_bDisplayCloneGames && !!games[n]->isClone)
		{
			continue;
		}
		 
		fgames[nFilteredGames] = new c_game(nFilteredGames);
		
		//strcpy(fgames[nFilteredGames]->path		, games[n]->path);
		fgames[nFilteredGames]->GameID = games[n]->nGame;
		
		fgames[nFilteredGames]->isFavorite = games[n]->isFavorite;
		
		strcpy(fgames[nFilteredGames]->zipname	, games[n]->zipname);
		
		hashmap_position = games[n]->nSize;
		
        fba_drv = (FBA_DRV *)drvmap->data[hashmap_position].data;
		
        //fgames[nFilteredGames]->title = (char *)malloc(sizeof(char)*11);
        //snprintf(fgames[nFilteredGames]->title,11,fba_drv->szTitle);
        fgames[nFilteredGames]->title = fba_drv->szTitle;
		//strcpy(fgames[nFilteredGames]->title, fba_drv->szTitle);
		//1//strcpy(fgames[nFilteredGames]->title	, games[n]->title);
		fgames[nFilteredGames]->def_core_id	= games[n]->def_core_id;
		strcpy(fgames[nFilteredGames]->sysmask	, games[n]->sysmask);
        if (games[n]->core_id > 0)
                snprintf(key, KEY_MAX_LENGTH, "%s%s%d", fba_drv->szSystem, fba_drv->szName, games[n]->core_id);
		else
            snprintf(key, KEY_MAX_LENGTH, "%s%s%d", fba_drv->szSystem, fba_drv->szName, fba_drv->nCoreID);
		ret = hashmap_get(gamesmap, key, (void**)(&fba_games));
		fgames[nFilteredGames]->nSize = ret;
		//printf("key: %s - Ret: %d\n", key, ret);
		//memcpy(&fgames[nFilteredGames]->nSize	, &games[n]->nSize, sizeof(uint64_t));
		memcpy(&fgames[nFilteredGames]->bAvailable	, &games[n]->bAvailable, sizeof(bool));

		nFilteredGames++;
	}

	if(nFilteredGames < 1) {
		bProcessingGames = false;
		////ResetPreviewImage();
		return;
	}

	// Sort Filtered list by Title
	qsort(fgames, nFilteredGames, sizeof(c_game*), _FcCompareStruct);
	bProcessingGames = false;

//	for(int n=0; n < nTotalGames; n++) {
//        printf("%d - %s - %d\n", n , fgames[nFilteredGames]->title, fgames[nFilteredGames]->GameID);
//	}
	nStatus = STATUS_UPDATEPREVIEW;
	nBurnSelected = fgames[0]->GameID;
	//printf("InitFilterList END\n");
}

void c_fbaRL::EndFilterList()
{
	bProcessingGames = true;

	for(int n = 0; n < nFilteredGames; n++)	{
		SAFE_DELETE(fgames[n])
	}
	SAFE_FREE(fgames);

	nSelectedGame = 0;
	nGameListTop = 0;
	nFilteredGames = 0;

	bProcessingGames = false;
}
