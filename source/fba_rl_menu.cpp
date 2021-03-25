// =======================================================================
// UI / MENUS

#include "capp.h"
#include "misc.h"
#include <zip.h>

void c_fbaRL::InitMainMenu()
{
	main_menu = new c_Menu(7);

	main_menu->AddItem((char*)"GAME LIST");
	main_menu->AddItem((char*)"OPTIONS");
	main_menu->AddItem((char*)"LOAD \"FB Neo Core\" MOD");
	main_menu->AddItem((char*)"LOAD \"Iris Manager\"");
	main_menu->AddItem((char*)"LOAD \"multiMAN\"");
	main_menu->AddItem((char*)"EXIT");
}

void c_fbaRL::EndMainMenu()
{
	SAFE_DELETE(main_menu)
}

void c_fbaRL::InitOptionsMenu()
{
	options_menu = new c_Menu(7); // 7 -> num max of rows shown on screen

	options_menu->AddItem((char*)"AUTO CREATE ALL INPUT .CFG");
	options_menu->AddItem((char*)"BACKGROUND MUSIC");
	options_menu->AddItem((char*)"RETRO ARCH MENU");
	options_menu->AddItem((char*)"DISPLAY CLONE GAMES");
	options_menu->AddItem((char*)"USE NEO-GEO UNI-BIOS");
	options_menu->AddItem((char*)"DISPLAY MISSING GAMES");
	options_menu->AddItem((char*)"MEGADRIVE DEFAULT CORE");

	options_menu->AddItem((char*)"FILTER [CAPCOM MISC]");
	options_menu->AddItem((char*)"FILTER [CAVE]");
	options_menu->AddItem((char*)"FILTER [CPS1]");
	options_menu->AddItem((char*)"FILTER [CPS2]");
	options_menu->AddItem((char*)"FILTER [CPS3]");
	options_menu->AddItem((char*)"FILTER [DATA EAST]");
	options_menu->AddItem((char*)"FILTER [GALAXIAN]");
	options_menu->AddItem((char*)"FILTER [IREM]");
	options_menu->AddItem((char*)"FILTER [KANEKO]");
	options_menu->AddItem((char*)"FILTER [KONAMI]");
	options_menu->AddItem((char*)"FILTER [NEO GEO]");
	options_menu->AddItem((char*)"FILTER [PAC-MAN]");
	options_menu->AddItem((char*)"FILTER [PGM]");
	options_menu->AddItem((char*)"FILTER [PSIKYO]");
	options_menu->AddItem((char*)"FILTER [SEGA]");
	options_menu->AddItem((char*)"FILTER [SEGA32]");
	options_menu->AddItem((char*)"FILTER [SETA]");
	options_menu->AddItem((char*)"FILTER [TAITO]");
	options_menu->AddItem((char*)"FILTER [TECHNOS]");
	options_menu->AddItem((char*)"FILTER [TOAPLAN]");
	options_menu->AddItem((char*)"FILTER [MISC PRE 90\'s]");
	options_menu->AddItem((char*)"FILTER [MISC POST 90\'s]");
	options_menu->AddItem((char*)"FILTER [MIDWAY]");
	options_menu->AddItem((char*)"FILTER [SNES]");
	options_menu->AddItem((char*)"FILTER [MEGA DRIVE]");
	options_menu->AddItem((char*)"FILTER [AMIGA]");
	options_menu->AddItem((char*)"FILTER [COLECO]");
	options_menu->AddItem((char*)"FILTER [TURBOGRAFX-16]");
	options_menu->AddItem((char*)"FILTER [PC ENGINE]");
	options_menu->AddItem((char*)"FILTER [SUPERGRAFX]");
	options_menu->AddItem((char*)"FILTER [FAVORITE]");

	options_menu->AddItem((char*)"ROMs PATH #1");
	options_menu->AddItem((char*)"ROMs PATH #2");
	options_menu->AddItem((char*)"ROMs PATH #3");
	options_menu->AddItem((char*)"ROMs PATH #4");
	options_menu->AddItem((char*)"ROMs PATH #5");
	options_menu->AddItem((char*)"ROMs PATH #6");
	options_menu->AddItem((char*)"ROMs PATH #7");
	options_menu->AddItem((char*)"ROMs PATH #8");
	options_menu->AddItem((char*)"ROMs PATH #9");
	options_menu->AddItem((char*)"ROMs PATH #10");
	options_menu->AddItem((char*)"ROMs PATH #11");
	options_menu->AddItem((char*)"ROMs PATH #12");

	options_menu->AddItem((char*)"[CAPCOM MISC] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[CAVE] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[CPS1] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[CPS2] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[CPS3] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[DATA EAST] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[GALAXIAN] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[IREM] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[KANEKO] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[KONAMI] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[NEO GEO] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[PAC-MAN] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[PGM] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[PSIKYO] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[SEGA] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[SEGA32] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[SETA] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[TAITO] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[TECHNOS] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[TOAPLAN] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[MISC PRE 90\'s] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[MISC POST 90's] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[MIDWAY] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[SNES] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[MEGA DRIVE] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[AMIGA] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[COLECO] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[TURBOGRAFX-16] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[PC ENGINE] INPUT .CFG PATH");
	options_menu->AddItem((char*)"[SUPERGRAFX] INPUT .CFG PATH");
}

