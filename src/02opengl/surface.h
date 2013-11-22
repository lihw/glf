/**
 * surface.h
 * Hongwei Li
 *
 * A 2D rendering target.
 */

#include <GL/glus.h>

class SurfaceManager;

#ifndef SURFACE_H
#define SURFACE_H

class Surface
{
    friend class SurfaceManager;
private:
    // Constructor. This class can't be initanced by users.
    Surface(SurfaceManager* parent);

public:
    // Create a render-to-texture target.
    // @param ncolors the number of color buffers.
    GLUSboolean createAsTexture(
            GLUSsizei width, 
            GLUSsizei height,
            GLUSenum  colorType,
            GLUSsizei ncolors,
            GLUSenum  depthStencilType);

    // Create a render-to-renderbuffer target.
    // @param ncolors the number of color buffers.
    GLUSboolean createAsRenderbuffer(
            GLUSsizei width, 
            GLUSsizei height,
            GLUSenum  colorType,
            GLUSsizei ncolors,
            GLUSenum  depthStencilType);

    // This surface is the default window render buffer.
    void initializeAsWindowBuffer(
            GLUSsizei width,
            GLUSsizei height,
            GLUSsizei colorBits,
            GLUSsizei depthBits,
            GLUSsizei stencilBits);

    // Set this surface the current render target.
    void enable();

    // Unbind this surface from the pipeline. 
    void disable();

    // Destroy this surface. The window surface can't be destroyed.
    void destroy();

    // Get the color type of this surface.
    GLUSenum getColorType();
    // Get the depth color type of this surface.
    GLUSenum getDepthStencilType();
    // Get the width of this surface.
    GLUSsizei getWidth();
    // Get the height of this surface.
    GLUSsizei getHeight();

private:
    GLUSuint _framebuffer;
    GLUSuint _width;
    GLUSuint _height;
    GLUSenum _colorType;
    GLUSuint _nColorBuffers;
    GLUSenum _depthStencilType;
    GLUSuint _colorTextures[16];
    GLUSuint _colorRenderBuffers[16];
    GLUSuint _depthStencilTexture;
    GLUSuint _depthStencilRenderBuffer;
        
    SurfaceManager* _parent;

    GLUSboolean checkFrameBufferStatus() const;
};


#endif 
