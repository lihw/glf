// --------------------------------------------------------------
// glf_texture.cpp
// The texture wrapper
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_texture.h"

#include "glf_glerror.h"

#include "../00system/glf_assert.h"
#include "../00system/glf_log.h"

GLF_NAMESPACE_BEGIN

Texture::Texture()
{
    _texture = 0;
    _width = 0;
    _height = 0;
    _wrap[0] = GL_CLAMP_TO_EDGE;
    _wrap[1] = GL_CLAMP_TO_EDGE;
    _wrap[2] = GL_CLAMP_TO_EDGE;
    _filtering[0] = GL_NEAREST;
    _filtering[1] = GL_NEAREST;
    _filtering[2] = GL_NEAREST;
    _bytes = 0;
    _enabled = false;
}

Texture::~Texture()
{
    if (glIsTexture(_texture))
    {
        glDeleteTextures(1, &_texture);
    }
}

bool Texture::create(GLubyte* data, GLuint width, GLuint height, GLenum format)
{
    if (_texture != 0)
    {
        GLF_LOGWARNING("Texture is already created");
        return true;
    }

    glGenTextures(1, &_texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap[1]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filtering[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filtering[1]);

    _width = width;
    _height = height;
    _format = format;

    GLenum dataFormat = GL_R;
    GLenum dataType = GL_UNSIGNED_BYTE;

    switch (format)
    {
        case GL_R8:      _bytes = _width * _height;     dataFormat = GL_RED;    break;
        case GL_RG8:     _bytes = _width * _height * 2; dataFormat = GL_RG;   break;
        case GL_RGB8:    _bytes = _width * _height * 3; dataFormat = GL_RGB;  break;
        case GL_RGBA8:   _bytes = _width * _height * 4; dataFormat = GL_RGBA; break;
        case GL_R32F:    _bytes = _width * _height * sizeof(GLfloat);     dataFormat = GL_RED;    break;
        case GL_RG32F:   _bytes = _width * _height * sizeof(GLfloat) * 2; dataFormat = GL_RG;   break;
        case GL_RGB32F:  _bytes = _width * _height * sizeof(GLfloat) * 3; dataFormat = GL_RGB;  break;
        case GL_RGBA32F: _bytes = _width * _height * sizeof(GLfloat) * 4; dataFormat = GL_RGBA; break;

        case GL_DEPTH_COMPONENT24: _bytes = _width * _height * 3; dataFormat = GL_DEPTH_COMPONENT; dataType = GL_FLOAT; break;
        default:
            GLF_ASSERT_NOT_REACHABLE();
            break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, dataFormat, dataType, data);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (!checkGLError(__FUNCTION__))
    {
        return false;
    }

    return true;
}


void Texture::setFiltering(GLenum minFiltering, GLenum magFiltering)
{
    if (_texture != 0)
    {
        if (!_enabled)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _texture);
        }

        if (_filtering[0] != minFiltering)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFiltering);
            _filtering[0] = minFiltering;
        }
        if (_filtering[1] != magFiltering)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFiltering);
            _filtering[1] = magFiltering;
        }

        if (!_enabled)
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

void Texture::setWrapping(GLenum wrapS, GLenum wrapT, GLenum wrapR)
{
    if (_texture != 0)
    {
        if (!_enabled)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _texture);
        }

        if (_wrap[0] != wrapS)
        {
            _wrap[0] = wrapS;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        }
        if (_wrap[1] != wrapT)
        {
            _wrap[1] = wrapT;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        }
        if (_wrap[2] != wrapR)
        {
            _wrap[2] = wrapR;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapR);
        }
        
        if (!_enabled)
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

void Texture::enable(GLuint textureUnit)
{
    GLF_ASSERT(!_enabled);
    _enabled = true;
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, _texture);
    _textureUnit = textureUnit;
}

void Texture::disable()
{
    GLF_ASSERT(_enabled);
    _enabled = false;
    glActiveTexture(GL_TEXTURE0 + _textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void* Texture::getImage() const
{
    if (!_enabled)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _texture);
    }

    GLenum dataType;
    GLenum dataFormat;
    GLuint bpp = 1;

    switch (_format)
    {
    case GL_R8UI:
    case GL_RG8UI:     
    case GL_RGB8UI:    
    case GL_RGBA8UI:
    case GL_RGBA8:
        bpp = 1;
        dataType = GL_UNSIGNED_BYTE;
        break;

    case GL_R32UI:     
    case GL_RG32UI:     
    case GL_RGB32UI:    
    case GL_RGBA32UI:   
        bpp = 4;
        dataType = GL_UNSIGNED_INT;
        break;

    case GL_R32I:     
    case GL_RG32I:     
    case GL_RGB32I:    
    case GL_RGBA32I:   
        bpp = 4;
        dataType = GL_INT;
        break;

    case GL_R32F:     
    case GL_RG32F:     
    case GL_RGB32F:    
    case GL_RGBA32F:   
        bpp = 4;
        dataType = GL_FLOAT;
        break;

    default:
        GLF_ASSERT_NOT_REACHABLE();
        break;
    }
    switch (_format)
    {
    case GL_R8I:     
    case GL_R8UI:
    case GL_R32F:
    case GL_R32UI:
    case GL_R32I:
        dataFormat = GL_RED;
        break;
    case GL_RG8I:     
    case GL_RG8UI:
    case GL_RG32F:
    case GL_RG32I:
    case GL_RG32UI:
        bpp *= 2;
        dataFormat = GL_RG;
        break;
    case GL_RGB8I:     
    case GL_RGB8UI:
    case GL_RGB32F:
    case GL_RGB32I:
    case GL_RGB32UI:
        bpp *= 3;
        dataFormat = GL_RGB;
        break;
    case GL_RGBA8I:  
    case GL_RGBA8:
    case GL_RGBA8UI:
    case GL_RGBA32F:
    case GL_RGBA32I:
    case GL_RGBA32UI:
        bpp *= 4;
        dataFormat = GL_RGBA;
        break;
    default:
        GLF_ASSERT_NOT_REACHABLE();
        break;
    }

    GLubyte* retImage = new GLubyte [bpp * _width * _height];
    glGetTexImage(GL_TEXTURE0, 0, dataFormat, dataType, retImage);

    if (!_enabled)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    return retImage;
}

GLF_NAMESPACE_END
