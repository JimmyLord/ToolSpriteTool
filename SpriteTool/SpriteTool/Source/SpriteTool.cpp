#include "CommonHeader.h"

int main(int argc, char** argv)
{
    ImageBlockInfo* pImageInfo = SpriteTool_ParseArgsAndCreateSpriteSheet(argc, argv);
    delete pImageInfo;
}

ImageBlockInfo* SpriteTool_ParseArgsAndCreateSpriteSheet(int argc, char** argv)
{
    bool invalidargs = false;

    if( argc < 2 )
        invalidargs = true;

    const char* setting_dirscr = 0;
    const char* setting_filename = 0;
    int setting_padding = 0;
    bool setting_trim = false;
    int setting_trimalpha = 0;
    bool setting_triangulate = false;

    ImageBlockInfo* pImageInfo = 0;
    ImageBlock* pImages = 0;

    for( int i=1; i<argc; i++ )
    {
        if( ( strcmp( argv[i], "-d" ) == 0 || strcmp( argv[i], "-dirsrc" ) == 0 ) )
        {
            if( i+1 >= argc )
                invalidargs = true;
            else
                setting_dirscr = argv[i+1];
        }
        if( ( strcmp( argv[i], "-f" ) == 0 || strcmp( argv[i], "-filename" ) == 0 ) )
        {
            if( i+1 >= argc )
                invalidargs = true;
            else
                setting_filename = argv[i+1];
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
        if( ( strcmp( argv[i], "-tri" ) == 0 || strcmp( argv[i], "-triangulate" ) == 0 ) )
        {
            setting_triangulate = true;
        }
    }
    
    if( setting_dirscr == 0 )
    {
        printf( "Source directory required - use -d\n" );
    }
    else if( setting_filename == 0 )
    {
        printf( "Filename required - use -f\n" );
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
        printf( "[-f filename] or -filename = output filename\n" );
        printf( "[-p pixels] or -padding = padding between sprites in pixels\n" );
        printf( "[-t minalpha] or -trim = enable trim with minimum alpha for trimming - generally 0\n" );
        printf( "[-tri] or -triangulate = triangulate the sprites(WIP)\n" );
    }
    else
    {
        printf( "Starting\n" );
        printf( "Source image directory -> %s\n", setting_dirscr );
        printf( "Output filename -> %s\n", setting_filename );
        printf( "Padding -> %d\n", setting_padding );
        if( setting_trim )
            printf( "Trim Enabled -> %d\n", setting_trimalpha );
        else
            printf( "Trim Disabled\n" );
        if( setting_triangulate )
            printf( "Triangulate Enabled\n" );
        else
            printf( "Triangulate Disabled\n" );

        pImageInfo = CreateSpriteSheet( setting_dirscr, setting_filename, setting_padding, setting_trim ? setting_trimalpha:-1, setting_triangulate );

        printf( "done\n" );
    }

    _getch();

	return pImageInfo;
}

ImageBlockInfo* CreateSpriteSheet(const char* srcdir, const char* filename, int padding, int trim, bool triangulate)
{
using namespace rbp;
using namespace boost::filesystem;

    path srcpath( srcdir );

    if( exists( srcpath ) == false )
        return 0;

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
        return 0;

    ImageBlockInfo* pImageInfo = new ImageBlockInfo;
    pImageInfo->pImages = new ImageBlock[filecount];
    pImageInfo->NumImages = filecount;

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
            pImageInfo->pImages[filecount].filename = new char[len+1];
            strcpy_s( pImageInfo->pImages[filecount].filename, len+1, filenamecstr );
            lodepng_decode32_file( &pImageInfo->pImages[filecount].imagebuffer,
                                   &pImageInfo->pImages[filecount].w, &pImageInfo->pImages[filecount].h,
                                   relativepathcstr );

            filecount++;
        }
    }

    // triangulate the sprites, this will also trim them.
    if( triangulate )
        TriangulateSprites( pImageInfo->pImages, filecount );
    else
        TrimSprites( pImageInfo->pImages, filecount, trim );

    // try to fit them into texture
    bool done = false;
    int sizex = 64;
    int sizey = 64;
    while( done == false )
    {
        done = PackTextures( pImageInfo->pImages, filecount, sizex, sizey, padding );

        if( done == false )
        {
            if( sizex <= sizey )
                sizex *= 2;
            else
                sizey *= 2;
        }

        if( sizex > 2048 && sizey > 2048 )
            break;
    }

    printf( "Dimensions (%d, %d)\n", sizex, sizey );

    // create the new image.
    unsigned char* pNewImage = 0;
    if( done == true )
    {
        pNewImage = new unsigned char[sizex*sizey*4];
        memset( pNewImage, 0, sizex*sizey*4 );

        for( int i=0; i<filecount; i++ )
        {
            CopyImageChunk( pNewImage, sizex, sizey, &pImageInfo->pImages[i] );
        }

        char outputfile[260];//MAX_PATH];
        sprintf_s( outputfile, 260, "%s.png", filename );
        lodepng_encode32_file( outputfile, pNewImage, sizex, sizey );


        cJSON* root = cJSON_CreateObject();
        cJSON* filearray = cJSON_CreateArray();
        cJSON_AddNumberToObject( root, "SpriteTool", 1 );
        cJSON_AddStringToObject( root, "file", outputfile );
        cJSON_AddNumberToObject( root, "width", sizex );
        cJSON_AddNumberToObject( root, "height", sizey );

        cJSON_AddItemToObject( root, "Files", filearray );

        for( int i=0; i<filecount; i++ )
        {
            if( done == true )
            {
                cJSON* fileobj = cJSON_CreateObject();
                cJSON_AddStringToObject( fileobj, "filename", pImageInfo->pImages[i].filename );
                cJSON_AddNumberToObject( fileobj, "origw", pImageInfo->pImages[i].w );
                cJSON_AddNumberToObject( fileobj, "origh", pImageInfo->pImages[i].h );
                cJSON_AddNumberToObject( fileobj, "posx", pImageInfo->pImages[i].posx );
                cJSON_AddNumberToObject( fileobj, "posy", pImageInfo->pImages[i].posy );
                cJSON_AddNumberToObject( fileobj, "trimx", pImageInfo->pImages[i].trimmedx );
                cJSON_AddNumberToObject( fileobj, "trimy", pImageInfo->pImages[i].trimmedy );
                cJSON_AddNumberToObject( fileobj, "trimw", pImageInfo->pImages[i].trimmedw );
                cJSON_AddNumberToObject( fileobj, "trimh", pImageInfo->pImages[i].trimmedh );
                cJSON_AddItemToArray( filearray, fileobj );
            }

            // free memory
            //free( pImages[i].imagebuffer );
            //delete[] pImages[i].filename;
        }

        char* jsonstr = cJSON_Print( root );

        //printf( "%s\n", jsonstr );
        char outputjsonfile[260];//MAX_PATH];
        sprintf_s( outputjsonfile, 260, "%s.json", filename );
        FILE* file;
        fopen_s( &file, outputjsonfile, "w" );
        fprintf( file, jsonstr );
        fclose( file );

        delete jsonstr;
        cJSON_Delete( root );

        delete[] pNewImage;
        //delete[] pImages;
    }

    return pImageInfo;
}

