#include "CommonHeader.h"

GameCore* g_pGameCore = 0;

GameCore::GameCore()
{
    g_pGameCore = this;

    m_pImageInfo = 0;
    m_pTextureShader = 0;

    m_CurrentTexture = 0;
}

GameCore::~GameCore()
{
    delete m_pImageInfo;
    delete m_pTextureShader;

    glDeleteBuffers( 1, &m_QuadVBO );
}

void GameCore::OneTimeInit(int argc, char** argv)
{
    m_pImageInfo = SpriteTool_ParseArgsAndCreateSpriteSheet( argc, argv );

    if( m_pImageInfo == 0 )
        return;

    m_pTextureShader = new ShaderProgram;
    m_pTextureShader->InitFromString(
"attribute vec2 a_Position; \
attribute vec2 a_UV; \
varying vec2 v_UV; \
void main() \
{ \
    gl_Position = vec4( a_Position, 0.0, 1.0 ); \
    v_UV = a_UV; \
} \
",
" \
uniform sampler2D u_Tex; \
varying vec2 v_UV; \
void main() \
{ \
    gl_FragColor = texture2D( u_Tex, v_UV ); \
} \
" );

    glGenBuffers( 1, &m_QuadVBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_QuadVBO );
    float quadverts[6*4] =
    {
        -1,  0.5f, 0, 0,
        -1, -0.5f, 0, 1,
         0,  0.5f, 1, 0,
         0,  0.5f, 1, 0,
        -1, -0.5f, 0, 1,
         0, -0.5f, 1, 1,
    };
    glBufferData( GL_ARRAY_BUFFER, 6*4*sizeof(float), quadverts, GL_STATIC_DRAW );

    for( unsigned int i=0; i<m_pImageInfo->NumImages; i++ )
    {
        glGenTextures( 1, &m_pImageInfo->pImages[i].texturehandle );
        glGenBuffers( 1, &m_pImageInfo->pImages[i].vbo );

        glBindTexture( GL_TEXTURE_2D, m_pImageInfo->pImages[i].texturehandle );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_pImageInfo->pImages[i].w, m_pImageInfo->pImages[i].h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pImageInfo->pImages[i].imageBuffer );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

        glBindBuffer( GL_ARRAY_BUFFER, m_pImageInfo->pImages[i].vbo );

        if( m_pImageInfo->pImages[i].cdts[0] )
        {
            std::vector<p2t::Triangle*> triangles = m_pImageInfo->pImages[i].cdts[0]->GetTriangles();
            unsigned int numtris = triangles.size();
            m_pImageInfo->pImages[i].numtris = numtris;
            float* verts = new float[numtris*3*4];

            for( unsigned int t=0; t<numtris; t++ )
            {
                p2t::Point* point0 = triangles[t]->GetPoint( 0 );
                p2t::Point* point1 = triangles[t]->GetPoint( 1 );
                p2t::Point* point2 = triangles[t]->GetPoint( 2 );

                verts[t*3*4 + 0*4 + 0] = (float)( point0->x / (float)m_pImageInfo->pImages[i].w );
                verts[t*3*4 + 0*4 + 1] = (float)( -point0->y / (float)m_pImageInfo->pImages[i].h + 0.5f );
                verts[t*3*4 + 0*4 + 2] = (float)( point0->x / (float)m_pImageInfo->pImages[i].w );
                verts[t*3*4 + 0*4 + 3] = (float)( point0->y / (float)m_pImageInfo->pImages[i].h );

                verts[t*3*4 + 1*4 + 0] = (float)( point2->x / (float)m_pImageInfo->pImages[i].w );
                verts[t*3*4 + 1*4 + 1] = (float)( -point2->y / (float)m_pImageInfo->pImages[i].h + 0.5f );
                verts[t*3*4 + 1*4 + 2] = (float)( point2->x / (float)m_pImageInfo->pImages[i].w );
                verts[t*3*4 + 1*4 + 3] = (float)( point2->y / (float)m_pImageInfo->pImages[i].h );

                verts[t*3*4 + 2*4 + 0] = (float)( point1->x / (float)m_pImageInfo->pImages[i].w );
                verts[t*3*4 + 2*4 + 1] = (float)( -point1->y / (float)m_pImageInfo->pImages[i].h + 0.5f );
                verts[t*3*4 + 2*4 + 2] = (float)( point1->x / (float)m_pImageInfo->pImages[i].w );
                verts[t*3*4 + 2*4 + 3] = (float)( point1->y / (float)m_pImageInfo->pImages[i].h );

                int bp = 1;
            }

            glBufferData( GL_ARRAY_BUFFER, numtris*3*4*sizeof(float), verts, GL_STATIC_DRAW );
    
            delete verts;
        }
        else
        {
            // make a smaller quad based on trimmed size.
            m_pImageInfo->pImages[i].numtris = 2;
            float x = (float)m_pImageInfo->pImages[i].trimmedX;
            float y = (float)m_pImageInfo->pImages[i].trimmedY;
            float w = (float)m_pImageInfo->pImages[i].trimmedW;
            float h = (float)m_pImageInfo->pImages[i].trimmedH;

            float origw = (float)m_pImageInfo->pImages[i].w;
            float origh = (float)m_pImageInfo->pImages[i].h;

            float quadverts[6*4] =
            {
                 0 + x/origw,      0.5f - y/origh,      x/origw,     y/origh,     // top left
                 0 + x/origw,      0.5f - (y+h)/origh,  x/origw,     (y+h)/origh, // bottom left
                 0 + (x+w)/origw,  0.5f - y/origh,      (x+w)/origw, y/origh,     // top right
                 0 + (x+w)/origw,  0.5f - y/origh,      (x+w)/origw, y/origh,     // top right
                 0 + x/origw,      0.5f - (y+h)/origh,  x/origw,     (y+h)/origh, // bottom left
                 0 + (x+w)/origw,  0.5f - (y+h)/origh,  (x+w)/origw, (y+h)/origh, // bottom right
            };
            glBufferData( GL_ARRAY_BUFFER, 6*4*sizeof(float), quadverts, GL_STATIC_DRAW );
        }
    }
}

