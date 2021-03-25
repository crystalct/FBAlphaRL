#ifndef CAPP_H
#define CAPP_H

#include <io/pad.h>
#include <sysutil/video.h>
#include <math.h>
#include "fba_rl.h"
#include "mesh.h"
#include "sqlite.h"

//
#define TOTAL_CORES         5
//
#define TEX_MAIN_MENU		0
#define TEX_GAME_LIST		1
#define TEX_ZIP_INFO		2
#define TEX_ROM_INFO		3
#define TEX_OPTIONS			4
#define TEX_FILEBROWSER		5
#define TEX_PREVIEW			6
#define TEX_PREVIEW_TITLES	7

// Button organized by values ( ex. (1<<0), (1<<1), etc... )
#define BT_SELECT	0
#define BT_L3		1
#define BT_R3		2
#define BT_START	3
#define BT_UP		4
#define BT_RIGHT	5
#define BT_DOWN		6
#define BT_LEFT		7
#define BT_L2		8
#define BT_R2		9
#define BT_L1		10
#define BT_R1		11
#define BT_TRIANGLE 12
#define BT_CIRCLE	13
#define BT_CROSS	14
#define BT_SQUARE	15

#include "image.h"
#include "misc.h"
#include "ini.h"
#include "fba_rl.h"

#include "miniz.h"
#include "hashmap.h"

#define DIRPATH01   "/dev_hdd0/ROMS/fba"
#define DIRPATH02	"/dev_hdd0/ROMS/"
#define DIRPATH03	"/dev_hdd0/game/ROMS00000/USRDIR"
#define DIRPATH04	"/dev_hdd0/game/ROMS00000/USRDIR/roms"
#define DIRPATH05	"/dev_hdd0/game/ROMS00000/USRDIR/roms/fba"
#define DIRPATH06	"/dev_hdd0/game/ROMS00000/USRDIR/fba"
#define DIRPATH07	"/dev_hdd0/game/FBNE00123/USRDIR/cores/roms"
#define DIRPATH08	"/dev_hdd0/game/FBNE00123/USRDIR/cores/roms/fba"
#define DIRPATH09	"/dev_hdd0/game/SSNE10000/USRDIR/cores/roms"
#define DIRPATH10	"/dev_hdd0/game/SSNE10000/USRDIR/cores/roms/fba"
#define DIRPATH11	"/dev_hdd0/game/FBNE00123/USRDIR/cores/roms"
#define DIRPATH12	"/dev_hdd0/game/FBNE00123/USRDIR/cores/roms"
#define NDIRPATH    12

#define DIR_SKIP_DOT			1			//dont include "."
#define DIR_SKIP_DOTDOT			2			//dont include ".."
#define DIR_FILES				4			//include files
#define DIR_DIRS				0x10		//include directories
#define DIR_SKIP_HIDDEN			0x20		//skip entries starting with "." besides dot and dotdot
#define DIR_NO_DOT_AND_DOTDOT	( DIR_SKIP_DOT | DIR_SKIP_DOTDOT )

#define GCM_APP_WAIT_LABEL_INDEX		128

#define HOSTBUFFER_SIZE		(64*1024*1024)

extern "C" {
	void memcpy16(void* dst, const void* src, size_t n);
}

enum CoreOptions {
	CoreOptionsInvalid,
	mame125,
	snes,
	megadriv,
	amiga,
	neocd,
	coleco,
	turbografx16,
	pcengine,
	supergrafx
};

typedef struct {
	uint32_t total;
	uint32_t avail;
} smeminfo;

//CoreOptions resolveCoreOption(string input);

typedef struct FBA_DRV_S
{
	char szName[128];
	char szParent[128];
	char szBoardROM[32];
	char szTitle[256];
	char szYear[8];
	char szCompany[96];
	char szSystem[32];
	char szSubSystem[32];
	char szResolution[11];
	char szAspectRatio[8];
	uint32_t nGameID;
	uint32_t nGameOrderID;
	uint32_t nMaxPlayers;
	uint32_t nDefCoreID;
	uint32_t nCoreID;
	char szSystemFilter[32];
	bool isClone;
	bool isAvailable;
	bool isFavorite;
	char key_string[KEY_MAX_LENGTH];
} FBA_DRV;

typedef struct FBA_GAMES_S
{
	char szPath[256];
	char key_string[KEY_MAX_LENGTH];
} FBA_GAMES;

struct tex_dim {
	u32 w;
	u32 h;
};

class c_tex
{
public:
	uint32_t				nTexture;
	char*					pszTexPath;
	bool					bTextureOK;
	pngData*				png;
	pngData*				pngSec;
	u32						pngBmpOffset;
	u32*					texture_buffer[MAXQUADXTEX];
	u32						texture_offset[MAXQUADXTEX];
	struct tex_dim			texture_dim[MAXQUADXTEX];
	u32						nQuads;
	SMeshBuffer*			quad[MAXQUADXTEX];

	c_tex(uint32_t _nTexture, char* szPath);
	c_tex(uint32_t _nTexture, uint32_t display_mode);
	void Render(u16 x, u16 y, u16 w, u16 h);
	int BindSecTexture(char* pszPath);
	c_tex(uint32_t _nTexture, unsigned width, unsigned height);
	int BindTexture(char* pszPath);
	~c_tex();

};

SMeshBuffer* createQuad(Point3 P1, Point3 P2, Point3 P3, Point3 P4);

using namespace std;

class CapApp
{
public:
	CapApp();
	~CapApp() { }

	bool bRun;
	gcmContextData* context;
	int						    nTextures;
	c_tex** textures;
	char                        cores[TOTAL_CORES][32];
	FBA_DRV* fba_drv;
	map_t                       drvMap;
	FBA_GAMES* fba_games;
	map_t                       gamesMap;
	unsigned int			    deviceWidth, deviceHeight;
	void*						host_addr;
	int 					    currentBuffer;
	videoState                  state;
	u16 					    width;
	u16 					    height;
	padInfo 				    padinfo;
	padData 				    paddata;
	vector<string>              tracks;
	vector<string>::iterator    trackID;
	FILE* ftrack;

	bool		onInit(int argc, char** argv);
	void		onRender();
	void		onShutdown();
	bool		onUpdate();
	int		    InitDB();
	void		FontInit();
	void		initSPUSound();
	void		initGraphics();
	void		deinitSPUSound();
	void		remakegamesMaps();

	uint64_t	mFrame;

	uint32_t    nTotalBurnDrivers;
	uint32_t    nTotalGamesWithDup;
	uint32_t    nMissingGames;

	// input
	bool		mIsButtPressed[16];
	bool		buttPressedNow[16];

	uint32_t	mValRStickX, mValRStickY;
	uint32_t	mValLStickX, mValLStickY;

	uint32_t	nButtons[16];

	int			InputInit();
	int			npad_read();
	void		InputFrameStart();
	void		InputFrame();
	void		InputFrameEnd();
	void		InputExit();

	bool		ftp_service;

protected:
private:
};

extern CapApp app;


#endif
