#ifndef __GLExtensions_H__
#define __GLExtensions_H__

#include "glext.h"

void OpenGL_InitExtensions();

extern PFNGLUNIFORM1FPROC                   glUniform1f;
extern PFNGLUNIFORM2FPROC                   glUniform2f;
extern PFNGLUNIFORM3FPROC                   glUniform3f;
extern PFNGLUNIFORM4FPROC                   glUniform4f;
extern PFNGLUNIFORM1IPROC                   glUniform1i;
extern PFNGLUNIFORM2IPROC                   glUniform2i;
extern PFNGLUNIFORM3IPROC                   glUniform3i;
extern PFNGLUNIFORM4IPROC                   glUniform4i;
extern PFNGLUNIFORM1FVPROC                  glUniform1fv;
extern PFNGLUNIFORM2FVPROC                  glUniform2fv;
extern PFNGLUNIFORM3FVPROC                  glUniform3fv;
extern PFNGLUNIFORM4FVPROC                  glUniform4fv;
extern PFNGLUNIFORM1IVPROC                  glUniform1iv;
extern PFNGLUNIFORM2IVPROC                  glUniform2iv;
extern PFNGLUNIFORM3IVPROC                  glUniform3iv;
extern PFNGLUNIFORM4IVPROC                  glUniform4iv;

extern PFNGLUNIFORMMATRIX4FVPROC            glUniformMatrix4fv;
extern PFNGLVERTEXATTRIB1FPROC              glVertexAttrib1f;
extern PFNGLVERTEXATTRIB2FPROC              glVertexAttrib2f;
extern PFNGLVERTEXATTRIB3FPROC              glVertexAttrib3f;
extern PFNGLVERTEXATTRIB4FPROC              glVertexAttrib4f;
extern PFNGLVERTEXATTRIB1FVPROC             glVertexAttrib1fv;
extern PFNGLVERTEXATTRIB2FVPROC             glVertexAttrib2fv;
extern PFNGLVERTEXATTRIB3FVPROC             glVertexAttrib3fv;
extern PFNGLVERTEXATTRIB4FVPROC             glVertexAttrib4fv;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC     glEnableVertexAttribArray;
extern PFNGLBINDATTRIBLOCATIONPROC          glBindAttribLocation;

extern PFNGLDISABLEVERTEXATTRIBARRAYPROC    glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC         glVertexAttribPointer;

extern PFNGLCREATESHADERPROC                glCreateShader;
extern PFNGLSHADERSOURCEPROC                glShaderSource;
extern PFNGLCOMPILESHADERPROC               glCompileShader;
extern PFNGLCREATEPROGRAMPROC               glCreateProgram;
extern PFNGLATTACHSHADERPROC                glAttachShader;
extern PFNGLLINKPROGRAMPROC                 glLinkProgram;
extern PFNGLDETACHSHADERPROC                glDetachShader;
extern PFNGLDELETEPROGRAMPROC               glDeleteProgram;
extern PFNGLDELETESHADERPROC                glDeleteShader;
extern PFNGLUSEPROGRAMPROC                  glUseProgram;
extern PFNGLGETSHADERINFOLOGPROC            glGetShaderInfoLog;

extern PFNGLGETATTRIBLOCATIONPROC           glGetAttribLocation;
extern PFNGLGETPROGRAMINFOLOGPROC           glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC                glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC            glGetShaderInfoLog;
extern PFNGLGETSHADERSOURCEPROC             glGetShaderSource;
extern PFNGLGETSHADERIVPROC                 glGetShaderiv;
extern PFNGLGETUNIFORMLOCATIONPROC          glGetUniformLocation;

extern PFNGLACTIVETEXTUREPROC               glActiveTexture;

extern PFNGLGENBUFFERSPROC                  glGenBuffers;
extern PFNGLDELETEBUFFERSPROC               glDeleteBuffers;
extern PFNGLBINDBUFFERPROC                  glBindBuffer;
extern PFNGLBUFFERDATAPROC                  glBufferData;

#endif //__GLExtensions_H__
