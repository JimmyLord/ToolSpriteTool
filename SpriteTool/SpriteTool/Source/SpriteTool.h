#ifndef __SPRITETOOL_H__
#define __SPRITETOOL_H__

class ImageBlock;

class ImageBlock
{
public:
    char* filename;
    unsigned char* imageBuffer;
    unsigned int w; // Original width and height.
    unsigned int h;
    unsigned int x; // Position in final spritesheet texture.
    unsigned int y;
    unsigned int trimmedX; // Start x and y after trim.
    unsigned int trimmedY;
    unsigned int trimmedW; // Width and height after trimming.
    unsigned int trimmedH;
    rbp::Rect binRect;
    std::vector<p2t::CDT*> cdts; // TODO: Detect multiple discrete shapes in the same sprite and add them as new cdts.

#if SPRITETOOLGUI
    GLuint texturehandle;
    GLuint vbo;
    unsigned int numtris;
#endif

    ImageBlock()
    {
        filename = 0;
        imageBuffer = 0;

        w = 0;
        h = 0;
        x = 0;
        y = 0;
        trimmedX = 0;
        trimmedY = 0;
        trimmedW = 0;
        trimmedH = 0;

        //cdts.clear();

#if SPRITETOOLGUI
        texturehandle = 0;
        vbo = 0;
        numtris = 0;
#endif
    }

    ~ImageBlock()
    {
        delete[] filename;
        free( imageBuffer );
        //delete cdt;
        //TODO: delete the polyline inside the cdt object.
        //for( std::vector<p2t::Point*>::iterator p = polyline.begin(); p != polyline.end(); p++ )
        //    delete *p;

#if SPRITETOOLGUI
        glDeleteTextures( 1, &texturehandle );
        glDeleteBuffers( 1, &vbo );
#endif
    }
};

class ImageBlockInfo
{
public:
    ImageBlock* pImages;
    unsigned int NumImages;

    ImageBlockInfo()
    {
        pImages = 0;
    }
    ~ImageBlockInfo()
    {
        delete[] pImages;
    }
};

struct SettingsStruct
{
    const char* inputDir;
    const char* outputFilename;
    int padding;
    bool trim;
    int trimAlpha;
    bool triangulate;
    unsigned int minTextureSize;
    unsigned int maxTextureSize;
    bool growWide;
    bool createStrip;
    bool originAtBottomLeft;
    bool splitExistingStrips;
    unsigned int splitSpritesheetsWidth;

    SettingsStruct()
    {
        inputDir = 0;
        outputFilename = 0;
        padding = 0;
        trim = false;
        trimAlpha = 0;
        triangulate = false;
        minTextureSize = 64;
        maxTextureSize = 2048;
        growWide = false;
        createStrip = false;
        originAtBottomLeft = false;
        splitExistingStrips = false;
        splitSpritesheetsWidth = 32;
    }
};

int main(int argc, char** argv);
ImageBlockInfo* SpriteTool_ParseArgsAndCreateSpriteSheet(int argc, char** argv);
ImageBlockInfo* SplitSpriteSheets(SettingsStruct settings);
ImageBlockInfo* CreateSpriteSheet(SettingsStruct settings);
bool PackTextures(ImageBlock* pImages, int fileCount, unsigned int textureWidth, unsigned int textureHeight, int padding);
bool PackTextures_SpriteStrip(ImageBlock* pImages, int fileCount, unsigned int textureWidth, unsigned int textureHeight, int padding, bool createFromBottomLeft, bool allowMultipleLines);
void CopyImageChunk(unsigned char* dest, unsigned int destWidth, unsigned int destHeight, ImageBlock* src, unsigned int startX = 0, unsigned int startY = 0, unsigned int widthToCopy = 0, unsigned int heightToCopy = 0);
void TriangulateSprites(ImageBlock* pImages, int fileCount);
void TrimSprites(ImageBlock* pImages, int fileCount, int trim);

#endif //__SPRITETOOL_H__
