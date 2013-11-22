// -------------------------------------------------------------- 
// glf_shader.h
// The shader wrapper
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_shader.h"

#include "glf_glerror.h"

#include "../01system/glf_log.h"
#include "../01system/glf_assert.h"

GLF_NAMESPACE_BEGIN

Shader::Shader()
{
    _program = 0;
    _enabled = false;
}

Shader::~Shader()
{
    if (glIsProgram(_program))
    {
        glDeleteProgram(_program);
    }

    UniformIterator iterator = _uniforms.begin();
    while (iterator != _uniforms.end())
    {
        delete iterator->second;
        ++iterator;
    }
}

bool Shader::loadFromFiles(const char* vertexSourceFile,
                           const char* fragmentSourceFile,
                           const char* tessellationControlSourceFile,
                           const char* tessellationEvalSourceFile,
                           const char* geometrySourceFile)
{
    char* vertexSource              = readTextFile(vertexSourceFile); 
    char* fragmentSource            = readTextFile(fragmentSourceFile); 
    char* tessellationControlSource = readTextFile(tessellationControlSourceFile); 
    char* tessellationEvalSource    = readTextFile(tessellationEvalSourceFile); 
    char* geometrySource            = readTextFile(geometrySourceFile); 

    bool ret = loadFromMemory(vertexSource, 
                              fragmentSource,
                              tessellationControlSource,
                              tessellationEvalSource,
                              geometrySource);

    delete [] vertexSource;
    delete [] fragmentSource;
    delete [] tessellationControlSource;
    delete [] tessellationEvalSource;
    delete [] geometrySource;

    return ret;
}

bool Shader::loadFromMemory(const char* vertexSource,
                            const char* fragmentSource,
                            const char* tessellationControlSource,
                            const char* tessellationEvalSource,
                            const char* geometrySource)
{
    // -------------------------------------------------------------- 
    // Sanity check
    // -------------------------------------------------------------- 
    GLF_ASSERT(vertexSource != NULL && fragmentSource != NULL);

    if (vertexSource == NULL)
    {
        GLF_LOGERROR("Failed to find vertex source");
        return false;
    }

    if (fragmentSource == NULL)
    {
        GLF_LOGERROR("Failed to find fragment source");
        return false;
    }

    // -------------------------------------------------------------- 
    // Compile shaders and create program.
    // -------------------------------------------------------------- 

    GLuint vertexShader              = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader            = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    GLuint tessellationControlShader = compileShader(GL_TESS_CONTROL_SHADER, tessellationControlSource);
    GLuint tessellationEvalShader    = compileShader(GL_TESS_EVALUATION_SHADER, tessellationEvalSource);
    GLuint geometryShader            = compileShader(GL_GEOMETRY_SHADER, geometrySource);

    GLF_ASSERT(vertexShader != 0 && fragmentShader != 0);
    if (vertexShader == 0 ||
            fragmentShader == 0 ||
            (tessellationControlSource != NULL && tessellationControlShader == 0) ||
            (tessellationEvalSource != NULL && tessellationEvalShader == 0) ||
            (geometrySource != NULL && geometryShader == 0))
    {
        return false;
    }

    GLuint program = glCreateProgram();

    if (vertexShader != 0)
    {
        glAttachShader(program, vertexShader);
    }
    if (fragmentShader != 0)
    {
        glAttachShader(program, fragmentShader);
    }
    if (tessellationControlShader != 0)
    {
        glAttachShader(program, tessellationControlShader);
    }
    if (tessellationEvalShader != 0)
    {
        glAttachShader(program, tessellationEvalShader);
    }
    if (geometryShader != 0)
    {
        glAttachShader(program, geometryShader);
    }

    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(tessellationControlShader);
    glDeleteShader(tessellationEvalShader);
    glDeleteShader(geometryShader);

    if (!validateProgram(program))
    {
        return false;
    }

    _program = program;

    // -------------------------------------------------------------- 
    // Fetch uniforms from the program.
    // -------------------------------------------------------------- 
    GLint numUniforms;
    glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);
    for (GLint i = 0; i < numUniforms; ++i)
    {
        char uniformName[1024];
        GLenum type;
        GLsizei length;
        GLint size = -1;
        glGetActiveUniform(_program, i, 1024, &length, &size, &type, uniformName);

        // It is an array uniform. On AMD cards, uniform name will be like
        // someUniform[0], we need to get rid of '[x]'.
        char uniformNamePrefix[1024];
        // FIXME: on AMD cards, an atomic counter will have size of 2.
        if (size != 1 && type != GL_UNSIGNED_INT_ATOMIC_COUNTER)
        {
            strcpy(uniformNamePrefix, uniformName);
            char* p = strchr(uniformNamePrefix, '[');
            if (p != NULL)
            {
                *p = 0;
            }
        }

        for (GLint j = 0; j < size; ++j)
        {
            if (size != 1 && type != GL_UNSIGNED_INT_ATOMIC_COUNTER)
            {
                sprintf(uniformName, "%s[%d]", uniformNamePrefix, j);
            }

            ShaderUniform* uniform = NULL;

            GLint loc = glGetUniformLocation(_program, uniformName);

            switch (type)
            {
                case GL_FLOAT:
                    uniform = new ShaderUniform(uniformName, loc, 0.0f);
                    break;
                case GL_FLOAT_VEC2:
                    uniform = new ShaderUniform(uniformName, loc, 0.0f, 0.0f);
                    break;
                case GL_FLOAT_VEC3:
                    uniform = new ShaderUniform(uniformName, loc, 0.0f, 0.0f, 0.0f);
                    break;
                case GL_FLOAT_VEC4: 
                    uniform = new ShaderUniform(uniformName, loc, 0.0f, 0.0f, 0.0f, 0.0f);
                    break;
                case GL_INT:
                case GL_SAMPLER_2D:
                case GL_SAMPLER_CUBE:
                    uniform = new ShaderUniform(uniformName, loc, GLuint(0));
                    break;
                case GL_INT_VEC2:
                case GL_UNSIGNED_INT_VEC2:
                    uniform = new ShaderUniform(uniformName, loc, GLuint(0), GLuint(0));
                    break;
                case GL_INT_VEC3:
                case GL_UNSIGNED_INT_VEC3:
                    uniform = new ShaderUniform(uniformName, loc, GLuint(0), GLuint(0), GLuint(0));
                    break;
                case GL_INT_VEC4:
                case GL_UNSIGNED_INT_VEC4:
                    uniform = new ShaderUniform(uniformName, loc, GLuint(0), GLuint(0), GLuint(0), GLuint(0));
                    break;
                case GL_FLOAT_MAT4:
                    uniform = new ShaderUniform(uniformName, loc, glm::mat4());
                    break;
                case GL_UNSIGNED_INT_ATOMIC_COUNTER:
                case GL_UNSIGNED_INT_IMAGE_1D:
                case GL_UNSIGNED_INT_IMAGE_2D:
                case GL_INT_IMAGE_1D:
                case GL_INT_IMAGE_2D:
                    break;
                default:
                    GLF_ASSERT_NOT_REACHABLE();
                    break;
            }
            
            if (uniform != NULL)
            {
                char* name = const_cast<char*>(uniform->getName().c_str()); 
                _uniforms.insert(std::make_pair(name, uniform));
            }
        }
    }

    glUseProgram(0);

    if (!checkGLError(__FUNCTION__))
    {
        return false;
    }

    return true;
}

