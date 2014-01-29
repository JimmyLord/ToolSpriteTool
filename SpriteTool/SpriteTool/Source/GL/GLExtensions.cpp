#include "CommonHeader.h"

PFNGLUNIFORM1FPROC                  glUniform1f = 0;
PFNGLUNIFORM2FPROC                  glUniform2f = 0;
PFNGLUNIFORM3FPROC                  glUniform3f = 0;
PFNGLUNIFORM4FPROC                  glUniform4f = 0;
PFNGLUNIFORM1IPROC                  glUniform1i = 0;
PFNGLUNIFORM2IPROC                  glUniform2i = 0;
PFNGLUNIFORM3IPROC                  glUniform3i = 0;
PFNGLUNIFORM4IPROC                  glUniform4i = 0;
PFNGLUNIFORM1FVPROC                 glUniform1fv = 0;
PFNGLUNIFORM2FVPROC                 glUniform2fv = 0;
PFNGLUNIFORM3FVPROC                 glUniform3fv = 0;
PFNGLUNIFORM4FVPROC                 glUniform4fv = 0;
PFNGLUNIFORM1IVPROC                 glUniform1iv = 0;
PFNGLUNIFORM2IVPROC                 glUniform2iv = 0;
PFNGLUNIFORM3IVPROC                 glUniform3iv = 0;
PFNGLUNIFORM4IVPROC                 glUniform4iv = 0;

PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv = 0;
PFNGLVERTEXATTRIB1FPROC             glVertexAttrib1f = 0;
PFNGLVERTEXATTRIB2FPROC             glVertexAttrib2f = 0;
PFNGLVERTEXATTRIB3FPROC             glVertexAttrib3f = 0;
PFNGLVERTEXATTRIB4FPROC             glVertexAttrib4f = 0;
PFNGLVERTEXATTRIB1FVPROC            glVertexAttrib1fv = 0;
PFNGLVERTEXATTRIB2FVPROC            glVertexAttrib2fv = 0;
PFNGLVERTEXATTRIB3FVPROC            glVertexAttrib3fv = 0;
PFNGLVERTEXATTRIB4FVPROC            glVertexAttrib4fv = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray = 0;
PFNGLBINDATTRIBLOCATIONPROC         glBindAttribLocation = 0;

PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray = 0;
PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer = 0;

PFNGLCREATESHADERPROC               glCreateShader = 0;
PFNGLSHADERSOURCEPROC               glShaderSource = 0;
PFNGLCOMPILESHADERPROC              glCompileShader = 0;
PFNGLCREATEPROGRAMPROC              glCreateProgram = 0;
PFNGLATTACHSHADERPROC               glAttachShader = 0;
PFNGLLINKPROGRAMPROC                glLinkProgram = 0;
PFNGLDETACHSHADERPROC               glDetachShader = 0;
PFNGLDELETEPROGRAMPROC              glDeleteProgram = 0;
PFNGLDELETESHADERPROC               glDeleteShader = 0;
PFNGLUSEPROGRAMPROC                 glUseProgram = 0;

PFNGLGETATTRIBLOCATIONPROC          glGetAttribLocation = 0;
PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog = 0;
PFNGLGETPROGRAMIVPROC               glGetProgramiv = 0;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog = 0;
PFNGLGETSHADERSOURCEPROC            glGetShaderSource = 0;
PFNGLGETSHADERIVPROC                glGetShaderiv = 0;
PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation = 0;

PFNGLACTIVETEXTUREPROC              glActiveTexture = 0;

PFNGLGENBUFFERSPROC                 glGenBuffers = 0;
PFNGLDELETEBUFFERSPROC              glDeleteBuffers = 0;
PFNGLBINDBUFFERPROC                 glBindBuffer = 0;
PFNGLBUFFERDATAPROC                 glBufferData = 0;

