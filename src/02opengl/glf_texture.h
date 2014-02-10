// -------------------------------------------------------------- 
// glf_texture.h
// The texture wrapper
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#ifndef GLF_TEXTURE_H
#define GLF_TEXTURE_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

class Texture
{
public:
    Texture();
    ~Texture();

    // Create a 2D texture from given data, usually for converting an image to an
    // OpenGL texture.
    bool create(GLubyte* data, GLuint width, GLuint height, GLenum format);
    // Create an empty 1D, 2D or 3D texture.
    bool create(GLuint width, GLenum format);
    bool create(GLuint width, GLuint height, GLenum format);
    bool create(GLuint width, GLuint height, GLuint depth, GLenum format);

    void setFiltering(GLenum minFiltering, GLenum magFiltering);
    void setWrapping(GLenum wrapS, GLenum wrapT, GLenum wrapR);

    GLF_INLINE GLuint getWidth() const { return _width; }
    GLF_INLINE GLuint getHeight() const { return _height; }
    GLF_INLINE GLenum getFormat() const { return _format; }
    GLF_INLINE GLuint getTextureHandle() const { return _texture; }

    void enable(GLuint textureUnit);
    void disable();

    void* getImage() const;

private: 
    GLuint _width;
    GLuint _height;
    GLuint _depth;
    GLenum _format;
    GLuint _texture;
    bool   _enabled;
    GLuint _bytes;
    GLenum _target;
    GLenum _wrap[3]; // s, t, r
    GLenum _filtering[2]; // min and mag
    GLuint _textureUnit;
};


GLF_NAMESPACE_END


#endif // !GLF_TEXTURE_H



