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

#ifndef GLF_SHADER_H
#define GLF_SHADER_H

#include "../glf_common.h"

#include "glf_shader_uniform.h"
#include "glf_shader_library.h"

#include <string>
#include <map>
#include <vector>


GLF_NAMESPACE_BEGIN

class Shader
{
private:
    Shader(const Shader& other) {}
    void operator=(const Shader& other) {}

public:
    Shader();
    ~Shader();

    // Create the shader program from a list of source files.
    // When some file paths is NULL, the corresponding stage uses the default shader.
    // Vertex and fragment shader can't be omitted.
    bool loadFromFiles(const char* vertexSourceFile,
                       const char* fragmentSourceFile,
                       const char* tessellationControlSourceFile = NULL,
                       const char* tessellationEvalSourceFile = NULL,
                       const char* geometrySourceFile = NULL);
    
    bool loadFromMemory(const char* vertexSource,
                        const char* fragmentSource,
                        const char* tessellationControlSource = NULL,
                        const char* tessellationEvalSource = NULL,
                        const char* geometrySource = NULL);

    bool loadFromLibrary(GLuint vertexShader,
                         GLuint fragmentShader,
                         GLuint tessellationControlShader = ShaderLibrary::NONE,
                         GLuint tessellationEvalShader = ShaderLibrary::NONE,
                         GLuint geometryShader = ShaderLibrary::NONE);

    ShaderUniform* getUniform(const char* name);

    std::vector<char*> getUniformNames() const;

    GLF_INLINE GLuint getShaderHandle() const
    { return _program; }

    void enable();
    void disable();

    static char* readTextFile(const char* filename);

private:
    bool validateProgram(GLuint program);
    GLuint compileShader(GLenum shaderType, const char* source);

private:
    GLuint _program;
    bool _enabled;

    class StringLess
    {
    public:
        bool operator()(const char* left, const char* right) const
        {
            return strcmp(left, right) < 0;
        }
    };

    typedef std::map<char*, ShaderUniform*, StringLess> UniformMap;
    typedef UniformMap::iterator UniformIterator;
    typedef UniformMap::const_iterator UniformConstIterator;
    UniformMap _uniforms;
};

GLF_NAMESPACE_END


#endif // !GLF_SHADER_H