bool PackTextures(ImageBlock* pImages, int filecount, int texw, int texh, int padding)
{
    rbp::MaxRectsBinPack m_BinPack;
    m_BinPack.Init( texw, texh );

    m_BinPack.MyHack_AllowRotation = false;

    for( int i=0; i<filecount; i++ )
    {
        if( pImages[i].trimmedw == 0 )
            pImages[i].binrect = m_BinPack.Insert( pImages[i].w+padding, pImages[i].h+padding, rbp::MaxRectsBinPack::RectContactPointRule );
        else
            pImages[i].binrect = m_BinPack.Insert( pImages[i].trimmedw+padding, pImages[i].trimmedh+padding, rbp::MaxRectsBinPack::RectContactPointRule );
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
    unsigned int sourceoffsetx = 0;
    unsigned int sourceoffsety = 0;
    unsigned int width = src->w;
    unsigned int height = src->h;

    if( src->trimmedw != 0 )
    {
        sourceoffsetx = src->trimmedx;
        sourceoffsety = src->trimmedy;
        width = src->trimmedw;
        height = src->trimmedh;
    }

    for( unsigned int y=0; y<height; y++ )
    {
        for( unsigned int x=0; x<width; x++ )
        {
            int destoffset = ((src->binrect.y+y)*destw + (src->binrect.x+x));
            int srcoffset = ((sourceoffsety + y)*src->w + sourceoffsetx + x);

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

            bool added = false;

            if( prevpoint.x != -1 )
            {
                if( prevprevpoint.x == -1 )
                {
                    //printf( "Adding first point - (%f, %f)\n", prevpoint.x, prevpoint.y );
                    polyline.push_back( new p2t::Point( prevpoint ) );
                    added = true;
                }
                else
                {
                    // ignore straight axis-aligned lines
                    if( ( currpoint.x == prevpoint.x && currpoint.x == prevprevpoint.x ) ||
                        ( currpoint.y == prevpoint.y && currpoint.y == prevprevpoint.y ) )
                    {
                    }
                    // ignore straight diagonal lines
                    else if( (abs(currpoint.x - prevpoint.x) == abs(currpoint.y - prevpoint.y)) && 
                             (abs(prevprevpoint.x - prevpoint.x) == abs(prevprevpoint.y - prevpoint.y)) )
                    {

                    }
                    // ignore tight single pixel corners going inwards
                    else if( (currpoint.x == prevpoint.x-1 && prevprevpoint.y == prevpoint.y-1) ||
                             (currpoint.y == prevpoint.y+1 && prevprevpoint.x == prevpoint.x-1) ||
                             (currpoint.x == prevpoint.x+1 && prevprevpoint.y == prevpoint.y+1) ||
                             (currpoint.y == prevpoint.y-1 && prevprevpoint.x == prevpoint.x+1) )
                    {
                        //polyline.push_back( new p2t::Point( prevpoint ) );
                        //added = true;
                    }
                    else
                    {
                        //printf( "Adding point       - (%f, %f)\n", prevpoint.x, prevpoint.y );
                        polyline.push_back( new p2t::Point( prevpoint ) );
                        added = true;
                    }
                }
            }

            if( added )
                prevprevpoint = prevpoint;
            prevpoint = currpoint;
        }

        //printf( "Adding last point  - (%f, %f)\n", prevpoint.x, prevpoint.y );
        polyline.push_back( new p2t::Point( prevpoint ) );

        // triangulate
        p2t::CDT* cdt = new p2t::CDT( polyline );

        cdt->Triangulate();

        std::vector<p2t::Triangle*> triangles = cdt->GetTriangles();

        //std::list<p2t::Triangle*> map = cdt->GetMap();

        printf( "Number of triangles: %d\n", triangles.size() );

        for( unsigned int t=0; t<triangles.size(); t++ )
        {
            p2t::Point* point0 = triangles[t]->GetPoint( 0 );
            p2t::Point* point1 = triangles[t]->GetPoint( 1 );
            p2t::Point* point2 = triangles[t]->GetPoint( 2 );
            printf( "triangles %d: (%0.2f,%0.2f), (%0.2f,%0.2f), (%0.2f,%0.2f)\n",
                     t, point0->x, point0->y, point1->x, point1->y, point2->x, point2->y );
        }

        // Cleanup
        pImages[i].cdt = cdt;
        //delete cdt;
        //for( std::vector<p2t::Point*>::iterator p = polyline.begin(); p != polyline.end(); p++ )
        //    delete *p;

        polyline.clear();
    }
}

void TrimSprites(ImageBlock* pImages, int filecount, int trim)
{
    for( int i=0; i<filecount; i++ )
    {
        unsigned int w = pImages[i].w;
        unsigned int h = pImages[i].h;
        unsigned char* pPixels = pImages[i].imagebuffer;

        // find edges of sprite
        unsigned int top = 0;
        unsigned int bottom = pImages[i].h;
        unsigned int left = 0;
        unsigned int right = pImages[i].w;

        // find the top
        for( unsigned int y=0; y<h; y++ )
        {
            for( unsigned int x=0; x<w; x++ )
            {
                if( pPixels[(y*w + x)*4 + 3] > trim )
                {
                    top = y;
                    x = w;
                    y = h;
                }
            }
        }

        // find the bottom
        for( unsigned int y=h-1; y<h; y-- )
        {
            for( unsigned int x=0; x<w; x++ ) // unsigned int fun
            {
                if( pPixels[(y*w + x)*4 + 3] > trim )
                {
                    bottom = y;
                    x = w;
                    y = h+1;
                }
            }
        }

        // find the left side
        for( unsigned int x=0; x<w; x++ )
        {
            for( unsigned int y=0; y<h; y++ )
            {
                if( pPixels[(y*w + x)*4 + 3] > trim )
                {
                    left = x;
                    x = w;
                    y = h;
                }
            }
        }

        // find the right side
        for( unsigned int x=w-1; x<w; x-- ) // unsigned int fun
        {
            for( unsigned int y=0; y<h; y++ )
            {
                if( pPixels[(y*w + x)*4 + 3] > trim )
                {
                    right = x;
                    x = w+1;
                    y = h;
                }
            }
        }

        pImages[i].trimmedx = left;
        pImages[i].trimmedy = top;
        pImages[i].trimmedw = right-left + 1;
        pImages[i].trimmedh = bottom-top + 1;

        int bp = 1;
    }
}
