#include "CommonHeader.h"

int main(int argc, char** argv)
{
    ImageBlockInfo* pImageInfo = SpriteTool_ParseArgsAndCreateSpriteSheet(argc, argv);
    delete pImageInfo;
}

ImageBlockInfo* SpriteTool_ParseArgsAndCreateSpriteSheet(int argc, char** argv)
{
    bool invalidArgs = false;

    if( argc < 2 )
        invalidArgs = true;

    SettingsStruct settings;

    ImageBlockInfo* pImageInfo = 0;
    ImageBlock* pImages = 0;

    for( int i=1; i<argc; i++ )
    {
        if( ( strcmp( argv[i], "-i" ) == 0 || strcmp( argv[i], "-input" ) == 0 ) )
        {
            if( i+1 >= argc )
                invalidArgs = true;
            else
                settings.inputDir = argv[i+1];
        }
        if( ( strcmp( argv[i], "-o" ) == 0 || strcmp( argv[i], "-output" ) == 0 ) )
        {
            if( i+1 >= argc )
                invalidArgs = true;
            else
                settings.outputFilename = argv[i+1];
        }
        if( ( strcmp( argv[i], "-e" ) == 0 || strcmp( argv[i], "-extension" ) == 0 ) )
        {
            if( i+1 >= argc )
                invalidArgs = true;
            else
                settings.extensionString = argv[i+1];
        }
        if( ( strcmp( argv[i], "-p" ) == 0 || strcmp( argv[i], "-padding" ) == 0 ) )
        {
            if( i+1 >= argc )
                invalidArgs = true;
            else
                settings.padding = atoi( argv[i+1] );
        }
        if( ( strcmp( argv[i], "-ex" ) == 0 || strcmp( argv[i], "-extrude" ) == 0 ) )
        {
            settings.extrude = true;
        }
        if( ( strcmp( argv[i], "-t" ) == 0 || strcmp( argv[i], "-trim" ) == 0 ) )
        {
            settings.trim = true;

            if( i+1 >= argc )
                invalidArgs = true;
            else
                settings.trimAlpha = atoi( argv[i+1] );
        }
        if( ( strcmp( argv[i], "-s" ) == 0 || strcmp( argv[i], "-strip" ) == 0 ) )
        {
            settings.createStrip = true;
        }
        if( ( strcmp( argv[i], "-min" ) == 0 || strcmp( argv[i], "-min" ) == 0 ) )
        {
            if( i+1 >= argc )
                invalidArgs = true;
            else
                settings.minTextureSize = atoi( argv[i+1] );
        }
        if( ( strcmp( argv[i], "-m" ) == 0 || strcmp( argv[i], "-max" ) == 0 ) )
        {
            if( i+1 >= argc )
                invalidArgs = true;
            else
                settings.maxTextureSize = atoi( argv[i+1] );
        }
        if( ( strcmp( argv[i], "-w" ) == 0 || strcmp( argv[i], "-wide" ) == 0 ) )
        {
            settings.growWide = true;
        }
        if( ( strcmp( argv[i], "-bl" ) == 0 || strcmp( argv[i], "-bottomleft" ) == 0 ) )
        {
            settings.originAtBottomLeft = true;
        }
        if( ( strcmp( argv[i], "-tri" ) == 0 || strcmp( argv[i], "-triangulate" ) == 0 ) )
        {
            settings.triangulate = true;
        }
        if( ( strcmp( argv[i], "-sp" ) == 0 || strcmp( argv[i], "-split" ) == 0 ) )
        {
            settings.splitExistingStrips = true;

            if( i+1 >= argc )
            {
                invalidArgs = true;
            }
            else
            {
                int n = sscanf_s( argv[i+1], "%dx%d", &settings.splitSpritesheetsWidth, &settings.splitSpritesheetsHeight );
            }
        }
    }
    
    if( invalidArgs )
    {
        printf( "Invalid arguments\n" );
        printf( "\n" );
        printf( "[-i directory] or -input = relative or absolute path of input directory\n" );
        printf( "[-o name] or -output = output name without extension\n" );
        printf( "[-e extension] or -extension = custom extension (include the .), default to .json\n" );
        printf( "[-p pixels] or -padding = padding between sprites in pixels - default is 0\n" );
        printf( "[-ex] or -extrude = extrude sprites by 1 pixel in each direction, will disable padding setting\n" );
        printf( "[-t minalpha] or -trim = enable trim with minimum alpha for trimming - specify alpha from 0 to 255\n" );
        printf( "[-min size] or -min = minimum output texture size - default is 64\n" );
        printf( "[-m size] or -max = maximum output texture size - default is 2048\n" );
        printf( "[-w] or -wide = prefer wide output textures - default is square textures\n" );
        printf( "[-s] or -strip = create sprite strip, maintaining order of files, disables padding, trim and triangulate\n" );
        printf( "[-bl] or -bottomleft = for spritestrips, start at bottom left corner\n" );
        printf( "[-tri] or -triangulate = triangulate the sprites(WIP)\n" );
        printf( "[-sp WxH] or -split = split existing spritestrips - specify width and height of sprites in source image - default is 32x32\n" );
    }
    else if( settings.inputDir == 0 )
    {
        printf( "Input directory required - use -i\n" );
    }
    else if( settings.outputFilename == 0 )
    {
        printf( "Output filename required - use -o\n" );
    }
    else if( settings.padding < 0 || settings.padding > 10 )
    {
        printf( "Invalid padding amount - must be value between 0 and 10\n" );
    }
    else if( settings.trim && (settings.trimAlpha < 0 || settings.trimAlpha > 255) )
    {
        printf( "Invalid trim alpha threshhold - must be value between 0 and 255\n" );
    }
    else if( settings.splitExistingStrips && settings.splitSpritesheetsWidth <= 0 )
    {
        printf( "Invalid split width - must be greater than 0\n" );
    }
    else
    {
        printf( "Starting\n" );
        printf( "Source image directory -> %s\n", settings.inputDir );
        
        if( settings.splitExistingStrips )
        {
            printf( "Spliting existing strips -> %dx%d - disabling everything else\n", settings.splitSpritesheetsWidth, settings.splitSpritesheetsHeight );
            printf( "    Treating output filename as folder: %s\n", settings.outputFilename );
            settings.createStrip = false;
            settings.growWide = false;
            settings.originAtBottomLeft = false;
            settings.padding = 0;
            settings.trim = false;
            settings.triangulate = false;

            pImageInfo = SplitSpriteSheets( settings );
        }
        else
        {
            printf( "Output filename -> %s\n", settings.outputFilename );

            if( settings.createStrip )
            {
                printf( "Create strip -> %d - disabling padding, trim and triangulate\n", settings.createStrip );
                settings.padding = 0;
                settings.trim = false;
                settings.triangulate = false;

                if( settings.originAtBottomLeft )
                {
                    printf( "    Starting at bottom left\n" );
                }
            }
            if( settings.extrude )
            {
                printf( "Extrude -> Enabled - disabling padding\n" );
                // PackTextures will put both pixels on the right and bottom, so we'll shift by 1 pixel each way to center the image.
                settings.padding = 2;
            }
            printf( "Min texture size -> %d\n", settings.minTextureSize );
            printf( "Max texture size -> %d\n", settings.maxTextureSize );
            printf( "Texture shape -> %s\n", settings.growWide ? "wide" : "square" );
            printf( "Padding -> %d\n", settings.padding );
            if( settings.trim )
                printf( "Trim -> Enabled -> %d\n", settings.trimAlpha );
            else
                printf( "Trim -> Disabled\n" );
            if( settings.triangulate )
                printf( "Triangulate -> Enabled\n" );
            else
                printf( "Triangulate -> Disabled\n" );
            if( settings.originAtBottomLeft )
                printf( "Origin -> Bottom Left\n" );
            else
                printf( "Origin -> Top Left\n" );

            pImageInfo = CreateSpriteSheet( settings );
        }

        printf( "done\n" );
    }

#if _DEBUG

#if !MYFW_WINDOWS
    _getch();
#else
    getchar();
#endif

#endif // _DEBUG

	return pImageInfo;
}

