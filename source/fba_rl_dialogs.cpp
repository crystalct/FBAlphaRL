// =======================================================================
// DIALOGS

#include "capp.h"
#include <sysutil/msg.h>
#include <lv2/sysfs.h>
#include <sysutil/sysutil.h>
#include <sys/process.h>
#include <sys/thread.h>
#include <sys/systime.h>


CoreOptions resolveCoreOption(string lower) {
    if (strstr(lower.c_str(), "mame125")) return mame125;
    if (strstr(lower.c_str(), "snes")) return snes;
    if (strstr(lower.c_str(), "megadriv")) return megadriv;
    if (strstr(lower.c_str(), "amiga")) return amiga;
    if (strstr(lower.c_str(), "neocd")) return neocd;
    if (strstr(lower.c_str(), "coleco")) return coleco;
    if (strstr(lower.c_str(), "tg16")) return turbografx16;
    if (strstr(lower.c_str(), "pce")) return pcengine;
    if (strstr(lower.c_str(), "sgx")) return supergrafx;
    //...
    return CoreOptionsInvalid;
}

static void RomScan2(void* arg) {
    int rc = 1;

    //sleep(5);
    sqlite3* mdb;
    char buf[256];
    char szMsg[256] = "";
    snprintf(szMsg, sizeof(szMsg), "Scanning ROM(s), please wait...\n\nNotice: If you have MAME complete romset, be patient, the process could take a few minutes.");
    vs32 progressbar_action = 0;
    char sql[350];
    //char pszFilePath[256];
    u64 tsec1, tnsec1, tsec2, tnsec2;
    hashmap_map* gamesmap, * drvmap;
    gamesmap = (hashmap_map*)app.gamesMap;
    drvmap = (hashmap_map*)app.drvMap;
    FBA_DRV* fba_drv;
    FBA_GAMES* fba_games;
    //fbaRL->nStatus = STATUS_ROMSCAN_END;
    //sysThreadExit(0);
    
    msgDialogOpen2((msgType)(MSG_DIALOG_DOUBLE_PROGRESSBAR | MSG_DIALOG_DISABLE_CANCEL_ON | MSG_DIALOG_MUTE_ON),
        szMsg, NULL, NULL, NULL);
    msgDialogProgressBarSetMsg(MSG_PROGRESSBAR_INDEX0, "");
    msgDialogProgressBarReset(MSG_PROGRESSBAR_INDEX0);
    msgDialogProgressBarSetMsg(MSG_PROGRESSBAR_INDEX1, "");
    msgDialogProgressBarReset(MSG_PROGRESSBAR_INDEX1);
    msgDialogProgressBarInc(MSG_PROGRESSBAR_INDEX0, 0);
    msgDialogProgressBarInc(MSG_PROGRESSBAR_INDEX1, 0);
    //waitFlip();
    //app.Flip();

    
    snprintf(buf, sizeof(buf), "/dev_hdd0/game/FBNE00123/USRDIR/retro.db");

    rc = sqlite3_open_v2(buf, &mdb,
        SQLITE_OPEN_READWRITE,
        NULL);
    
    if (mdb == NULL) {
        printf("Open DB failed!\n");
        return;
    }
    while (!progressbar_action) {

        sqlite3_stmt* stmt;

        snprintf(sql, sizeof(sql), "DELETE FROM games_available");
        rc = sqlite3_prepare_v2(mdb, sql, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            printf("DELETE games_available failed!\n");
            return;
        }
        rc = sqlite3_step(stmt);

        sqlite3_finalize(stmt);


        int coreid;
        char system[64];
        char* lower = NULL;
        std::vector<std::string> files;
        std::vector<std::string> dir;
        std::vector<std::string>::iterator it;
        uint32_t nPaths = sizeof(g_opt_szROMPaths) / 2048;

        size_t found;

        int error;
        int nDrv = 0;
        char* errmsg;
        int barUp = 0;
        int numfiles = 0;
        int barDown = 0;
        int step = 0;
        int deltadir = 0;
        char msg[256] = "";
        char key[KEY_MAX_LENGTH];
        fbaRL->nTotalGames = 0;
        int dup = 0;
        uint32_t x;

        for (x = 0; x < (uint32_t)drvmap->table_size; x++) {  // Reset available
            if (drvmap->data[x].in_use == 1) {
                fba_drv = (FBA_DRV*)drvmap->data[x].data;
                fba_drv->isAvailable = false;
            }
        }
        dir.clear();
        for (x = 0; x < nPaths; x++) {
            if (dirExist(g_opt_szROMPaths[x]))
                dir.push_back(g_opt_szROMPaths[x]);
        }

        //for (x = 0; x < nPaths; x++) {
        while (dir.size() > 0) {
            barUp++;
            //msgDialogProgressBarReset(MSG_PROGRESSBAR_INDEX1);
            sprintf(msg, "%d / %d - Folders.", barUp, (int)dir.size() + deltadir);
            msgDialogProgressBarSetMsg(MSG_PROGRESSBAR_INDEX0, msg);
            msgDialogProgressBarInc(MSG_PROGRESSBAR_INDEX0, (int)round(1.4 + 100 / (dir.size() + deltadir)));
            msgDialogProgressBarReset(MSG_PROGRESSBAR_INDEX1);
            msgDialogProgressBarInc(MSG_PROGRESSBAR_INDEX1, 0);

            //if (dirExist(g_opt_szROMPaths[x])) {
                //printf("DIR: %s size: %d\n", dir.back().c_str(), (int) dir.size());
                files.clear();
                //readDir(g_opt_szROMPaths[x], DIR_NO_DOT_AND_DOTDOT | DIR_FILES | DIR_DIRS, files, dir);
                readDir(dir.back().c_str(), DIR_NO_DOT_AND_DOTDOT | DIR_FILES | DIR_DIRS, files, dir);
                it = files.begin();
                barDown = 0;
                while (it < files.end()) {

                    barDown++;
                    numfiles = static_cast<int>(files.size());
                    step = (int)floor(0.9 + (numfiles / 10));

                    found = (*it).find_last_of(".");
                    if (found > 0) {
                        sysGetCurrentTime(&tsec1, &tnsec1);
                        lower = toLowerCase((char *)dir.back().c_str(), strlen(dir.back().c_str()));
                        switch (resolveCoreOption(lower))
                        {
                            case mame125: {
                                coreid = 3;
                                snprintf(system, sizeof(system), "mame");
                                break;
                            }
                            case snes: {
                                coreid = 2;
                                snprintf(system, sizeof(system), "snes");
                                break;
                            }
                            case megadriv: {
                                coreid = 4;
                                snprintf(system, sizeof(system), "megadrive");
                                break;
                            }
                            case amiga: {
                                coreid = 5;
                                snprintf(system, sizeof(system), "amiga");
                                break;
                            }
                            case neocd: {
                                coreid = 1;
                                snprintf(system, sizeof(system), "neocd");
                                break;
                            }
                            case coleco: {
                                coreid = 1;
                                snprintf(system, sizeof(system), "coleco");
                                break;
                            }
                            case turbografx16: {
                                coreid = 1;
                                snprintf(system, sizeof(system), "turbografx16");
                                break;
                            }
                            case pcengine: {
                                coreid = 1;
                                snprintf(system, sizeof(system), "pcengine");
                                break;
                            }
                            case supergrafx: {
                                coreid = 1;
                                snprintf(system, sizeof(system), "supergrafx");
                                break;
                            }
                                        // handles Option_Invalid and any other missing/unmapped cases
                            default: {
                                coreid = 1;
                                snprintf(system, sizeof(system), "mame");
                                break;
                            }
                        }
                        /*if (strstr(lower, "mame125")) {
                            coreid = 3;
                            snprintf(system, sizeof(system), "mame");
                        }
                        else
                            if (strstr(lower, "snes")) {
                                coreid = 2;
                                snprintf(system, sizeof(system), "snes");
                            }
                            else {
                                if (strstr(lower, "megadriv")) {
                                    coreid = 4;
                                    snprintf(system, sizeof(system), "megadrive");
                                }
                                else {
                                    if (strstr(lower, "amiga")) {
                                        coreid = 5;
                                        snprintf(system, sizeof(system), "amiga");
                                    }
                                    else {
                                        if (strstr(lower, "neocd")) {
                                            coreid = 1;
                                            snprintf(system, sizeof(system), "neocd");
                                        }
                                        else {
                                            coreid = 1;
                                            snprintf(system, sizeof(system), "mame");
                                        }
                                    }
                                }
                            }*/

                        
                        free(lower);
                        lower = toLowerCase((char*)(*it).substr(0, found).c_str(), (*it).substr(0, found).length());
                        snprintf(key, KEY_MAX_LENGTH, "%s%s", system, lower);
                        free(lower);

                        if (((*it).substr((*it).find_last_of(".") + 1) != "bin" && (*it).substr((*it).find_last_of(".") + 1) != "BIN") || strcmp(system, "neocd") != 0)
                            if (hashmap_get(drvmap, key, (void**)(&fba_drv)) >= 0) {
                                fba_games = (FBA_GAMES*)malloc(sizeof(FBA_GAMES));
                                if (fba_games == NULL) {
                                    printf("Exit.... memory error malloc(sizeof(FBA_GAMES)\n");
                                    exit(0);
                                }
                                snprintf(fba_games->szPath, 256, "%s/%s", dir.back().c_str(), (*it).c_str());
                                snprintf(fba_games->key_string, KEY_MAX_LENGTH, "%s%s%d", system, fba_drv->szName, coreid);
                                error = hashmap_put(gamesmap, fba_games->key_string, fba_games);
                                if (error == MAP_OK) {
                                    //printf("Error: %d - %s\n", error, fba_games->szPath);
                                    fbaRL->nTotalGames++;
                                }
                                else
                                    dup++;
                                fba_drv->isAvailable = true;
                                snprintf(sql, sizeof(sql), "INSERT INTO games_available (game_id, path, coreid) "
                                    "values(%d, '%s', %d)", fba_drv->nGameID, fba_games->szPath, coreid);
                                rc = sqlite3_exec(mdb, sql, NULL, NULL, &errmsg);

                            }


                        if (barDown % step == 0) {
                            sysGetCurrentTime(&tsec2, &tnsec2);
                            sprintf(msg, "%d / %d - Files. Time: %f", barDown, static_cast<int>(files.size()), (double)(tsec2 - tsec1) +
                                (tsec2 - tsec1) > 0 ? (double)((long)(1000000000L - tnsec2 + tnsec1)) / 1000000000L : (double)((long)(tnsec2 - tnsec1)) / 1000000000L);
                            msgDialogProgressBarSetMsg(MSG_PROGRESSBAR_INDEX1, msg);
                            msgDialogProgressBarInc(MSG_PROGRESSBAR_INDEX1, (int)round(1.4 + (numfiles / step)));
                            usleep(100000);
                            //printf("%f -%f\n", (float)numfiles / step, round(1.4 + (numfiles / step)));


                            //waitFlip();
                            //app.Flip();
                            //sysUtilCheckCallback();
                            //usleep(1000);
                        }
                    }

                    ++it;
                    numfiles = 0;

                }
                nDrv++;
            
            //}
            msgDialogProgressBarInc(MSG_PROGRESSBAR_INDEX1, 100);
            usleep(200000);
            dir.pop_back();
            deltadir++;
        }
        
        snprintf(sql, sizeof(sql), "select count(g.id) from games g left join games_available ga on g.id = ga.game_id");
        rc = sqlite3_prepare_v2(mdb, sql, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            printf("Query failed!\n");
            sqlite3_finalize(stmt);
            sqlite3_close(mdb);
            return;
        }
        rc = sqlite3_step(stmt);
        app.nTotalGamesWithDup = sqlite3_column_int(stmt, 0);
        app.nMissingGames = app.nTotalGamesWithDup - fbaRL->nTotalGames - dup;

        sqlite3_finalize(stmt);
        rc = sqlite3_close(mdb);
        //printf("nTotalGamesWithDup: %d - nTotalGames: %d - nMissingGames: %d\n", app.nTotalGamesWithDup, fbaRL->nTotalGames, app.nMissingGames);

        if (rc)
            printf("CLOSE PROBLEM:select count(*) %d\n", rc);

        break;
    }
    hashmap_free(app.drvMap);
    hashmap_free(app.gamesMap);
    app.drvMap = hashmap_new();
    app.gamesMap = hashmap_new();
    app.InitDB();
    fbaRL->ParseGameListCache();
    fbaRL->bProcessingGames = false;
    fbaRL->nStatus = STATUS_ROMSCAN_END;
    //msgDialogAbort();

    //waitFlip();
    sysThreadExit(0);
}