ShaderUniform* Shader::getUniform(const char* name)
{
    char* uniformName = const_cast<char*>(name);
    UniformIterator iterator = _uniforms.find(uniformName);
    GLF_ASSERT(iterator != _uniforms.end());
    if (iterator == _uniforms.end())
    {
        return NULL;
    }

    return _uniforms[uniformName];
}

std::vector<char*> Shader::getUniformNames() const
{
    std::vector<char*> ret(_uniforms.size());

    GLuint i = 0;
    UniformConstIterator iterator = _uniforms.begin();
    while (iterator != _uniforms.end())
    {
        ret[i++] = iterator->first; 
        ++iterator;
    }

    return ret;
}
    
void Shader::enable()
{
    GLF_ASSERT(!_enabled);
    _enabled = true;
    glUseProgram(_program);

    UniformIterator iterator = _uniforms.begin();
    while (iterator != _uniforms.end())
    {
        iterator->second->upload();
        ++iterator;
    }
}

void Shader::disable()
{
    GLF_ASSERT(_enabled);
    _enabled = false;
    glUseProgram(0);
}

bool Shader::validateProgram(GLuint program)
{
    GLint result = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    int infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (!result && infoLogLength > 0)
    {
        std::vector<char> buffer(std::max(infoLogLength, int(1)));
        glGetProgramInfoLog(program, infoLogLength, NULL, &buffer[0]);
        GLF_LOGINFO(&buffer[0]);
    }

    // TODO: validate the program.

    return result == GL_TRUE;
}

GLuint Shader::compileShader(GLenum shaderType, const char* source)
{
    GLuint shader = 0;

    if (source != NULL)
    {
        shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        GLint result = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

        if (!result)
        {
            GLF_LOGERROR("Failed to compile shader");
            GLint infoLogLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0)
            {
                std::vector<char> buffer(infoLogLength + 1);
                glGetShaderInfoLog(shader, infoLogLength, NULL, &buffer[0]);

                GLF_LOGINFO(&buffer[0]);
            }

            return 0;
        }
    }

    return shader;
}

char* Shader::readTextFile(const char* filename)
{
    FILE* fp;
    char* content = NULL;

    int count = 0;

    if (filename != NULL) 
    {
        fp = fopen(filename, "rb");

        if (fp != NULL) 
        {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

            if (count > 0) 
            {
                content = new char [count + 1];
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }

    return content;
}
    
GLF_NAMESPACE_END
