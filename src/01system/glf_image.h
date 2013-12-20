// -------------------------------------------------------------- 
// glf_image.h
// Read and write images (jpg, png, ppm, tga)
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_IMAGE_H
#define GLF_IMAGE_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

class Image
{
public:
    Image();
    ~Image();

    bool createFromFile(const char* filename);
    bool createFromMemory(GLubyte* data, GLuint width, GLuint height, GLuint nchannels);

    bool write(const char* filename);

    const GLubyte* get(GLuint x, GLuint y) const;
    GLubyte* get(GLuint x, GLuint y);

    GLF_INLINE GLuint getWidth() const
    { return _width; }
    GLF_INLINE GLuint getHeight() const
    { return _height; }
    GLF_INLINE GLuint getNumChannels() const
    { return _nchannels; }
    GLF_INLINE GLubyte* getData() const
    { return _data; }

    void flipVertical();

private:
    GLuint    _width;
    GLuint    _height;
    GLuint    _nchannels;
    GLubyte*  _data;
};


GLF_NAMESPACE_END


#endif // !GLF_IMAGE_H

