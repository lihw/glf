// -------------------------------------------------------------- 
// glf_image_storage.cpp
// Image load and store
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_image_storage.h"

#include "../01api/glf_glerror.h"

#include "../00system/glf_log.h"
#include "../00system/glf_assert.h"

GLF_NAMESPACE_BEGIN

ImageStorage::ImageStorage()
{
    _texture = 0;
    _enabled = false;
    _bytes = 0;
}

ImageStorage::~ImageStorage()
{
    if (_texture != 0)
    {
        glDeleteTextures(1, &_texture);
    }
}

bool ImageStorage::create2D(GLenum format, GLsizei width, GLsizei height)
{
    if (_texture != 0)
    {
        GLF_LOGWARNING("Texture storage has been already created");
        return true;
    }

    glGenTextures(1, &_texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexStorage2D(GL_TEXTURE_2D, GLint(1), format, width, height);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    GLenum dataFormat = GL_R;
    GLenum dataType = GL_UNSIGNED_BYTE;
    switch (format)
    {
        case GL_R8I:      
        case GL_R8UI:    _bytes = _width * _height;     break;
        case GL_RG8I:    
        case GL_RG8UI:   _bytes = _width * _height * 2; break;
        case GL_RGB8I:   
        case GL_RGB8UI:  _bytes = _width * _height * 3; break;
        case GL_RGBA8I:  
        case GL_RGBA8UI: _bytes = _width * _height * 4; break;

        case GL_R32I:
        case GL_R32UI:
        case GL_R32F:    _bytes = _width * _height * sizeof(GLfloat);     break;
        case GL_RG32I:
        case GL_RG32UI:
        case GL_RG32F:   _bytes = _width * _height * sizeof(GLfloat) * 2; break;
        case GL_RGB32I:
        case GL_RGB32UI:
        case GL_RGB32F:  _bytes = _width * _height * sizeof(GLfloat) * 3; break;
        case GL_RGBA32I:
        case GL_RGBA32UI:
        case GL_RGBA32F: _bytes = _width * _height * sizeof(GLfloat) * 4; break;
        
        default:
            GLF_ASSERT_NOT_REACHABLE();
            break;
    }

    if (!checkGLError(__FUNCTION__))
    {
        return false;
    }

    _target = GL_TEXTURE_2D;
    _format = format;
    _width = width;
    _height = height;

    return true;
}

bool ImageStorage::create1D(GLenum format, GLsizei width)
{
    if (_texture != 0)
    {
        GLF_LOGWARNING("Texture storage has been already created");
        return true;
    }

    glGenTextures(1, &_texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, _texture);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 1);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexStorage1D(GL_TEXTURE_1D, GLint(1), format, width);

    glBindTexture(GL_TEXTURE_1D, 0);
    
    switch (format)
    {
        case GL_R8I:      
        case GL_R8UI:    _bytes = _width;     break;
        case GL_RG8I:    
        case GL_RG8UI:   _bytes = _width * 2; break;
        case GL_RGB8I:   
        case GL_RGB8UI:  _bytes = _width * 3; break;
        case GL_RGBA8I:  
        case GL_RGBA8UI: _bytes = _width * 4; break;

        case GL_R32I:
        case GL_R32UI:
        case GL_R32F:    _bytes = _width * sizeof(GLfloat);     break;
        case GL_RG32I:
        case GL_RG32UI:
        case GL_RG32F:   _bytes = _width * sizeof(GLfloat) * 2; break;
        case GL_RGB32I:
        case GL_RGB32UI:
        case GL_RGB32F:  _bytes = _width * sizeof(GLfloat) * 3; break;
        case GL_RGBA32I:
        case GL_RGBA32UI:
        case GL_RGBA32F: _bytes = _width * sizeof(GLfloat) * 4; break;
        
        default:
            GLF_ASSERT_NOT_REACHABLE();
            break;
    }

    if (!checkGLError(__FUNCTION__))
    {
        GLF_LOGINFO("Probably the size of texture exceeds the limitation (e.g, 65536)");
        return false;
    }

    _target = GL_TEXTURE_1D;
    _format = format;
    _width = width;
    _height = 1;

    return true;
}

void ImageStorage::enableRead(GLuint location)
{
    GLF_ASSERT(!_enabled);
    glBindImageTexture(location, _texture, 0, GL_FALSE, 0, GL_READ_ONLY, _format);
    _enabled = true;
    _location = location;
}

void ImageStorage::enableWrite(GLuint location)
{
    GLF_ASSERT(!_enabled);
    glBindImageTexture(location, _texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, _format);
    _enabled = true;
    _location = location;
}

void ImageStorage::enableReadAndWrite(GLuint location)
{
    GLF_ASSERT(!_enabled);
    glBindImageTexture(location, _texture, 0, GL_FALSE, 0, GL_READ_WRITE, _format);
    _enabled = true;
    _location = location;
}

void ImageStorage::disable()
{
    GLF_ASSERT(_enabled);
    _enabled = false;
}
    
void ImageStorage::uploadData(void* data)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(_target, _texture);
    
    GLenum dataFormat = GL_RED;
    GLenum dataType = GL_UNSIGNED_BYTE;
    switch (_format)
    {
        case GL_R8UI:
        case GL_RG8UI:     
        case GL_RGB8UI:    
        case GL_RGBA8UI:   
            dataType = GL_UNSIGNED_BYTE;
            break;

        case GL_R32UI:     
        case GL_RG32UI:     
        case GL_RGB32UI:    
        case GL_RGBA32UI:   
            dataType = GL_UNSIGNED_INT;
            break;

        case GL_R32I:     
        case GL_RG32I:     
        case GL_RGB32I:    
        case GL_RGBA32I:   
            dataType = GL_INT;
            break;

        case GL_R32F:     
        case GL_RG32F:     
        case GL_RGB32F:    
        case GL_RGBA32F:   
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
            dataFormat = GL_RG;
            break;
        case GL_RGB8I:     
        case GL_RGB8UI:
        case GL_RGB32F:
        case GL_RGB32I:
        case GL_RGB32UI:
            dataFormat = GL_RGB;
            break;
        case GL_RGBA8I:     
        case GL_RGBA8UI:
        case GL_RGBA32F:
        case GL_RGBA32I:
        case GL_RGBA32UI:
            dataFormat = GL_RGBA;
            break;
        default:
            GLF_ASSERT_NOT_REACHABLE();
            break;
    }
    
    if (_target == GL_TEXTURE_1D)
    {
        glTexSubImage1D(
                GL_TEXTURE_1D, 
                0, 
                0, 
                _width, 
                dataFormat, 
                dataType, 
                data);
    }
    else if (_target == GL_TEXTURE_2D)
    {
        glTexSubImage2D(
                GL_TEXTURE_2D, 
                0, 
                0, 
                0, 
                _width, 
                _height,
                dataFormat, 
                dataType, 
                data);
    }
    else
    {
        GLF_ASSERT_NOT_REACHABLE();
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(_target, 0);

    checkGLError(__FUNCTION__);
}

GLF_NAMESPACE_END



