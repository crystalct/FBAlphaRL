// ---------------------------------------------------------------------
// FB Neo Retro Loader Plus (CaptainCPS-X, 2013) - (CrystalCT, 2020)
// ---------------------------------------------------------------------
// Misc utility modules
#include <lv2/sysfs.h>
#include <algorithm>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sys/dirent.h>
#include "misc.h"
#include <sysutil/msg.h>
#include <sys/stat.h>
#include "fba_rl.h"

// Struct compare function for QSORT
// [v1.01] should now be memory leak free
int _FcCompareStruct(const void *a, const void *b)
{
	//char* cmp1 = (char*)toLowerCase((* (class c_game **) a)->title, 256);
	//char* cmp2 = (char*)toLowerCase((* (class c_game **) b)->title, 256);

	//int nRet = strcmp(cmp1, cmp2);
	int nRet = strcmp((* (class c_game **) a)->title, (* (class c_game **) b)->title);
    //int nRet = 0;
	//if(cmp1) {	free(cmp1); *&cmp1 = NULL; }
	//if(cmp2) {	free(cmp2); *&cmp2 = NULL; }

	return nRet;
}

// String compare function for QSORT
int _FcCompareStr(const void *pStrA, const void *pStrB)
{
	const bool bAscending = true;	// Always ascending [left as reference]
    return bAscending ?
		strcmp(*(char**)pStrA, *(char**)pStrB):	// Ascending [Ex. A,B,C]
		strcmp(*(char**)pStrB, *(char**)pStrA); // Descending [Ex. C,B,A]
}

#define _ADDCH(str, ch)			\
	str[strlen(str)]	= ch;	\
	str[strlen(str)+1]	= '\0';

char* GetNumWithCommas(int64_t nNumber)
{
	bool bIsNegative				= false;
	char szNumber[256]				= "";
	char szFlippedNumber[256]		= "";
	char szCommaFlippedNumber[256]	= "";

	// Check negative number
	if(nNumber < 0) {
		bIsNegative = true;
	}

	// Convert integer to string
	sprintf(szNumber, "%lld", (long long)nNumber);

	// Remove negative sign if needed
	if(bIsNegative) {
		char* pszTmp = szNumber + 1;
		sprintf(szNumber, "%s", pszTmp);
	}

	// Flip number string
	for(unsigned int nDigit = 0; nDigit < strlen(szNumber); nDigit++) {
		_ADDCH(szFlippedNumber, szNumber[strlen(szNumber)-(nDigit+1)])
	}

	// Add commas each 3 digits
	unsigned int nDigit = 0, nCount = 0;

	while(nDigit < strlen(szFlippedNumber))
	{
		if(nCount == 3) {
			_ADDCH(szCommaFlippedNumber, ',')
			nCount = 0;
			continue;
		}
		_ADDCH(szCommaFlippedNumber, szFlippedNumber[nDigit])
		nDigit++;
		nCount++;
	}

	// Avoid memory issues
	char* szCommaNumber	= (char*)malloc(strlen(szCommaFlippedNumber)+2); // +2 for the '-' and '\0'

	// Flip the new formatted number
	for(nDigit = 0; nDigit < strlen(szCommaFlippedNumber); nDigit++) {
		_ADDCH(szCommaNumber, szCommaFlippedNumber[strlen(szCommaFlippedNumber)-(nDigit+1)])
	}

	// Add negative sign if needed
	if(bIsNegative) {
		char szNegative[256] = "";
		sprintf(szNegative, "-%s", szCommaNumber);
		strcpy(szCommaNumber, szNegative);
	}

	return szCommaNumber;
}

// Simple function to convert a string to lower case
char* toLowerCase(char* pszStr, size_t nLen)
{
	char* pszNewStr = NULL;
	pszNewStr = (char*)malloc(sizeof(char) * nLen+1);
	memcpy(pszNewStr, pszStr, nLen);
	pszNewStr[nLen] = 0; // null-terminated string

	for(int ch = 0; pszNewStr[ch]; ch++) {
		pszNewStr[ch] = tolower(pszNewStr[ch]);
	}

	return pszNewStr;
}

// Simple function to convert a string to upper case
char* toUpperCase(char* pszStr, size_t nLen)
{
	char* pszNewStr = NULL;
	pszNewStr = (char*)malloc(sizeof(char) * nLen+1);
	memcpy(pszNewStr, pszStr, nLen);
	pszNewStr[nLen] = 0; // null-terminated string

	for(int ch = 0; pszNewStr[ch]; ch++) {
		pszNewStr[ch] = toupper(pszNewStr[ch]);
	}

	return pszNewStr;
}

