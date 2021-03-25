// =======================================================================
// UTILITY MODULES

#include "capp.h"
#include "misc.h"
#include <zip.h>
#include <sys/process.h>

void c_fbaRL::MakeAmigaCFG(char *romPath, char *subsystem, char** argv)
{
    uint32_t i, len;
	struct zip *za;
    struct zip_stat sb;
    struct zip_file *zf;
    int err;

    bool hdf = false;
    argv[0] = (char *)malloc( 2 + 1);
	strcpy(argv[0], "-s");
	argv[1] = (char *)malloc(128);
	argv[2] = (char *)malloc( 2 + 1);
	strcpy(argv[2], "-s");
	argv[3] = (char *)malloc(128);
	if (strcmp(subsystem, "amigaocs") == 0) {
		strcpy(argv[1], "chipset=ocs");
		strcpy(argv[3], "kickstart_rom_file=/dev_hdd0/game/FBNE00123/USRDIR/EUAE/kick34005.A500");
		}
    else if (strcmp(subsystem, "amigaecs") == 0) {
                strcpy(argv[1], "chipset=ecs");
                strcpy(argv[3], "kickstart_rom_file=/dev_hdd0/game/FBNE00123/USRDIR/EUAE/kick40063.A600");
        }
        else {
            strcpy(argv[1], "chipset=aga");
            strcpy(argv[3], "kickstart_rom_file=/dev_hdd0/game/FBNE00123/USRDIR/EUAE/kick40068.A1200");
        }



	if (strstr(romPath,".hdf"))
        if (strlen(strstr(romPath,".hdf"))== 4)
            hdf = true;
    if (strstr(romPath,".HDF"))
        if (strlen(strstr(romPath,".HDF"))== 4)
            hdf = true;
    if (strstr(romPath,".Hdf"))
        if (strlen(strstr(romPath,".Hdf"))== 4)
            hdf = true;

    if (hdf) {
            if (strcmp(subsystem, "amigaocs") == 0)
            {
                strcpy(argv[3], "kickstart_rom_file=/dev_hdd0/game/FBNE00123/USRDIR/EUAE/kick40063.A600");
            }
            argv[4] = (char *)malloc( 2 + 1);
            strcpy(argv[4], "-s");
            argv[5] = (char *)malloc(128);
            strcpy(argv[5], "hardfile=readwrite,32,1,2,512,/dev_hdd0/game/FBNE00123/USRDIR/EUAE/WHDLOAD.HDF");
            argv[6] = (char *)malloc( 2 + 1);
            strcpy(argv[6], "-s");
            argv[7] = (char *)malloc(256);
            sprintf(argv[7], "hardfile=readwrite,32,1,2,512,%s",romPath);
            /*argv[8] = (char *)malloc( 2 + 1);
            strcpy(argv[8], "-r");
            argv[7] = (char *)malloc(128);
            strcpy(argv[7], "/dev_hdd0/game/FBNE00123/USRDIR/EUAE/kick40063.A600");*/
            argv[8] = NULL;
            return;
    }



    if ((za = zip_open(romPath, 0, &err)) == NULL) {
                //printf("Can't open zip archive %s.\n", romPath );
                argv[2] = (char *)malloc( 2 + 1);
                strcpy(argv[2], "-0");
                argv[3] = (char *)malloc(strlen(romPath) + 1);
                strcpy(argv[3], romPath);
                argv[4] = NULL;
                argv[5] = NULL;
                argv[6] = NULL;
                return;
    }
    //char argvtxt[256];

    bool ipf = false;

    uint8_t f = 0;
    for (i = 0; i < (uint32_t)zip_get_num_files(za); i++) {
                if (zip_stat_index(za, i, 0, &sb) == 0) {

                        if(!strchr(sb.name, '/')) {
                        //if (sb.name[strlen(sb.name) - 1] != '/') {
                                //printf("File: %s\n", sb.name);
                                f++;
                                if (strstr(romPath,".ipf"))
                                    if (strlen(strstr(romPath,".ipf"))== 4)
                                        ipf = true;
                                if (strstr(romPath,".IPF"))
                                    if (strlen(strstr(romPath,".IPF"))== 4)
                                        ipf = true;
                                if (strstr(romPath,".Ipf"))
                                    if (strlen(strstr(romPath,".Ipf"))== 4)
                                        ipf = true;
                        }
                }
    }
    if (f == 1) {
        zip_close(za);
        argv[2] = (char *)malloc( 2 + 1);
        strcpy(argv[2], "-0");
        argv[3] = (char *)malloc(strlen(romPath) + 1);
        strcpy(argv[3], romPath);
        argv[4] = NULL;
        argv[5] = NULL;
        argv[6] = NULL;
        return;
    }

    char floppyName[512];
    char buf[100];

    FILE* fp = NULL;
	uint32_t sum;

    f = 0;

    int nfloppy = 4;
    if (ipf)            //if it is IPF file, mount only one floppy
        nfloppy = 1;

    for (i = 0; i < (uint32_t)zip_get_num_files(za); i++) {
            if (zip_stat_index(za, i, 0, &sb) == 0) {
                    //printf("%s\n", sb.name);
                    if(!strchr(sb.name, '/')) {
                    //if (sb.name[strlen(sb.name) - 1] != '/') {
                        snprintf(floppyName,512,"/dev_hdd0/game/FBNE00123/USRDIR/EUAE/FLOPPYTMP/%s", sb.name);

                        //printf("%s\n", floppyName);
                        if (f < nfloppy) {
                            argv[2+f*2] = (char *)malloc( 2 + 1);
                            sprintf(argv[2+f*2], "-%d", f);
                            argv[3+f*2] = (char *)malloc(strlen(floppyName) + 3);
                            sprintf(argv[3+f*2], "%s", floppyName);
                            f++;
                        }
                        zf = zip_fopen_index(za, i, 0);
                        if (!zf) {
                            printf("Zip open file error\n");
                            return;
                        }
                        fp = fopen(floppyName, "w");
                        if(fp < 0)
                        {
                            printf("Write error file\n");
                            return;
                        }
                        sum = 0;
                        while (sum != sb.size) {
                            len = zip_fread(zf, buf, 100);
                            if (len < 0) {
                                printf("Zip read file error\n");
                                return;
                            }
                            fwrite(buf, 1, len, fp);
                            sum += len;
                        }
                        fclose(fp);
                        zip_fclose(zf);

                    }
            }
            else {
                    printf("File[%s] Line[%d]\n", __FILE__, __LINE__);
            }
    }
    argv[2+f*2] = NULL;
    if (zip_close(za) == -1) {
                printf("Can't close zip archive %s.\n", romPath);
    }

}