void c_fbaRL::EndOptionsMenu()
{
	SAFE_DELETE(options_menu)
}

void c_fbaRL::InitZipInfoMenu()
{
	if(nFilteredGames < 1) return;

	zipinfo_menu = new c_Menu(34);

	uint32_t		i;
	//mz_bool			bZipStatus;
	//size_t			uncomp_size;
	////mz_zip_archive	zip_archive;
	struct zip *za;
    //struct zip_file *zf;
    struct zip_stat sb;
    int err;


    //int len;

	// Now try to open the archive.
	////memset(&zip_archive, 0, sizeof(zip_archive));
	hashmap_map *gamesmap;
    gamesmap =  (hashmap_map *) app.gamesMap;
    FBA_GAMES *fba_games;
    uint32_t hashmap_position;
    hashmap_position = fgames[nSelectedGame]->nSize;
    fba_games = (FBA_GAMES *)gamesmap->data[hashmap_position].data;

	if ((za = zip_open(fba_games->szPath, 0, &err)) == NULL) {
        printf("Can't open zip archive %s.\n", fba_games->szPath );
        SAFE_DELETE(zipinfo_menu)
        nSection = SECTION_GAMELIST;

		return;
    }

    for (i = 0; i < (uint32_t)zip_get_num_files(za); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {
            char* pszTmp = NULL;
            pszTmp = (char*)malloc(1024);
            memset(pszTmp, 0, 1024);

            sprintf(pszTmp, " [ %s ] [SIZE: %u bytes] [CRC32: %08X] \n",
                sb.name,
                (uint32_t)sb.size,
                //(uint32_t)file_stat.m_comp_size,
                (uint32_t)sb.crc
            );

            zipinfo_menu->AddItem((char*)pszTmp);

            SAFE_FREE(pszTmp)
        }
        else {
            printf("File[%s] Line[%d]\n", __FILE__, __LINE__);
        }
    }

    if (zip_close(za) == -1) {
        printf("Can't close zip archive %s.\n", fba_games->szPath);

    }


}

void c_fbaRL::EndZipInfoMenu()
{
	SAFE_DELETE(zipinfo_menu)
}

// TEMP
#define ROMINFO_DB	"/dev_hdd0/game/FBNE00123/USRDIR/fbarl-rominfo.txt"

#define _getBasicInfo(out)				\
	pch = strchr(pszLine, ';');			\
	if(pch) {							\
		int sep	= (int)(pch-pszLine);	\
		memset(&out, 0, sizeof(out));	\
		strncpy(out, pszLine, sep);		\
	}

#define _getline()						\
	memset(pszLine, 0, 2048+1);			\
	fgets(pszLine, 2048, fp);

struct BurnRomInfo {
	char szName[128]; //CRYSTAL (64)
	uint32_t nLen;
	uint32_t nCrc;
	char szType[128];
};

struct BurnSampleInfo {
	char szName[128]; //CRYSTAL (64)
	uint32_t nFlags;
};

struct FBA_ROMINFO {
	unsigned int nDrv;
	char szName[128]; //CRYSTAL (32)
	char szBoardrom[32];

	BurnRomInfo roms[100];
	unsigned int nRoms;

	BurnRomInfo board_roms[100];
	unsigned int nBoardRoms;

