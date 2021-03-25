// ---------------------------------------------------------------------
// FB Neo Retro Loader Plus (CaptainCPS-X, 2013) - (CrystalCT, 2020)
// ---------------------------------------------------------------------
#include "capp.h"
#include <sys/file.h>

char g_opt_szROMPaths[NDIRPATH][2048] = {
	DIRPATH01,
	DIRPATH02,
	DIRPATH03,
	DIRPATH04,
	DIRPATH05,
	DIRPATH06,
	DIRPATH07,
	DIRPATH08,
	DIRPATH09,
	DIRPATH10,
	DIRPATH11,
	DIRPATH12
};

char g_opt_szInputCFG[][2048] = {
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/capcom_misc.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/cave.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/cps1.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/cps2.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/cps3.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/data_east.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/galaxian.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/irem.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/kaneko.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/konami.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/neogeo.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/pacman.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/pgm.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/psikyo.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/sega.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/sega32.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/seta.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/taito.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/technos.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/toaplan.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/misc_pre90.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/misc_post90.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/midway.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/snes.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/megadrive.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/amiga.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/coleco.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/turbografx-16.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/pcengine.cfg",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/supergrafx.cfg"

};

char g_opt_szTextures[][2048] = {
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/borders/default/MAIN_MENU.PNG",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/borders/default/GAME_LIST.PNG",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/borders/default/ZIP_INFO.PNG",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/borders/default/ROM_INFO.PNG",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/borders/default/OPTIONS_MENU.PNG",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/borders/default/FILE_BROWSER.PNG",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/borders/default/PREVIEW.PNG",
	"/dev_hdd0/game/FBNE00123/USRDIR/cores/borders/default/TITLES.PNG"
};

char g_opt_sRetroArchMenu[][10] = {
	RETROARCH_MENU_GLUI_S,
	RETROARCH_MENU_OZONE_S,
	RETROARCH_MENU_RGUI_S,
	RETROARCH_MENU_XMB_S
};

char g_opt_sMegaDriveCores[][11] = {
	"FBNEO","GENESIS+FX"
};

unsigned int g_opt_nRetroArchMenu = 2;
unsigned int g_opt_nMegaDriveDefaultCore = 0;
unsigned int g_opt_nTrackID = 0;

bool g_opt_bAutoInputCfgCreate	= true;
bool g_opt_bMusic           	= true;
bool g_opt_bDisplayMissingGames = false;
bool g_opt_bDisplayCloneGames   = false;

unsigned int g_opt_nActiveSysFilter = MASKALL;

bool g_opt_bCustomSysFilter[MASKCUSTOM+1] =
{
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
	true // <--- fillers
};

bool g_opt_bUseUNIBIOS = false;

void cfgUpdateByte(const char* cfgfile, const char* option, char value) {
	char pathfilein[128];
	strcpy(pathfilein, FBNEO_PATH);
	strcat(pathfilein, cfgfile);
	FILE* fin = NULL;
	fin = fopen(pathfilein, "r+");
	if (fin == 0) {
		printf("Errore nel read\n");
		return;
	}

	int maximumLineLength = 256;
	char* lineBuffer = (char*)malloc(sizeof(char) * maximumLineLength);
	
	while (fgets(lineBuffer, maximumLineLength, fin)) {
		if (strstr(lineBuffer, option) != NULL) {
			fseek(fin, -3, SEEK_CUR);
			fwrite(&value, 1, 1, fin);
			break;
		}
	}
	if (lineBuffer)
		SAFE_FREE(lineBuffer);
	fclose(fin);
}

int cfgWrite(const char *cfgfile, const char *option, const char *value) {
    char pathfilein[128];
    char pathfileout[128];
    strcpy(pathfilein,FBNEO_PATH);
    strcat(pathfilein,cfgfile);
    strcpy(pathfileout,FBNEO_PATH);
    strcat(pathfileout,"cfg.bak");

    FILE* fin = NULL;
	fin = fopen(pathfilein, "r");
    if (fin == 0) {
        printf("Errore nel read\n");
        return 0;
        }
    FILE* fout = NULL;
	fout = fopen(pathfileout, "w");
	if (fout == 0){
        printf("Errore nel write\n");
        return 0;
        }
    int maximumLineLength = 256;
    char *lineBuffer = (char *)malloc(sizeof(char) * maximumLineLength);
    //char * line = NULL;

    while(fgets(lineBuffer, maximumLineLength, fin)) {
        if (strstr(lineBuffer, option) != NULL) {
            strcpy(lineBuffer, option);
            strcat(lineBuffer, " = \"");
			strcat(lineBuffer,value);
			strcat(lineBuffer,"\"\n");
            fputs(lineBuffer,fout);
            }
        else
        fputs(lineBuffer,fout);

    }
    fclose(fin);
    fclose(fout);
    if (lineBuffer)
        SAFE_FREE(lineBuffer);

    if (sysLv2FsUnlink(pathfilein))
        return 0;
    if (sysLv2FsRename(pathfileout,pathfilein))
        return 0;

    return 1;
}