void c_fbaRL::LaunchFBACore(char* arg1, char* arg2, char* arg3, char* arg4, char* arg5, char* arg6) //CRYSTAL
{
	// TODO: Add error checking for arguments...

    //printf("Arg1: %s - arg5: %s\n", arg1, arg5);


	if (strcmp(fgames[nSelectedGame]->sysmask, "MASKAMIGA") == 0){
		char core_path[]	= "/dev_hdd0/game/FBNE00123/USRDIR/EUAE/e-uae.SELF";
		char *argv[9];
		MakeAmigaCFG(arg1, arg5, argv);

//		char core_path[]	= "/dev_hdd0/game/FBNE00123/USRDIR/cores/fb_neo.SELF";
//		argv[11] = (char *)malloc(13);
//        argv[12] = (char *)malloc(3);
//        strcpy(argv[11], "--subsystem");
//        strcpy(argv[12], "md");
		//printf("ARGS: %s %s %s %s %s %s\n", argv[0],argv[1],argv[2],argv[3],argv[4],argv[5]);
		////sys_game_process_exitspawn(snes9x_core_path, (const char**)argv, NULL, NULL, 0, 1000, SYS_PROCESS_PRIMARY_STACK_SIZE_1M);
		printf("%s %s %s %s %s %s %s %s %s\n",core_path, argv[0], argv[1],argv[2],argv[3], argv[4], argv[5], argv[6], argv[7]);
		//return;
		sysProcessExitSpawn2( core_path, (const char**)argv, NULL, NULL, 0, 1001, SYS_PROCESS_SPAWN_STACK_SIZE_1M );
		return;

	}

	char* argv[14] = { 0 };	//CRYSTAL
	/*argv[2] = (char*)malloc( strlen(arg3) + 1);
	argv[3] = (char*)malloc( strlen(arg4) + 1);
	argv[4] = (char*)malloc( strlen(arg5) + 1);
	argv[5] = (char*)malloc( strlen(arg6) + 1); */ //CRYSTAL
	argv[0] = (char*)malloc( strlen(arg1) + 1);
	strcpy(argv[0], arg1);	// [ROM / ZIP path]			optional, can be dummy / filler argument
	argv[1] = (char*)malloc( strlen(arg2) + 1);

	strcpy(argv[1], arg2);	// [FBARL RELOAD.SELF path] MUST be the valid external launcher path
    argv[2] = (char *)malloc( 2 + 1);
    argv[3] = (char *)malloc(64);
    argv[4] = (char *)malloc(3);
    argv[5] = (char *)malloc(64);
    argv[6] = (char *)malloc(3);
    argv[7] = (char *)malloc(64);
    argv[8] = (char *)malloc(3);
    argv[9] = (char *)malloc(64);
    argv[10] = (char *)malloc(3);
    strcpy(argv[2], "-s");
	strcpy(argv[3], "/dev_hdd0/game/FBNE00123/USRDIR/cores/savefiles");
	strcpy(argv[4], "-c");
	strcpy(argv[5], "/dev_hdd0/game/FBNE00123/USRDIR/retroarch.cfg");
	strcpy(argv[6], "-S");
	strcpy(argv[7], "/dev_hdd0/game/FBNE00123/USRDIR/cores/savestates");
	strcpy(argv[8], "-L");
	strcpy(argv[9], "/dev_hdd0/game/FBNE00123/USRDIR/cores/fb_neo.SELF");
	strcpy(argv[10], "-v");
	argv[11] = NULL;
	argv[12] = NULL;
	argv[13] = NULL;
	/*strcpy(argv[2], arg3);	// [input preset cfg]		optional, can be dummy / filler argument
	strcpy(argv[3], arg4);	// [aspect ratio]			optional, can be dummy / filler argument
	strcpy(argv[4], arg5);	// [alternate menu key combo ] optional, can be dummy / filler argument
	strcpy(argv[5], arg6);	// [UNIBIOS for Neo Geo games ] optional, can be dummy / filler argument  //CRYSTAL
	argv[6] = NULL;*/
//    int kk;
//    for (kk=0; kk< 12; kk++)
//		printf("argv[%d] = %s\n", kk,argv[kk]);
//
//	printf("Print: nSelectedGame: %d - fgames[nSelectedGame]->sysmask: %s - def core: %d\n",nSelectedGame,fgames[nSelectedGame]->sysmask, app.cores[fgames[nSelectedGame]->def_core_id] );
	//printf("Path rom: %s\n",pathUpper);
	if (fgames[nSelectedGame]->def_core_id > 0) {
            char core_path[256];
            sprintf(core_path,"/dev_hdd0/game/FBNE00123/USRDIR/%s%s.SELF",
                    fgames[nSelectedGame]->def_core_id == 3 ? "MAME125/" : "cores/", app.cores[fgames[nSelectedGame]->def_core_id - 1]);
            sysProcessExitSpawn2( core_path, (const char**)argv, NULL, NULL, 0, 1001, SYS_PROCESS_SPAWN_STACK_SIZE_1M );
            return;
	}
	if (strcmp(fgames[nSelectedGame]->sysmask, "MASKMEGADRIVE") == 0){
		char core_path[256];
		if (strcmp(fgames[nSelectedGame]->subsystem, "megacd") == 0 || g_opt_nMegaDriveDefaultCore == 1)
			sprintf(core_path, "/dev_hdd0/game/FBNE00123/USRDIR/cores/genesis_plus_gx.SELF");
		else
			sprintf(core_path, "/dev_hdd0/game/FBNE00123/USRDIR/cores/fb_neo.SELF");
//		char core_path[]	= "/dev_hdd0/game/FBNE00123/USRDIR/cores/fb_neo.SELF";
//		argv[11] = (char *)malloc(13);
//        argv[12] = (char *)malloc(3);
//        strcpy(argv[11], "--subsystem");
//        strcpy(argv[12], "md");
		//printf("ARGS: %s %s %s %s %s %s\n", argv[0],argv[1],argv[2],argv[3],argv[4],argv[5]);
		////sys_game_process_exitspawn(snes9x_core_path, (const char**)argv, NULL, NULL, 0, 1000, SYS_PROCESS_PRIMARY_STACK_SIZE_1M);
		sysProcessExitSpawn2( core_path, (const char**)argv, NULL, NULL, 0, 1001, SYS_PROCESS_SPAWN_STACK_SIZE_1M );
		return;

	}
	if (strcmp(fgames[nSelectedGame]->sysmask, "MASKSNES") == 0){
		char snes9x_core_path[]	= "/dev_hdd0/game/FBNE00123/USRDIR/cores/snes9x.SELF";

		//printf("ARGS: %s %s %s %s %s %s\n", argv[0],argv[1],argv[2],argv[3],argv[4],argv[5]);
		////sys_game_process_exitspawn(snes9x_core_path, (const char**)argv, NULL, NULL, 0, 1000, SYS_PROCESS_PRIMARY_STACK_SIZE_1M);
		sysProcessExitSpawn2( snes9x_core_path, (const char**)argv, NULL, NULL, 0, 1001, SYS_PROCESS_SPAWN_STACK_SIZE_1M );
		//return;

	} else
		{
		char *pathUpper = toUpperCase(argv[0], strlen(argv[0])); //Check if is a MAME 0.125   // CRYSTAL START

		if (strstr(pathUpper, "MAME125") != NULL) { //|| strcmp(fgames[nSelectedGame]->sysmask, "MASKMIDWAY") == 0){
						free(pathUpper);
						char mame125_core_path[]	= "/dev_hdd0/game/FBNE00123/USRDIR/MAME125/mame125.SELF";

						////sys_game_process_exitspawn(mame125_core_path, (const char**)argv, NULL, NULL, 0, 1000, SYS_PROCESS_PRIMARY_STACK_SIZE_1M);
                        sysProcessExitSpawn2( mame125_core_path, (const char**)argv, NULL, NULL, 0, 1001, SYS_PROCESS_SPAWN_STACK_SIZE_1M );

						/*LaunchFBACore(
									(char*)fgames[nSelectedGame]->path,
									(char*)mame125_core_path,
									(char*)("DUMMY_ARG"),
									(char*)("DUMMY_ARG"),
									(char*)("DUMMY_ARG"),
									(char*)("DUMMY_ARG")
									);*/

		}
		else {
				free(pathUpper);
				char fba_core_path[] = "/dev_hdd0/game/FBNE00123/USRDIR/cores/fb_neo.SELF";
				sysProcessExitSpawn2( fba_core_path, (const char**)argv, NULL, NULL, 0, 1001, SYS_PROCESS_SPAWN_STACK_SIZE_1M );

				////sys_game_process_exitspawn(fba_core_path, (const char**)argv, NULL, NULL, 0, 1000, SYS_PROCESS_PRIMARY_STACK_SIZE_1M);
			}
		}
}







