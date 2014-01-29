#include "CommonHeader.h"

int main(int argc, char** argv)
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
        if( ( strcmp( argv[i], "-t" ) == 0 || strcmp( argv[i], "-trim" ) == 0 ) )
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
            strcpy_s( pImages[filecount].filename, len+1, filenamecstr );
            lodepng_decode32_file( &pImages[filecount].imagebuffer, &pImages[filecount].w, &pImages[filecount].h, relativepathcstr );

            filecount++;
        }
    }

    // triangulate the sprites, this will also trim them.
    TriangulateSprites( pImages, filecount );

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
    rbp::MaxRectsBinPack m_BinPack;
    m_BinPack.Init( texw, texh );

    for( int i=0; i<filecount; i++ )
    {
        pImages[i].binrect = m_BinPack.Insert( pImages[i].w, pImages[i].h, rbp::MaxRectsBinPack::RectContactPointRule );
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

void TriangulateSprites(ImageBlock* pImages, int filecount)
{
    for( int i=0; i<filecount; i++ )
    {
        unsigned int w = pImages[i].w;
        unsigned int h = pImages[i].h;
        unsigned char* pPixels = pImages[i].imagebuffer;

        // find edges of sprite
        std::list<vec2> pPoints;
        MarchingSquares march;
        march.DoMarch( pPixels, w, h, &pPoints );

        // add them to a vector of points for poly2tri.
        std::vector<p2t::Point*> polyline;

        std::list<vec2>::iterator p;
        p2t::Point prevpoint(-1, -1), prevprevpoint(-1, -1);
        for( p = pPoints.begin(); p != pPoints.end(); p++ )
        {
            p2t::Point currpoint( p->x, p->y );

            if( prevpoint.x != -1 )
            {
                if( prevprevpoint.x == -1 )
                {
                    printf( "Adding first point - (%f, %f)\n", prevpoint.x, prevpoint.y );
                    polyline.push_back( new p2t::Point( prevpoint ) );
                }
                else
                {
                    if( ( currpoint.x == prevpoint.x && currpoint.x == prevprevpoint.x ) ||
                        ( currpoint.y == prevpoint.y && currpoint.y == prevprevpoint.y ) )
                    {
                    }
                    else
                    {
                        printf( "Adding point       - (%f, %f)\n", prevpoint.x, prevpoint.y );
                        polyline.push_back( new p2t::Point( prevpoint ) );
                    }
                }
            }

            prevprevpoint = prevpoint;
            prevpoint = currpoint;
        }

        printf( "Adding last point  - (%f, %f)\n", prevpoint.x, prevpoint.y );
        polyline.push_back( new p2t::Point( prevpoint ) );

        // triangulate
        p2t::CDT* cdt = new p2t::CDT( polyline );

        cdt->Triangulate();

        std::vector<p2t::Triangle*> triangles = cdt->GetTriangles();

        //std::list<p2t::Triangle*> map = cdt->GetMap();

        printf( "Number of triangles: %d\n", triangles.size() );

        for( unsigned int i=0; i<triangles.size(); i++ )
        {
            p2t::Point* point0 = triangles[i]->GetPoint( 0 );
            p2t::Point* point1 = triangles[i]->GetPoint( 1 );
            p2t::Point* point2 = triangles[i]->GetPoint( 2 );
            printf( "triangles %d: (%0.2f,%0.2f), (%0.2f,%0.2f), (%0.2f,%0.2f)\n",
                     i, point0->x, point0->y, point1->x, point1->y, point2->x, point2->y );
        }

        // Cleanup
        delete cdt;
        for( std::vector<p2t::Point*>::iterator p = polyline.begin(); p != polyline.end(); p++ )
            delete *p;

        polyline.clear();
    }
}
