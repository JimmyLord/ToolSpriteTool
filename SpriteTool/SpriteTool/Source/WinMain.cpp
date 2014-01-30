#include "CommonHeader.h"

#define SCREEN_WIDTH    600
#define SCREEN_HEIGHT   600

HGLRC hRenderingContext = 0;
HDC hDeviceContext = 0;
HWND hWnd = 0;
HINSTANCE hInstance;

int g_WindowWidth = 0;
int g_WindowHeight = 0;
bool g_KeyStates[256];
bool g_KeyStatesOld[256];
bool g_WindowIsActive = true;
bool g_FullscreenMode = true;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GLvoid ResizeGLScene(GLsizei width, GLsizei height)
{
    if( height <= 0 ) height = 1;
    if( width <= 0 ) width = 1;

    g_WindowWidth = width;
    g_WindowHeight = height;
 
    if( g_pGameCore )
        g_pGameCore->OnSurfaceChanged( width, height );
}

GLvoid KillGLWindow()
{
    if( g_FullscreenMode )
    {
        ChangeDisplaySettings( 0, 0 );
        ShowCursor( true );
    }

    if( hRenderingContext )
    {
        if( !wglMakeCurrent( 0, 0 ) )
        {
            MessageBox( 0, "Release Of Device Context And Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        }

        if( !wglDeleteContext( hRenderingContext ) )
        {
            MessageBox( 0, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        }
        
        hRenderingContext = 0;
    }

    if( hDeviceContext && !ReleaseDC( hWnd, hDeviceContext ) )
    {
        MessageBox( 0, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        hDeviceContext = 0;
    }

    if( hWnd && !DestroyWindow( hWnd ) )
    {
        MessageBox( 0, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        hWnd = 0;
    }

    if( !UnregisterClass( "OpenGL", hInstance ) )
    {
        MessageBox( 0, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        hInstance = 0;
    }
}

bool CreateGLWindow(char* title, int width, int height, int colorbits, int zbits, int stencilbits, bool fullscreenflag)
{
    GLuint PixelFormat;

    WNDCLASS wc;
    DWORD dwExStyle;
    DWORD dwStyle;

    RECT WindowRect;
    WindowRect.left = (long)0;
    WindowRect.right = (long)width;
    WindowRect.top = (long)0;
    WindowRect.bottom = (long)height;

    g_FullscreenMode = fullscreenflag;

    hInstance = GetModuleHandle( 0 );               // Grab An Instance For Our Window
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Redraw On Move, And Own DC For Window
    wc.lpfnWndProc = (WNDPROC)WndProc;              // WndProc Handles Messages
    wc.cbClsExtra = 0;                              // No Extra Window Data
    wc.cbWndExtra = 0;                              // No Extra Window Data
    wc.hInstance = hInstance;                       // Set The Instance
    wc.hIcon = LoadIcon( 0, IDI_WINLOGO );          // Load The Default Icon
    wc.hCursor = LoadCursor( 0, IDC_ARROW );        // Load The Arrow Pointer
    wc.hbrBackground = 0;                           // No Background Required For GL
    wc.lpszMenuName = 0;                            // We Don't Want A Menu
    wc.lpszClassName = "OpenGL";                    // Set The Class Name

    if( !RegisterClass( &wc ) )                     // Attempt To Register The Window Class
    {
        MessageBox( 0, "Failed To Register The Window Class.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return false;
    }

    if( g_FullscreenMode )
    {
        DEVMODE dmScreenSettings;                                   // Device Mode
        memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) ); // Makes Sure Memory's Cleared
        dmScreenSettings.dmSize = sizeof( dmScreenSettings );       // Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth  = width;                      // Selected Screen Width
        dmScreenSettings.dmPelsHeight = height;                     // Selected Screen Height
        dmScreenSettings.dmBitsPerPel = colorbits;                  // Selected Bits Per Pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
        {
            // If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
            if( MessageBox( 0, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "", MB_YESNO|MB_ICONEXCLAMATION ) == IDYES )
            {
                g_FullscreenMode = false;
            }
            else
            {
                MessageBox( 0, "Program Will Now Close.", "ERROR", MB_OK|MB_ICONSTOP );
                return false;
            }
        }
    }

    if( g_FullscreenMode )
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP;
        ShowCursor( false );
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );   // Adjust Window To True Requested Size

    if( !( hWnd = CreateWindowEx( dwExStyle,                            // Extended Style For The Window
                                  "OpenGL",                             // Class Name
                                  title,                                // Window Title
                                  WS_CLIPSIBLINGS | WS_CLIPCHILDREN |   // Required Window Style
                                    dwStyle,                            // Selected Window Style
                                  0, 0,                                 // Window Position
                                  WindowRect.right-WindowRect.left,     // Calculate Adjusted Window Width
                                  WindowRect.bottom-WindowRect.top,     // Calculate Adjusted Window Height
                                  0,                                    // No Parent Window
                                  0,                                    // No Menu
                                  hInstance,                            // Instance
                                  0)))                                  // Don't Pass Anything To WM_CREATE
    {
        KillGLWindow();
        MessageBox( 0, "Window Creation Error.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return false;
    }

    static PIXELFORMATDESCRIPTOR pfd =  // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor
        1,                              // Version Number
        PFD_DRAW_TO_WINDOW |            // Format Must Support Window
          PFD_SUPPORT_OPENGL |          // Format Must Support OpenGL
          PFD_DOUBLEBUFFER,             // Must Support Double Buffering
        PFD_TYPE_RGBA,                  // Request An RGBA Format
        colorbits,                      // Select Our Color Depth
        0, 0, 0, 0, 0, 0,               // Color Bits Ignored
        0,                              // No Alpha Buffer
        0,                              // Shift Bit Ignored
        0,                              // No Accumulation Buffer
        0, 0, 0, 0,                     // Accumulation Bits Ignored
        zbits,                          // Bits for Z-Buffer (Depth Buffer)
        stencilbits,                    // Stencil bits
        0,                              // No Auxiliary Buffer
        PFD_MAIN_PLANE,                 // Main Drawing Layer
        0,                              // Reserved
        0, 0, 0                         // Layer Masks Ignored
    };

    if( !( hDeviceContext = GetDC(hWnd) ) ) // Did We Get A Device Context?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Create A GL Device Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !( PixelFormat = ChoosePixelFormat(hDeviceContext, &pfd) ) ) // Did Windows Find A Matching Pixel Format?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !SetPixelFormat( hDeviceContext, PixelFormat, &pfd) ) // Are We Able To Set The Pixel Format?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Set The PixelFormat.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !( hRenderingContext = wglCreateContext(hDeviceContext) ) ) // Are We Able To Get A Rendering Context?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Create A GL Rendering Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !wglMakeCurrent( hDeviceContext, hRenderingContext ) ) // Try To Activate The Rendering Context
    {
        KillGLWindow();
        MessageBox( 0, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    ShowWindow( hWnd, SW_SHOW );     // Show The Window
    SetForegroundWindow( hWnd );     // Slightly Higher Priority
    SetFocus( hWnd );                // Sets Keyboard Focus To The Window
    ResizeGLScene( width, height );  // Set Up Our Perspective GL Screen

    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd,       // Handle For This Window
                         UINT uMsg,       // Message For This Window
                         WPARAM wParam,   // Additional Message Information
                         LPARAM lParam)   // Additional Message Information
{
    switch( uMsg )
    {
        case WM_ACTIVATE:
        {
            if( !HIWORD(wParam) )                    // Check Minimization State
            {
                g_WindowIsActive = true;
            }
            else
            {
                g_WindowIsActive = false;
            }
 
            return 0;
        }

        case WM_SYSCOMMAND:                     // Intercept System Commands
        {
            switch( wParam )                    // Check System Calls
            {
                case SC_SCREENSAVE:             // Screensaver Trying To Start?
                case SC_MONITORPOWER:           // Monitor Trying To Enter Powersave?
                    return 0;                       // Prevent From Happening
            }
            break;
        }

        case WM_CLOSE:                          // Did We Receive A Close Message?
        {
            PostQuitMessage(0);                 // Send A Quit Message
            return 0;
        }

        case WM_KEYDOWN:
        {
            g_KeyStates[wParam] = true;
            return 0;
        }

        case WM_KEYUP:
        {
            g_KeyStates[wParam] = false;
            return 0;
        }

        case WM_SIZE: // window was resized
        {
            ResizeGLScene( LOWORD(lParam), HIWORD(lParam) );
            return 0;
        }
    }

    // Pass all unhandled messages to DefWindowProc
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

double MyGetSystemTime()
{
    SYSTEMTIME time;
    GetSystemTime( &time );

    return time.wSecond + time.wMilliseconds / 1000.0;
}

// found here and modified: http://www.cmake.org/pipermail/cmake/2004-June/005170.html
void ParseCommandLine(LPSTR lpCmdLine, int nCmdShow, int* out_argc, char*** out_argv)
{
    int          argc; 
    //int          retVal; 
    char**       argv;
    unsigned int i; 
    int          j; 

    // parse a few of the command line arguments 
    // a space delimites an argument except when it is inside a quote 

    argc = 1; 
    int pos = 0; 
    for (i = 0; i < strlen(lpCmdLine); i++) 
    { 
        while (lpCmdLine[i] == ' ' && i < strlen(lpCmdLine)) 
        { 
            i++; 
        } 
        if (lpCmdLine[i] == '\"') 
        { 
            i++; 
            while (lpCmdLine[i] != '\"' && i < strlen(lpCmdLine)) 
            { 
                i++; 
                pos++; 
            } 
            argc++; 
            pos = 0; 
        } 
        else 
        { 
            while (lpCmdLine[i] != ' ' && i < strlen(lpCmdLine)) 
            { 
                i++; 
                pos++; 
            } 
            argc++; 
            pos = 0; 
        } 
    } 

    argv = (char**)malloc(sizeof(char*)* (argc+1)); 

    argv[0] = (char*)malloc(1024); 
    ::GetModuleFileName(0, argv[0],1024); 

    for(j=1; j<argc; j++) 
    { 
        argv[j] = (char*)malloc(strlen(lpCmdLine)+10); 
    } 
    argv[argc] = 0; 

    argc = 1; 
    pos = 0; 
    for (i = 0; i < strlen(lpCmdLine); i++) 
    { 
        while (lpCmdLine[i] == ' ' && i < strlen(lpCmdLine)) 
        { 
            i++; 
        } 
        if (lpCmdLine[i] == '\"') 
        { 
            i++; 
            while (lpCmdLine[i] != '\"' && i < strlen(lpCmdLine)) 
            { 
                argv[argc][pos] = lpCmdLine[i]; 
                i++; 
                pos++; 
            } 
            argv[argc][pos] = '\0'; 
            argc++; 
            pos = 0; 
        } 
        else 
        { 
            while (lpCmdLine[i] != ' ' && i < strlen(lpCmdLine)) 
            { 
                argv[argc][pos] = lpCmdLine[i]; 
                i++; 
                pos++; 
            } 
            argv[argc][pos] = '\0'; 
            argc++; 
            pos = 0; 
        } 
    } 
    argv[argc] = 0; 


    *out_argv = argv;
    *out_argc = argc;
    //// Initialize the processes and start the application. 
    //retVal = MyMain(argc, argv); 

    //// Delete arguments 
    //for(j=0; j<argc; j++) 
    //{ 
    //    free(argv[j]); 
    //} 
    //free(argv); 

    //return retVal;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    bool done = false;

    char** argv = 0;
    int argc;
    ParseCommandLine( lpCmdLine, nCmdShow, &argc, &argv );

    for( int i=0; i<256; i++ )
    {
        g_KeyStates[i] = false;
        g_KeyStatesOld[i] = false;
    }

    // Create Our OpenGL Window
    if( !CreateGLWindow( "OpenGL Window", SCREEN_WIDTH, SCREEN_HEIGHT, 32, 31, 1, false ) )
    {
        return 0;
    }

    // Initialize OpenGL Extensions, must be done after OpenGL Context is created
    OpenGL_InitExtensions();

    // Create and initialize our GameCore object.
    GameCore* pGameCore = new GameCore;
    g_pGameCore = pGameCore;

    pGameCore->OneTimeInit( argc, argv );
    pGameCore->OnSurfaceChanged( SCREEN_WIDTH, SCREEN_HEIGHT );

    double lasttime = MyGetSystemTime();

    // Main loop
    while( !done )
    {
        double currtime = MyGetSystemTime();
        double timepassed = currtime - lasttime;
        lasttime = currtime;

        if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
        {
            if( msg.message == WM_QUIT )
            {
                done = true;
            }
            else
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
        else
        {
            if( g_WindowIsActive )
            {
                if( g_KeyStates[VK_ESCAPE] )
                {
                    done = true;
                }
                else
                {
                    pGameCore->Tick( timepassed );
                    pGameCore->OnDrawFrame();

                    for( int i=0; i<256; i++ )
                        g_KeyStatesOld[i] = g_KeyStates[i];

                    SwapBuffers( hDeviceContext );
                }
            }
        }
    }

    delete( pGameCore );

    KillGLWindow();

    // Delete arguments 
    for( int j=0; j<argc; j++ )
    { 
        free( argv[j] );
    } 
    free( argv );

    return msg.wParam;
}