void listdir(std::vector<std::string>* pFileList, const char *name, const char* ext)
{
    DIR* dir;
    struct dirent* entry;

    if( (dir = opendir(name)) == 0 )
        return;

    while( (entry = readdir(dir)) != 0 )
    {
        char path[PATH_MAX];

        if( entry->d_type == DT_DIR )
        {
            if( strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 )
                continue;

            sprintf_s( path, sizeof(path), "%s/%s", name, entry->d_name );
            listdir( pFileList, path, ext );
        }
        else
        {
            int len = strlen( entry->d_name );
            if( len > 4 )
            {
                if( strcmp( &entry->d_name[len-4], ext ) == 0 )
                {
                    sprintf_s( path, sizeof(path), "%s/%s", name, entry->d_name );
                    pFileList->push_back( path );
                }
            }
        }
    }

    closedir(dir);
}

ImageBlockInfo* SplitSpriteSheets(SettingsStruct settings)
{
    std::vector<std::string> fileList;
    listdir( &fileList, settings.inputDir, ".png" );
    CreateDirectory( settings.outputFilename, 0 );

    int fileCount = fileList.size();
    if( fileList.size() == 0 )
        return 0;

    ImageBlockInfo* pImageInfo = new ImageBlockInfo;
    pImageInfo->pImages = new ImageBlock[fileCount];
    pImageInfo->NumImages = fileCount;

    unsigned int highestX = 0;
    unsigned int highestY = 0;

    // Load all the images.
    for( unsigned int i=0; i<fileList.size(); i++ )
    {
        const char* relativePathCStr = fileList[i].c_str();
        const char* filenameCStr = fileList[i].c_str();

        int len = strlen( filenameCStr );
        int srcDirNameLen = strlen( settings.inputDir );
        int finalLen = len - (srcDirNameLen+1) + 1;
        pImageInfo->pImages[i].filename = new char[finalLen];
        strcpy_s( pImageInfo->pImages[i].filename, finalLen, &filenameCStr[srcDirNameLen+1] );
        lodepng_decode32_file( &pImageInfo->pImages[i].imageBuffer,
                               &pImageInfo->pImages[i].w, &pImageInfo->pImages[i].h,
                               relativePathCStr );
    }

    // Create the new images.
    for( int i=0; i<fileCount; i++ )
    {
        unsigned int spriteWidth = settings.splitSpritesheetsWidth;
        unsigned int spriteHeight = settings.splitSpritesheetsHeight;
        if( settings.splitSpritesheetsHeight == -1 )
            spriteHeight = pImageInfo->pImages[i].h;

        unsigned int numFramesX = pImageInfo->pImages[i].w / spriteWidth;
        unsigned int numFramesY = pImageInfo->pImages[i].h / spriteHeight;
        for( unsigned int frame=0; frame<numFramesX*numFramesY; frame++ )
        {
            int frameX = frame % numFramesX;
            int frameY = frame / numFramesX;

            unsigned char* pNewImage = 0;
            pNewImage = new unsigned char[spriteWidth*spriteHeight*4];
            memset( pNewImage, 0, spriteWidth*spriteHeight*4 );

            unsigned int x = spriteWidth * frameX;
            unsigned int y = spriteHeight * frameY;
            CopyImageChunk( pNewImage, spriteWidth, spriteHeight, &pImageInfo->pImages[i], x, y, spriteWidth, spriteHeight );

            char outputfile[PATH_MAX];
            char filenameWithoutExtension[PATH_MAX];
            strncpy_s( filenameWithoutExtension, pImageInfo->pImages[i].filename, strlen(pImageInfo->pImages[i].filename)-4 );
            sprintf_s( outputfile, 260, "%s/%s_%04d.png", settings.outputFilename, filenameWithoutExtension, frame+1 );
            lodepng_encode32_file( outputfile, pNewImage, spriteWidth, spriteHeight );

            delete[] pNewImage;
        }
    }

    return pImageInfo;
}

