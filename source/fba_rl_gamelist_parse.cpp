// =======================================================================
// GAMELIST (PARSING)

#include "capp.h"
#include <sysutil/msg.h>
#include <vector>
#include <string>
#include <algorithm>
#include <lv2/sysfs.h>
#include <sys/dirent.h>
#include <sys/stat.h>
#include "sqlite.h"
#include "misc.h"
#include <sys/systime.h>
#include <sysutil/sysutil.h>
#include <sys/thread.h>


#define DIR_SKIP_DOT			1			//dont include "."
#define DIR_SKIP_DOTDOT			2			//dont include ".."
#define DIR_FILES				4			//include files
#define DIR_DIRS				0x10		//include directories
#define DIR_SKIP_HIDDEN			0x20		//skip entries starting with "." besides dot and dotdot
#define DIR_NO_DOT_AND_DOTDOT	( DIR_SKIP_DOT | DIR_SKIP_DOTDOT )


#define _GETTAG_STR_VALUE(_opentag, _closetag, _outstr)	\
	fgets(pszLine, 2048, fp);							\
	pch = strstr(pszLine, _opentag);					\
	if(pch)	{											\
		char* pszOption =  NULL;						\
		pszOption = (char*)malloc(2048+1);				\
		memset(pszOption, 0, 2048+1);					\
		strncpy(pszOption, pch+(strlen(_opentag)), 2048);\
		pch = strstr(pszOption, _closetag);				\
		pszOption[(int)(pch-pszOption)] = 0;			\
		strcpy(_outstr, pszOption);						\
		SAFE_FREE(pszOption)							\
	}

#define _GETTAG_INT_VALUE(_opentag, _closetag, _outint)	\
	fgets(pszLine, 2048, fp);							\
	pch = strstr(pszLine, _opentag);					\
	if(pch)	{											\
		char* pszOption =  NULL;						\
		pszOption = (char*)malloc(2048+1);				\
		memset(pszOption, 0, 2048+1);					\
		strncpy(pszOption, pch+(strlen(_opentag)), 2048);\
		pch = strstr(pszOption, _closetag);				\
		pszOption[(int)(pch-pszOption)] = 0;			\
		sscanf(pszOption, "%"PRIu64, _outint);			\
		SAFE_FREE(pszOption)							\
	}

#define _GETTAG_BOOL_VALUE(_opentag, _closetag, _outbool)\
	fgets(pszLine, 2048, fp);							\
	pch = strstr(pszLine, _opentag);					\
	if(pch)	{											\
		char* pszOption =  NULL;						\
		pszOption = (char*)malloc(2048+1);				\
		memset(pszOption, 0, 2048+1);					\
		strncpy(pszOption, pch+(strlen(_opentag)), 2048);\
		pch = strstr(pszOption, _closetag);				\
		pszOption[(int)(pch-pszOption)] = 0;			\
		if(strcmp(pszOption, "yes")==0) {_outbool = true;}	\
		if(strcmp(pszOption, "no")==0)	{_outbool = false;}	\
		SAFE_FREE(pszOption)							\
	}


