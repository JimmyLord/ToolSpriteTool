#ifndef __COMMONHEADER_H__
#define __COMMONHEADER_H__

#if SPRITETOOLGUI

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/GLExtensions.h"
#include "GameCore.h"

#else

#include <SDKDDKVer.h>

#endif

#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <assert.h>

#include <boost/filesystem.hpp>

#include "cJSON/cJSON.h"
#include "lodepng/lodepng.h"
#include "BinPackers/MaxRectsBinPack.h"
#include "poly2tri/poly2tri.h"

struct vec2
{
    int x;
    int y;
};

#include "MarchingSquares.h"
#include "SpriteTool.h"

#endif //__COMMONHEADER_H__