static vs32 dialog_action = 0;
static int dialog = 0;

/*
static void DlgCallbackFunction(msgButton buttonType,void *usrData)
{
	vs32 *p_action = (vs32 *)usrData;
	switch(buttonType) {
		case MSG_DIALOG_BTN_OK:
			*p_action = 1;
			break;
		case MSG_DIALOG_BTN_NO:
		case MSG_DIALOG_BTN_ESCAPE:
			*p_action = 2;
			break;
		case MSG_DIALOG_BTN_NONE:
			*p_action = -1;
			break;
		default:
			break;
	}
}*/

static void dialog_handler(msgButton button,void *usrData) {

	int nDialog =  *((int*)(usrData));
	//printf("Button: %d - %d\n", button,nDialog);

    //printf("Dialog: button: %d - Dialog: %d\n", (int)button, nDialog);
	switch( button )
	{
		case MSG_DIALOG_BTN_YES:
		{
			if (nDialog == STATUS_MISSING_CORE_2 || nDialog == STATUS_MISSING_CORE_4) {
                msgDialogAbort();
                break;
			}

			if(nDialog == STATUS_ROMSCAN_DLG)
			{
				msgDialogAbort();
				/////cellMsgDialogAbort();

				iniWrite();			// save settings

				// delete old cache
				sqlite3 *mdb;
				char buf[256];
				int rc;
				char *errmsg;
				snprintf(buf, sizeof(buf), "/dev_hdd0/game/FBNE00123/USRDIR/retro.db");
                rc = sqlite3_open_v2(buf, &mdb,
                               SQLITE_OPEN_READWRITE,
                               NULL);
                if(mdb == NULL) {
                        printf("Open DB failed!\n");
                        return ;
                    }

                //sqlite3_stmt *stmt;
                snprintf(buf, sizeof(buf), "DELETE FROM games_available");
                rc = sqlite3_exec(mdb, buf, NULL, NULL, &errmsg);
                        if(rc) {
                            printf( "DELETE games_available failed - %s: failed -- %s\n",
                              buf, errmsg);
                            sqlite3_free(errmsg);
                            sqlite3_close(mdb);
                        }

                //printf("\nOld CACHE deleted\n");
                rc = sqlite3_close(mdb);
                if (rc)
                    printf("CLOSE PROBLEM POST DELETE: %d\n",rc);

				//sysFsUnlink("/dev_hdd0/game/FBNE00123/USRDIR/FBA.GAMELIST.CACHE.DAT");

				////cellFsUnlink("/dev_hdd0/game/FBNE00123/USRDIR/FBA.GAMELIST.CACHE.DAT");

				fbaRL->RefreshGameList();	// Refresh gamelist
                //printf("EndFilterList start\n");
                //fbaRL->EndFilterList();
                //printf("InitGameLIST\n");
                //InitGameList();
                //printf("InitFILTER start\n");
                //InitFilterList();
                fbaRL->nStatus = STATUS_ROMSCAN_START;
				break;
			}

			if(nDialog == STATUS_EXIT_MMAN)
			{
				msgDialogAbort();


				char path[] = "/dev_hdd0/game/BLES80608/USRDIR/RELOAD.SELF";
				if(fileExist(path))
				{
					iniWrite(); // save settings

					app.onShutdown();
					sysProcessExitSpawn2( path, NULL, NULL, NULL, 0, 1001, SYS_PROCESS_SPAWN_STACK_SIZE_1M );
				}
				//fbaRL->EndMainMenu();
				break;
			}


			if(nDialog == STATUS_EXIT_IMANAGER)
			{
				msgDialogAbort();


				char path[] = "/dev_hdd0/game/IRISMAN00/USRDIR/RELOAD.SELF";
				if(fileExist(path))
				{
					iniWrite(); // save settings

					app.onShutdown();
					sysProcessExitSpawn2( path, NULL, NULL, NULL, 0, 1001, SYS_PROCESS_SPAWN_STACK_SIZE_1M );
				}
				//fbaRL->EndMainMenu();
				break;
			}

			 /*////
			if(nDialog == STATUS_EXIT_FBARL_RA)
			{
				cellMsgDialogAbort();

				iniWrite(); // save settings

				app.onShutdown();

				char fba_rl_path[]	= "/dev_hdd0/game/FBNE00123/USRDIR/RELOAD.SELF";
				char rom_path[]		= "DUMMY_ARG";
				char preset_cfg[]	= "DUMMY_ARG";
				char aspect_ratio[]	= "DUMMY_ARG";

				if(fbaRL->FileExist(fba_rl_path))
				{
					fbaRL->LaunchFBACore(
						(char*)rom_path,
						(char*)fba_rl_path,
						(char*)preset_cfg,
						(char*)aspect_ratio,
						(char*)(g_opt_bUseAltMenuKeyCombo ? "yes" : "no"),
						(char*)(g_opt_bUseUNIBIOS ? "yes" : "no")  //CRYSTAL
					);
				}
				fbaRL->EndMainMenu();
				break;
			}
            */
			if(nDialog == STATUS_EXIT)
			{
				////cellMsgDialogAbort();
                //msgDialogAbort();
				iniWrite(); // save settings
				app.onShutdown();

				fbaRL->EndMainMenu();
				exit(0);
				break;
			}
			break;
		}

		case MSG_DIALOG_BTN_NO:
		    if (nDialog == STATUS_ROMSCAN_DLG)
                fbaRL->ResetPreviewImage();
		case MSG_DIALOG_BTN_ESCAPE:
		case MSG_DIALOG_BTN_NONE:

			break;

		default:

			break;
	}

	dialog_action = 1;
}

