#ifndef __ShaderProgram_H__
#define __ShaderProgram_H__

class ShaderProgram
{
public:
    char* m_VertShaderString;
    char* m_FragShaderString;

    GLuint m_VertShader;
    GLuint m_FragShader;
    GLuint m_Program;

public:
    ShaderProgram();
    virtual ~ShaderProgram();

    void Cleanup();

    GLuint CompileShader(GLenum shaderType, const char* shaderstring);
    bool InitFromDisk(const char* vertfilename, const char* fragfilename);
    bool InitFromString(const char* vertstring, const char* fragstring);
    bool Init();
};

#endif //__ShaderProgram_H__
