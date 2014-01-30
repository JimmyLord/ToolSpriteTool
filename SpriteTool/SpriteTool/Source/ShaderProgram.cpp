#include "CommonHeader.h"

char* LoadCompleteFile(const char* filename)
{
    FILE* file;
    errno_t err = fopen_s( &file, filename, "rb" );

    char* buffer;

    if( file )
    {
        fseek( file, 0, SEEK_END );
        long size = ftell( file );
        rewind( file );

        buffer = new char[size+1];
        fread( buffer, size, 1, file );
        buffer[size] = 0;

        fclose( file );

        return buffer;
    }

    return 0;
}

ShaderProgram::ShaderProgram()
{
    m_VertShaderString = 0;
    m_FragShaderString = 0;

    m_VertShader = 0;
    m_FragShader = 0;
    m_Program = 0;
}

ShaderProgram::~ShaderProgram()
{
    Cleanup();
}

void ShaderProgram::Cleanup()
{
    delete[] m_VertShaderString;
    delete[] m_FragShaderString;

    glDetachShader( m_Program, m_VertShader );
    glDetachShader( m_Program, m_FragShader );

    if( m_VertShader )
        glDeleteShader( m_VertShader );
    if( m_FragShader )
        glDeleteShader( m_FragShader );
    if( m_Program )
        glDeleteProgram( m_Program );

    m_VertShaderString = 0;
    m_FragShaderString = 0;

    m_VertShader = 0;
    m_FragShader = 0;
    m_Program = 0;
}

GLuint ShaderProgram::CompileShader(GLenum shaderType, const char* shaderstring)
{
    GLuint shaderhandle = glCreateShader( shaderType );

    const char* strings[] = { shaderstring };
    glShaderSource( shaderhandle, 1, strings, 0 );

    glCompileShader( shaderhandle );

    GLenum errorcode = glGetError();

    int compiled = 0;
    glGetShaderiv( shaderhandle, GL_COMPILE_STATUS, &compiled );
    if( compiled == 0 )
    {
        int infolen = 0;
        glGetShaderiv( shaderhandle, GL_INFO_LOG_LENGTH, &infolen );

        char* infobuffer = new char[infolen+1];
        glGetShaderInfoLog( shaderhandle, infolen+1, 0, infobuffer );
        assert( false );
        delete infobuffer;

        glDeleteShader( shaderhandle );
        return 0;
    }

    return shaderhandle;
}

bool ShaderProgram::InitFromDisk(const char* vertfilename, const char* fragfilename)
{
    assert( vertfilename != 0 && fragfilename != 0 );
    assert( m_VertShaderString == 0 && m_FragShaderString == 0 );
    assert( m_VertShader == 0 && m_FragShader == 0 && m_Program == 0 );

    m_VertShaderString = LoadCompleteFile( vertfilename );
    m_FragShaderString = LoadCompleteFile( fragfilename );

    return Init();
}

bool ShaderProgram::InitFromString(const char* vertstring, const char* fragstring)
{
    m_VertShaderString = new char[strlen(vertstring)+1];
    m_FragShaderString = new char[strlen(fragstring)+1];

    strcpy_s( m_VertShaderString, strlen(vertstring)+1, vertstring );
    strcpy_s( m_FragShaderString, strlen(fragstring)+1, fragstring );

    return Init();
}

bool ShaderProgram::Init()
{
    m_VertShader = CompileShader( GL_VERTEX_SHADER, m_VertShaderString );
    m_FragShader = CompileShader( GL_FRAGMENT_SHADER, m_FragShaderString );

    if( m_VertShader == 0 || m_FragShader == 0 )
    {
        Cleanup();
        return false;
    }

    m_Program = glCreateProgram();
    glAttachShader( m_Program, m_VertShader );
    glAttachShader( m_Program, m_FragShader );

    glLinkProgram( m_Program );

    int linked = 0;
    glGetProgramiv( m_Program, GL_LINK_STATUS, &linked );
    if( linked == 0 )
    {
        int infolen = 0;
        glGetProgramiv( m_Program, GL_INFO_LOG_LENGTH, &infolen );

        char* infobuffer = new char[infolen+1];
        glGetProgramInfoLog( m_Program, infolen+1, 0, infobuffer );
        assert( false );
        delete infobuffer;

        Cleanup();
        return false;
    }

    return true;
}