int c_fbaRL::ParseGameListCache()
{
	//printf("START ParseGameListCache\n");
	bProcessingGames = true;
    int row = 0;
    char key[KEY_MAX_LENGTH];
    bool flag_available;
    char* str = NULL;
    int ret = 0;

    // New Parse List Chace
    hashmap_map *gamesmap, *drvmap;
    gamesmap =  (hashmap_map *) app.gamesMap;
    drvmap = (hashmap_map *) app.drvMap;
    FBA_DRV *fba_drv;
    FBA_GAMES *fba_games;
    nTotalGames = gamesmap->size;

    if (nTotalGames == 0) {

                bProcessingGames = false;
                return 0;
    }

    int i, j;
    //nMissingGames = 0;
    for (i=0; i < drvmap->table_size; i++)
    {
        if (drvmap->data[i].data != NULL) {
                fba_drv = (FBA_DRV *)drvmap->data[i].data;
                games[row] = new c_game(row);
                //1//snprintf(games[row]->title, 256, "%s", fba_drv->szTitle);
                strcpy(games[row]->name, fba_drv->szName);
                //snprintf(games[row]->name, 128, "%s", fba_drv->szName);		// title
                strcpy(games[row]->sysmask, fba_drv->szSystemFilter);
                //snprintf(games[row]->sysmask, 32, "%s", fba_drv->szSystemFilter);	// sys filter
                snprintf(games[row]->company, 96, "%s", fba_drv->szCompany);	// sys filter
                snprintf(games[row]->year, 8, "%s", fba_drv->szYear);	// sys filter
                snprintf(games[row]->system, 32, "%s", fba_drv->szSystem);	// sys filter
                snprintf(games[row]->subsystem, 32, "%s", fba_drv->szSubSystem);	// sys filter
                snprintf(games[row]->resolution, 11, "%s", fba_drv->szResolution);	// sys filter
                snprintf(games[row]->aspectratio, 8, "%s", fba_drv->szAspectRatio);	// sys filter
                //fbaRL->games[row]->parent_id = sqlite3_column_int(stmt, COL08);
                games[row]->players = fba_drv->nMaxPlayers;
                games[row]->def_core_id = fba_drv->nDefCoreID;

                games[row]->nSize = i;				// Hasp map position
                games[row]->GameID = fba_drv->nGameID;
                games[row]->bAvailable = fba_drv->isAvailable;
                games[row]->isClone = fba_drv->isClone;
                games[row]->core_id = 0;
                if (fba_drv->isClone)
                    snprintf(games[row]->parent_name, 128,  "%s", fba_drv->szParent);

                games[row]->isFavorite = fba_drv->isFavorite;

                if (!fba_drv->isAvailable) {
                        //nMissingGames++;
                        row++;

                }
                else {
                    flag_available = false;
                    for (j=1; j<=TOTAL_CORES ; j++) {
                            snprintf(key, KEY_MAX_LENGTH, "%s%s%d", fba_drv->szSystem, fba_drv->szName, j);
                            ret = hashmap_get(gamesmap, key, (void**)(&fba_games));
                            if (ret >= 0) {
                                if (flag_available) {
                                    games[row] = new c_game(row);
                                    //1//snprintf(games[row]->title, 256, "%s", fba_drv->szTitle);
                                    snprintf(games[row]->name, 128, "%s", fba_drv->szName);		// title
                                    snprintf(games[row]->sysmask, 32, "%s", fba_drv->szSystemFilter);	// sys filter
                                    snprintf(games[row]->company, 96, "%s", fba_drv->szCompany);	// sys filter
                                    snprintf(games[row]->year, 8, "%s", fba_drv->szYear);	// sys filter
                                    snprintf(games[row]->system, 32, "%s", fba_drv->szSystem);	// sys filter
                                    snprintf(games[row]->subsystem, 32, "%s", fba_drv->szSubSystem);	// sys filter
                                    snprintf(games[row]->resolution, 11, "%s", fba_drv->szResolution);	// sys filter
                                    snprintf(games[row]->aspectratio, 8, "%s", fba_drv->szAspectRatio);	// sys filter
                                    //fbaRL->games[row]->parent_id = sqlite3_column_int(stmt, COL08);
                                    games[row]->players = fba_drv->nMaxPlayers;
                                    games[row]->def_core_id = fba_drv->nDefCoreID;
                                    games[row]->core_id = j;
                                    games[row]->nSize = i;				// hash map position
                                    games[row]->GameID = fba_drv->nGameID;
                                    games[row]->bAvailable = fba_drv->isAvailable;
                                    games[row]->isClone = fba_drv->isClone;
                                    if (fba_drv->isClone)
                                            snprintf(games[row]->parent_name, 128,  "%s", fba_drv->szParent);
                                    games[row]->isFavorite = fba_drv->isFavorite;
                                }
                                else
                                    games[row]->core_id = j;
                                flag_available = true;
                                //snprintf(fbaRL->games[row]->path, 256, "%s", fba_games->szPath);
                                str = strrchr(fba_games->szPath,'/');
                                if (str != NULL)
                                        str++;
                                sprintf(fbaRL->games[row]->zipname, "%s", str);
                                row++;
                            }

                    }


                }






        }

	}
	app.nTotalGamesWithDup = row;
	return 1;

    //OLD PART TO BE REMOVED
    //End New Parse List Chace
    /*
	//check if there is availables games
	int rc = 0;
	sqlite3 *mdb;
	char buf[512];
	snprintf(buf, sizeof(buf), "/dev_hdd0/game/FBNE00123/USRDIR/retro.db");

    //mdb = db_open_cpp(buf);
    rc = sqlite3_open_v2(buf, &mdb,
			   SQLITE_OPEN_READWRITE,
		       NULL);
    if(mdb == NULL) {
            printf("Open DB failed!\n");
            return 1;
    }
    sqlite3_stmt *stmt;


            snprintf(buf, sizeof(buf), "SELECT count(game_id) FROM games_available");
            rc = sqlite3_prepare_v2(mdb, buf, -1, &stmt, 0);
            if( rc != SQLITE_OK ) {
                    printf("Query failed!\n");
                    return 1;
            }
            rc = sqlite3_step(stmt);
            nTotalGames = sqlite3_column_int(stmt, 0);
            //printf("Games available: %d\n", nTotalGames);
            sqlite3_finalize(stmt);
            if (nTotalGames == 0) {

                rc = sqlite3_close(mdb);
                if (rc)
                    printf("CLOSE PROBLEM: count(game_id) %d\n",rc);
                //InitFilterList();
                bProcessingGames = false;
                return 0;
            }


    snprintf(buf, sizeof(buf), "select g.id COL00, g.title COL01, g.order_id  COL02, ga.path COL03, "
                "s.sysmask COL04, g.cloneof COL05, g.aspectratio COL06, g.name COL07, "
                "g.publisher COL08, g.year COL09, g.subsystem COL10, g.players COL11, g.resolution COL12, "
                "g.aspectratio COL13, g.def_core_id COL14 from games g inner join systems s "
                "on g.subsystem = s.name left join games_available ga on g.id = ga.game_id "
                //"left join games_favorite fa on g.order_id = fa.ord_game_id order by order_id");
                "order by order_id");
    rc = sqlite3_prepare_v2(mdb, buf, -1, &stmt, 0);
    if( rc != SQLITE_OK ) {
            printf("Query failed!\n");
            sqlite3_finalize(stmt);
            sqlite3_close(mdb);
            return 1;
    }
    rc = sqlite3_step(stmt);
    row = 0;
    nTotalGames = 0;

    nMissingGames = 0;
    while (rc == SQLITE_ROW) {
                fbaRL->games[row] = new c_game(row);
                sprintf(fbaRL->games[row]->title, "%s", (char *)sqlite3_column_text(stmt, COL01));
                sprintf(fbaRL->games[row]->name, "%s", (char *)sqlite3_column_text(stmt, COL07));		// title
                sprintf(fbaRL->games[row]->sysmask, "%s", (char *)sqlite3_column_text(stmt, COL04));	// sys filter
                snprintf(fbaRL->games[row]->company, 256, "%s", (char *)sqlite3_column_text(stmt, COL08) == NULL ? "" : (char *)sqlite3_column_text(stmt, COL08));	// sys filter
                snprintf(fbaRL->games[row]->year, 5, "%s", (char *)sqlite3_column_text(stmt, COL09));	// sys filter
                snprintf(fbaRL->games[row]->system, 256, "%s", (char *)sqlite3_column_text(stmt, COL10));	// sys filter
                snprintf(fbaRL->games[row]->resolution, 10, "%s", (char *)sqlite3_column_text(stmt, COL12));	// sys filter
                snprintf(fbaRL->games[row]->aspectratio, 7, "%s", (char *)sqlite3_column_text(stmt, COL13));	// sys filter
                //fbaRL->games[row]->parent_id = sqlite3_column_int(stmt, COL08);
                fbaRL->games[row]->players = sqlite3_column_int(stmt, COL11);
                fbaRL->games[row]->def_core_id = sqlite3_column_int(stmt, COL14);

                fbaRL->games[row]->nSize = 0;				// Size in bytes
                fbaRL->games[row]->GameID = sqlite3_column_int(stmt, COL02) - 1;
                if ((char *)sqlite3_column_text(stmt, COL03) == NULL) {
                        nMissingGames++;
                        fbaRL->games[row]->bAvailable = false;
                }
                else {
                    fbaRL->games[row]->bAvailable = true;
                    nTotalGames++;
                    snprintf(fbaRL->games[row]->path, 256, "%s", (char *)sqlite3_column_text(stmt, COL03));
                    str = strrchr((char *)sqlite3_column_text(stmt, COL03),'/');
                    if (str != NULL)
                            //str = ++str;
                            str++;
                    sprintf(fbaRL->games[row]->zipname, "%s", str);

                }
                if (sqlite3_column_text(stmt, COL05) != NULL) { // is a clone?
                        fbaRL->games[row]->isClone = true;
                        snprintf(fbaRL->games[row]->parent_name, 128, "%s", (char *)sqlite3_column_text(stmt, COL05));

                }
                else
                        fbaRL->games[row]->isClone = false;

                //if ((char *)sqlite3_column_text(stmt, COL14) == NULL)
                fbaRL->games[row]->isFavorite = false;
                //else
                        //fbaRL->games[row]->isFavorite = true;

                row++;

                rc = sqlite3_step(stmt);

    }
    app.nTotalGamesWithDup = row;
    sqlite3_finalize(stmt);
    rc = sqlite3_close(mdb);

    if (rc)
        printf("CLOSE PROBLEM:END ParseGameListCache %d\n",rc);
//    printf("END ParseGameListCache\n");
    //InitFilterList();

    snprintf(buf, sizeof(buf), "/dev_hdd0/game/FBNE00123/USRDIR/retro_local.db");

    //mdb = db_open_cpp(buf);
    rc = sqlite3_open_v2(buf, &mdb,
			   SQLITE_OPEN_READONLY,
		       NULL);
    if(mdb == NULL) {
            printf("Open DB failed!\n");
            return 1;
    }

    snprintf(buf, sizeof(buf), "SELECT ord_game_id COL00 FROM games_favorite");
    rc = sqlite3_prepare_v2(mdb, buf, -1, &stmt, 0);
    if( rc != SQLITE_OK ) {
            printf("Query failed!\n");
            sqlite3_finalize(stmt);
            sqlite3_close(mdb);
            return 1;
    }
    rc = sqlite3_step(stmt);
    row = 0;

    while (rc == SQLITE_ROW) {
        row = sqlite3_column_int(stmt, COL00);
        fbaRL->games[row]->isFavorite = true;
        rc = sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    rc = sqlite3_close(mdb);

    if (rc)
        printf("CLOSE PROBLEM:END ParseGameListCache %d\n",rc);
    return 1;
*/
}

