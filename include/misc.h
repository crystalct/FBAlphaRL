#include <string>
#include <vector>
#include "capp.h"

#ifndef MISC_H
#define MISC_H
// ---------------------------------------------------------------------
// FB Neo Retro Loader Plus (CaptainCPS-X, 2013) - (CrystalCT, 2020)
// ---------------------------------------------------------------------
int			_FcCompareStruct(const void *pStructA, const void *pStructB);
int			_FcCompareStr(const void *pStrA, const void *pStrB);
char*		GetNumWithCommas(int64_t nNumber);
char*		toLowerCase(char* pszStr, size_t nLen);
char*		toUpperCase(char* pszStr, size_t nLen);
double		GetByteUnit(int64_t nSize);
const char* GetByteUnitStr(int64_t nSize);
char        *adjustQuotation(char *str);
int readDir(const char *path, u32 flags, std::vector<std::string>& files, std::vector<std::string> &dirs);
bool		dirExist(const char* szDir);
bool		fileExist(const char* szFile);
uint64_t	getFileSize(const char* szFilePath);
void wrap(char *text, const int length);
int justifyLine(char *inbuf,char **outbuf,size_t linelen, char ** endptr,size_t indent,int nospacing);
void removeCarraigeReturn(char * str);

#endif
