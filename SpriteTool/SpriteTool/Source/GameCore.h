#ifndef __GameCore_H__
#define __GameCore_H__

class GameCore;

extern GameCore* g_pGameCore;

class GameCore
{
public:

public:
    GameCore();
    virtual ~GameCore();

    virtual void OneTimeInit();
    virtual void Tick(double TimePassed);

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);

    virtual void OnDrawFrame();
};

#endif //__GameCore_H__
