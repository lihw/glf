// 
// texture_manager.h
// Hongwei Li (lihw81@gmail.com)
// To manage all OpenGL textures used in the application.
//

#include <GL/glus.h>

#include <vector>

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANGAER_H

class Texture;

class TextureManager
{
private:
    // It is a singleton so that we need to disable the public constructor.
    TextureManager();

    // It is a singleton so that we need to disable the public copy constructor.
    TextureManager(const TextureManager& textureManager);
public:
    // Fetch the handle of the texture manager object.
    static TextureManager* instance();

    // Destroy the texture manager and release all textures.
    void destroy();

    // Create a specific texture.
    Texture* createTexture1D(GLUSsizei width, GLUSenum internalFormat);
    Texture* createTexture2D(GLUSsizei width, GLUSsizei height, GLUSenum internalFormat);
    Texture* createTexture3D(GLUSsizei width, GLUSsizei height, GLUSsizei depth, GLUSenum internalFormat);
    Texture* createTexture1DArray(GLUSsizei width, GLUSsizei size, GLUSenum internalFormat);
    Texture* createTexture2DArray(GLUSsizei width, GLUSsizei height, GLUSsizei size, GLUSenum internalFormat);
    Texture* createTextureCubemap(GLUSsizei width, GLUSenum internalFormat);

private:
    std::vector<Texture*> _textures; 

    static TextureManager* _instance;
};


#endif 