void GameCore::OnSurfaceChanged(unsigned int width, unsigned int height)
{
    glViewport( 0, 0, width, height );
}

extern bool g_KeyStates[256];
extern bool g_KeyStatesOld[256];

void GameCore::Tick(double TimePassed)
{
    if( g_KeyStates[VK_LEFT] == true && g_KeyStatesOld[VK_LEFT] == false )
    {
        if( m_CurrentTexture > 0 )
            m_CurrentTexture--;
    }

    if( g_KeyStates[VK_RIGHT] == true && g_KeyStatesOld[VK_RIGHT] == false )
    {
        if( m_pImageInfo->NumImages != 0 && m_CurrentTexture < m_pImageInfo->NumImages-1 )
            m_CurrentTexture++;
    }

    if( g_KeyStates['X'] == true && g_KeyStatesOld['X'] == false )
    {
        m_ShowTriangles = !m_ShowTriangles;
    }
    
}

void GameCore::OnDrawFrame()
{
    glClearColor( 0.0f, 0.0f, 0.5f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    glUseProgram( m_pTextureShader->m_Program );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glBindTexture( GL_TEXTURE_2D, m_pImageInfo->pImages[m_CurrentTexture].texturehandle );

    // draw the quad on the left
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_QuadVBO );
        glUniform1f( 0, 0 );
        glVertexAttribPointer( 0, 2, GL_FLOAT, false, 4*4, 0 );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 1, 2, GL_FLOAT, false, 4*4, (void*)8 );
        glEnableVertexAttribArray( 1 );

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glDrawArrays( GL_TRIANGLES, 0, 6 );
    }

    // draw the mesh on the right
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_pImageInfo->pImages[m_CurrentTexture].vbo );
        glUniform1f( 0, 0 );
        glVertexAttribPointer( 0, 2, GL_FLOAT, false, 4*4, 0 );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 1, 2, GL_FLOAT, false, 4*4, (void*)8 );
        glEnableVertexAttribArray( 1 );

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glDrawArrays( GL_TRIANGLES, 0, m_pImageInfo->pImages[m_CurrentTexture].numtris*3 );

        if( m_ShowTriangles )
        {
            glUseProgram( 0 );
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glDrawArrays( GL_TRIANGLES, 0, m_pImageInfo->pImages[m_CurrentTexture].numtris*3 );
        }
    }
}