	BurnSampleInfo samples_info[100];
	unsigned int nSamples;
};
/*
int getRominfo(FILE* fp, char* szRomset, FBA_ROMINFO *rinfo)
{
	if(!fp) return NULL;

	rewind(fp);

	char* pszLine = NULL;
	pszLine = (char*)malloc(2048+1);
	memset(pszLine, 0, 2048+1);

	FBA_ROMINFO rominfo;
	memset(&rominfo, 0, sizeof(FBA_ROMINFO));

	while (!feof(fp))
	{
		_getline();

		// end of database
		if(strncmp("<//EOF>", pszLine, 7) == 0) { break; }

		// ignore comments
		if(strncmp("//", pszLine, 2) == 0) { continue; }

		// rominfo segment end
		if(strncmp("<//>", pszLine, 4) == 0) { rominfo.nDrv++; continue; }

		// rominfo segment start
		if(strncmp("<--->", pszLine, strlen("<--->")) == 0)
		{
			char* pch = NULL;

			_getline();
			char szTmp[16] = { 0 };
			_getBasicInfo(szTmp);
			sscanf(szTmp, "%d", &rominfo.nDrv);

			_getline();
			_getBasicInfo(rominfo.szName);

			if(strcmp(rominfo.szName, szRomset) == 0)
			{
				_getline();
				_getBasicInfo(rominfo.szBoardrom);

				_getline(); // <--- this line is a comment
				_getline();

				if(pszLine[0] == '{')
				{
					rominfo.nRoms = 0;

					while (pszLine[0] != '}')
					{
						_getline();
						if(pszLine[0] == '}') break;

						int nField = 0;

						pch = strtok(pszLine, "/");
						while (pch != NULL)
						{
							if(pch[0] == '\n') break;

							switch(nField)
							{
								// rom name
								case 0:
								{
									strcpy(rominfo.roms[rominfo.nRoms].szName, pch);
									break;
								}
								// rom size (bytes)
								case 1:
								{
									char szTmp[32] = { 0 };
									strcpy(szTmp, pch);
									sscanf(szTmp, "%d", &rominfo.roms[rominfo.nRoms].nLen);
									break;
								}
								// crc32 (hex)
								case 2:
								{
									char szTmp[32] = { 0 };
									strcpy(szTmp, pch);
									sscanf(szTmp, "%X", &rominfo.roms[rominfo.nRoms].nCrc);
									break;
								}
								// ROM Type
								case 3:
								{
									strcpy(rominfo.roms[rominfo.nRoms].szType, pch);
									break;
								}
							} // switch

							pch = strtok(NULL, "/");
							nField++;
						} // while
						rominfo.nRoms++;
					} // while
				} // if

				_getline(); // <--- this line is a comment
				_getline();

				if(pszLine[0] == '{')
				{

					rominfo.nBoardRoms = 0;
					while (pszLine[0] != '}')
					{
						_getline();
						if(pszLine[0] == '}') break;
						if(strncmp(pszLine, "NULL/", strlen("NULL/"))==0) break;

						int nField = 0;

						pch = strtok(pszLine, "/");
						while (pch != NULL)
						{
							if(pch[0] == '\n') break;

							switch(nField)
							{
								// rom name
								case 0:
								{
									strcpy(rominfo.board_roms[rominfo.nBoardRoms].szName, pch);
									break;
								}
								// rom size (bytes)
								case 1:
								{
									char szTmp[32] = { 0 };
									strcpy(szTmp, pch);
									sscanf(szTmp, "%d", &rominfo.board_roms[rominfo.nBoardRoms].nLen);
									break;
								}
								// crc32 (hex)
								case 2:
								{
									char szTmp[32] = { 0 };
									strcpy(szTmp, pch);
									sscanf(szTmp, "%X", &rominfo.board_roms[rominfo.nBoardRoms].nCrc);
									break;
								}
								// ROM Type
								case 3:
								{
									strcpy(rominfo.board_roms[rominfo.nBoardRoms].szType, pch);
									break;
								}
							} // switch
							pch = strtok(NULL, "/");
							nField++;
						} // while
						rominfo.nBoardRoms++;
					} // while
				} // if

				_getline(); // <--- this line is a comment
				_getline();

				if(pszLine[0] == '{')
				{
					rominfo.nSamples = 0;
					while (pszLine[0] != '}')
					{
						_getline();
						if(pszLine[0] == '}') break;
						if(strncmp(pszLine, "NULL/", strlen("NULL/"))==0) break;

						int nField = 0;

						pch = strtok(pszLine, "/");
						while (pch != NULL)
						{
							if(pch[0] == '\n') break;

							switch(nField)
							{
								// sample name
								case 0:
								{
									strcpy(rominfo.samples_info[rominfo.nSamples].szName, pch);
									break;
								}
								// sample flags
								case 1:
								{
									char szTmp[32] = { 0 };
									strcpy(szTmp, pch);
									sscanf(szTmp, "%d", &rominfo.samples_info[rominfo.nSamples].nFlags);
									break;
								}
							} // switch
							pch = strtok(NULL, "/");
							nField++;
						} // while
						rominfo.nSamples++;
					} // while
				} // if

				memcpy(rinfo, &rominfo, sizeof(FBA_ROMINFO));
				return 1;
			} // if
		} // if
	} // while
	return 0;
}
*/