ImageBlockInfo* CreateSpriteSheet(SettingsStruct settings)
{
    std::vector<std::string> fileList;
    listdir( &fileList, settings.inputDir, ".png" );

    int fileCount = fileList.size();
    if( fileList.size() == 0 )
        return 0;

    ImageBlockInfo* pImageInfo = new ImageBlockInfo;
    pImageInfo->pImages = new ImageBlock[fileCount];
    pImageInfo->NumImages = fileCount;

    unsigned int highestX = 0;
    unsigned int highestY = 0;

    // Load all the images.
    for( unsigned int i=0; i<fileList.size(); i++ )
    {
        const char* relativePathCStr = fileList[i].c_str();
        const char* filenameCStr = fileList[i].c_str();

        int len = strlen( filenameCStr );
        int srcDirNameLen = strlen( settings.inputDir );
        int finalLen = len - (srcDirNameLen+1) + 1;
        pImageInfo->pImages[i].filename = new char[finalLen];
        strcpy_s( pImageInfo->pImages[i].filename, finalLen, &filenameCStr[srcDirNameLen+1] );
        lodepng_decode32_file( &pImageInfo->pImages[i].imageBuffer,
                               &pImageInfo->pImages[i].w, &pImageInfo->pImages[i].h,
                               filenameCStr );
    }

    // Triangulate the sprites, this will also trim them.
    if( settings.triangulate )
        TriangulateSprites( pImageInfo->pImages, fileCount );
    else
        TrimSprites( pImageInfo->pImages, fileCount, settings.trim ? settings.trimAlpha : -1 );

    // Try to fit them into texture.
    bool done = false;
    unsigned int sizeX = settings.minTextureSize;
    unsigned int sizeY = settings.minTextureSize;
    while( done == false )
    {
        if( settings.createStrip )
        {
            bool allowMultipleLines = false;
            if( sizeX == settings.maxTextureSize )
                allowMultipleLines = true;

            done = PackTextures_SpriteStrip( pImageInfo->pImages, fileCount, sizeX, sizeY, settings.padding, settings.originAtBottomLeft, allowMultipleLines );
        }
        else
        {
            done = PackTextures( pImageInfo->pImages, fileCount, sizeX, sizeY, settings.padding, settings.extrude );
        }

        if( done == false )
        {
            if( settings.createStrip || settings.growWide )
            {
                sizeX *= 2;
                if( sizeX > settings.maxTextureSize )
                {
                    sizeX = settings.minTextureSize;
                    sizeY *= 2;
                }
            }
            else
            {
                if( sizeX <= sizeY )
                    sizeX *= 2;
                else
                    sizeY *= 2;
            }
        }

        if( sizeX > settings.maxTextureSize && sizeY > settings.maxTextureSize )
            break;
    }

    printf( "Dimensions (%d, %d)\n", sizeX, sizeY );

    // Create the new image.
    unsigned char* pNewImage = 0;
    if( done == true )
    {
        pNewImage = new unsigned char[sizeX*sizeY*4];
        memset( pNewImage, 0, sizeX*sizeY*4 );

        for( int i=0; i<fileCount; i++ )
        {
            CopyImageChunk( pNewImage, sizeX, sizeY, &pImageInfo->pImages[i], 0, 0, 0, 0, settings.extrude );
        }

        char outputfile[PATH_MAX];
        sprintf_s( outputfile, 260, "%s.png", settings.outputFilename );
        lodepng_encode32_file( outputfile, pNewImage, sizeX, sizeY );

        cJSON* jRoot = cJSON_CreateObject();
        cJSON* jSpriteArray = cJSON_CreateArray();

        cJSON_AddNumberToObject( jRoot, "spritetool", 3 );
        cJSON_AddStringToObject( jRoot, "filename", outputfile );
        cJSON* jSize = cJSON_CreateArray();
        cJSON_AddItemToObject( jRoot, "size", jSize );
        cJSON_AddItemToArray( jSize, cJSON_CreateNumber(sizeX) );
        cJSON_AddItemToArray( jSize, cJSON_CreateNumber(sizeY) );

        cJSON_AddItemToObject( jRoot, "sprites", jSpriteArray );

        for( int i=0; i<fileCount; i++ )
        {
            if( done == true )
            {
                // Treat bottom left as (0,0), but not for sprite strips (they're flipped in PackTextures_SpriteStrip).
                if( settings.originAtBottomLeft && settings.createStrip == false )
                {
                    pImageInfo->pImages[i].y = sizeY - pImageInfo->pImages[i].y - pImageInfo->pImages[i].h;
                    assert( pImageInfo->pImages[i].y < sizeY );
                }

                char filenameWithoutExtension[PATH_MAX];
                int fullNameLen = strlen(pImageInfo->pImages[i].filename);
                int extensionLen = 0;
                for( int j=fullNameLen-1; j>=1; j-- )
                {
                    if( pImageInfo->pImages[i].filename[j] == '.' )
                    {
                        extensionLen = fullNameLen - j;
                        break;
                    }
                }
                strncpy_s( filenameWithoutExtension, pImageInfo->pImages[i].filename, fullNameLen-extensionLen );

                cJSON* jSprite = cJSON_CreateObject();
                cJSON_AddStringToObject( jSprite, "name", filenameWithoutExtension );
                
                cJSON* jPos = cJSON_CreateArray();
                cJSON_AddItemToObject( jSprite, "pos", jPos );
                cJSON_AddItemToArray( jPos, cJSON_CreateNumber(pImageInfo->pImages[i].x) );
                cJSON_AddItemToArray( jPos, cJSON_CreateNumber(pImageInfo->pImages[i].y) );
                
                cJSON* jSize = cJSON_CreateArray();
                cJSON_AddItemToObject( jSprite, "size", jSize );
                cJSON_AddItemToArray( jSize, cJSON_CreateNumber(pImageInfo->pImages[i].w) );
                cJSON_AddItemToArray( jSize, cJSON_CreateNumber(pImageInfo->pImages[i].h) );
                
                if( settings.trim )
                {
                    cJSON* jPos = cJSON_CreateArray();
                    cJSON_AddItemToObject( jSprite, "trimpos", jPos );
                    cJSON_AddItemToArray( jPos, cJSON_CreateNumber(pImageInfo->pImages[i].trimmedX) );
                    cJSON_AddItemToArray( jPos, cJSON_CreateNumber(pImageInfo->pImages[i].trimmedY) );

                    cJSON* jSize = cJSON_CreateArray();
                    cJSON_AddItemToObject( jSprite, "trimsize", jSize );
                    cJSON_AddItemToArray( jSize, cJSON_CreateNumber(pImageInfo->pImages[i].trimmedW) );
                    cJSON_AddItemToArray( jSize, cJSON_CreateNumber(pImageInfo->pImages[i].trimmedH) );
                }
                cJSON_AddItemToArray( jSpriteArray, jSprite );

                if( pImageInfo->pImages[i].cdts.size() > 0 && pImageInfo->pImages[i].cdts[0] )
                {
                    cJSON* jVertexArray = cJSON_CreateArray();
                    cJSON_AddItemToObject( jSprite, "verts", jVertexArray );

                    cJSON* jIndexArray = cJSON_CreateArray();
                    cJSON_AddItemToObject( jSprite, "indices", jIndexArray );

                    std::vector<p2t::Triangle*> triangles = pImageInfo->pImages[i].cdts[0]->GetTriangles();

                    //std::list<p2t::Triangle*> map = cdt->GetMap();

                    std::vector<p2t::Point> vertices;
                    std::vector<unsigned int> indices;

                    for( unsigned int t=0; t<triangles.size(); t++ )
                    {
                        for( unsigned int v=0; v<3; v++ )
                        {
                            p2t::Point* point = triangles[t]->GetPoint( v );

                            std::vector<p2t::Point>::iterator it = std::find( vertices.begin(), vertices.end(), *point );
                            if( it != vertices.end() )
                            {
                                // Duplicate vertex.
                                int index = it - vertices.begin();
                                indices.push_back( index );
                            }
                            else
                            {
                                // New vertex.
                                int index = vertices.size();
                                vertices.push_back( *point );
                                indices.push_back( index );
                            }
                        }
                    }

                    for( unsigned int i=0; i<vertices.size(); i++ )
                    {
                        cJSON_AddItemToArray( jVertexArray, cJSON_CreateNumber( vertices[i].x ) );
                        cJSON_AddItemToArray( jVertexArray, cJSON_CreateNumber( vertices[i].y ) );
                    }

                    for( unsigned int i=0; i<indices.size(); i++ )
                    {
                        cJSON_AddItemToArray( jIndexArray, cJSON_CreateNumber( indices[i] ) );
                    }
                }
            }

            // Free memory.
            //free( pImages[i].imageBuffer );
            //delete[] pImages[i].filename;
        }

        char* jsonstr = cJSON_Print( jRoot );

        //printf( "%s\n", jsonstr );
        char outputjsonfile[PATH_MAX];
        sprintf_s( outputjsonfile, 260, "%s%s", settings.outputFilename, settings.extensionString );
        FILE* file;
#if WIN32
        fopen_s( &file, outputjsonfile, "w" );
#else
        file = fopen( outputjsonfile, "w" );
#endif
        fprintf( file, jsonstr );
        fclose( file );

        delete jsonstr;
        cJSON_Delete( jRoot );

        delete[] pNewImage;
        //delete[] pImages;
    }

    return pImageInfo;
}