// RetroArch doesn't do this, so let's help it out :)
void c_fbaRL::MakeRetroDirs(char* szMainPath)
{
	if(!dirExist(szMainPath)) return;

	char szRetroDirs[][256] =
	{
		"previews",
		"presets",
		"presets/input",
		"roms",
		"roms/fba",
		"roms/snes",
		"roms/genesis",
		"roms/nes",
		"roms/neopocket",
		"roms/pce",
		"roms/doom",
		"roms/wonderswan",
		"roms/vba",
		"savestates",
		"sram",
		"system"
	};

	for(uint32_t n = 0; n < (sizeof(szRetroDirs) / 256); n++)
	{
		char* szDir = (char*)malloc(1024);
		sprintf(szDir, "%s/cores/%s", szMainPath, szRetroDirs[n]);

		if(!dirExist(szDir)) {
			////cellFsMkdir(szDir, CELL_FS_S_IFDIR | 0777);
		}
		SAFE_FREE(szDir)
	}
}

void c_fbaRL::CreateAllInputCFG()
{
	if(dirExist((char*)"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input"))
	{
		char szInputCFG[][2048] = {
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
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/midway.cfg",  //CRYSTAL
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/neogeo.cfg",
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/pacman.cfg",
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/pgm.cfg",
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/psikyo.cfg",
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/sega.cfg",
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/seta.cfg",
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/taito.cfg",
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/technos.cfg",
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/toaplan.cfg",
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/misc_pre90.cfg",
			"/dev_hdd0/game/FBNE00123/USRDIR/cores/presets/input/misc_post90.cfg"
		};
		for(uint32_t n = 0; n < sizeof(szInputCFG) / 2048; n++)
		{
			if(!fileExist(szInputCFG[n]))
			{
				FILE* fp = fopen(szInputCFG[n], "w");
				if(fp)
				{
					fwrite(example_cfg, 20570, 1, fp);
					fclose(fp);
					*&fp = NULL;
				}
			}
		}
	}
}
