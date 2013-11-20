// -------------------------------------------------------------- 
// glf_atomic.h
// The atomic buffer wrapper
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#ifndef GLF_ATOMIC_H
#define GLF_ATOMIC_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

class Atomic
{
public:
    Atomic();
    ~Atomic();

    bool create();

    void set(GLuint value);

    void enable(GLuint location);
    void disable();

private:
    GLuint _buffer;
    GLuint _location; // bind location
    bool   _enabled;
};

GLF_NAMESPACE_END

#endif // !GLF_BUFFER_H
