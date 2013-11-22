// -------------------------------------------------------------- 
// glf_image_storage.h
// Image load and store
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_IMAGE_STORAGE_H
#define GLF_IMAGE_STORAGE_H


#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

class ImageStorage
{
public:
    ImageStorage();
    ~ImageStorage();

    bool create1D(GLenum format, GLsizei width);
    bool create2D(GLenum format, GLsizei width, GLsizei height);

    void enableRead(GLuint location);
    void enableWrite(GLuint location);
    void enableReadAndWrite(GLuint location);
    void disable();
    
    void uploadData(void* data);

    GLF_INLINE GLuint getNumBytes() const
    { return _bytes; }
    GLF_INLINE GLuint getTexture() const
    { return _texture; }

private:
    GLenum _target;
    GLuint _texture;
    GLuint _location;
    bool   _enabled;
    GLsizei _width;
    GLsizei _height;
    GLenum  _format;
    GLuint  _bytes;
};


GLF_NAMESPACE_END


#endif // !GLF_IMAGE_STORAGE_H
