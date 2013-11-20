// 
// texture_manager.cpp
// Hongwei Li (lihw81@gmail.com)
//

#include "texture_manager.h"

#include "texture.h"

TextureManager* TextureManager::_instance = NULL;

TextureManager::TextureManager()
{
}

TextureManager::TextureManager(const TextureManager& textureManager)
{
}

TextureManager* TextureManager::instance()
{
    if (_instance == NULL)
    {
        _instance = new TextureManager;
    }

    return _instance;
}

void TextureManager::destroy()
{
    for (size_t i = 0; i < _textures.size(); ++i)
    {
        delete _textures[i];
    }
}

Texture* TextureManager::createTexture1D(GLUSsizei width, GLUSenum internalFormat)
{
    Texture* texture = new Texture;

    texture->createTexture1D(width, internalFormat, this);

    _textures.push_back(texture);

    return texture;
}

Texture* TextureManager::createTexture2D(GLUSsizei width, GLUSsizei height, GLUSenum internalFormat)
{
    Texture* texture = new Texture;

    texture->createTexture2D(width, height, internalFormat, this);

    _textures.push_back(texture);

    return texture;
}

Texture* TextureManager::createTexture3D(GLUSsizei width, GLUSsizei height, GLUSsizei depth, GLUSenum internalFormat)
{
    Texture* texture = new Texture;

    texture->createTexture3D(width, height, depth, internalFormat, this);

    _textures.push_back(texture);

    return texture;
}

Texture* TextureManager::createTexture1DArray(GLUSsizei width, GLUSsizei size, GLUSenum internalFormat)
{
    Texture* texture = new Texture;

    texture->createTexture1DArray(width, size, internalFormat, this);

    _textures.push_back(texture);

    return texture;
}

Texture* TextureManager::createTexture2DArray(GLUSsizei width, GLUSsizei height, GLUSsizei size, GLUSenum internalFormat)
{
    Texture* texture = new Texture;

    texture->createTexture2DArray(width, height, size, internalFormat, this);

    _textures.push_back(texture);

    return texture;
}

Texture* TextureManager::createTextureCubemap(GLUSsizei width, GLUSenum internalFormat)
{
    Texture* texture = new Texture;

    texture->createTextureCubemap(width, internalFormat, this);

    _textures.push_back(texture);

    return texture;
}
