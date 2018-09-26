#ifndef __COMMONHEADER_H__
#define __COMMONHEADER_H__

#if !WIN32
#define strcpy_s(a,b,c)             strcpy(a,c)
#define sprintf_s                   snprintf
#define snprintf_s                  snprintf
#define strcat_s(a,b,c)             strcat(a,c)
#define strncpy_s(a,b,c,d)          strncpy(a,c,d)
#define sscanf_s                    sscanf
#define vsnprintf_s(a,b,c,d,e)      vsnprintf(a,b,d,e)
#define _stricmp                    strcasecmp
#endif //WIN32

#if SPRITETOOLGUI

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/GLExtensions.h"

#else

//#include <SDKDDKVer.h>

#endif

#include <stdio.h>
//#include <tchar.h>
#include <string.h>
//#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <assert.h>

//#include <boost/filesystem.hpp>
#if WIN32
#include "dirent/dirent.h"
#else
#include <dirent.h>
#endif

struct ivec2
{
    int x;
    int y;
};

#include "BinPackers/MaxRectsBinPack.h"
#include "cJSON/cJSON.h"
#include "lodepng/lodepng.h"
#include "MarchingSquares/MarchingSquares.h"
#include "poly2tri/poly2tri.h"
#undef max // psimpl uses max as a variable name, I don't use it anywhere so just undefining it to avoid the issue.
#include "psimpl/psimpl.h"

#include "SpriteTool.h"

#if SPRITETOOLGUI
#include "ShaderProgram.h"
#include "GameCore.h"
#endif

#endif //__COMMONHEADER_H__