bool PackTextures(ImageBlock* pImages, int fileCount, unsigned int textureWidth, unsigned int textureHeight, int padding, bool extrude)
{
    rbp::MaxRectsBinPack m_BinPack;
    m_BinPack.Init( textureWidth, textureHeight );

    m_BinPack.MyHack_AllowRotation = false;

    for( int i=0; i<fileCount; i++ )
    {
        if( pImages[i].trimmedW == 0 )
            pImages[i].binRect = m_BinPack.Insert( pImages[i].w+padding, pImages[i].h+padding, rbp::MaxRectsBinPack::RectContactPointRule );
        else
            pImages[i].binRect = m_BinPack.Insert( pImages[i].trimmedW+padding, pImages[i].trimmedH+padding, rbp::MaxRectsBinPack::RectContactPointRule );
        //float occupancy = m_BinPack.Occupancy();

        if( extrude )
        {
            // When extrude is set, padding is set to 2 with both pixels on the right and bottom
            // so we'll shift by 1 pixel each way to center the image in the rect.
            assert( padding == 2 );
            pImages[i].binRect.x += 1;
            pImages[i].binRect.y += 1;
        }

        if( pImages[i].binRect.width == 0 )
            return false;

        pImages[i].x = pImages[i].binRect.x;
        pImages[i].y = pImages[i].binRect.y;

        assert( pImages[i].y < textureHeight );

        //if( rect.x + rect.width > (int)highestX )
        //    highestX = rect.x + rect.width;

        //if( rect.y + rect.height > (int)highestY )
        //    highestY = rect.y + rect.height;

        //printf( "%s (%d,%d)\n", pImages[i].filename, pImages[i].w, pImages[i].h );
        //printf( "%s occupancy %f%%\n", filenameCStr, occupancy );
    }

    return true;
}