void c_fbaRL::InitRomInfoMenu()
{
	int nCols=0;
	char *line = NULL;
    char *posB, *posE1, *posE2;
    char buff[258];
    char intxt[110];
    char *in, *out;
	
	switch (app.state.displayMode.resolution) {
		case VIDEO_RESOLUTION_720:
				nCols = 83;
				break;
		case VIDEO_RESOLUTION_1080:
				nCols = 83;
				break;
		case VIDEO_RESOLUTION_576:
		case VIDEO_RESOLUTION_480:
				nCols = 57;
				break;
	}

	if(nFilteredGames < 1) return;

	rominfo_menu = new c_Menu(34);

    hashmap_map *drvmap;
    FBA_DRV *fba_drv;
    drvmap = (hashmap_map *) app.drvMap;
	hashmap_position = games[fgames[nSelectedGame]->GameID]->nSize;
    fba_drv = (FBA_DRV *)drvmap->data[hashmap_position].data;
	//snprintf(buff,256,"%s", fgames[nSelectedGame]->title);
	snprintf(buff,256,"%s", fba_drv->szTitle);
	wrap(buff, nCols);
	posB = buff;
	posE1 = strchr(posB, '\015');
	if (posE1 != NULL) {
        buff[(int)(posE1 - buff)] = '\0';
        rominfo_menu->AddItem(buff);
        posB = posE1 + 1;

	}
	rominfo_menu->AddItem(posB);
    //rominfo_menu->AddItem(fgames[nSelectedGame]->title);
    rominfo_menu->AddItem((char *)"\n");
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
            return ;
    }
    sqlite3_stmt *stmt;


    snprintf(buf, sizeof(buf), "SELECT name COL00, desc COL01, info COL03 FROM games where id = %d", games[nBurnSelected]->GameID);
    //printf("\nname: %s - ids: %d %d\n", games[nBurnSelected]->name , games[nBurnSelected]->GameID, games[nBurnSelected]->nGame);
    rc = sqlite3_prepare_v2(mdb, buf, -1, &stmt, 0);
    if( rc != SQLITE_OK ) {
                  printf("Query failed!\n");
                  return ;
    }
    rc = sqlite3_step(stmt);
    //printf("Name: %s\n", sqlite3_column_text(stmt, COL00));

    int sl = 0;
    int j;
    if (sqlite3_column_text(stmt, COL02) != NULL) {
        line = (char *)malloc(strlen((char *)sqlite3_column_text(stmt, COL02)) + 1);
        if (line == NULL) {
            printf("Memory error - ROM Infor\n");
            exit(0);
        }
        strcpy(line, (const char *)sqlite3_column_text(stmt, COL02));
        //printf("LINE BEFORE %s\n", line);
        wrap(line, nCols);
        posB = line;
        sl = strlen(line);

        while ((posB - line) < sl) {
            j=0;
            posE1 = strchr(posB, '\015');
            posE2 = strchr(posB, '\012');

            if (posE2 != NULL && posE1 != NULL)
                if (posE2 < posE1) {
                        posE1 = posE2;
                        j = 1;
                }
                else ;
            else
                if (posE1 == NULL && posE2 == NULL)
                    break;
                else
                    if (posE2 != NULL) {
                            posE1 = posE2;
                            j = 1;
                    }

            line[(int)(posE1 - line)] = '\0';
            if (j == 0) {
                //intxt[0]= '\0'; buff[0] = '\0';
                sprintf(intxt,"%s", posB);
                in = intxt;
                out = buff;
                justifyLine(in,&out,nCols,&in,0,0);
                rominfo_menu->AddItem(out);
            }
            else
                rominfo_menu->AddItem(posB);

            posB = posE1 +1;
        }
        rominfo_menu->AddItem(posB);
        //printf("LINE %s\n", line);
    }
    if (line != NULL)
        SAFE_FREE(line);
    if (sqlite3_column_text(stmt, COL01) != NULL) {
        rominfo_menu->AddItem((char *)"\n");
        rominfo_menu->AddItem((char *)" - OTHER INFO -");
        rominfo_menu->AddItem((char *)"\n");
        line = (char *)malloc(strlen((char *)sqlite3_column_text(stmt, COL01)) + 1);
        if (line == NULL) {
            printf("Memory error - ROM Infor\n");
            exit(0);
        }
        strcpy(line, (const char *)sqlite3_column_text(stmt, COL01));

        wrap(line, nCols);
        //printf("LINE %s\n", line);
        posB = line;
        sl = strlen(line);
                while ((posB - line) < sl) {
            j=0;
            posE1 = strchr(posB, '\015');
            posE2 = strchr(posB, '\012');

            if (posE2 != NULL && posE1 != NULL)
                if (posE2 < posE1) {
                        posE1 = posE2;
                        j = 1;
                }
                else ;
            else
                if (posE1 == NULL && posE2 == NULL)
                    break;
                else
                    if (posE2 != NULL) {
                            posE1 = posE2;
                            j = 1;
                    }

            line[(int)(posE1 - line)] = '\0';
            if (j == 0) {
                //intxt[0]= '\0'; buff[0] = '\0';
                sprintf(intxt,"%s", posB);
                in = intxt;
                out = buff;
                justifyLine(in,&out,nCols,&in,0,0);
                rominfo_menu->AddItem(out);
            }
            else
                rominfo_menu->AddItem(posB);

            posB = posE1 +1;
        }
        rominfo_menu->AddItem(posB);

    }
    sqlite3_finalize(stmt);


    rc = sqlite3_close(mdb);
    if (rc)
            printf("CLOSE PROBLEM: rom game info %d\n",rc);

