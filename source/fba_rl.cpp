// ========================================================================================================
// FB Neo Retro Loader Plus (CaptainCPS-X, 2013) - (CrystalCT, 2020)
// ========================================================================================================
/*

Custom RETROARCH changes for FB Alpha RL (v1.XX):

* platform_ps3.c

	- Changed custom RetroArch "EMULATOR_CONTENT_DIR" macro to "FBNE00123" (previously set to "SSNE10000"),
	this will allow this custom RetroArch to be independent from an existing RetroArch installation
	on the PS3.

	- Implemented extra agument checking support when launched externally

	Note: Arguments should be passed as follows (argv[])

		> Argument 1 :	This should have a ROM / ZIP valid path in case of loading a game.
		Or could just be a dummy argument.

		> Argument 2 :	This should have the path of the external Application RELOAD.SELF
		!!! Must be set in this custom RetroArch build !!!

		> Argument 3 :  This should have a valid path of a Input Preset Configuration file.
		(Ex. "/dev_hdd0/.../neogeo.cfg")
		Or could just be a dummy argument.

		> Argument 4 : This should have the Aspect Ratio of the game (Ex. "4:3" as char string)
		Or could be just a dummy argument

		> Argument 5 : Use alternate RetroArch menu key combo ("yes" or "no" as char string)

* rmenu.c / rmenu.h

	- Added code to handle Alternate Meny Key combos.

	- Modified / Added menu items for in-game menu and main menu so RetroArch display the
	proper name if launched from FB Alpha RL (Ex. "Return to FB alpha RL").

* Makefile.ps3

	- edited with custom signing using scetool for Salamander / CORE

========================================================================================================*/

//#define FBARL_DEBUG
#include <lv2/sysfs.h>
#include <net/netctl.h>
#include "capp.h"

c_fbaRL* fbaRL;

char ipaddress[16] = "0.0.0.0"; //ipv4

c_fbaRL::c_fbaRL(bool StartWithGamesList)
{
	////CellNetCtlInfo info;
	///cellNetCtlGetInfo(CELL_NET_CTL_INFO_IP_ADDRESS, &info);
	///strcpy(ipaddress, info.ip_address);
#ifdef FDEBUG
    FILE* fdebug = NULL;
	fdebug = fopen("/dev_hdd0/game/FBNE00123/USRDIR/fdebug.log", "a");
	if (fdebug == 0){
        printf("Errore nel write\n");
        exit(0);
        }
    fprintf(fdebug,"Start init fbaRL class \n");
    fflush(fdebug);
#endif // FDEBUG
	s32 state = 0;

    if(netCtlGetState(&state)>=0 && state == NET_CTL_STATE_IPObtained) {
            union net_ctl_info netctl_info;
            netctl_info.ip_address[0] = '\0';
            netCtlGetInfo(NET_CTL_INFO_IP_ADDRESS, &netctl_info);
            if (netctl_info.ip_address[0] != '\0') strcpy(ipaddress, netctl_info.ip_address);
    }
	//union net_ctl_info netctl_info;
	//netCtlGetInfo(NET_CTL_INFO_IP_ADDRESS, &netctl_info);
#ifdef FDEBUG
    fprintf(fdebug,"netCtlGetInfo OK\n");
    fflush(fdebug);
#endif // FDEBUG
	//if (netctl_info.ip_address != NULL)
        //strcpy(ipaddress, netctl_info.ip_address);

	nFrameStep = 0; // for text shadow

	nBurnSelected = -1;

	bProcessingGames = false;
	nSection = SECTION_MAIN;

	MakeRetroDirs((char*)"/dev_hdd0/game/FBNE00123/USRDIR");
#ifdef FDEBUG
    fprintf(fdebug,"MakeRetroDirs OK\n");
    fflush(fdebug);
#endif // FDEBUG
	/*////if(dirExist((char*)"/dev_hdd0/game/FBNE00123/USRDIR"))
	{
		MakeRetroDirs((char*)"/dev_hdd0/game/SSNE10000/USRDIR");
	}////*/

	if(g_opt_bAutoInputCfgCreate) {
		CreateAllInputCFG();
	}

#ifdef FDEBUG
    fprintf(fdebug,"CreateAllInputCFG OK\n");
    fflush(fdebug);
#endif // FDEBUG

	//InitMainMenu(); // CRYSTAL START
	if (StartWithGamesList) {
		//printf("StartwithFileSection = TRUE\n");
		//InitMainMenu();
		//EndMainMenu();
		//InitGameList();
        //fbaRL->InitGameList();
        //InitFilterList();
		nSection = SECTION_GAMELIST;
		//EndMainMenu();
	}
	else {
		//printf("StartwithFileSection = FALSE\n");

		InitMainMenu();
#ifdef FDEBUG
    fprintf(fdebug,"InitMainMenu OK\n");
    fclose(fdebug);
#endif // FDEBUG
	} //CRYSTAL END
}


