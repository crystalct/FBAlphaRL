#ifndef INI_H
#define INI_H
// ---------------------------------------------------------------------
// FB Neo Retro Loader Plus (CaptainCPS-X, 2013) - (CrystalCT, 2020)
// ---------------------------------------------------------------------
#include "fba_rl.h"

#define FBARL_INI_PATH "/dev_hdd0/game/FBNE00123/USRDIR/FBA_RL.ini"
#define FBNEO_PATH "/dev_hdd0/game/FBNE00123/USRDIR/"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

extern char g_opt_szROMPaths[12][2048];
extern char g_opt_szInputCFG[][2048];
extern char g_opt_szTextures[][2048];
extern bool g_opt_bAutoInputCfgCreate;
extern bool g_opt_bCustomSysFilter[MASKCUSTOM+1];
extern bool g_opt_bMusic;
extern bool g_opt_bDisplayMissingGames;
extern unsigned int g_opt_nActiveSysFilter;
extern bool g_opt_bDisplayCloneGames;
extern unsigned int g_opt_nRetroArchMenu;
extern char g_opt_sRetroArchMenu[][10];
extern bool g_opt_bUseUNIBIOS;
extern unsigned int g_opt_nTrackID;
extern unsigned int g_opt_nMegaDriveDefaultCore;
extern char g_opt_sMegaDriveCores[][11];

int iniWrite();
int iniRead();
int cfgWrite(const char *cfgfile, const char *option, const char *value);
char* getStrOption(FILE* fp, const char* option);
int getBoolOption(FILE* fp, const char* option, bool *bOption);
int getIntOption(FILE* fp, const char* option, unsigned int *nOption);
void cfgUpdateByte(const char* cfgfile, const char* option, char value);

#endif