int iniWrite()
{

	//printf("ini write start\n");
	FILE* fp = NULL;
	fp = fopen(FBARL_INI_PATH, "w");
	if(fp)
	{
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// FB Neo RL Plus (was FB Alpha RL) Options \n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// If you edit this manually, make sure you do so properly, any malformation \n");
		fprintf(fp, "// of this configuration will force the application to generate a new one, resulting \n");
		fprintf(fp, "// in the loss of any previous configuration you made. \n");
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// Custom ROM Paths [up to 12 alternate paths]\n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "rompath_1:\"%s\"\n"			, g_opt_szROMPaths[0]);
		fprintf(fp, "rompath_2:\"%s\"\n"			, g_opt_szROMPaths[1]);
		fprintf(fp, "rompath_3:\"%s\"\n"			, g_opt_szROMPaths[2]);
		fprintf(fp, "rompath_4:\"%s\"\n"			, g_opt_szROMPaths[3]);
		fprintf(fp, "rompath_5:\"%s\"\n"			, g_opt_szROMPaths[4]);
		fprintf(fp, "rompath_6:\"%s\"\n"			, g_opt_szROMPaths[5]);
		fprintf(fp, "rompath_7:\"%s\"\n"			, g_opt_szROMPaths[6]);
		fprintf(fp, "rompath_8:\"%s\"\n"			, g_opt_szROMPaths[7]);
		fprintf(fp, "rompath_9:\"%s\"\n"			, g_opt_szROMPaths[8]);
		fprintf(fp, "rompath_10:\"%s\"\n"			, g_opt_szROMPaths[9]);
		fprintf(fp, "rompath_11:\"%s\"\n"			, g_opt_szROMPaths[10]);
		fprintf(fp, "rompath_12:\"%s\"\n"			, g_opt_szROMPaths[11]);
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// Custom System Filters. Choose specific systems to show on this category.\n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "filter_capcom_misc:\"%s\"\n"	, g_opt_bCustomSysFilter[0] ? "yes" : "no");
		fprintf(fp, "filter_cave:\"%s\"\n"			, g_opt_bCustomSysFilter[1] ? "yes" : "no");
		fprintf(fp, "filter_cps1:\"%s\"\n"			, g_opt_bCustomSysFilter[2] ? "yes" : "no");
		fprintf(fp, "filter_cps2:\"%s\"\n"			, g_opt_bCustomSysFilter[3] ? "yes" : "no");
		fprintf(fp, "filter_cps3:\"%s\"\n"			, g_opt_bCustomSysFilter[4] ? "yes" : "no");
		fprintf(fp, "filter_dataeast:\"%s\"\n"		, g_opt_bCustomSysFilter[5] ? "yes" : "no");
		fprintf(fp, "filter_galaxian:\"%s\"\n"		, g_opt_bCustomSysFilter[6] ? "yes" : "no");
		fprintf(fp, "filter_irem:\"%s\"\n"			, g_opt_bCustomSysFilter[7] ? "yes" : "no");
		fprintf(fp, "filter_kaneko:\"%s\"\n"		, g_opt_bCustomSysFilter[8] ? "yes" : "no");
		fprintf(fp, "filter_konami:\"%s\"\n"		, g_opt_bCustomSysFilter[9] ? "yes" : "no");
		fprintf(fp, "filter_neogeo:\"%s\"\n"		, g_opt_bCustomSysFilter[10] ? "yes" : "no");
		fprintf(fp, "filter_pacman:\"%s\"\n"		, g_opt_bCustomSysFilter[11] ? "yes" : "no");
		fprintf(fp, "filter_pgm:\"%s\"\n"			, g_opt_bCustomSysFilter[12] ? "yes" : "no");
		fprintf(fp, "filter_psikyo:\"%s\"\n"		, g_opt_bCustomSysFilter[13] ? "yes" : "no");
		fprintf(fp, "filter_sega:\"%s\"\n"			, g_opt_bCustomSysFilter[14] ? "yes" : "no");
		fprintf(fp, "filter_sega32:\"%s\"\n"		, g_opt_bCustomSysFilter[15] ? "yes" : "no");
		fprintf(fp, "filter_seta:\"%s\"\n"			, g_opt_bCustomSysFilter[16] ? "yes" : "no");
		fprintf(fp, "filter_taito:\"%s\"\n"			, g_opt_bCustomSysFilter[17] ? "yes" : "no");
		fprintf(fp, "filter_technos:\"%s\"\n"		, g_opt_bCustomSysFilter[18] ? "yes" : "no");
		fprintf(fp, "filter_toaplan:\"%s\"\n"		, g_opt_bCustomSysFilter[19] ? "yes" : "no");
		fprintf(fp, "filter_misc_pre90:\"%s\"\n"	, g_opt_bCustomSysFilter[20] ? "yes" : "no");
		fprintf(fp, "filter_misc_post90:\"%s\"\n"	, g_opt_bCustomSysFilter[21] ? "yes" : "no");
		fprintf(fp, "filter_midway:\"%s\"\n"		, g_opt_bCustomSysFilter[22] ? "yes" : "no");
		fprintf(fp, "filter_snes:\"%s\"\n"			, g_opt_bCustomSysFilter[23] ? "yes" : "no");
		fprintf(fp, "filter_megadrive:\"%s\"\n"		, g_opt_bCustomSysFilter[24] ? "yes" : "no");
		fprintf(fp, "filter_amiga:\"%s\"\n"	    	, g_opt_bCustomSysFilter[25] ? "yes" : "no");
		fprintf(fp, "filter_coleco:\"%s\"\n"		, g_opt_bCustomSysFilter[26] ? "yes" : "no");
		fprintf(fp, "filter_turbografx-16:\"%s\"\n"	, g_opt_bCustomSysFilter[27] ? "yes" : "no");
		fprintf(fp, "filter_pcengine:\"%s\"\n"		, g_opt_bCustomSysFilter[28] ? "yes" : "no");
		fprintf(fp, "filter_supergrafx:\"%s\"\n"	, g_opt_bCustomSysFilter[29] ? "yes" : "no");
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// Active System Filter \n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "filter_active:\"%d\"\n"		, g_opt_nActiveSysFilter);
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// Enable / Disable Neo-Geo UNI-BIOS \n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "use_ng_unibios:\"%s\"\n"		, g_opt_bUseUNIBIOS ? "yes" : "no");
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// Enable / Disable the display of missing games \n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "display_missing:\"%s\"\n"		, g_opt_bDisplayMissingGames ? "yes" : "no");
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// Enaable / Disable background MP3 music \n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "background_music:\"%s\"\n"	, g_opt_bMusic ? "yes" : "no");
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// Start MP3 track number \n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "starting_track:\"%d\"\n"	, (int)(app.trackID - app.tracks.begin()));
		fprintf(fp, "\n");

        fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// RetroArch Menu Driver \n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "retro_arch_menu:\"%d\"\n"	, g_opt_nRetroArchMenu);
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// Enaable / Disable the display of clone games \n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "display_clone_games:\"%s\"\n"	, g_opt_bDisplayCloneGames ? "yes" : "no");
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// Select default core for MegaDrive games \n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "megadrive_default_core:\"%d\"\n", g_opt_nMegaDriveDefaultCore);
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// Input preset configuration file paths for specific systems.\n");
		fprintf(fp, "// When configured, they will be auto-loaded on game launch, \n");
		fprintf(fp, "// unless you specify an individual game configuration.\n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "input_capcom_misc:\"%s\"\n"	, g_opt_szInputCFG[0]);
		fprintf(fp, "input_cave:\"%s\"\n"			, g_opt_szInputCFG[1]);
		fprintf(fp, "input_cps1:\"%s\"\n"			, g_opt_szInputCFG[2]);
		fprintf(fp, "input_cps2:\"%s\"\n"			, g_opt_szInputCFG[3]);
		fprintf(fp, "input_cps3:\"%s\"\n"			, g_opt_szInputCFG[4]);
		fprintf(fp, "input_dataeast:\"%s\"\n"		, g_opt_szInputCFG[5]);
		fprintf(fp, "input_galaxian:\"%s\"\n"		, g_opt_szInputCFG[6]);
		fprintf(fp, "input_irem:\"%s\"\n"			, g_opt_szInputCFG[7]);
		fprintf(fp, "input_kaneko:\"%s\"\n"			, g_opt_szInputCFG[8]);
		fprintf(fp, "input_konami:\"%s\"\n"			, g_opt_szInputCFG[9]);
		fprintf(fp, "input_neogeo:\"%s\"\n"			, g_opt_szInputCFG[10]);
		fprintf(fp, "input_pacman:\"%s\"\n"			, g_opt_szInputCFG[11]);
		fprintf(fp, "input_pgm:\"%s\"\n"			, g_opt_szInputCFG[12]);
		fprintf(fp, "input_psikyo:\"%s\"\n"			, g_opt_szInputCFG[13]);
		fprintf(fp, "input_sega:\"%s\"\n"			, g_opt_szInputCFG[14]);
		fprintf(fp, "input_sega32:\"%s\"\n"			, g_opt_szInputCFG[15]);
		fprintf(fp, "input_seta:\"%s\"\n"			, g_opt_szInputCFG[16]);
		fprintf(fp, "input_taito:\"%s\"\n"			, g_opt_szInputCFG[17]);
		fprintf(fp, "input_technos:\"%s\"\n"		, g_opt_szInputCFG[18]);
		fprintf(fp, "input_toaplan:\"%s\"\n"		, g_opt_szInputCFG[19]);
		fprintf(fp, "input_misc_pre90:\"%s\"\n"		, g_opt_szInputCFG[20]);
		fprintf(fp, "input_misc_post90:\"%s\"\n"	, g_opt_szInputCFG[21]);
		fprintf(fp, "input_midway:\"%s\"\n"			, g_opt_szInputCFG[22]);
		fprintf(fp, "input_snes:\"%s\"\n"			, g_opt_szInputCFG[23]);
		fprintf(fp, "input_megadrive:\"%s\"\n"		, g_opt_szInputCFG[24]);
		fprintf(fp, "input_amiga:\"%s\"\n"		    , g_opt_szInputCFG[25]);
		fprintf(fp, "input_coleco:\"%s\"\n"			, g_opt_szInputCFG[26]);
		fprintf(fp, "input_turbografx-16:\"%s\"\n"	, g_opt_szInputCFG[27]);
		fprintf(fp, "input_pcengine:\"%s\"\n"		, g_opt_szInputCFG[28]);
		fprintf(fp, "input_supergrafx:\"%s\"\n"		, g_opt_szInputCFG[29]);
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// When missing, automatically create all basic Input Configuration files \n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "input_auto_create:\"%s\"\n"	, g_opt_bAutoInputCfgCreate ? "yes" : "no");
		fprintf(fp, "\n");

		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "// Custom GUI Textures (PNG) paths \n");
		fprintf(fp, "// --------------------------------------------------------------------------\n");
		fprintf(fp, "texture_main_menu:\"%s\"\n"	    , g_opt_szTextures[TEX_MAIN_MENU]);
		fprintf(fp, "texture_gamelist:\"%s\"\n"		    , g_opt_szTextures[TEX_GAME_LIST]);
		fprintf(fp, "texture_zipinfo:\"%s\"\n"		    , g_opt_szTextures[TEX_ZIP_INFO]);
		fprintf(fp, "texture_rominfo:\"%s\"\n"		    , g_opt_szTextures[TEX_ROM_INFO]);
		fprintf(fp, "texture_options:\"%s\"\n"		    , g_opt_szTextures[TEX_OPTIONS]);
		fprintf(fp, "texture_filebrowser:\"%s\"\n"	    , g_opt_szTextures[TEX_FILEBROWSER]);
		fprintf(fp, "texture_preview:\"%s\"\n"		    , g_opt_szTextures[TEX_PREVIEW]);
		fprintf(fp, "texture_preview_titles:\"%s\"\n"	, g_opt_szTextures[TEX_PREVIEW_TITLES]);
		fprintf(fp, "\n");

		fclose(fp);
		*&fp = NULL;

		//return 1; //CRYSTAL
	} else {
		// error...
		return 0;
	}
	if (cfgWrite("retroarch.cfg","menu_driver",g_opt_sRetroArchMenu[g_opt_nRetroArchMenu]) == 0) {
		printf("Error write retroarch.cfg\n");
		return 0;
		}
	if (cfgWrite("retroarch-core-options.cfg","fbneo-neogeo-mode",g_opt_bUseUNIBIOS ? "UNIBIOS" : "DIPSWITCH") == 0){
		printf("Error write retroarch-core-options.cfg\n");
		return 0;
		}

	return 1;
}