bool PackTextures_SpriteStrip(ImageBlock* pImages, int fileCount, unsigned int textureWidth, unsigned int textureHeight, int padding, bool createFromBottomLeft, bool allowMultipleLines)
{
    unsigned int currx = 0;
    unsigned int curry = 0;
    unsigned int highestY = 0;

    for( int i=0; i<fileCount; i++ )
    {
        if( curry + pImages[i].h > (unsigned int)textureHeight )
        {
            return false;
        }

        if( currx + pImages[i].w > (unsigned int)textureWidth )
        {
            if( allowMultipleLines == false )
                return false;

            currx = 0;
            curry += pImages[i].h;

            if( curry > (unsigned int)textureHeight )
                return false;
        }

        {
            pImages[i].x = currx;
            pImages[i].y = curry;

            if( pImages[i].x >= textureWidth )
                return false;
            if( pImages[i].y >= textureHeight )
                return false;

            assert( pImages[i].y < textureHeight );

            currx += pImages[i].w;
        
            if( curry + pImages[i].h > highestY )
                highestY = curry + pImages[i].h;

            if( highestY > textureHeight )
                return false;
        }
    }

    if( createFromBottomLeft )
    {
        for( int i=0; i<fileCount; i++ )
        {
            pImages[i].y = textureHeight - pImages[i].y - pImages[i].h;
            assert( pImages[i].y < textureHeight );
        }
    }

    return true;
}

