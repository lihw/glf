// -------------------------------------------------------------- 
// glf_buffer.cpp
// The buffer wrapper
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_atomic.h"

#include "../02opengl/glf_glerror.h"

#include "../01system/glf_assert.h"
#include "../01system/glf_log.h"

GLF_NAMESPACE_BEGIN
    
Atomic::Atomic()
{
    _buffer = 0;
    _location = 0;
    _enabled = false;
}

Atomic::~Atomic()
{
    if (_buffer != 0)
    {
        glDeleteBuffers(1, &_buffer);
    }
}

bool Atomic::create()
{
    if (_buffer != 0)
    {
        GLF_LOGWARNING("Atomic object has been created already");
        return true;
    }

    glGenBuffers(1, &_buffer);

    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _buffer);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

    if (!checkGLError(__FUNCTION__))
    {
        return false;
    }

    return true;
}

void Atomic::set(GLuint value)
{

    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _buffer);

    GLuint* ptr = (GLuint*)glMapBufferRange(
            GL_ATOMIC_COUNTER_BUFFER, 
            0,
            sizeof(GLuint),
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    *ptr = value;

    glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
    
    checkGLError(__FUNCTION__);
}

void Atomic::enable(GLuint location)
{
    GLF_ASSERT(!_enabled);
    _enabled = true;
    _location = location;
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, location, _buffer);
}

void Atomic::disable()
{
    GLF_ASSERT(_enabled);
    _enabled = false;
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
}


GLF_NAMESPACE_END