char* getStrOption(FILE* fp, const char* option)
{
	if(!fp) return NULL;

	rewind(fp);

	char* pszLine = NULL;
	pszLine = (char*)malloc(2048+1);
	memset(pszLine, 0, 2048+1);

	while (!feof(fp))
	{
		fgets(pszLine, 2048, fp);

		char* pch = NULL;
		pch = strstr(pszLine, option);

		if(pch) {
			char* pszOption =  NULL;
			pszOption = (char*)malloc(2048+1);
			memset(pszOption, 0, 2048+1);

			strncpy(pszOption, pch+(strlen(option)), 2048);

			pch = strrchr(pszOption, '"');
			pszOption[(int)(pch-pszOption)] = 0;

			if(pszLine) { free(pszLine); *&pszLine = NULL; }

			if(!pszOption){
				// debug...
				FILE* fpd = fopen("/dev_hdd0/game/FBNE00123/USRDIR/inidebug.txt", "w");
				if(fpd)
				{
					fprintf(fpd, "error: getStrOption() option--> %s \n", option);
					fclose(fpd);
					fpd = NULL;
				}
			}

			return pszOption;
		}
	}
	return NULL;
}

int getBoolOption(FILE* fp, const char* option, bool *bOption)
{
	if(!fp) return 0;

	rewind(fp);

	char* pszLine = NULL;
	pszLine = (char*)malloc(2048+1);
	memset(pszLine, 0, 2048+1);

	while (!feof(fp))
	{
		fgets(pszLine, 2048, fp);

		char* pch = NULL;
		pch = strstr(pszLine, option);

		if(pch) {
			char* pszOption =  NULL;
			pszOption = (char*)malloc(2048+1);
			memset(pszOption, 0, 2048+1);

			strncpy(pszOption, pch+(strlen(option)), 2048);

			pch = strrchr(pszOption, '"');
			pszOption[(int)(pch-pszOption)] = 0;

			if(pszLine) { free(pszLine); *&pszLine = NULL; }

			if(!pszOption){
				// debug...
				FILE* fpd = fopen("/dev_hdd0/game/FBNE00123/USRDIR/inidebug.txt", "w");
				if(fpd)
				{
					fprintf(fpd, "error: getBoolOption() option--> %s \n", option);
					fclose(fpd);
					fpd = NULL;
				}
				return 0;
			}

			if( strcmp(pszOption, "yes")==0 ||
				strcmp(pszOption, "YES")==0 ||
				strcmp(pszOption, "Yes")==0 ||
				strcmp(pszOption, "true")==0 ||
				strcmp(pszOption, "True")==0 ||
				strcmp(pszOption, "TRUE")==0 ||
				strcmp(pszOption, "1")==0 )
			{
				if(pszOption) { free(pszOption); *&pszOption = NULL; }
				*bOption = true;
				return 1;
			}
			if( strcmp(pszOption, "no")==0 ||
				strcmp(pszOption, "NO")==0 ||
				strcmp(pszOption, "No")==0 ||
				strcmp(pszOption, "false")==0 ||
				strcmp(pszOption, "False")==0 ||
				strcmp(pszOption, "FALSE")==0 ||
				strcmp(pszOption, "0")==0 )
			{
				if(pszOption) { free(pszOption); *&pszOption = NULL; }
				*bOption = false;
				return 1;
			}


		}
	}
	return 0;
}