void OpenGL_InitExtensions()
{
    glUniform1i                 = (PFNGLUNIFORM1IPROC)                  wglGetProcAddress( "glUniform1i" );
    glUniform2i                 = (PFNGLUNIFORM2IPROC)                  wglGetProcAddress( "glUniform2i" );
    glUniform3i                 = (PFNGLUNIFORM3IPROC)                  wglGetProcAddress( "glUniform3i" );
    glUniform4i                 = (PFNGLUNIFORM4IPROC)                  wglGetProcAddress( "glUniform4i" );
    glUniform1iv                = (PFNGLUNIFORM1IVPROC)                 wglGetProcAddress( "glUniform1iv" );
    glUniform2iv                = (PFNGLUNIFORM2IVPROC)                 wglGetProcAddress( "glUniform2iv" );
    glUniform3iv                = (PFNGLUNIFORM3IVPROC)                 wglGetProcAddress( "glUniform3iv" );
    glUniform4iv                = (PFNGLUNIFORM4IVPROC)                 wglGetProcAddress( "glUniform4iv" );
    glUniform1f                 = (PFNGLUNIFORM1FPROC)                  wglGetProcAddress( "glUniform1f" );
    glUniform2f                 = (PFNGLUNIFORM2FPROC)                  wglGetProcAddress( "glUniform2f" );
    glUniform3f                 = (PFNGLUNIFORM3FPROC)                  wglGetProcAddress( "glUniform3f" );
    glUniform4f                 = (PFNGLUNIFORM4FPROC)                  wglGetProcAddress( "glUniform4f" );
    glUniform1fv                = (PFNGLUNIFORM1FVPROC)                 wglGetProcAddress( "glUniform1fv" );
    glUniform2fv                = (PFNGLUNIFORM2FVPROC)                 wglGetProcAddress( "glUniform2fv" );
    glUniform3fv                = (PFNGLUNIFORM3FVPROC)                 wglGetProcAddress( "glUniform3fv" );
    glUniform4fv                = (PFNGLUNIFORM4FVPROC)                 wglGetProcAddress( "glUniform4fv" );
    glUniformMatrix4fv          = (PFNGLUNIFORMMATRIX4FVPROC)           wglGetProcAddress( "glUniformMatrix4fv" );
    glVertexAttrib1f            = (PFNGLVERTEXATTRIB1FPROC)             wglGetProcAddress( "glVertexAttrib1f" );
    glVertexAttrib2f            = (PFNGLVERTEXATTRIB2FPROC)             wglGetProcAddress( "glVertexAttrib2f" );
    glVertexAttrib3f            = (PFNGLVERTEXATTRIB3FPROC)             wglGetProcAddress( "glVertexAttrib3f" );
    glVertexAttrib4f            = (PFNGLVERTEXATTRIB4FPROC)             wglGetProcAddress( "glVertexAttrib4f" );
    glVertexAttrib1fv           = (PFNGLVERTEXATTRIB1FVPROC)            wglGetProcAddress( "glVertexAttrib1fv" );
    glVertexAttrib2fv           = (PFNGLVERTEXATTRIB2FVPROC)            wglGetProcAddress( "glVertexAttrib2fv" );
    glVertexAttrib3fv           = (PFNGLVERTEXATTRIB3FVPROC)            wglGetProcAddress( "glVertexAttrib3fv" );
    glVertexAttrib4fv           = (PFNGLVERTEXATTRIB4FVPROC)            wglGetProcAddress( "glVertexAttrib4fv" );
    glEnableVertexAttribArray   = (PFNGLENABLEVERTEXATTRIBARRAYPROC)    wglGetProcAddress( "glEnableVertexAttribArray" );
    glBindAttribLocation        = (PFNGLBINDATTRIBLOCATIONPROC)         wglGetProcAddress( "glBindAttribLocation" );

    glDisableVertexAttribArray  = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)   wglGetProcAddress( "glDisableVertexAttribArray" );
    glVertexAttribPointer       = (PFNGLVERTEXATTRIBPOINTERPROC)        wglGetProcAddress( "glVertexAttribPointer" );

    glCreateShader              = (PFNGLCREATESHADERPROC)               wglGetProcAddress( "glCreateShader" );
    glShaderSource              = (PFNGLSHADERSOURCEPROC)               wglGetProcAddress( "glShaderSource" );
    glCompileShader             = (PFNGLCOMPILESHADERPROC)              wglGetProcAddress( "glCompileShader" );
    glCreateProgram             = (PFNGLCREATEPROGRAMPROC)              wglGetProcAddress( "glCreateProgram" );
    glAttachShader              = (PFNGLATTACHSHADERPROC)               wglGetProcAddress( "glAttachShader" );
    glLinkProgram               = (PFNGLLINKPROGRAMPROC)                wglGetProcAddress( "glLinkProgram" );
    glDetachShader              = (PFNGLDETACHSHADERPROC)               wglGetProcAddress( "glDetachShader" );
    glDeleteShader              = (PFNGLDELETESHADERPROC)               wglGetProcAddress( "glDeleteShader" );
    glDeleteProgram             = (PFNGLDELETEPROGRAMPROC)              wglGetProcAddress( "glDeleteProgram" );
    glUseProgram                = (PFNGLUSEPROGRAMPROC)                 wglGetProcAddress( "glUseProgram" );

    glGetAttribLocation         = (PFNGLGETATTRIBLOCATIONPROC)          wglGetProcAddress( "glGetAttribLocation" );
    glGetProgramInfoLog         = (PFNGLGETPROGRAMINFOLOGPROC)          wglGetProcAddress( "glGetProgramInfoLog" );
    glGetProgramiv              = (PFNGLGETPROGRAMIVPROC)               wglGetProcAddress( "glGetProgramiv" );
    glGetShaderInfoLog          = (PFNGLGETSHADERINFOLOGPROC)           wglGetProcAddress( "glGetShaderInfoLog" );
    glGetShaderSource           = (PFNGLGETSHADERSOURCEPROC)            wglGetProcAddress( "glShaderSource" );
    glGetShaderiv               = (PFNGLGETSHADERIVPROC)                wglGetProcAddress( "glGetShaderiv" );
    glGetUniformLocation        = (PFNGLGETUNIFORMLOCATIONPROC)         wglGetProcAddress( "glGetUniformLocation" );

    glActiveTexture             = (PFNGLACTIVETEXTUREPROC)              wglGetProcAddress( "glActiveTexture" );

    glGenBuffers                = (PFNGLGENBUFFERSPROC)                 wglGetProcAddress( "glGenBuffers" );
    glDeleteBuffers             = (PFNGLDELETEBUFFERSPROC)              wglGetProcAddress( "glDeleteBuffers" );
    glBindBuffer                = (PFNGLBINDBUFFERPROC)                 wglGetProcAddress( "glBindBuffer" );
    glBufferData                = (PFNGLBUFFERDATAPROC)                 wglGetProcAddress( "glBufferData" );
}
