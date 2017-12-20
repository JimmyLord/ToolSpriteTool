#ifndef __SPRITETOOL_H__
#define __SPRITETOOL_H__

class ImageBlock;

class ImageBlock
{
public:
    char* filename;
    unsigned char* imagebuffer;
    unsigned int w; // original width and height
    unsigned int h;
    unsigned int posx; // position in final spritesheet texture
    unsigned int posy;
    unsigned int trimmedx; // start x and y after trim
    unsigned int trimmedy;
    unsigned int trimmedw; // width and height after trimming
    unsigned int trimmedh;
    rbp::Rect binrect;
    std::vector<p2t::CDT*> cdts; // TODO: detect multiple discrete shapes in the same sprite and add them as new cdts.

#if SPRITETOOLGUI
    GLuint texturehandle;
    GLuint vbo;
    unsigned int numtris;
#endif

    ImageBlock()
    {
        filename = 0;
        imagebuffer = 0;

        w = 0;
        h = 0;
        posx = 0;
        posy = 0;
        trimmedx = 0;
        trimmedy = 0;
        trimmedw = 0;
        trimmedh = 0;

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
        free( imagebuffer );
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
    const char* dirsrc;
    const char* outputfilename;
    int padding;
    bool trim;
    int trimalpha;
    bool triangulate;
    unsigned int mintexturesize;
    unsigned int maxtexturesize;
    bool growwide;
    bool createstrip;
    bool originatbottomleft;

    SettingsStruct()
    {
        dirsrc = 0;
        outputfilename = 0;
        padding = 0;
        trim = false;
        trimalpha = 0;
        triangulate = false;
        mintexturesize = 64;
        maxtexturesize = 2048;
        growwide = false;
        originatbottomleft = false;
        createstrip = false;
    }
};

int main(int argc, char** argv);
ImageBlockInfo* SpriteTool_ParseArgsAndCreateSpriteSheet(int argc, char** argv);
ImageBlockInfo* CreateSpriteSheet(SettingsStruct settings);
bool PackTextures(ImageBlock* pImages, int filecount, unsigned int texw, unsigned int texh, int padding);
bool PackTextures_SpriteStrip(ImageBlock* pImages, int filecount, unsigned int texw, unsigned int texh, int padding, bool createfrombottomleft, bool allowmultiplelines);
void CopyImageChunk(unsigned char* dest, unsigned int destw, unsigned int desth, ImageBlock* src);
void TriangulateSprites(ImageBlock* pImages, int filecount);
void TrimSprites(ImageBlock* pImages, int filecount, int trim);

#endif //__SPRITETOOL_H__
