// -------------------------------------------------------------- 
// glf_framebuffer.h
// The framebuffer wrapper
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_FRAMEBUFFER_H
#define GLF_FRAMEBUFFER_H

#include "../glf_common.h"

#include "../00system/glf_assert.h"

GLF_NAMESPACE_BEGIN

class Texture;


class Framebuffer
{
private:
    Framebuffer(const Framebuffer& other) {}
    void operator=(const Framebuffer& other) {}

public:
    Framebuffer();
    ~Framebuffer();

    bool create(GLuint width, 
                GLuint height, 
                const GLenum* colorFormat,
                GLuint numColorBuffers,
                GLenum depthFormat,
                GLenum stencilFormat);

    GLF_INLINE Texture* getColorBuffer(GLuint attachment) const
    { GLF_ASSERT(attachment < _numColorBuffers); return _colorBuffers[attachment]; }
    GLF_INLINE Texture* getDepthBuffer() const
    { return _depthBuffer; }
    GLF_INLINE Texture* getStencilBuffer() const
    { return _stencilBuffer; }

    static void clear(bool color, bool depth, bool stencil);

    GLF_INLINE GLuint getFramebufferHandle() const
    { return _framebuffer; }
    GLF_INLINE GLuint getWidth() const
    { return _width; }
    GLF_INLINE GLuint getHeight() const
    { return _height; }


    // Enable or disable
    void enable();
    void disable();

private:
    bool checkFramebufferStatus();

private:
    GLuint    _numColorBuffers;
    Texture*  _colorBuffers[32];
    Texture*  _depthBuffer;
    Texture*  _stencilBuffer;

    GLuint _framebuffer;
    GLuint _width;
    GLuint _height;

    bool _enabled;
};

GLF_NAMESPACE_END



#endif // !GLF_FRAMEBUFFER_H
