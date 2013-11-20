// -------------------------------------------------------------- 
// glf_framebuffer.cpp
// The framebuffer wrapper
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_framebuffer.h"

#include "glf_texture.h"

#include "../00system/glf_log.h"

GLF_NAMESPACE_BEGIN

Framebuffer::Framebuffer()
{
    _enabled = false;
    _framebuffer = 0;
    _width = 0;
    _height = 0;

    _numColorBuffers = 0;
    memset(_colorBuffers, 0, sizeof(Texture*) * 32);

    _depthBuffer = NULL;
    _stencilBuffer = NULL;
}

Framebuffer::~Framebuffer()
{
    if (_framebuffer != 0)
    {
        for (GLuint i = 0; i < _numColorBuffers; ++i)
        {
            delete _colorBuffers[i];
        } 
        delete _depthBuffer;
        delete _stencilBuffer;

        glDeleteFramebuffers(1, &_framebuffer);
    }
}

bool Framebuffer::create(GLuint width, 
                         GLuint height, 
                         const GLenum* colorFormats,
                         GLuint numColorBuffers,
                         GLenum depthFormat,
                         GLenum stencilFormat)
{
    if (_framebuffer != 0)
    {
        GLF_LOGWARNING("Framebuffer is already created");
        return true;
    }

    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    _width = width;
    _height = height;

    // -------------------------------------------------------------- 
    // Color attachment
    // -------------------------------------------------------------- 
    _numColorBuffers = numColorBuffers;
    for (GLuint i = 0; i < numColorBuffers; ++i)
    {
        _colorBuffers[i] = new Texture;
        if (!_colorBuffers[i]->create(NULL, width, height, colorFormats[i]))
        {
            return false;
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 
                _colorBuffers[i]->getTextureHandle(), 0);
    }

    // -------------------------------------------------------------- 
    // Depth attachment
    // -------------------------------------------------------------- 
    if (depthFormat != 0)
    {
        _depthBuffer = new Texture;
        if (_depthBuffer->create(NULL, width, height, depthFormat))
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 
                    _depthBuffer->getTextureHandle(), 0);
        }
        else
        {
            return false;
        }
    }

    // -------------------------------------------------------------- 
    // Stencil attachment
    // -------------------------------------------------------------- 
    if (stencilFormat != 0)
    {
        _stencilBuffer = new Texture;
        if (_stencilBuffer->create(NULL, width, height, stencilFormat))
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 
                    _stencilBuffer->getTextureHandle(), 0);
        }
        else
        {
            return false;
        }
    }

    // -------------------------------------------------------------- 
    // Check framebuffer status
    // -------------------------------------------------------------- 
    if (_numColorBuffers == 0)
    {
        glDrawBuffer(GL_NONE);
    }

    if (!checkFramebufferStatus())
    {
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}
    
void Framebuffer::enable()
{
    GLF_ASSERT(!_enabled);
    _enabled = true;
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
}

void Framebuffer::disable()
{
    GLF_ASSERT(_enabled);
    _enabled = false;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
    
void Framebuffer::clear(bool color, bool depth, bool stencil)
{
    GLuint bits = 0;

    if (color)
    {
        bits |= GL_COLOR_BUFFER_BIT;
    }
    if (depth)
    {
        bits |= GL_DEPTH_BUFFER_BIT;
    }
    if (stencil)
    {
        bits |= GL_STENCIL_BUFFER_BIT;
    }

    glClear(bits);
}

bool Framebuffer::checkFramebufferStatus()
{
    GLuint result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    bool ret = true;

    if (result != GL_FRAMEBUFFER_COMPLETE)
    {
        glDeleteFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

        if (result == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
        {
            GLF_LOGERROR("Frame buffer couldn't be constructed: incomplete attachment");
        }
        else if (result == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
        {
            GLF_LOGERROR("Frame buffer couldn't be constructed: missing attachment");
        }
        
        ret = false;
    }

    return ret;
}

GLF_NAMESPACE_END