void CopyImageChunk(unsigned char* dest, unsigned int destWidth, unsigned int destHeight, ImageBlock* src, unsigned int startX, unsigned int startY, unsigned int widthToCopy, unsigned int heightToCopy, bool extrude)
{
    unsigned int sourceOffsetX = startX;
    unsigned int sourceOffsetY = startY;
    unsigned int width = widthToCopy;
    unsigned int height = heightToCopy;

    if( width == 0 )
    {
        width = src->w;
        height = src->h;
    }

    if( src->trimmedW != 0 )
    {
        sourceOffsetX = src->trimmedX;
        sourceOffsetY = src->trimmedY;
        width = src->trimmedW;
        height = src->trimmedH;
    }

    for( unsigned int y=0; y<height; y++ )
    {
        for( unsigned int x=0; x<width; x++ )
        {
            int destoffset = ((src->y+y)*destWidth + (src->x+x));
            int srcoffset = ((sourceOffsetY + y)*src->w + sourceOffsetX + x);

            ((int*)dest)[destoffset] = ((int*)src->imageBuffer)[srcoffset];
        }
    }

    if( extrude )
    {
        for( unsigned int y=0; y<height; y++ )
        {
            // Extrude left.
            int destoffset = ((src->y+y)*destWidth + (src->x-1));
            int srcoffset = ((sourceOffsetY + y)*src->w + sourceOffsetX);
            ((int*)dest)[destoffset] = ((int*)src->imageBuffer)[srcoffset];
        
            // Extrude right.
            destoffset = ((src->y+y)*destWidth + (src->x+width));
            srcoffset = ((sourceOffsetY + y)*src->w + sourceOffsetX + width-1);
            ((int*)dest)[destoffset] = ((int*)src->imageBuffer)[srcoffset];
        }

        for( unsigned int x=0; x<width; x++ )
        {
            // Extrude top.
            int destoffset = ((src->y-1)*destWidth + (src->x+x));
            int srcoffset = ((sourceOffsetY)*src->w + sourceOffsetX + x);
            ((int*)dest)[destoffset] = ((int*)src->imageBuffer)[srcoffset];
        
            // Extrude bottom.
            destoffset = ((src->y+height)*destWidth + (src->x+x));
            srcoffset = ((sourceOffsetY + height-1)*src->w + sourceOffsetX + x);
            ((int*)dest)[destoffset] = ((int*)src->imageBuffer)[srcoffset];
        }

        // Extrude top left.
        {
            int destoffset = ((src->y-1)*destWidth + (src->x-1));
            int srcoffset = ((sourceOffsetY)*src->w + sourceOffsetX);
            ((int*)dest)[destoffset] = ((int*)src->imageBuffer)[srcoffset];
        }

        // Extrude top right.
        {
            int destoffset = ((src->y-1)*destWidth + (src->x+width));
            int srcoffset = ((sourceOffsetY)*src->w + sourceOffsetX + width-1);
            ((int*)dest)[destoffset] = ((int*)src->imageBuffer)[srcoffset];
        }

        // Extrude bottom left.
        {
            int destoffset = ((src->y+height)*destWidth + (src->x-1));
            int srcoffset = ((sourceOffsetY + height-1)*src->w + sourceOffsetX);
            ((int*)dest)[destoffset] = ((int*)src->imageBuffer)[srcoffset];
        }

        // Extrude bottom right.
        {
            int destoffset = ((src->y+height)*destWidth + (src->x+width));
            int srcoffset = ((sourceOffsetY + height-1)*src->w + sourceOffsetX + width-1);
            ((int*)dest)[destoffset] = ((int*)src->imageBuffer)[srcoffset];
        }
    }
}