int getIntOption(FILE* fp, const char* option, unsigned int *nOption)
{
	if(!fp) return 0;

	rewind(fp);

	char* pszLine = NULL;
	pszLine = (char*)malloc(2048+1);
	memset(pszLine, 0, 2048+1);

	while (!feof(fp))
	{
		fgets(pszLine, 2048, fp);

		char* pch = NULL;
		pch = strstr(pszLine, option);

		if(pch) {
			char* pszOption =  NULL;
			pszOption = (char*)malloc(2048+1);
			memset(pszOption, 0, 2048+1);

			strncpy(pszOption, pch+(strlen(option)), 2048);

			pch = strrchr(pszOption, '"');
			pszOption[(int)(pch-pszOption)] = 0;

			if(pszLine) { free(pszLine); *&pszLine = NULL; }

			if(!pszOption){
				// debug...
				FILE* fpd = fopen("/dev_hdd0/game/FBNE00123/USRDIR/inidebug.txt", "w");
				if(fpd)
				{
					fprintf(fpd, "error: getIntOption() option--> %s \n", option);
					fclose(fpd);
					fpd = NULL;
				}
				return 0;
			}

			sscanf(pszOption, "%d", nOption);

			return 1;
		}
	}
	return 0;
}

int iniRead()
{
	FILE* fp = NULL;
	fp = fopen(FBARL_INI_PATH, "r");
	if(fp)
	{
		char* pszStrOption = NULL;

		// -----------------------------------------------------------------------------------------
		// Rom path optionsto
		// -----------------------------------------------------------------------------------------
		pszStrOption = getStrOption(fp, "rompath_1:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[0], pszStrOption);

		pszStrOption = getStrOption(fp, "rompath_2:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[1], pszStrOption);

		pszStrOption = getStrOption(fp, "rompath_3:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[2], pszStrOption);

		pszStrOption = getStrOption(fp, "rompath_4:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[3], pszStrOption);

		pszStrOption = getStrOption(fp, "rompath_5:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[4], pszStrOption);

		pszStrOption = getStrOption(fp, "rompath_6:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[5], pszStrOption);

		pszStrOption = getStrOption(fp, "rompath_7:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[6], pszStrOption);

		pszStrOption = getStrOption(fp, "rompath_8:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[7], pszStrOption);

		pszStrOption = getStrOption(fp, "rompath_9:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[8], pszStrOption);

		pszStrOption = getStrOption(fp, "rompath_10:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[9], pszStrOption);

		pszStrOption = getStrOption(fp, "rompath_11:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[10], pszStrOption);

		pszStrOption = getStrOption(fp, "rompath_12:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szROMPaths[11], pszStrOption);

		// -----------------------------------------------------------------------------------------
		// Custom System Filters options
		// -----------------------------------------------------------------------------------------
		if(!getBoolOption(fp, "filter_capcom_misc:\""	, &g_opt_bCustomSysFilter[0])) {return 0;}
		if(!getBoolOption(fp, "filter_cave:\""			, &g_opt_bCustomSysFilter[1])) {return 0;}
		if(!getBoolOption(fp, "filter_cps1:\""			, &g_opt_bCustomSysFilter[2])) {return 0;}
		if(!getBoolOption(fp, "filter_cps2:\""			, &g_opt_bCustomSysFilter[3])) {return 0;}
		if(!getBoolOption(fp, "filter_cps3:\""			, &g_opt_bCustomSysFilter[4])) {return 0;}
		if(!getBoolOption(fp, "filter_dataeast:\""		, &g_opt_bCustomSysFilter[5])) {return 0;}
		if(!getBoolOption(fp, "filter_galaxian:\""		, &g_opt_bCustomSysFilter[6])) {return 0;}
		if(!getBoolOption(fp, "filter_irem:\""			, &g_opt_bCustomSysFilter[7])) {return 0;}
		if(!getBoolOption(fp, "filter_kaneko:\""		, &g_opt_bCustomSysFilter[8])) {return 0;}
		if(!getBoolOption(fp, "filter_konami:\""		, &g_opt_bCustomSysFilter[9])) {return 0;}
		if(!getBoolOption(fp, "filter_neogeo:\""		, &g_opt_bCustomSysFilter[10])) {return 0;}
		if(!getBoolOption(fp, "filter_pacman:\""		, &g_opt_bCustomSysFilter[11])) {return 0;}
		if(!getBoolOption(fp, "filter_pgm:\""			, &g_opt_bCustomSysFilter[12])) {return 0;}
		if(!getBoolOption(fp, "filter_psikyo:\""		, &g_opt_bCustomSysFilter[13])) {return 0;}
		if(!getBoolOption(fp, "filter_sega:\""			, &g_opt_bCustomSysFilter[14])) {return 0;}
		if(!getBoolOption(fp, "filter_sega32:\""		, &g_opt_bCustomSysFilter[15])) {return 0;}
		if(!getBoolOption(fp, "filter_seta:\""			, &g_opt_bCustomSysFilter[16])) {return 0;}
		if(!getBoolOption(fp, "filter_taito:\""			, &g_opt_bCustomSysFilter[17])) {return 0;}
		if(!getBoolOption(fp, "filter_technos:\""		, &g_opt_bCustomSysFilter[18])) {return 0;}
		if(!getBoolOption(fp, "filter_toaplan:\""		, &g_opt_bCustomSysFilter[19])) {return 0;}
		if(!getBoolOption(fp, "filter_misc_pre90:\""	, &g_opt_bCustomSysFilter[20])) {return 0;}
		if(!getBoolOption(fp, "filter_misc_post90:\""	, &g_opt_bCustomSysFilter[21])) {return 0;}
		if(!getBoolOption(fp, "filter_midway:\""		, &g_opt_bCustomSysFilter[22])) {return 0;}
		if(!getBoolOption(fp, "filter_snes:\""			, &g_opt_bCustomSysFilter[23])) {return 0;}
        if(!getBoolOption(fp, "filter_megadrive:\""		, &g_opt_bCustomSysFilter[24])) {return 0;}
        if(!getBoolOption(fp, "filter_amiga:\""		    , &g_opt_bCustomSysFilter[25])) {return 0;}
		if(!getBoolOption(fp, "filter_coleco:\""		, &g_opt_bCustomSysFilter[26])) {return 0;}
		if(!getBoolOption(fp, "filter_turbografx-16:\""	, &g_opt_bCustomSysFilter[27])) {return 0;}
		if(!getBoolOption(fp, "filter_pcengine:\""		, &g_opt_bCustomSysFilter[28])) {return 0;}
		if(!getBoolOption(fp, "filter_supergrafx:\""	, &g_opt_bCustomSysFilter[29])) {return 0;}

		// -----------------------------------------------------------------------------------------
		// Enable / Disable Neo-Geo UNI-BIOS
		// -----------------------------------------------------------------------------------------
		if(!getBoolOption(fp, "use_ng_unibios:\"", &g_opt_bUseUNIBIOS)) {return 0;}


		// -----------------------------------------------------------------------------------------
		// Active System Filter
		// -----------------------------------------------------------------------------------------
		if(!getIntOption(fp, "filter_active:\"", &g_opt_nActiveSysFilter)) {return 0;}

		// -----------------------------------------------------------------------------------------
		// Enable / Disable display of missing games
		// -----------------------------------------------------------------------------------------
		if(!getBoolOption(fp, "display_missing:\"", &g_opt_bDisplayMissingGames)) {return 0;}

		// -----------------------------------------------------------------------------------------
		// Bckground MP3 music
		// -----------------------------------------------------------------------------------------
		if(!getBoolOption(fp, "background_music:\"", &g_opt_bMusic)) {return 0;}

		// -----------------------------------------------------------------------------------------
		// Start MP3 track number
		// -----------------------------------------------------------------------------------------
		if(!getIntOption(fp, "starting_track:\"", &g_opt_nTrackID)) {return 0;}

		// -----------------------------------------------------------------------------------------
		// RetroArch Menu Driver option
		// -----------------------------------------------------------------------------------------
		if(!getIntOption(fp, "retro_arch_menu:\"", &g_opt_nRetroArchMenu)) {return 0;}

		// -----------------------------------------------------------------------------------------
		// Enable / Disable display of clone games
		// -----------------------------------------------------------------------------------------
		if(!getBoolOption(fp, "display_clone_games:\"", &g_opt_bDisplayCloneGames)) {return 0;}

		// -----------------------------------------------------------------------------------------
		// Select default core for megadrive games
		// -----------------------------------------------------------------------------------------
		if (!getIntOption(fp, "megadrive_default_core:\"", &g_opt_nMegaDriveDefaultCore)) { return 0; }

		// -----------------------------------------------------------------------------------------
		// Game System Input preset options
		// -----------------------------------------------------------------------------------------
		pszStrOption = getStrOption(fp, "input_capcom_misc:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[0], pszStrOption);

		pszStrOption = getStrOption(fp, "input_cave:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[1], pszStrOption);

		pszStrOption = getStrOption(fp, "input_cps1:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[2], pszStrOption);

		pszStrOption = getStrOption(fp, "input_cps2:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[3], pszStrOption);

		pszStrOption = getStrOption(fp, "input_cps3:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[4], pszStrOption);

		pszStrOption = getStrOption(fp, "input_dataeast:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[5], pszStrOption);

		pszStrOption = getStrOption(fp, "input_galaxian:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[6], pszStrOption);

		pszStrOption = getStrOption(fp, "input_irem:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[7], pszStrOption);

		pszStrOption = getStrOption(fp, "input_kaneko:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[8], pszStrOption);

		pszStrOption = getStrOption(fp, "input_konami:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[9], pszStrOption);

		pszStrOption = getStrOption(fp, "input_neogeo:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[10], pszStrOption);

		pszStrOption = getStrOption(fp, "input_pacman:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[11], pszStrOption);

		pszStrOption = getStrOption(fp, "input_pgm:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[12], pszStrOption);

		pszStrOption = getStrOption(fp, "input_psikyo:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[13], pszStrOption);

		pszStrOption = getStrOption(fp, "input_sega:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[14], pszStrOption);

		pszStrOption = getStrOption(fp, "input_sega32:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[15], pszStrOption);

		pszStrOption = getStrOption(fp, "input_seta:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[16], pszStrOption);

		pszStrOption = getStrOption(fp, "input_taito:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[17], pszStrOption);

		pszStrOption = getStrOption(fp, "input_technos:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[18], pszStrOption);

		pszStrOption = getStrOption(fp, "input_toaplan:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[19], pszStrOption);

		pszStrOption = getStrOption(fp, "input_misc_pre90:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[20], pszStrOption);

		pszStrOption = getStrOption(fp, "input_misc_post90:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[21], pszStrOption);

		pszStrOption = getStrOption(fp, "input_midway:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[22], pszStrOption);

		pszStrOption = getStrOption(fp, "input_snes:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[23], pszStrOption);

		pszStrOption = getStrOption(fp, "input_megadrive:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[24], pszStrOption);

		pszStrOption = getStrOption(fp, "input_amiga:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szInputCFG[25], pszStrOption);

		pszStrOption = getStrOption(fp, "input_coleco:\"");
		if (pszStrOption == NULL) { return 0; }
		strcpy(g_opt_szInputCFG[26], pszStrOption);

		pszStrOption = getStrOption(fp, "input_turbografx-16:\"");
		if (pszStrOption == NULL) { return 0; }
		strcpy(g_opt_szInputCFG[27], pszStrOption);

		pszStrOption = getStrOption(fp, "input_pcengine:\"");
		if (pszStrOption == NULL) { return 0; }
		strcpy(g_opt_szInputCFG[28], pszStrOption);

		pszStrOption = getStrOption(fp, "input_supergrafx:\"");
		if (pszStrOption == NULL) { return 0; }
		strcpy(g_opt_szInputCFG[29], pszStrOption);


		// -----------------------------------------------------------------------------------------
		// Auto Input Configuration creation option
		// -----------------------------------------------------------------------------------------
		if(!getBoolOption(fp, "input_auto_create:\"", &g_opt_bAutoInputCfgCreate)) {return 0;}

		// -----------------------------------------------------------------------------------------
		// Texture options
		// -----------------------------------------------------------------------------------------
		pszStrOption = getStrOption(fp, "texture_main_menu:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szTextures[TEX_MAIN_MENU], pszStrOption);

		pszStrOption = getStrOption(fp, "texture_gamelist:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szTextures[TEX_GAME_LIST], pszStrOption);

		pszStrOption = getStrOption(fp, "texture_zipinfo:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szTextures[TEX_ZIP_INFO], pszStrOption);

		pszStrOption = getStrOption(fp, "texture_rominfo:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szTextures[TEX_ROM_INFO], pszStrOption);

		pszStrOption = getStrOption(fp, "texture_options:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szTextures[TEX_OPTIONS], pszStrOption);

		pszStrOption = getStrOption(fp, "texture_filebrowser:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szTextures[TEX_FILEBROWSER], pszStrOption);

		pszStrOption = getStrOption(fp, "texture_preview:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szTextures[TEX_PREVIEW], pszStrOption);

		pszStrOption = getStrOption(fp, "texture_preview_titles:\"");
		if(pszStrOption == NULL) {return 0;}
		strcpy(g_opt_szTextures[TEX_PREVIEW_TITLES], pszStrOption);

		// -----------------------------------------------------------------------------------------

		if(pszStrOption) { free(pszStrOption); *&pszStrOption = NULL; }

		fclose(fp);

		return 1;
	} else {
		return 0;
	}
	return 0;
}