int c_fbaRL::SaveGameListCache()
{

	return 0;
}

int c_fbaRL::ParseGameList()
{
	//printf("ParseGameList START\n");
	bProcessingGames = true;
	fbaRL->nStatus = STATUS_ROMSCAN_START;
	//fbaRL->nStatus = STATUS_ROMSCAN_DLG;
	return 1;
}

void c_fbaRL::RefreshGameList()
{
	//printf("EndGameLIST\n");
	EndGameList();
	EndFilterList();
	//printf("InitGameLIST\n");
	InitGameList();
	//printf("InitFILTER start\n");
	InitFilterList();
	//printf("nBurnSelected: %d\n", nBurnSelected);
}

void c_fbaRL::InitGameList()
{
	nTotalGames = 0;
	//nMissingGames = 0;

	bProcessingGames = true;
    //printf("Pre malloc games\n");
	games = (c_game**)malloc(sizeof(c_game) * MAX_GAMES);
	//printf("Post malloc games\n");
	if (games == NULL) {
        printf("Exit memory fail\n");
        exit(0);
	}
	//memset(games, 0, sizeof(c_game) * MAX_GAMES);

    // Check for FBA.GAMELIST.CACHE.DAT...
	if(!ParseGameListCache())
	{
		// Not found, rescan ROMs
		ParseGameList();
	}
	//InitFilterList();


}

void c_fbaRL::EndGameList()
{
	bProcessingGames = true;
    
	for(unsigned n = 0; n < app.nTotalGamesWithDup; n++)
	{
		if (games[n] != NULL) {
            //delete (games[n]);
            SAFE_DELETE(games[n])

            }
	}
	SAFE_FREE(games);
    app.remakegamesMaps();
    //printf("hashmap_free(app.gamesMap)\n");


    nTotalGames = 0;


	bProcessingGames = false;

	//EndFilterList();
}






