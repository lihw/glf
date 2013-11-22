// 
// renderer.h
// Hongwei Li (lihw81@gmail.com)
// 
// The rendering context. Only rendering context can exist globally.
// 

#include <GL/glus.h>

#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
public:
    // Get the global instance of the renderer.
    static Renderer* instance();

    // Release the OpenGL resource before quit.
    GLUSboolean destroy();

    // Check OpenGL error
    void checkOpenGLError(const char* message);

    // Begin rendering. 
    // Apply the OpenGL context change here.
    void apply();

    // FPS information.
    GLUSfloat getFPS();

private:
    // Prevent the constructor from being called outside.
    Renderer();
    // Disable the copy constructor.
    Renderer(const Renderer* renderer) {};

private:
    static Renderer* _instance;

    // TODO: FPS information.
};

#endif 
