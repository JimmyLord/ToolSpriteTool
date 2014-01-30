#ifndef __SPRITETOOL_H__
#define __SPRITETOOL_H__

class ImageBlock;

class ImageBlock
{
public:
    char* filename;
    unsigned char* imagebuffer;
    unsigned int w;
    unsigned int h;
    unsigned int posx;
    unsigned int posy;
    unsigned int offsetx; // how much was trimmed
    unsigned int offsety; // how much was trimmed
    rbp::Rect binrect;
    p2t::CDT* cdt;

#if SPRITETOOLGUI
    GLuint texturehandle;
    GLuint vbo;
    unsigned int numtris;
#endif

    ImageBlock()
    {
        filename = 0;
        imagebuffer = 0;
        cdt = 0;

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
        delete cdt;
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
    int NumImages;

    ImageBlockInfo()
    {
        pImages = 0;
    }
    ~ImageBlockInfo()
    {
        delete[] pImages;
    }
};

int main(int argc, char** argv);
ImageBlockInfo* SpriteTool_ParseArgsAndCreateSpriteSheet(int argc, char** argv);
ImageBlockInfo* CreateSpriteSheet(const char* srcdir, int padding, int trim);
bool PackTextures(ImageBlock* pImages, int filecount, int texw, int texh);
void CopyImageChunk(unsigned char* dest, unsigned int destw, unsigned int desth, ImageBlock* src);
void TriangulateSprites(ImageBlock* pImages, int filecount);

#endif //__SPRITETOOL_H__
