#include "CommonHeader.h"

int main(int argc, const char* argv[])
{
    bool invalidargs = false;

    if( argc < 2 )
        invalidargs = true;

    const char* setting_dirscr = 0;
    int setting_padding = 0;
    bool setting_trim = false;
    int setting_trimalpha = 0;

    for( int i=1; i<argc; i++ )
    {
        if( ( strcmp( argv[i], "-d" ) == 0 || strcmp( argv[i], "-dirsrc" ) == 0 ) )
        {
            if( i+1 >= argc )
                invalidargs = true;
            else
                setting_dirscr = argv[i+1];
        }
        if( ( strcmp( argv[i], "-p" ) == 0 || strcmp( argv[i], "-padding" ) == 0 ) )
        {
            if( i+1 >= argc )
                invalidargs = true;
            else
                setting_padding = atoi( argv[i+1] );
        }
        if( ( strcmp( argv[i], "-d" ) == 0 || strcmp( argv[i], "-dirsrc" ) == 0 ) )
        {
            setting_trim = true;

            if( i+1 >= argc )
                invalidargs = true;
            else
                setting_trimalpha = atoi( argv[i+1] );
        }
    }
    
    if( setting_dirscr == 0 )
    {
        printf( "Source directory required - use -d\n" );
    }
    else if( setting_padding < 0 || setting_padding > 10 )
    {
        printf( "Invalid padding amount - must be value between 0 and 10\n" );
    }
    else if( setting_trim && (setting_trimalpha < 0 || setting_trimalpha > 255) )
    {
        printf( "Invalid trim alpha threshhold- must be value between 0 and 255\n" );
    }
    else if( invalidargs )
    {
        printf( "Invalid arguments\n" );
        printf( "\n" );
        printf( "[-d imagedir] or -dirscr = supply a relative or absolute path\n" );
        printf( "[-p pixels] or -padding = padding between sprites in pixels\n" );
        printf( "[-t minalpha] or -trim = enable trim with minimum alpha for trimming - generally 0\n" );
    }
    else
    {
        printf( "Starting\n" );
        printf( "Source image directory -> %s\n", setting_dirscr );
        printf( "Padding -> %d\n", setting_padding );
        if( setting_trim )
            printf( "Trim Enabled -> %d\n", setting_trimalpha );
        else
            printf( "Trim Disabled\n" );

        CreateSpriteSheet( setting_dirscr, setting_padding, setting_trim ? setting_trimalpha:-1 );

        printf( "done\n" );
    }

    _getch();

	return 0;
}

bool CreateSpriteSheet(const char* srcdir, int padding, int trim)
{
using namespace rbp;
using namespace boost::filesystem;

    path srcpath( srcdir );

    if( exists( srcpath ) == false )
        return false;

    recursive_directory_iterator end_itr; // default construction yields past-the-end

    int filecount = 0;
    for( recursive_directory_iterator itr( srcpath ); itr != end_itr; itr++ )
    {
        if( itr->path().extension() == ".png" )
        {
            filecount++;
        }
    }

    if( filecount == 0 )
        return false;

    ImageBlock* pImages = new ImageBlock[filecount];

    unsigned int highestx = 0;
    unsigned int highesty = 0;

    // load all the images
    filecount = 0;
    for( recursive_directory_iterator itr( srcpath ); itr != end_itr; itr++ )
    {
        if( itr->path().extension() == ".png" )
        {
            path relativepath = itr->path().relative_path();
            std::string relativepathstr = relativepath.string();
            const char* relativepathcstr = relativepathstr.c_str();

            path filename = itr->path().filename();
            std::string filenamestr = filename.string();
            const char* filenamecstr = filenamestr.c_str();

            int len = strlen(filenamecstr);
            pImages[filecount].filename = new char[len+1];
            strcpy( pImages[filecount].filename, filenamecstr );
            lodepng_decode32_file( &pImages[filecount].imagebuffer, &pImages[filecount].w, &pImages[filecount].h, relativepathcstr );

            filecount++;
        }
    }

    // try to fit them into texture
    bool done = false;
    int sizex = 64;
    int sizey = 64;
    while( done == false )
    {
        done = PackTextures( pImages, filecount, sizex, sizey );

        if( done == false )
            sizex *= 2;

        if( sizex > 2048 )
        {
            sizex = 64;
            sizey *= 2;
        }

        if( sizey > 2048 )
            break;
    }

    printf( "Dimensions (%d, %d)\n", sizex, sizey );

    // create the new image.
    unsigned char* pNewImage = 0;
    if( done == true )
    {
        pNewImage = new unsigned char[sizex*sizey*4];

        for( int i=0; i<filecount; i++ )
        {
            CopyImageChunk( pNewImage, sizex, sizey, &pImages[i] );
        }

        lodepng_encode32_file( "test.png", pNewImage, sizex, sizey );
    }

    cJSON* root = cJSON_CreateObject();
    cJSON* filearray = cJSON_CreateArray();
    cJSON_AddItemToObject( root, "Files", filearray );

    for( int i=0; i<filecount; i++ )
    {
        if( done == true )
        {
            cJSON* fileobj = cJSON_CreateObject();
            cJSON_AddStringToObject( fileobj, "filename", pImages[i].filename );
            cJSON_AddNumberToObject( fileobj, "origwidth", pImages[i].w );
            cJSON_AddNumberToObject( fileobj, "origheight", pImages[i].h );
            cJSON_AddNumberToObject( fileobj, "posx", pImages[i].posx );
            cJSON_AddNumberToObject( fileobj, "posy", pImages[i].posy );
            cJSON_AddItemToArray( filearray, fileobj );
        }

        // free memory
        free( pImages[i].imagebuffer );
        delete[] pImages[i].filename;
    }

    char* jsonstr = cJSON_Print( root );

    //printf( "%s\n", jsonstr );

    delete jsonstr;
    cJSON_Delete( root );

    delete[] pNewImage;
    delete[] pImages;

    return true;
}

bool PackTextures(ImageBlock* pImages, int filecount, int texw, int texh)
{
using namespace rbp;

    MaxRectsBinPack m_BinPack;
    m_BinPack.Init( texw, texh );

    for( int i=0; i<filecount; i++ )
    {
        pImages[i].binrect = m_BinPack.Insert( pImages[i].w, pImages[i].h, MaxRectsBinPack::RectContactPointRule );
        //float occupancy = m_BinPack.Occupancy();

        if( pImages[i].binrect.width == 0 )
            return false;

        pImages[i].posx = pImages[i].binrect.x;
        pImages[i].posy = pImages[i].binrect.y;

        //if( rect.x + rect.width > (int)highestx )
        //    highestx = rect.x + rect.width;

        //if( rect.y + rect.height > (int)highesty )
        //    highesty = rect.y + rect.height;

        //printf( "%s (%d,%d)\n", pImages[i].filename, pImages[i].w, pImages[i].h );
        //printf( "%s occupancy %f%%\n", filenamecstr, occupancy );
    }

    return true;
}

void CopyImageChunk(unsigned char* dest, unsigned int destw, unsigned int desth, ImageBlock* src)
{
using namespace rbp;

    for( int y=0; y<src->binrect.height; y++ )
    {
        for( int x=0; x<src->binrect.width; x++ )
        {
            int destoffset = ((src->binrect.y+y)*destw + (src->binrect.x+x));
            int srcoffset = (y*src->binrect.width + x);

            ((int*)dest)[destoffset] = ((int*)src->imagebuffer)[srcoffset];
        }
    }
}
