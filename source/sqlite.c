#include <ps3sqlite/ps3sqlite.h>


int db_init_cpp(char* cache_path) {
	int i = 0;
	i = db_init(cache_path);
	return i;
}

sqlite3* db_open_cpp(const char* path) {
	sqlite3* mdb;
	mdb = db_open(path, DB_OPEN_CASE_SENSITIVE_LIKE);
	//i = (void *) mdb;
	return mdb;
}

int db_one_statement_cpp(sqlite3* db, const char* sql, const char* src) {
	int i = 0;
	i = db_one_statement(db, sql, src);
	return i;
}
