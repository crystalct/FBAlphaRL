#include <ps3sqlite/sqlite3.h>

#define COL_ID				0
#define COL_SYSTEM			1
#define COL_SUBSYSTEM		2
#define COL_NAME			3
#define COL_CLONEOF			4
#define COL_TITLE			5
#define COL_YEAR			6
#define COL_DESC			7
#define COL_INFO			8
#define COL_RELEASEDATE		9
#define COL_DEVELOPER		10
#define COL_PUBLISHER		11
#define COL_GENRES			12
#define COL_PLAYERS			13
#define COL_ASPECTRATIO		14
#define COL_RESOLUTION      15
#define COL_ORDER_ID        16
#define COL_PATH            18
#define COL_SYSMASK			21

#define COL00			0
#define COL01    		1
#define COL02  			2
#define COL03			3
#define COL04  			4
#define COL05			5
#define COL06			6
#define COL07			7
#define COL08    		8
#define COL09    		9
#define COL10   		10
#define COL11			11
#define COL12			12
#define COL13		    13
#define COL14           14
#define COL15           15
#define COL16           16
#define COL17           17

extern "C" {
	int db_init_cpp(char* cache_path);
	sqlite3* db_open_cpp(const char* path);
	int db_one_statement_cpp(sqlite3* db, const char* sql, const char* src);
}