void TriangulateSprites(ImageBlock* pImages, int fileCount)
{
    for( int filei=0; filei<fileCount; filei++ )
    {
        int scale = 1;

        unsigned int origW = pImages[filei].w;
        unsigned int origH = pImages[filei].h;

        unsigned int scaledW;
        unsigned int scaledH;
        unsigned char* pscaledPixels;

        // Experimenting with catching corners more closely without changing marchingsquares code.
        if( scale != 1 )
        {
            scaledW = pImages[filei].w * scale;
            scaledH = pImages[filei].h * scale;
            pscaledPixels = new unsigned char[scaledW*scaledH*4];
            
            for( unsigned int oh=0; oh<origH; oh++ )
            {
                for( unsigned int ow=0; ow<origW; ow++ )
                {
                    for( int y=0; y<scale; y++ )
                    {
                        for( int x=0; x<scale; x++ )
                        {
                            assert( (oh*scale+y)*scaledW + ow+x < scaledW*scaledH*4 );
                            pscaledPixels[((oh*scale+y)*scaledW + ow*scale+x)*4 + 0] = pImages[filei].imageBuffer[(oh*origW + ow)*4 + 0];
                            pscaledPixels[((oh*scale+y)*scaledW + ow*scale+x)*4 + 1] = pImages[filei].imageBuffer[(oh*origW + ow)*4 + 1];
                            pscaledPixels[((oh*scale+y)*scaledW + ow*scale+x)*4 + 2] = pImages[filei].imageBuffer[(oh*origW + ow)*4 + 2];
                            pscaledPixels[((oh*scale+y)*scaledW + ow*scale+x)*4 + 3] = pImages[filei].imageBuffer[(oh*origW + ow)*4 + 3];
                        }
                    }
                }
            }
        }
        else
        {
            scaledW = origW;
            scaledH = origH;
            pscaledPixels = pImages[filei].imageBuffer;
        }

        // Find edges of sprite.
        std::list<ivec2> pPoints;
        MarchingSquares march;
        march.DoMarch( pscaledPixels, scaledW, scaledH, &pPoints );

        if( scale != 1 )
            delete pscaledPixels;

        // Add them to a vector of points for poly2tri.
        std::vector<p2t::Point*> polyLine;

        std::list<ivec2>::iterator p;

        if( true ) // Attempt to reduce the number of obvious points manually.
        {
            p2t::Point prevPoint(-1, -1), prevPrevPoint(-1, -1);
            for( p = pPoints.begin(); p != pPoints.end(); p++ )
            {
                p2t::Point currPoint( p->x, p->y );

                bool added = false;

                if( prevPoint.x != -1 )
                {
                    if( prevPrevPoint.x == -1 )
                    {
                        //printf( "Adding first point - (%f, %f)\n", prevPoint.x, prevPoint.y );
                        polyLine.push_back( new p2t::Point( prevPoint ) );
                        added = true;
                    }
                    else
                    {
                        // Ignore straight axis-aligned lines.
                        if( ( currPoint.x == prevPoint.x && currPoint.x == prevPrevPoint.x ) ||
                            ( currPoint.y == prevPoint.y && currPoint.y == prevPrevPoint.y ) )
                        {
                        }
                        // Ignore straight diagonal lines.
                        else if( (std::abs(currPoint.x - prevPoint.x) == std::abs(currPoint.y - prevPoint.y)) && 
                                 (std::abs(prevPrevPoint.x - prevPoint.x) == std::abs(prevPrevPoint.y - prevPoint.y)) )
                        {
                        }
                        // Ignore tight single pixel corners going inwards.
                        else if( (currPoint.x == prevPoint.x-1 && prevPrevPoint.y == prevPoint.y-1) ||
                                 (currPoint.y == prevPoint.y+1 && prevPrevPoint.x == prevPoint.x-1) ||
                                 (currPoint.x == prevPoint.x+1 && prevPrevPoint.y == prevPoint.y+1) ||
                                 (currPoint.y == prevPoint.y-1 && prevPrevPoint.x == prevPoint.x+1) )
                        {
                        }
                        else
                        {
                            //printf( "Adding point       - (%f, %f)\n", prevPoint.x, prevPoint.y );
                            polyLine.push_back( new p2t::Point( prevPoint ) );
                            added = true;
                        }
                    }
                }

                if( added )
                    prevPrevPoint = prevPoint;
                prevPoint = currPoint;
            }

            //printf( "Adding last point  - (%f, %f)\n", prevPoint.x, prevPoint.y );
            polyLine.push_back( new p2t::Point( prevPoint ) );
        }
        else // include all points from the marching squares
        {
            for( p = pPoints.begin(); p != pPoints.end(); p++ )
            {
                polyLine.push_back( new p2t::Point( p->x/(float)scale, p->y/(float)scale ) );
            }
        }

        // Eliminate unneccessary vertices using "psimpl::simplify_douglas_peucker".
        if( true )
        {
            // Add first point at the end to make a loop.
            ivec2 firstPoint = pPoints.front();
            polyLine.push_back( new p2t::Point( firstPoint.x, firstPoint.y ) );

            std::deque<double> polyLineQueue;
            double* result = new double[polyLine.size()*2];

            for( unsigned int i=0; i<polyLine.size(); i++ )
            {
                polyLineQueue.push_back( polyLine[i]->x );
                polyLineQueue.push_back( polyLine[i]->y );
            }

            double* endofresult = psimpl::simplify_douglas_peucker<2>( polyLineQueue.begin(), polyLineQueue.end(), 2.0f, result );
            double* resultcopy = result;

            polyLine.clear();
            while( result != endofresult - 2 ) // Don't copy last point, it should be the same as the first.
            {
                polyLine.push_back( new p2t::Point( result[0], result[1] ) );
                result += 2;
            }

            delete[] resultcopy;
        }

        // Triangulate.
        p2t::CDT* cdt = new p2t::CDT( polyLine );
        cdt->Triangulate();
        pImages[filei].cdts.push_back( cdt );

        // Print some status.
        std::vector<p2t::Triangle*> triangles = cdt->GetTriangles();
        printf( "Number of triangles: %d\n", triangles.size() );
        //for( unsigned int t=0; t<triangles.size(); t++ )
        //{
        //    p2t::Point* point0 = triangles[t]->GetPoint( 0 );
        //    p2t::Point* point1 = triangles[t]->GetPoint( 1 );
        //    p2t::Point* point2 = triangles[t]->GetPoint( 2 );
        //    printf( "triangles %d: (%0.2f,%0.2f), (%0.2f,%0.2f), (%0.2f,%0.2f)\n",
        //             t, point0->x, point0->y, point1->x, point1->y, point2->x, point2->y );
        //}
    }
}

void TrimSprites(ImageBlock* pImages, int fileCount, int trim)
{
    for( int i=0; i<fileCount; i++ )
    {
        unsigned int w = pImages[i].w;
        unsigned int h = pImages[i].h;
        unsigned char* pPixels = pImages[i].imageBuffer;

        // Find edges of sprite.
        unsigned int top = 0;
        unsigned int bottom = pImages[i].h;
        unsigned int left = 0;
        unsigned int right = pImages[i].w;

        // Find the top.
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

        // Find the bottom.
        for( unsigned int y=h-1; y<h; y-- )
        {
            for( unsigned int x=0; x<w; x++ )
            {
                if( pPixels[(y*w + x)*4 + 3] > trim )
                {
                    bottom = y;
                    x = w;
                    y = h+1;
                }
            }
        }

        // Find the left side.
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

        // Find the right side.
        for( unsigned int x=w-1; x<w; x-- )
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

        pImages[i].trimmedX = left;
        pImages[i].trimmedY = top;
        pImages[i].trimmedW = right-left + 1;
        pImages[i].trimmedH = bottom-top + 1;
    }
}