// ---
    /*
	FILE* fp = NULL;
	fp = fopen(ROMINFO_DB, "r");

	if(fp)
	{
		FBA_ROMINFO *rominfo = NULL;
		rominfo = (FBA_ROMINFO*) malloc(sizeof(FBA_ROMINFO));
		memset(rominfo, 0, sizeof(FBA_ROMINFO));

		int nLen = strlen(fgames[nSelectedGame]->zipname);
		char szRomset[32] = {0};
		strncpy(szRomset, fgames[nSelectedGame]->zipname, nLen-4); // without ".zip"
		if(!getRominfo(fp, szRomset, rominfo)) {
			//printf("ROM info not found on database...\n");
		} else {
			// ---------------------------
			// DEBUG OUTPUT
			// ---------------------------

			//printf("-- driver basic info --\n");


			//printf("-- roms --\n");

			for(unsigned int n = 0; n < rominfo->nRoms; n++)
			{
				char* pszTmp = NULL;
				pszTmp = (char*)malloc(1024);
				memset(pszTmp, 0, 1024);

				sprintf(pszTmp,
					"[ %s ][SIZE: %d bytes][CRC32: %X]",
					rominfo->roms[n].szName,
					rominfo->roms[n].nLen,
					rominfo->roms[n].nCrc
				);

				rominfo_menu->AddItem((char*)pszTmp);
				SAFE_FREE(pszTmp)
			}

			//printf("-- board roms --\n");

			for(unsigned int n = 0; n < rominfo->nBoardRoms; n++)
			{
				char* pszTmp = NULL;
				pszTmp = (char*)malloc(1024);
				memset(pszTmp, 0, 1024);

				sprintf(pszTmp,
					"[ %s ][SIZE: %d bytes][CRC32: %X]",
					rominfo->board_roms[n].szName,
					rominfo->board_roms[n].nLen,
					rominfo->board_roms[n].nCrc
				);

				rominfo_menu->AddItem((char*)pszTmp);
				SAFE_FREE(pszTmp)
			}

			//printf("-- samples --\n");

		}

		fclose(fp);
		fp = NULL;
	}*/
	if (line != NULL)
        SAFE_FREE(line);
}

void c_fbaRL::EndRomInfoMenu()
{
	SAFE_DELETE(rominfo_menu)
}

