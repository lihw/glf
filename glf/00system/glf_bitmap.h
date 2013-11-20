// -------------------------------------------------------------- 
// glf_bitmap.h
// Read and write bitmaps
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_BITMAP_H
#define GLF_BITMAP_H

#include "../glf_common.h"

GLF_NAMESPACE_BEGIN

class Bitmap
{
public:
    Bitmap();
    ~Bitmap();

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

private:
    GLuint _width;
    GLuint _height;
    GLuint _nchannels;
    GLubyte* _data;
};



GLF_NAMESPACE_END



#endif // !GLF_BITMAP_H
