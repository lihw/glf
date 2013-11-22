//
// surface_manager.h
// Hongwei Li (lihw81@gmail.com)
// 
// To manage the surfaces. The manager is globally unique (singleton).
//

#include <GL/glus.h>

#include <vector>

class Surface;

#ifndef SURFACE_MANAGER_H
#define SURFACE_MANAGER_H

class SurfaceManager
{
    friend class Surface;
public:
    static SurfaceManager* instance();

    // Initialize the object. All the parameters are parameters for the window buffer.
    void initialize(GLUSint windowWidth, GLUSint windowHeight, 
            GLUSint redBits, GLUSint blueBits, GLUSint greenBits, GLUSint alphaBits,
            GLUSint depthBits, GLUSint stencilBits);

    // Destroy the object.
    void destroy();

    // Return the current surface. 
    Surface* getCurrentSurface();

    // Get new surface and return its pointer.
    Surface* createSurface();

    // Update the current viewport.
    void updateViewport(GLUSint x, GLUSint y, GLUSint width, GLUSint height);

    // Convert the position from window coordinate to screen coordinate (-1, 1).
    void convertCoordinateFromWindowToScreen(const GLUSint x, const GLUSint y, GLUSfloat& cx, GLUSfloat& cy);

private: 
    // Constructor. Disable it.
    SurfaceManager() {};

    // Copy constructor. Disable it.
    SurfaceManager(const SurfaceManager& rhs) {};

    // Set the current surface
    void setCurrentSurface(Surface* surface);
private:
    GLUSint _x, _y, _width, _height; // The current viewport.
    GLUSint _windowWidth, _windowHeight; // The dimension of the window.

    std::vector<Surface*> _surfaces;
    Surface* _currentSurface;

    static SurfaceManager* _instance; // It is a singleton class.
};

#endif
