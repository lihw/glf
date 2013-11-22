//
// surface.cpp
// Hongwei Li (lihw81@gmail.com)
//
//

#include "surface.h"

#include "01wrapper/surface_manager.h"

Surface::Surface(SurfaceManager* parent)
{
    _framebuffer = 0;

    _width  = 1;
    _height = 1;
    
    _nColorBuffers = 0;

    for (GLUSint i = 0; i < 16; ++i)
    {
        _colorTextures[i] = 0;
        _colorRenderBuffers[i] = 0;
    }
    _depthStencilTexture = 0;
    _depthStencilRenderBuffer = 0;
}

GLUSboolean Surface::createAsRenderbuffer(
            GLUSsizei width, 
            GLUSsizei height,
            GLUSenum  colorType,
            GLUSsizei ncolors,
            GLUSenum  depthStencilType)
{
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    // Create color buffers.
    glGenRenderbuffers(ncolors, _colorRenderBuffers);
    for (GLUSsizei i = 0; i < ncolors; i++)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffers[i]);
	    glRenderbufferStorage(GL_RENDERBUFFER, colorType, width, height);

	    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, _colorRenderBuffers[i]);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Create depth and stencil buffer
    glGenRenderbuffers(1, &_depthStencilRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthStencilRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, depthStencilType, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthStencilRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilRenderBuffer);
	
    // Check the completeness of the frame buffer.
    //if (!checkCompleteness())
    //{
    //    return GLUS_FALSE;
    //}

    _width  = width;
    _height = height;
    _colorType = colorType;
    _nColorBuffers = ncolors;
    _depthStencilType = depthStencilType;

    return GLUS_TRUE;
}

GLUSboolean Surface::createAsTexture(
            GLUSsizei width, 
            GLUSsizei height,
            GLUSenum  colorType,
            GLUSsizei ncolors,
            GLUSenum  depthStencilType)
{
    /*
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    // Create color textures.
    glGenTextures(ncolors, _colorTextures);
    for (GLUSsizei i = 0; i < ncolors; i++)
    {
        glBindTexture(GL_TEXTURE_2D, _colorTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, colorType, width, height, 0, GL_RGBA, GL_FLOAT, 0); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, _colorRenderBuffers[i]);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Create depth and stencil buffer
    glGenREnderbuffers(1, _depthStencilRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthStencilRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, depthStencilType, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthStencilRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, _depthStencilRenderBuffer);
	
    // Check the completeness of the frame buffer.
    if (!checkCompleteness())
    {
        return GLUS_FALSE;
    } 
    */

    _width  = width;
    _height = height;
    _colorType = colorType;
    _nColorBuffers = ncolors;
    _depthStencilType = depthStencilType;

    return GLUS_TRUE;
}

void Surface::initializeAsWindowBuffer(
        GLUSsizei width,
        GLUSsizei height,
        GLUSsizei colorBits,
        GLUSsizei depthBits,
        GLUSsizei stencilBits)
{
    _framebuffer = 0;

    _width  = width;
    _height = height;

    switch (colorBits)
    {
        case 24: _colorType = GL_RGB8UI; break;
        case 32: _colorType = GL_RGBA8UI; break;
    }

    if (depthBits == 24 && stencilBits == 8)
    {
        _depthStencilType = GL_DEPTH24_STENCIL8;
    }
    else if (depthBits == 32 && stencilBits == 8)
    {
        _depthStencilType = GL_DEPTH32F_STENCIL8;
    }
    else
    {
#if defined _DEBUG
        glusLogPrint(GLUS_LOG_WARNING, "%s: don't support such depth and stencil combination", __FUNCTION__);
#endif
        return ;
    }
}

void Surface::enable()
{
    if (_parent->getCurrentSurface() != this)
    {
        _parent->setCurrentSurface(this);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    }
}

void Surface::disable()
{
    if (_parent->getCurrentSurface() == this)
    {
        _parent->setCurrentSurface(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void Surface::destroy()
{
    if (_framebuffer != 0)
    {
        glDeleteFramebuffers(1, &_framebuffer);
    }
}

GLUSenum Surface::getColorType()
{
    return _colorType;
}

GLUSenum Surface::getDepthStencilType()
{
    return _depthStencilType;
}

GLUSsizei Surface::getWidth()
{
    return _width;
}

GLUSsizei Surface::getHeight()
{
    return _height;
}
