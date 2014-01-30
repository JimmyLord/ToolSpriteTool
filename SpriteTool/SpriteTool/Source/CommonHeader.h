#ifndef __COMMONHEADER_H__
#define __COMMONHEADER_H__

#if SPRITETOOLGUI

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/GLExtensions.h"

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

struct vec2
{
    int x;
    int y;
};

#include "BinPackers/MaxRectsBinPack.h"
#include "cJSON/cJSON.h"
#include "lodepng/lodepng.h"
#include "MarchingSquares/MarchingSquares.h"
#include "poly2tri/poly2tri.h"

#include "SpriteTool.h"

#if SPRITETOOLGUI
#include "ShaderProgram.h"
#include "GameCore.h"
#endif

#endif //__COMMONHEADER_H__
