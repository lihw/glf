// -------------------------------------------------------------- 
// glf_shader_uniform.cpp
// The shader uniform 
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_shader_uniform.h"

#include "glf_glerror.h"

#include "../01system/glf_assert.h"
#include "../01system/glf_log.h"

GLF_NAMESPACE_BEGIN

ShaderUniform::ShaderUniform(const char* name, GLint location, GLfloat x)
    : _location(location)
    , _type(SHADER_UNIFORM_FLOAT)
    , _name(name)
{
    _value.vec[0] = x;
   
    _uploadFunction = &ShaderUniform::uploadFloat;
}

ShaderUniform::ShaderUniform(const char* name, GLint location, GLfloat x, GLfloat y)
    : _location(location)
    , _type(SHADER_UNIFORM_VEC2)
    , _name(name)
{
    _value.vec[0] = x;
    _value.vec[1] = y;
    
    _uploadFunction = &ShaderUniform::uploadVec2;
}

ShaderUniform::ShaderUniform(const char* name, GLint location, GLfloat x, GLfloat y, GLfloat z)
    : _location(location)
    , _type(SHADER_UNIFORM_VEC3)
    , _name(name)
{
    _value.vec[0] = x;
    _value.vec[1] = y;
    _value.vec[2] = z;
    
    _uploadFunction = &ShaderUniform::uploadVec3;
}

ShaderUniform::ShaderUniform(const char* name, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    : _location(location)
    , _type(SHADER_UNIFORM_VEC4)
    , _name(name)
{
    _value.vec[0] = x;
    _value.vec[1] = y;
    _value.vec[2] = z;
    _value.vec[3] = w;
    
    _uploadFunction = &ShaderUniform::uploadVec4;
}

ShaderUniform::ShaderUniform(const char* name, GLint location, GLuint x)
    : _location(location)
    , _type(SHADER_UNIFORM_UINT)
    , _name(name)
{
    _value.uvec[0] = x;
    
    _uploadFunction = &ShaderUniform::uploadUint;
}

ShaderUniform::ShaderUniform(const char* name, GLint location, GLuint x, GLuint y)
    : _location(location)
    , _type(SHADER_UNIFORM_UVEC2)
    , _name(name)
{
    _value.uvec[0] = x;
    _value.uvec[1] = y;
    
    _uploadFunction = &ShaderUniform::uploadUvec2;
}

ShaderUniform::ShaderUniform(const char* name, GLint location, GLuint x, GLuint y, GLuint z)
    : _location(location)
    , _type(SHADER_UNIFORM_UVEC3)
    , _name(name)
{
    _value.uvec[0] = x;
    _value.uvec[1] = y;
    _value.uvec[2] = z;
    
    _uploadFunction = &ShaderUniform::uploadUvec3;
}

ShaderUniform::ShaderUniform(const char* name, GLint location, GLuint x, GLuint y, GLuint z, GLuint w)
    : _location(location)
    , _type(SHADER_UNIFORM_UVEC4)
    , _name(name)
{
    _value.uvec[0] = x;
    _value.uvec[1] = y;
    _value.uvec[2] = z;
    _value.uvec[3] = w;
    
    _uploadFunction = &ShaderUniform::uploadUvec4;
}

ShaderUniform::ShaderUniform(const char* name, GLint location, const glm::mat4& matrix)
    : _location(location)
    , _type(SHADER_UNIFORM_MATRIX4X4)
    , _name(name)
{
    memcpy(_value.mat4, &matrix[0][0], sizeof(GLfloat) * 16);
    
    _uploadFunction = &ShaderUniform::uploadMatrix4x4;
}

ShaderUniform::~ShaderUniform()
{
}

void ShaderUniform::setValue(GLfloat x)
{
    GLF_ASSERT(_type == SHADER_UNIFORM_FLOAT);
    _value.vec[0] = x;
}

void ShaderUniform::setValue(GLfloat x, GLfloat y)
{
    GLF_ASSERT(_type == SHADER_UNIFORM_VEC2); 
    _value.vec[0] = x;
    _value.vec[1] = y;
}

void ShaderUniform::setValue(GLfloat x, GLfloat y, GLfloat z)
{
    GLF_ASSERT(_type == SHADER_UNIFORM_VEC3); 
    _value.vec[0] = x;
    _value.vec[1] = y;
    _value.vec[2] = z;
}

void ShaderUniform::setValue(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    GLF_ASSERT(_type == SHADER_UNIFORM_VEC4); 
    _value.vec[0] = x;
    _value.vec[1] = y;
    _value.vec[2] = z;
    _value.vec[3] = w;
}

void ShaderUniform::setValue(GLuint x)
{
    GLF_ASSERT(_type == SHADER_UNIFORM_UINT); 
    _value.uvec[0] = x;
}

void ShaderUniform::setValue(GLuint x, GLuint y)
{
    GLF_ASSERT(_type == SHADER_UNIFORM_UVEC2); 
    _value.uvec[0] = x;
    _value.uvec[1] = y;
}

void ShaderUniform::setValue(GLuint x, GLuint y, GLuint z)
{
    GLF_ASSERT(_type == SHADER_UNIFORM_UVEC3); 
    _value.uvec[0] = x;
    _value.uvec[1] = y;
    _value.uvec[2] = z;
}

void ShaderUniform::setValue(GLuint x, GLuint y, GLuint z, GLuint w)
{
    GLF_ASSERT(_type == SHADER_UNIFORM_UVEC4); 
    _value.uvec[0] = x;
    _value.uvec[1] = y;
    _value.uvec[2] = z;
    _value.uvec[3] = w;
}

void ShaderUniform::setValue(const glm::mat4& matrix)
{
    GLF_ASSERT(_type == SHADER_UNIFORM_MATRIX4X4); 
    memcpy(_value.mat4, &matrix[0][0], sizeof(GLfloat) * 16);
}

void ShaderUniform::upload()
{
    (this->*_uploadFunction)();
}

void ShaderUniform::uploadFloat()
{
    glUniform1f(_location, _value.vec[0]);
}

void ShaderUniform::uploadVec2()
{
    glUniform2f(_location, _value.vec[0], _value.vec[1]);
}

void ShaderUniform::uploadVec3()
{
    glUniform3f(_location, _value.vec[0], _value.vec[1], _value.vec[2]);
}

void ShaderUniform::uploadVec4()
{
    glUniform4f(_location, _value.vec[0], _value.vec[1], _value.vec[2], _value.vec[3]);
}

void ShaderUniform::uploadUint()
{
    glUniform1i(_location, _value.uvec[0]);
}

void ShaderUniform::uploadUvec2()
{
    glUniform2ui(_location, _value.uvec[0], _value.uvec[1]);
}

void ShaderUniform::uploadUvec3()
{
    glUniform3ui(_location, _value.uvec[0], _value.uvec[1], _value.uvec[2]);
}

void ShaderUniform::uploadUvec4()
{
    glUniform4ui(_location, _value.uvec[0], _value.uvec[1], _value.uvec[2], _value.uvec[3]);
}

void ShaderUniform::uploadMatrix4x4()
{
    glUniformMatrix4fv(_location, 1, GL_FALSE, _value.mat4);
}
    
GLF_NAMESPACE_END
