// -------------------------------------------------------------- 
// glf_shader_uniform.h
// The shader uniform 
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_SHADER_UNIFORM_H
#define GLF_SHADER_UNIFORM_H

#include "../glf_common.h"

#include <string>


GLF_NAMESPACE_BEGIN

enum ShaderUniformTypeEnum
{
    SHADER_UNIFORM_FLOAT,
    SHADER_UNIFORM_VEC2,
    SHADER_UNIFORM_VEC3,
    SHADER_UNIFORM_VEC4,
    SHADER_UNIFORM_UINT,
    SHADER_UNIFORM_UVEC2,
    SHADER_UNIFORM_UVEC3,
    SHADER_UNIFORM_UVEC4,
    SHADER_UNIFORM_MATRIX4X4,

    SHADER_UNIFORM_TYPE_FIRST = SHADER_UNIFORM_FLOAT,
    SHADER_UNIFORM_TYPE_LAST = SHADER_UNIFORM_MATRIX4X4,
    SHADER_UNIFORM_TYPE_NUMBER = SHADER_UNIFORM_TYPE_LAST - SHADER_UNIFORM_TYPE_FIRST + 1,
};

class ShaderUniform
{
    friend class Shader;

private:
    ShaderUniform(const char* name, GLint location, GLfloat x);
    ShaderUniform(const char* name, GLint location, GLfloat x, GLfloat y);
    ShaderUniform(const char* name, GLint location, GLfloat x, GLfloat y, GLfloat z);
    ShaderUniform(const char* name, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    
    ShaderUniform(const char* name, GLint location, GLuint x);
    ShaderUniform(const char* name, GLint location, GLuint x, GLuint y);
    ShaderUniform(const char* name, GLint location, GLuint x, GLuint y, GLuint z);
    ShaderUniform(const char* name, GLint location, GLuint x, GLuint y, GLuint z, GLuint w);

    ShaderUniform(const char* name, GLint location, const glm::mat4& matrix);

    ~ShaderUniform();

    ShaderUniform(const ShaderUniform &other) {}
    void operator=(const ShaderUniform& other) {}

public:
    void setValue(GLfloat x);
    void setValue(GLfloat x, GLfloat y);
    void setValue(GLfloat x, GLfloat y, GLfloat z);
    void setValue(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void setValue(GLuint x);
    void setValue(GLuint x, GLuint y);
    void setValue(GLuint x, GLuint y, GLuint z);
    void setValue(GLuint x, GLuint y, GLuint z, GLuint w);
    void setValue(const glm::mat4& matrix);

    GLF_INLINE const std::string& getName() const
    { return _name; }

private:
    void upload();
    
    void uploadFloat();
    void uploadVec2();
    void uploadVec3();
    void uploadVec4();
    void uploadUint();
    void uploadUvec2();
    void uploadUvec3();
    void uploadUvec4();
    void uploadMatrix4x4();

private:
    std::string             _name;
    GLint                   _location;
    ShaderUniformTypeEnum   _type;
    union 
    {
        GLfloat   vec[4];
        GLuint    uvec[4];
        GLfloat   mat4[16];
    } _value;

    typedef void (ShaderUniform::*uploadFunc)(void);
    uploadFunc _uploadFunction;

    // TODO: no duplicated upload by adopting dirty flag.
};


GLF_NAMESPACE_END


#endif // !GLF_SHADER_UNIFORM_H
