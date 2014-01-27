#ifndef __SPRITETOOL_H__
#define __SPRITETOOL_H__

struct ImageBlock
{
    char* filename;
    unsigned char* imagebuffer;
    unsigned int w;
    unsigned int h;
    unsigned int posx;
    unsigned int posy;
    unsigned int offsetx; // how much was trimmed
    unsigned int offsety; // how much was trimmed
    rbp::Rect binrect;
};

bool CreateSpriteSheet(const char* srcdir, int padding, int trim);
bool PackTextures(ImageBlock* pImages, int filecount, int texw, int texh);
void CopyImageChunk(unsigned char* dest, unsigned int destw, unsigned int desth, ImageBlock* src);

#endif //__SPRITETOOL_H__
