#include "CommonHeader.h"

GameCore* g_pGameCore = 0;

GameCore::GameCore()
{
    g_pGameCore = this;
}

GameCore::~GameCore()
{
}

void GameCore::OneTimeInit()
{
}

void GameCore::Tick(double TimePassed)
{
}

void GameCore::OnSurfaceChanged(unsigned int width, unsigned int height)
{
    glViewport( 0, 0, width, height );
}

void GameCore::OnDrawFrame()
{
    glClearColor( 0.0f, 0.0f, 0.5f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT );
}