// lpByteSize / 1,024
#define CALCKILOBYTE(lpByteSize)	(double)((double)(lpByteSize) / (1024.0F))
// lpByteSize / 1,048,576
#define CALCMEGABYTE(lpByteSize)	(double)((double)(lpByteSize) / (1024.0F * 1024.0F))
// lpByteSize / 1,073,741,824
#define CALCGIGABYTE(lpByteSize)	(double)((double)(lpByteSize) / (1024.0F * 1024.0F * 1024.0F))
// lpByteSize / 1,099,511,627,776
#define CALCTERABYTE(lpByteSize)	(double)((double)(lpByteSize) / (1024.0F * 1024.0F * 1024.0F * 1024.0F))

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int64_t _ex(int64_t nNum, int64_t nEx)
{
	int64_t nFinalNum = nNum;
	for(int64_t m = 0; m < nEx-1; m++) {
		nFinalNum = nFinalNum * nNum;
	}
	return nFinalNum;
}

double GetByteUnit(int64_t nSize)
{
	if(nSize >= _ex(1024, 4)) { return CALCTERABYTE(nSize); }
	if(nSize >= _ex(1024, 3)) { return CALCGIGABYTE(nSize); }
	if(nSize >= _ex(1024, 2)) { return CALCMEGABYTE(nSize); }
	if(nSize >= _ex(1024, 1)) { return CALCKILOBYTE(nSize); }
	return (double)nSize;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

const char* GetByteUnitStr(int64_t nSize)
{
	if(nSize >= _ex(1024, 4)) { return "TB"; }
	if(nSize >= _ex(1024, 3)) { return "GB"; }
	if(nSize >= _ex(1024, 2)) { return "MB"; }
	if(nSize >= _ex(1024, 1)) { return "KB"; }
	return "Bytes";
}

//Adjust quotation for sql queries
char *adjustQuotation(char *str) {
    int len;
    char * out = NULL;
    len = strlen(str);
    int i = 0;
    int q = 0;
    while (i < len) {
        if(str[i] == '\'')
            q++;
        i++;
    }
    if (q == 0)
        return out;
    out = (char *)malloc(len+q+1);
    if (out == NULL)
        return out;
    int j = 0;
    for (i=0; i < len; i++) {
        if (str[i] == '\'') {
            out[j] = '\'';
            j++;
        }
        out[j] = str[i];
        j++;
    }
    out[j] = '\0';
    return out;
}

bool dirExist(const char* szDir)
{
	sysFSStat entry;
	if( sysFsStat( szDir, &entry ) )
		return false;
	bool ret = ( ( entry.st_mode & S_IFDIR ) != 0 );
	return ret;
}

bool fileExist( const char* path )
{
	sysFSStat entry;
	return( sysFsStat( path, &entry ) == 0 );
}

uint64_t getFileSize(const char* szFilePath)
{
	u64 ret = 0;
	sysFSStat stat;
	if( sysFsStat( szFilePath, &stat ) )
		return 0;
    else
        ret = stat.st_size;

    return ret;
}

int readDir( const char *path, u32 flags, std::vector<std::string>& files, std::vector<std::string> &dir)
{
	int i;
	s32 fd;
	sysFSDirent entry;
	u64 read;
	std::string fpath = path;

	//open dir
	i = sysFsOpendir( path, &fd );
	if( i )
	{
		printf("sysFsOpendir( %s ): %i\n", path, i );
		return 0;
	}

	while( !sysFsReaddir( fd, &entry , &read ) && read > 0 )
	{
		//printf("namelen: %i  type: %u  name: %s\n", entry.d_namlen, entry.d_type, entry.d_name );
		if( !entry.d_namlen )//wtf?!
			continue;

		std::string str = entry.d_name;
		if( entry.d_name[ 0 ] == '.' )
		{
			//dot
			if( entry.d_name[ 1 ] == 0 )
			{
				if( !( flags & DIR_SKIP_DOT ) )
					dir.push_back( str );

				continue;
			}
			//dotdot
			if( entry.d_name[ 1 ] == '.' && entry.d_name[ 2 ] == 0 )
			{
				if( !( flags & DIR_SKIP_DOTDOT ) )
					dir.push_back( str );

				continue;
			}

			//must be a hidden file if we made it this far
			if( flags & DIR_SKIP_HIDDEN )
				continue;
		}


		//sub dirs
		if( entry.d_type == 1 && ( flags & DIR_DIRS ) )
		{
			dir.insert(dir.begin(), fpath + "/" + str );
			continue;
		}
		//files
		if( entry.d_type == 2 && ( flags & DIR_FILES ) )
		{
			files.push_back( str );
			continue;
		}
	}

	//close dir
	sysFsClosedir( fd );

	//sort
	//std::sort( ds.begin(), ds.end() );
	//std::sort( files.begin(), files.end() );
	//files.insert(files.begin(), dir.begin(), dir.end() );
	//files.insert(files.end(), files.begin(), files.end() );

	return 1;
}

void removeCarraigeReturn(char * str) {
	if (str == NULL)
		return;
	unsigned i, j;
	j = 0;
	for (i=0; i < strlen(str); i++) {
		if (j > strlen(str) +1 )
			break;
		if (str[j] == '\015') {

			j++;

		}

		str[i] = str[j];

		j++;
	}
	str[i] = '\0';
}

void wrap(char *text, const int length)
{
    int last_space = 0;
    int counter = 0;
    removeCarraigeReturn(text);

    for (int current = 0; text[current] != '\0'; current++, counter++)
    {
        if (isspace(text[current])) {
            last_space = current;

        }

        if (text[current] == '\n') {
            counter = 0;

        }
        if (counter >= length)
        {
            text[last_space] = '\r';
            counter = 0;
        }
    }

}

int justifyLine(char *inbuf,char **outbuf,size_t linelen, char ** endptr,size_t indent,int nospacing)
{
    size_t textlen,tmp;
    size_t spctoadd,spcodd,spcin;
    size_t timetoodd = 0;
    size_t ibidx,obidx,k,wc;

    char * endp;
    char * outb=NULL;

    int retval=0;

    for(;;) { //Error control loop
        endp=inbuf;

        if (inbuf==NULL) {
            retval=1;break;
        }

        if (indent>linelen) {
            retval=2;break;
        }

        if (outbuf==NULL) {
            retval=3;break;
        }

        if (*outbuf==NULL) {
            if ( (*outbuf=(char *)malloc(linelen+1))==NULL ){
                retval=4;break;
            }
        }

        outb=*outbuf;

        //Leave right spaces
        while(*inbuf==' ')
            inbuf++;

        if (*inbuf==0) {
            endp=inbuf;
            *outb=0;
            break;  //exit from error loop without error!
        }

        linelen-=indent;

        //Count words and the minimum number of characters
        ibidx=0;
        wc=0;textlen=0;k=1;endp=NULL;
        while ( *(inbuf+ibidx)!=0 ) {
            if (*(inbuf+ibidx)==' ') {
                ibidx++;continue;
            }
            //There's a char!
            k=ibidx;    //last word start
            tmp=textlen;
            wc++;textlen++; //add the space after the words
            //textlen<linelen because textlen contains also the space after the word
//            while(textlen<=linelen && *(inbuf+ibidx)!=' ' && *(inbuf+ibidx) ) {
            while(*(inbuf+ibidx)!=' ' && *(inbuf+ibidx) ) {
                textlen++;ibidx++;
            }

            if (textlen>linelen+1) {
                endp=inbuf+k;
                textlen=tmp;
                wc--;
                break;
            }
        }

        textlen=textlen-wc;

        if (endp==NULL) {
            endp=inbuf+ibidx;
        }

        if (textlen<2) {
            *outb=0;
            break;  //exit from error loop without error!
        }

        //Prepare outbuf
        memset(outb,' ',linelen+indent);
        *(outb+linelen+indent)=0;

        ibidx=0;
        obidx=indent;
        if (wc>1) {
            if (!nospacing) {
                //The odds are max in number == wc-2
                spctoadd=linelen-textlen;
            } else {
                spctoadd=wc-1;
            }

            spcin=spctoadd/(wc-1);
            spcodd=spctoadd % (wc-1);
            if (spcodd)
                timetoodd=(wc-1)/spcodd;

            k=timetoodd;
            while(spctoadd) {
                while(*(inbuf+ibidx)!=' ') {
                    *(outb+obidx++)=*(inbuf+ibidx++);
                }
                obidx+=spcin;spctoadd-=spcin;
                if (spcodd && !(--k)) {
                    k=timetoodd;
                    spcodd--;
                    spctoadd--;
                    obidx++;
                }
                while(*(inbuf+ ++ibidx)==' ');
            }
        }

        while(*(outb+obidx) && *(inbuf+ibidx) && *(inbuf+ibidx)!=' ')
            *(outb+obidx++)=*(inbuf+ibidx++);

        //There're words longer then the line!!!
        if (*(inbuf+ibidx) && *(inbuf+ibidx)!=' ')
            endp=inbuf+ibidx;

        break;  //Terminate error ctrl loop.
    }

    if (endptr!=NULL)
        *endptr=endp;

    return retval;
}

