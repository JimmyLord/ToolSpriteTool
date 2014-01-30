#ifndef __GameCore_H__
#define __GameCore_H__

class GameCore;

extern GameCore* g_pGameCore;

class GameCore
{
public:
    ImageBlockInfo* m_pImageInfo;

    ShaderProgram* m_pTextureShader;

    GLuint m_QuadVBO;

    unsigned int m_CurrentTexture;
    bool m_ShowTriangles;

public:
    GameCore();
    virtual ~GameCore();

    virtual void OneTimeInit(int argc, char** argv);
    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);

    virtual void Tick(double TimePassed);
    
    virtual void OnDrawFrame();
};

#endif //__GameCore_H__