void c_fbaRL::DlgDisplayFrame()
{
    //printf("Status; %d\n", nStatus);
    switch(nStatus)
	{
		case STATUS_ROMPATH_CHANGED: // TODO!!!!!!
		{
			//::cellMsgDialogOpen2(
			//	CELL_MSGDIALOG_TYPE_SE_TYPE_NORMAL,
			//	"Remeber to rescan ROMs after you finish configuring all paths,\nyou can do so at the Game List section.",
			//	DlgCallbackFunction, (void*)STATUS_ROMPATH_CHANGED, NULL
			//);

			// Force Re-Scan
			// delete old cache
			//sysFsUnlink("/dev_hdd0/game/FBNE00123/USRDIR/FBA.GAMELIST.CACHE.DAT");
				sqlite3 *mdb;
				char buf[256];
				int rc;
				char *errmsg;
				snprintf(buf, sizeof(buf), "/dev_hdd0/game/FBNE00123/USRDIR/retro.db");
                rc = sqlite3_open_v2(buf, &mdb,
                               SQLITE_OPEN_READWRITE,
                               NULL);
                if(mdb == NULL) {
                        printf("Open DB failed!\n");
                        return ;
                    }

                //sqlite3_stmt *stmt;
                snprintf(buf, sizeof(buf), "DELETE FROM games_available");
                rc = sqlite3_exec(mdb, buf, NULL, NULL, &errmsg);
                        if(rc) {
                            printf( "DELETE games_available failed - %s: failed -- %s\n",
                              buf, errmsg);
                            sqlite3_free(errmsg);
                            sqlite3_close(mdb);
                        }


                rc = sqlite3_close(mdb);
                if (rc)
                    printf("CLOSE PROBLEM POST DELETE: %d\n",rc);

				//sysFsUnlink("/dev_hdd0/game/FBNE00123/USRDIR/FBA.GAMELIST.CACHE.DAT");
				//printf("\nOld CACHE deleted\n");
				nTotalGames = 0;
                break;
		}

		case STATUS_ROMSCAN_DLG:
		{
			//int i = STATUS_ROMSCAN_DLG;
            dialog = STATUS_ROMSCAN_DLG;
			//printf("Pre dialod %d\n",nStatus);
			msgDialogOpen2((msgType)((MSG_DIALOG_BTN_TYPE_YESNO | MSG_DIALOG_DEFAULT_CURSOR_NO)),
                  "Do you want to Rescan all configured paths for ROM(s) ?",
                  dialog_handler,(void *)&dialog,NULL);

            /*dialog_action = 0;
            while(dialog_action == 0) {
                    sysUtilCheckCallback();
                    waitFlip();
                    app.Flip();
            }
            msgDialogAbort();*/
//            ::cellMsgDialogOpen2(
//				CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO|
//				CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_NO,
//				"Do you want to Rescan all configured paths for ROM(s) ?",
//				ExitDlgCallbackFunction, (void*)STATUS_ROMSCAN_DLG, NULL
//			);
            //printf("Post dialod %d\n",nStatus);
			break;
		}

        case STATUS_MISSING_CORE_2:
        {
            dialog = STATUS_MISSING_CORE_2;
            msgDialogOpen2((msgType)((MSG_DIALOG_BTN_TYPE_OK
                                |MSG_DIALOG_DISABLE_CANCEL_ON)),
                                "Core SNES9X is missing. Install correct upgrade pack.",dialog_handler,(void *)&dialog,NULL);
            break;
        }

        case STATUS_MISSING_CORE_4:
        {
            dialog = STATUS_MISSING_CORE_4;
            msgDialogOpen2((msgType)((MSG_DIALOG_BTN_TYPE_OK
                                |MSG_DIALOG_DISABLE_CANCEL_ON)),
                                "Core GENESIS PLUS GX is missing. Install correct upgrade pack.",dialog_handler,(void *)&dialog,NULL);
            break;
        }

        case STATUS_MISSING_CORE_5:
        {
            dialog = STATUS_MISSING_CORE_5;
            msgDialogOpen2((msgType)((MSG_DIALOG_BTN_TYPE_OK
                                |MSG_DIALOG_DISABLE_CANCEL_ON)),
                                "Core E-UAE is missing. Install correct upgrade pack.",dialog_handler,(void *)&dialog,NULL);
            break;
        }

		case STATUS_MISSING_GAME_DLG:
		{
			//int i = STATUS_ROMSCAN_DLG;
            dialog = STATUS_ROMSCAN_DLG;
			msgDialogOpen2((msgType)((MSG_DIALOG_BTN_TYPE_YESNO | MSG_DIALOG_DEFAULT_CURSOR_NO)),
                "Error: There was a problem locating the specified game, if you had it on a USB Storage Device, \n"
                "make sure it is properly connected.\n\n"
				"Do you want to Re-Scan all configured paths for ROM(s) ?",
                dialog_handler,(void *)&dialog,NULL);

            /*dialog_action = 0;
            while(dialog_action == 0) {
                    sysUtilCheckCallback();
                    //waitFlip();
                    app.Flip();
            }
            msgDialogAbort();*/
//            ::cellMsgDialogOpen2(
//				CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO|
//				CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_NO,
//				"Error: There was a problem locating the specified game, if you had it on a USB Storage Device, \n"
//				"make sure it is properly connected.\n\n"
//				"Do you want to Re-Scan all configured paths for ROM(s) ?",
//				ExitDlgCallbackFunction, (void*)STATUS_ROMSCAN_DLG, NULL
//			);
			break;
		}


		case STATUS_CORRUPT_APP_DLG:
		{
			//int i = STATUS_EXIT;
            //dialog = STATUS_ROMSCAN_DLG;
            dialog = STATUS_EXIT;
			msgDialogOpen2((msgType)((MSG_DIALOG_NORMAL
                                |MSG_DIALOG_BTN_TYPE_OK
                                |MSG_DIALOG_DISABLE_CANCEL_ON)),
                                "Error: FB NEO RL installation is corrupted, please re-install it.",
                                dialog_handler,(void *)&dialog,NULL);

            /*dialog_action = 0;
            while(dialog_action == 0) {
                    sysUtilCheckCallback();
                    waitFlip();
                    app.Flip();

            }
            msgDialogAbort();*/
//            ::cellMsgDialogOpen2(
//				CELL_MSGDIALOG_TYPE_SE_TYPE_NORMAL
//				|CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK
//				|CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON
//				|CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_OK,
//				"Error: FB NEO RL installation is corrupted, please re-install it.",
//				ExitDlgCallbackFunction, (void*)STATUS_EXIT, NULL
//			);

			break;
		}

		case STATUS_EXIT:
		{
            //int i = STATUS_EXIT;
            dialog = STATUS_EXIT;
			msgDialogOpen2((msgType)((MSG_DIALOG_NORMAL | MSG_DIALOG_BTN_TYPE_YESNO | MSG_DIALOG_DISABLE_CANCEL_ON | MSG_DIALOG_DEFAULT_CURSOR_NO)),
                  "Do you want to exit FB NEO RL and return to XMB ?",
                  dialog_handler,(void *)&dialog,NULL);

			/*////::cellMsgDialogOpen2(
				CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO|
				CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_NO,
				"Do you want to exit FB NEO RL and return to XMB ?",
				ExitDlgCallbackFunction, (void*)STATUS_EXIT, NULL
			);*/
			break;
		}

		case STATUS_EXIT_MMAN:
		{
			dialog = STATUS_EXIT_MMAN;
			msgDialogOpen2((msgType)((MSG_DIALOG_NORMAL | MSG_DIALOG_BTN_TYPE_YESNO | MSG_DIALOG_DISABLE_CANCEL_ON | MSG_DIALOG_DEFAULT_CURSOR_NO)),
				"Do you want to exit and load \"multiMAN\" ?",
				dialog_handler,(void *)&dialog,NULL);
			break;
		}

		case STATUS_EXIT_IMANAGER:
		{
			dialog = STATUS_EXIT_IMANAGER;
			msgDialogOpen2((msgType)((MSG_DIALOG_NORMAL | MSG_DIALOG_BTN_TYPE_YESNO | MSG_DIALOG_DISABLE_CANCEL_ON | MSG_DIALOG_DEFAULT_CURSOR_NO)),
				"Do you want to exit and load \"Iris Manager\" ?",
				dialog_handler,(void *)&dialog,NULL);
			break;
		}
        /*////
		case STATUS_EXIT_FBARL_RA:
		{
			::cellMsgDialogOpen2(
				CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO|
				CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_NO,
				"Do you want to exit and load FB NEO RetroArch (modified core / fb_neo.SELF) ?",
				ExitDlgCallbackFunction, (void*)STATUS_EXIT_FBARL_RA, NULL
			);
			break;
		}

		case STATUS_EXIT_MMAN:
		{
			::cellMsgDialogOpen2(
				CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO|
				CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_NO,
				"Do you want to exit and load \"multiMAN\" ?",
				ExitDlgCallbackFunction, (void*)STATUS_EXIT_MMAN, NULL
			);
			break;
		}

		case STATUS_EXIT_IMANAGER:
		{
			::cellMsgDialogOpen2(
				CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO|
				CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_NO,
				"Do you want to exit and load \"Iris Manager\" ?",
				ExitDlgCallbackFunction, (void*)STATUS_EXIT_IMANAGER, NULL
			);
			break;
		}
        */
		case STATUS_UPDATEPREVIEW:
		{
			//printf("UPDATEPREVIEW nSelectedGame %d\n",nSelectedGame);
			//printf("fgames[nSelectedGame]->GameID %d\n",fgames[nSelectedGame]->GameID);
			nBurnSelected = fgames[nSelectedGame]->GameID;
			//UpdateBurnSelected(fgames[nSelectedGame]->zipname);
//			printf("AAA %s - %d - %d - BURNSELECTED = %d \n",
//                    fgames[nSelectedGame]->title, nBurnSelected, fgames[nSelectedGame]->nGame,
//                    nBurnSelected);
			UpdatePreviewImage();
			//printf("Dialog UpdatePreviewImage done\n");
			break;
		}

		case STATUS_RESETPREVIEW:
		{
			ResetPreviewImage();
			break;
		}

		case STATUS_ROMSCAN_START:
		{
			nStatus = STATUS_ROMSCAN_WORKING;
			s32 ret;
			sys_ppu_thread_t id;
			u64 prio = 0;
			
			size_t stacksize = 0x300000;
			void* threadarg = (void*)0x1337;
			ret = sysThreadCreate(&id, 
				RomScan2, 
				threadarg, 
				prio, 
				stacksize, 
				THREAD_JOINABLE, 
                (char *)"RomScanThread");
			if (ret)
			    printf("Thread romscan error!\n");
           
			break;
		}

		case STATUS_ROMSCAN_END:
		{
			//cellMsgDialogAbort();
			//printf("SaveGameListCache\n");
			//SaveGameListCache();

			//printf("ENd filter list\n");
			fbaRL->EndFilterList();

		    //printf("InitFilterList\n");
			fbaRL->InitFilterList();
            //printf("UpdatePreviewImage\n");
			fbaRL->UpdatePreviewImage();
			//printf("Status normal\n");
            msgDialogAbort();
			nStatus = STATUS_NORMAL;
			break;
		}
	}
    if (nStatus != STATUS_ROMSCAN_END && nStatus != STATUS_ROMSCAN_START && nStatus != STATUS_ROMSCAN_WORKING)
            nStatus = STATUS_NORMAL;
    //printf("Status: %d\n", nStatus);
}
