// =======================================================================
// INPUTS
#include <string>
#include <vector>
#include "misc.h"
#include "capp.h"
#include "hashmap.h"
#include "soundlib/spu_soundlib.h"
#include "soundlib/audioplayer.h"
#include <sysutil/msg.h>
#include <sysutil/sysutil.h>
#include <sys/process.h>
#include <sys/dirent.h>

/*static vs32 dialog_action2 = 0;

static void Ddialog_handler2(msgButton button,void *usrData)
{
	printf("Entrato\n");
	switch(button) {
		case MSG_DIALOG_BTN_OK:
			dialog_action2 = 1;
			break;
		case MSG_DIALOG_BTN_NO:
		case MSG_DIALOG_BTN_ESCAPE:
			dialog_action2 = 2;
			break;
		case MSG_DIALOG_BTN_NONE:
            dialog_action2 = -1;
			break;
		default:
			break;
	}
}*/


void c_fbaRL::InputFrame()
{
	if(bProcessingGames) return;

	static int nSelInputFrame = 0;
	static u64 start_seconds = 0, seconds, start_nseconds, nseconds;

	// ------------------------------------------------------
	// Navigation UP/DOWN with delay

	if(((app.mFrame + nSelInputFrame) - app.mFrame) == 3)
	{
		// DPAD UP / LEFT ANALOG UP
		if((app.mIsButtPressed[BT_UP] && app.buttPressedNow[BT_UP]) || (app.mValLStickY < 50))
		{
			switch(nSection)
			{
				case SECTION_MAIN:
				{
					if(main_menu->nSelectedItem > 0 && main_menu->nSelectedItem <= main_menu->nTotalItem)
					{
						main_menu->nSelectedItem--;
					}
					break;
				}

				case SECTION_OPTIONS:
				{
					if(options_menu->nSelectedItem > 0 && options_menu->nSelectedItem <= options_menu->nTotalItem)
					{
						options_menu->nSelectedItem--;
					}
					break;
				}

				case SECTION_GAMELIST:
				{
					if(nSelectedGame > 0 && nSelectedGame <= nFilteredGames)
					{
						nSelectedGame--;
						if(nFilteredGames >= 1) {
							nStatus = STATUS_UPDATEPREVIEW;
						} else {
							nStatus = STATUS_RESETPREVIEW;
						}
					}
					break;
				}

				case SECTION_FILEBROWSER:
				{
					if(filebrowser->nSelectedItem > 0 && filebrowser->nSelectedItem <= filebrowser->nTotalItem)
					{
						filebrowser->nSelectedItem--;
					}
					break;
				}

				case SECTION_ZIPINFO:
				{
					if(zipinfo_menu->nSelectedItem > 0 && zipinfo_menu->nSelectedItem <= zipinfo_menu->nTotalItem)
					{
						zipinfo_menu->nSelectedItem--;
					}
					break;
				}

				case SECTION_ROMINFO:
				{
					if(rominfo_menu->nSelectedItem > 0 && rominfo_menu->nSelectedItem <= rominfo_menu->nTotalItem)
					{
						rominfo_menu->nSelectedItem--;
					}
					break;
				}
			}
		}

		// DPAD DOWN / LEFT ANALOG DOWN
		if	( (app.mIsButtPressed[BT_DOWN] && app.buttPressedNow[BT_DOWN]) || (app.mValLStickY > 200) )
		{
			switch(nSection)
			{
				case SECTION_MAIN:
				{
					if(main_menu->nSelectedItem >= 0 && main_menu->nSelectedItem < main_menu->nTotalItem-1)
					{
						main_menu->nSelectedItem++;
					}
					break;
				}

				case SECTION_OPTIONS:
				{
					if(options_menu->nSelectedItem >= 0 && options_menu->nSelectedItem < options_menu->nTotalItem-1)
					{
						options_menu->nSelectedItem++;
					}
					break;
				}

				case SECTION_GAMELIST:
				{
					//printf("nSelectedGame %d - nFilteredGames %d\n",nSelectedGame, nFilteredGames);
					if(nSelectedGame >= 0 && nSelectedGame < nFilteredGames-1)
					{
						nSelectedGame++;
						if(nFilteredGames >= 1) {
							nStatus = STATUS_UPDATEPREVIEW;
						} else {
							nStatus = STATUS_RESETPREVIEW;
						}
					}
					break;
				}

				case SECTION_FILEBROWSER:
				{
					if(filebrowser->nSelectedItem >= 0 && filebrowser->nSelectedItem < filebrowser->nTotalItem-1)
					{
						filebrowser->nSelectedItem++;
					}
					break;
				}

				case SECTION_ZIPINFO:
				{
					if(zipinfo_menu->nSelectedItem >= 0 && zipinfo_menu->nSelectedItem < zipinfo_menu->nTotalItem-1)
					{
						zipinfo_menu->nSelectedItem++;
					}
					break;
				}

				case SECTION_ROMINFO:
				{
					if(rominfo_menu->nSelectedItem >= 0 && rominfo_menu->nSelectedItem < rominfo_menu->nTotalItem-1)
					{
						rominfo_menu->nSelectedItem++;
					}
					break;
				}
			}
		}
		nSelInputFrame = 0;
	}

	nSelInputFrame++;

	// ------------------------------------------------------
	// [ANALOG STICKS]

	/// RIGHT ANALOG [UP]
	//if(app.mValRStickY < 50){
	//}
	/// RIGHT ANALOG [DOWN]
	//if(app.mValRStickY > 200){
	//}
	/// RIGHT ANALOG [LEFT]
	//if(app.mValRStickX < 50){
	//}
	/// RIGHT ANALOG [RIGHT]
	//if(app.mValRStickX > 200){
	//}

	// ------------------------------------------------------
	// [ ] - SQUARE

	if(!app.mIsButtPressed[BT_SQUARE] && app.buttPressedNow[BT_SQUARE])
	{
		switch(nSection)
		{
			case SECTION_GAMELIST:
			{
				if(nFilteredGames < 1) break;
				if(!fgames[nSelectedGame]->bAvailable) break;

				nSection = SECTION_ZIPINFO;
				InitZipInfoMenu();
				//nSection = SECTION_ZIPINFO;
				break;
			}
			case SECTION_MAIN:
			{
				InitOptionsMenu();
				nSection = SECTION_OPTIONS;
				EndMainMenu();
				break;
			}
		}
	}

	// ------------------------------------------------------
	// (X) - CROSS

	if(!app.mIsButtPressed[BT_CROSS] && app.buttPressedNow[BT_CROSS])
	{
		switch(nSection)
		{
			case SECTION_FILEBROWSER:
			{
				filebrowser->NavigateSelectedDir();
				break;
			}

			case SECTION_OPTIONS:
			{
				int nMenuItem = options_menu->nSelectedItem;

				// Auto Input CFG create toggle
				if(nMenuItem == MENU_OPT_AUTO_CFG)
				{
					g_opt_bAutoInputCfgCreate = g_opt_bAutoInputCfgCreate ? false : true;
				}

				// Alternate RetroArch Menu Key combo
				if(nMenuItem == MENU_OPT_MUSIC)
				{
					if (g_opt_bMusic) {
                        g_opt_bMusic = false;
                        if (app.ftrack != NULL) {
                                    StopAudio();
                                    fclose(app.ftrack);
                        }
					}
					else {
                        g_opt_bMusic = true;
                        char tpath[256];
                        sprintf(tpath, "/dev_hdd0/game/FBNE00123/USRDIR/soundtracks/%s", (*app.trackID).c_str());
                        app.ftrack = fopen(tpath, "r");
                        if (app.ftrack) {
                                //printf("Play track: %s\n", (*app.trackID).c_str());
                                PlayAudiofd(app.ftrack, 0,  AUDIO_INFINITE_TIME);

                        }
					}
				}
				//printf("PREMUTO X\n");
				// Retro Arch Menu
				if(nMenuItem == MENU_OPT_RETROARCH_MENU)
				{
					switch (g_opt_nRetroArchMenu) {
						case RETROARCH_MENU_GLUI_N:
								g_opt_nRetroArchMenu = RETROARCH_MENU_OZONE_N;
								break;
						case RETROARCH_MENU_OZONE_N:
								g_opt_nRetroArchMenu = RETROARCH_MENU_RGUI_N;
								break;
						case RETROARCH_MENU_RGUI_N:
								g_opt_nRetroArchMenu = RETROARCH_MENU_XMB_N;
								break;

						case RETROARCH_MENU_XMB_N:
								g_opt_nRetroArchMenu = RETROARCH_MENU_GLUI_N;
								break;
						}

				}

				// MegaDrive Default Core
				if (nMenuItem == MENU_OPT_MD_DEF_CORE)
				{
					if (!fileExist("/dev_hdd0/game/FBNE00123/USRDIR/cores/genesis_plus_gx.SELF"))
						g_opt_nMegaDriveDefaultCore = 0;
					else
						g_opt_nMegaDriveDefaultCore = g_opt_nMegaDriveDefaultCore ? 0 : 1;
				}

				// Display clones
				if(nMenuItem == MENU_OPT_DISP_CLONES)
				{
					g_opt_bDisplayCloneGames = g_opt_bDisplayCloneGames ? false : true;
				}

				// Enable / Disable Neo-Geo UNI-BIOS
				if(nMenuItem == MENU_OPT_USE_UNIBIOS) {
					g_opt_bUseUNIBIOS = g_opt_bUseUNIBIOS ? false : true;
				}

				if(nMenuItem == MENU_OPT_DISP_MISS_GMS) {
					g_opt_bDisplayMissingGames = g_opt_bDisplayMissingGames ? false : true;
				}

				// Custom System Filters
				if(nMenuItem >= MENU_OPT_FILTER_START && nMenuItem <=  MASKFAVORITE+MENU_OPT_FILTER_START)
				{
					int nSysFilter = nMenuItem-MENU_OPT_FILTER_START;
					g_opt_bCustomSysFilter[nSysFilter] = g_opt_bCustomSysFilter[nSysFilter] ? false : true;
				}

				// Rom Paths (directories)
				if(nMenuItem > MASKFAVORITE+MENU_OPT_FILTER_START && nMenuItem <= MASKFAVORITE+MENU_OPT_FILTER_START+12)
				{
					nFileBrowserType = 0;
					InitFileBrowser();
					nSection = SECTION_FILEBROWSER;
				}

				// Input Preset Paths (CFG)
				if(nMenuItem > MASKFAVORITE+MENU_OPT_FILTER_START+12 && nMenuItem <= MASKFAVORITE+MENU_OPT_FILTER_START+12+25)
				{
					nFileBrowserType = 1;
					InitFileBrowser();
					nSection = SECTION_FILEBROWSER;
				}
				break;
			}

			case SECTION_GAMELIST:
			{
				if(nFilteredGames < 1) break;
				if(!fgames[nSelectedGame]->bAvailable) { break;	}

				// do not try to load system BIOS...
				if( strcmp(fgames[nSelectedGame]->zipname, "neogeo.zip")==0 ||
					strcmp(fgames[nSelectedGame]->zipname, "pgm.zip")==0 ) {
						break;
				}

				iniWrite(); // save settings
                char fba_core_path[256];
				char fba_rl_path[]		= "/dev_hdd0/game/FBNE00123/USRDIR/RELOAD.SELF";
				sprintf(fba_core_path, "/dev_hdd0/game/FBNE00123/USRDIR/cores/fb_neo.SELF");
				hashmap_map *gamesmap;
                gamesmap =  (hashmap_map *) app.gamesMap;
                FBA_GAMES *fba_games;
                hashmap_position = fgames[nSelectedGame]->nSize;
                fba_games = (FBA_GAMES *)gamesmap->data[hashmap_position].data;
				if (strcmp(fgames[nSelectedGame]->sysmask, "MASKSNES") == 0){
                            sprintf(fba_core_path, "/dev_hdd0/game/FBNE00123/USRDIR/cores/snes9x.SELF");
                            if (!fileExist(fba_core_path)) {
                                nStatus = STATUS_MISSING_CORE_2;
                                break;
                            }
				}
				if (strcmp(fgames[nSelectedGame]->sysmask, "MASKMEGADRIVE") == 0){
                            if (strcmp(fgames[nSelectedGame]->subsystem, "megacd") == 0 || g_opt_nMegaDriveDefaultCore == 1)
									sprintf(fba_core_path,"/dev_hdd0/game/FBNE00123/USRDIR/cores/genesis_plus_gx.SELF");
                            if (!fileExist(fba_core_path)) {
                                nStatus = STATUS_MISSING_CORE_4;
                                break;
                            }
				}
				if (strcmp(fgames[nSelectedGame]->sysmask, "MASKAMIGA") == 0){
                            sprintf(fba_core_path,"/dev_hdd0/game/FBNE00123/USRDIR/EUAE/e-uae.SELF");
                            if (!fileExist(fba_core_path)) {
                                nStatus = STATUS_MISSING_CORE_5;
                                break;
                            }
                            /*else {
                                MakeAmigaCFG(fba_games->szPath, games[fgames[nSelectedGame]->GameID]->subsystem);
                                return;
                            }*/
				}
				if( fileExist(fba_rl_path) && fileExist(fba_core_path) )
				{
					//if(fileExist(fgames[nSelectedGame]->path))
					char gamepath[256];
					if(fileExist(fba_games->szPath))
					{
						//printf("fba_games->szPath: %s\n", fba_games->szPath);
						snprintf(gamepath, sizeof(gamepath), "%s", fba_games->szPath);
						app.onShutdown();

						char aspect_ratio[12]	= { 0 };

						sprintf(aspect_ratio, "%s",games[nBurnSelected]->resolution);

						int nSysMask = GetSystemMaskId(games[nBurnSelected]->sysmask);


						LaunchFBACore(
							gamepath,
							(char*)fba_rl_path,
							(char*)(fileExist(g_opt_szInputCFG[nSysMask]) ?  g_opt_szInputCFG[nSysMask] : "DUMMY_ARG"),
							(char*)("DUMMY_ARG"),
							//(char*)(g_opt_bUseAltMenuKeyCombo ? "yes" : "no"),
							//(char*)(g_opt_bUseUNIBIOS ? "yes" : "no")  //CRYSTAL
                            (char*)(games[fgames[nSelectedGame]->GameID]->subsystem), //SUBSYSTEM
							(char*)("DUMMY_ARG")  //CRYSTAL
						);
					} else {
						// Missing game, probably USB is disconnected or file is no longer present for some reason
						nStatus = STATUS_MISSING_GAME_DLG;
						break;
					}
				} else {
					// Missing important program files...lol
					nStatus = STATUS_CORRUPT_APP_DLG;
					break;
				}
				break;
			}

			case SECTION_MAIN:
			{
				int nMenuItem = main_menu->nSelectedItem;
                //printf("nMenuItem: %d\n", nMenuItem);
				// Gamelist
				if(nMenuItem == MENU_MAIN_GAMELIST)
				{
					//printf("Pre init games\n");
					//InitGameList();
                    //EndFilterList();
					//InitFilterList();
					//printf("Post init games\n");
					nSection = SECTION_GAMELIST;
					EndMainMenu();
					//printf("EndmainMenu\n");

					break;
				}

				// Options
				if(nMenuItem == MENU_MAIN_OPTIONS)
				{
					InitOptionsMenu();
					nSection = SECTION_OPTIONS;
					EndMainMenu();
					break;
				}

				// Load FBNeo RetroArch Modded Core
				if(nMenuItem == MENU_MAIN_FBARL_RA)
				{
					iniWrite(); // save settings
					app.onShutdown();

					char* argv[1] = { 0 };
					argv[0] = NULL;

					char fba_retroarch_path[] = "/dev_hdd0/game/FBNE00123/USRDIR/cores/fb_neo.SELF";
					if(fileExist(fba_retroarch_path))
					{
						sysProcessExitSpawn2( fba_retroarch_path, (const char**)argv, NULL, NULL, 0, 1001, SYS_PROCESS_SPAWN_STACK_SIZE_1M );

					}
					break;
				}

				// Load multiMAN
				if(nMenuItem == MENU_MAIN_MMAN)
				{
					nStatus = STATUS_EXIT_MMAN;
					break;
				}

				if(nMenuItem == MENU_MAIN_IMANAGER)
				{
					nStatus = STATUS_EXIT_IMANAGER;
					break;
				}

				// Exit
				if(nMenuItem == MENU_MAIN_EXIT)
				{
					nStatus = STATUS_EXIT;
					break;
				}
				break;
			}
		}

	}

	// ------------------------------------------------------
	// (O) - CIRCLE

	if (!app.mIsButtPressed[BT_CIRCLE] && app.buttPressedNow[BT_CIRCLE])
	{
		switch(nSection)
		{
			case SECTION_MAIN:
			{
				nStatus = STATUS_EXIT;
				break;
			}

			case SECTION_GAMELIST:
			{
				//iniWrite(); // save settings
                //printf("Iniwrite end\n");
				InitMainMenu();
				//printf("InitMainMenu end\n");
				nSection = SECTION_MAIN;
				//EndGameList();
				//EndFilterList();
				//printf("EndGameList end\n");
				break;
			}

			case SECTION_FILEBROWSER:
			{
				filebrowser->NavigateDirUp();
				break;
			}

			case SECTION_ZIPINFO:
			{
				////EndZipInfoMenu();
				// no need to init GameList here...
				nSection = SECTION_GAMELIST;
				break;
			}

			case SECTION_ROMINFO:
			{
				////EndRomInfoMenu();
				// no need to init GameList here...
				nSection = SECTION_GAMELIST;
				break;
			}

			case SECTION_OPTIONS:
			{
				iniWrite(); // save settings

				EndOptionsMenu();
				InitMainMenu();
				EndFilterList();
				InitFilterList();
				if (nTotalGames == 0) {
                    nStatus = STATUS_ROMSCAN_DLG;
				}
				nSection = SECTION_MAIN;
				break;
			}
		}
	}

	// ------------------------------------------------------
	// (/\) - TRIANGLE

	if(!app.mIsButtPressed[BT_TRIANGLE] && app.buttPressedNow[BT_TRIANGLE])
	{
		switch(nSection)
		{
			case SECTION_GAMELIST:
			{
				if(nFilteredGames < 1) break;
				//if(!fgames[nSelectedGame]->bAvailable) break;

//				if (strcmp(fgames[nSelectedGame]->sysmask, "MASKSNES") == 0)
//					break;

				InitRomInfoMenu();
				nSection = SECTION_ROMINFO;
				break;
			}
			case SECTION_MAIN:
			{
				nSection = SECTION_GAMELIST;
                EndMainMenu();
				break;
			}

		}

	}

	// ------------------------------------------------------
	// [L1]

	if(!app.mIsButtPressed[BT_L1] && app.buttPressedNow[BT_L1])
	{
		switch(nSection)
		{
			case SECTION_GAMELIST:
				PrevSysFilter();
				if(nFilteredGames >= 1) {
					nStatus = STATUS_UPDATEPREVIEW;
				} else {
					nStatus = STATUS_RESETPREVIEW;
				}
				break;

			case SECTION_OPTIONS:
				// ...
				break;
		}
	}

	// ------------------------------------------------------
	// [R1]

	if(!app.mIsButtPressed[BT_R1] && app.buttPressedNow[BT_R1])
	{
		switch(nSection)
		{
			case SECTION_GAMELIST:
				NextSysFilter();
				if(nFilteredGames >= 1) {
					nStatus = STATUS_UPDATEPREVIEW;
				} else {
					nStatus = STATUS_RESETPREVIEW;
				}
				break;

			case SECTION_OPTIONS:
				// ...
				break;
		}
	}

	// ------------------------------------------------------
	// [L2]

	if(((app.mFrame + nSelInputFrame) - app.mFrame) == 3)
	{
		if(app.mIsButtPressed[BT_L2] && app.buttPressedNow[BT_L2])
		{
			switch(nSection)
			{
				case SECTION_MAIN:
				{
                    //printf("L2\n");
				 break;
				}

				case SECTION_GAMELIST:
				{
					nSelectedGame -= 19;

					if(nSelectedGame < 0) {
						nSelectedGame = 0;
					}

					if(nFilteredGames >= 1) {
						nStatus = STATUS_UPDATEPREVIEW;
					} else {
						nStatus = STATUS_RESETPREVIEW;
					}
					break;
				}

				case SECTION_ZIPINFO:
				{
					zipinfo_menu->nSelectedItem -= zipinfo_menu->nListMax;

					if(zipinfo_menu->nSelectedItem < 0) {
						zipinfo_menu->nSelectedItem = 0;
					}
					break;
				}

				case SECTION_ROMINFO:
				{
					rominfo_menu->nSelectedItem -= rominfo_menu->nListMax;

					if(rominfo_menu->nSelectedItem < 0) {
						rominfo_menu->nSelectedItem = 0;
					}
					break;
				}

				case SECTION_FILEBROWSER:
				{
					filebrowser->nSelectedItem -= filebrowser->nListMax;

					if(filebrowser->nSelectedItem < 0) {
						filebrowser->nSelectedItem = 0;
					}
					break;
				}
			}
		}

		// ------------------------------------------------------
		// [R2]

		if(app.mIsButtPressed[BT_R2] && app.buttPressedNow[BT_R2])
		{
            switch(nSection)
			{
				case SECTION_MAIN:
				{
                    /*msgType dialogType;
                    dialogType = (msgType)(MSG_DIALOG_NORMAL | MSG_DIALOG_BTN_TYPE_YESNO | MSG_DIALOG_DISABLE_CANCEL_ON | MSG_DIALOG_DEFAULT_CURSOR_NO);
                    msgDialogOpen2(dialogType,"Do you want to continuE?",Ddialog_handler2,NULL,NULL);
                    dialog_action2 = 0;
                    while(!dialog_action2) {
                        sysUtilCheckCallback();
                        waitFlip();
                        app.Flip();

                    }

                    msgDialogAbort();*/
				 break;
				}
				case SECTION_GAMELIST:
				{
					nSelectedGame += 19;
					if(nSelectedGame > nFilteredGames-1) {
						nSelectedGame = nFilteredGames-1;
					}

					if(nFilteredGames >= 1) {
						nStatus = STATUS_UPDATEPREVIEW;
					} else {
						nStatus = STATUS_RESETPREVIEW;
					}

					break;
				}

				case SECTION_FILEBROWSER:
				{
					filebrowser->nSelectedItem += filebrowser->nListMax;

					if(filebrowser->nSelectedItem > filebrowser->nTotalItem-1) {
						filebrowser->nSelectedItem = filebrowser->nTotalItem-1;
					}
					break;
				}

				case SECTION_ZIPINFO:
				{
					zipinfo_menu->nSelectedItem += zipinfo_menu->nListMax;

					if(zipinfo_menu->nSelectedItem > zipinfo_menu->nTotalItem-1) {
						zipinfo_menu->nSelectedItem = zipinfo_menu->nTotalItem-1;
					}
					break;
				}

				case SECTION_ROMINFO:
				{
					rominfo_menu->nSelectedItem += rominfo_menu->nListMax;

					if(rominfo_menu->nSelectedItem > rominfo_menu->nTotalItem-1) {
						rominfo_menu->nSelectedItem = rominfo_menu->nTotalItem-1;
					}
					break;
				}
			}
		}
	}

	// ------------------------------------------------------
	// [SELECT]

	if(!app.mIsButtPressed[BT_SELECT] && app.buttPressedNow[BT_SELECT])
	{
		switch(nSection)
		{
			case SECTION_FILEBROWSER:
			{
				nSection = SECTION_OPTIONS;
				EndFileBrowser();
				break;
			}
			case SECTION_GAMELIST:
				{
					if (nBurnSelected < 0)
                        break;

                    char *errmsg;
                    char buf[128] = "";
                    int rc = 0;
                    sqlite3 *mdb;
                    hashmap_map *drvmap;
                    drvmap = (hashmap_map *) app.drvMap;
                    FBA_DRV *fba_drv;
                    char key[KEY_MAX_LENGTH];
                    snprintf(key, KEY_MAX_LENGTH, "%s%s", games[nBurnSelected]->system, games[nBurnSelected]->name);
                    //printf("KEY: %s\n", key);
                    if (hashmap_get(drvmap, key, (void**)(&fba_drv)) == MAP_MISSING)
                           printf("ERROR - GAME NOT FOUND\n");

                    //printf("id: %d - %d\n",fba_drv->nGameID, fba_drv->nGameOrderID );
                    snprintf(buf, sizeof(buf), "/dev_hdd0/game/FBNE00123/USRDIR/retro_local.db");
                    mdb = db_open_cpp(buf);
                    if(mdb == NULL) {
                            printf("Open DB failed!\n");
                            return ;
                        }
					if(games[nBurnSelected]->isFavorite) {

                        snprintf(buf, sizeof(buf), "DELETE FROM games_favorite "
                                 "WHERE ord_game_id = %d", fba_drv->nGameOrderID + 1);
                        games[nBurnSelected]->isFavorite = false;
                        fgames[nSelectedGame]->isFavorite = false;
                        fba_drv->isFavorite = false;
					}
					else {
                        snprintf(buf, sizeof(buf), "INSERT INTO games_favorite VALUES(%d)",
                                 fba_drv->nGameOrderID + 1);
                        games[nBurnSelected]->isFavorite = true;
                        fgames[nSelectedGame]->isFavorite = true;
                        fba_drv->isFavorite = true;
					}
					rc = sqlite3_exec(mdb, buf, NULL, NULL, &errmsg);
                        if(rc) {
                            printf( "SQLITE(one_statement) - %s: failed -- %s",
                              buf, errmsg);
                            sqlite3_free(errmsg);
                        }
					rc = sqlite3_close(mdb);
//					if (hashmap_remove(drvmap, key) != MAP_OK)
//                            printf("ERROR - GAME NOT REMOVED\n");
//                    if (hashmap_put(drvmap, fba_drv->key_string, fba_drv) != MAP_OK)
//                            printf("ERROR - GAME NOT INSERTED\n");
                    if (rc)
                        printf("CLOSE PROBLEM: Favorite %d\n",rc);
					break;
				}

		}
	}

	// ------------------------------------------------------
	// [START]

	if (!app.mIsButtPressed[BT_START] && app.buttPressedNow[BT_START]) {
		sysGetCurrentTime(&start_seconds, &start_nseconds);
		return;
	}

	if (app.mIsButtPressed[BT_START] && app.buttPressedNow[BT_START] && start_seconds) {
		sysGetCurrentTime(&seconds, &nseconds);
		if ((seconds - start_seconds) * 1000000000L + (nseconds - start_nseconds) > 1000000000L) {
			//[START] HOLD
			
			start_seconds = 0;
		}
	}
		

	if (app.mIsButtPressed[BT_START] && !app.buttPressedNow[BT_START] && start_seconds) {
		if ((seconds - start_seconds) * 1000000000L + (nseconds - start_nseconds) < 1000000000L) {
			//[START] PRESSED

			switch (nSection)
			{
				case SECTION_FILEBROWSER:
				{
					if (filebrowser->nTotalItem < 1) break;

					int nMenuItem = options_menu->nSelectedItem;

					// Rom Paths (directories)
					if (nFileBrowserType == 0 && filebrowser->item[filebrowser->nSelectedItem]->nType == DT_DIR)
					{
						if (nMenuItem > MASKFAVORITE + MENU_OPT_FILTER_START && nMenuItem <= MASKFAVORITE + MENU_OPT_FILTER_START + 12)
						{
							int nRomPath = nMenuItem - (MASKFAVORITE + MENU_OPT_FILTER_START + 1);
							strcpy(g_opt_szROMPaths[nRomPath], filebrowser->item[filebrowser->nSelectedItem]->szPath);
						}
						nStatus = STATUS_ROMPATH_CHANGED;
					}

					// Input Preset Paths (CFG)
					if (nFileBrowserType == 1 && filebrowser->item[filebrowser->nSelectedItem]->nType == DT_REG)
					{

						if (nMenuItem > MASKFAVORITE + MENU_OPT_FILTER_START + 12 && nMenuItem <= MASKFAVORITE + MENU_OPT_FILTER_START + 12 + 25)
						{
							int nCfgPath = nMenuItem - (MASKFAVORITE + MENU_OPT_FILTER_START + 12 + 1);
							strcpy(g_opt_szInputCFG[nCfgPath], filebrowser->item[filebrowser->nSelectedItem]->szPath);
						}
					}

					nSection = SECTION_OPTIONS;
					EndFileBrowser();

					iniWrite();
					break;
				}

				case SECTION_GAMELIST:
				{
					nStatus = STATUS_ROMSCAN_DLG;
					break;
				}
			}
				
			start_seconds = 0;
		}
	}

	


	// ------------------------------------------------------
	// [L3]

	if(!app.mIsButtPressed[BT_L3] && app.buttPressedNow[BT_L3])
	{
        if (app.ftrack != NULL && g_opt_bMusic) {
            StopAudio();
            if (app.trackID == app.tracks.begin())
                    app.trackID = app.tracks.end();
            --app.trackID;
            fclose(app.ftrack);
            char tpath[256];
            sprintf(tpath, "/dev_hdd0/game/FBNE00123/USRDIR/soundtracks/%s", (*app.trackID).c_str());
            app.ftrack = fopen(tpath, "r");
            if (app.ftrack) {
                    //printf("Play track: %s\n", (*app.trackID).c_str());
                    PlayAudiofd(app.ftrack, 0,  AUDIO_INFINITE_TIME);

            }
        }
	}

	// ------------------------------------------------------
	// [R3]

	if((!app.mIsButtPressed[BT_R3] && app.buttPressedNow[BT_R3]))
	{
        if (app.ftrack != NULL && g_opt_bMusic) {

            StopAudio();
            ++app.trackID;
            if (app.trackID == app.tracks.end()) app.trackID = app.tracks.begin();

            fclose(app.ftrack);
            char tpath[256];
            sprintf(tpath, "/dev_hdd0/game/FBNE00123/USRDIR/soundtracks/%s", (*app.trackID).c_str());
            app.ftrack = fopen(tpath, "r");
            if (app.ftrack) {
                    //printf("Play track: %s\n", (*app.trackID).c_str());
                    PlayAudiofd(app.ftrack, 0,  AUDIO_INFINITE_TIME);

            }
         }

	}

}
