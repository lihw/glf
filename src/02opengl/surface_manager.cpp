//
// surface_manager.cpp
// Hongwei Li (lihw81@gmail.com)
//
//

#include "surface_manager.h"

#include "surface.h"

SurfaceManager* SurfaceManager::_instance = NULL;

SurfaceManager* SurfaceManager::instance()
{
    // FIXME: to make it thread-safe
    if (_instance == NULL)
    {
        _instance = new SurfaceManager();
    }
    
    return _instance;
}

void SurfaceManager::initialize(GLUSint windowWidth, GLUSint windowHeight, 
            GLUSint redBits, GLUSint blueBits, GLUSint greenBits, GLUSint alphaBits,
            GLUSint depthBits, GLUSint stencilBits)
{
    _windowWidth = windowWidth;
    _windowHeight = windowHeight;

    Surface* surface = createSurface();

    surface->initializeAsWindowBuffer(_windowWidth, _windowHeight, redBits + blueBits + greenBits + alphaBits,
            depthBits, stencilBits);

    _surfaces.push_back(surface);

    _currentSurface = surface;
}

void SurfaceManager::destroy()
{
    size_t n = _surfaces.size();

    for (size_t i = 0; i < n; ++i)
    {
        _surfaces[i]->destroy();
        delete [] _surfaces[i];
    }
}

Surface* SurfaceManager::getCurrentSurface()
{
    return _currentSurface;
}

void SurfaceManager::setCurrentSurface(Surface* surface)
{
    _currentSurface = surface;
}

Surface* SurfaceManager::createSurface()
{
    Surface* surface = new Surface(_instance);
    _surfaces.push_back(surface);
    return surface;
}

void SurfaceManager::updateViewport(GLUSint x, GLUSint y, GLUSint width, GLUSint height)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;

    glViewport(x, y, width, height);
}

void SurfaceManager::convertCoordinateFromWindowToScreen(const GLUSint x, const GLUSint y, GLUSfloat& cx, GLUSfloat& cy)
{
    // From window coordinate to the OpenGL coordinate.
    GLUSfloat fx = (GLUSfloat)x;
    GLUSfloat fy = (GLUSfloat)(_windowHeight - 1 - y);

    // To viewport coordinate.
    fx -= (GLUSfloat)_x;
    fy -= (GLUSfloat)_y;

    cx = fx / (GLUSfloat)(_width - 1) * 2.0f - 1.0f;
    cy = fy / (GLUSfloat)(_height - 1) * 2.0f - 1.0f;
}
