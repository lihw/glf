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

#include "glf_image.h"

#include "glf_log.h"
#include "glf_assert.h"

#include <stdio.h>

#include "images/glf_image_jpg_private.h"
#include "images/glf_image_png_private.h"
#include "images/glf_image_ppm_private.h"
#include "images/glf_image_tga_private.h"

GLF_NAMESPACE_BEGIN

Image::Image()
{
    _width = 0;
    _height = 0;
    _data = NULL;
    _nchannels = 1;
}

Image::~Image()
{
    delete [] _data;
}

bool Image::createFromFile(const char* filename)
{
    const char* suffix = strrchr(filename, '.');

    if (suffix == NULL)
    {
        GLF_LOGERROR("Failed to tell the image type of the file");
        return false;
    }

    FILE* fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        GLF_LOGERROR("Fail to read %s", filename);
        return false;
    }

    if (strncmp(suffix, ".ppm", 4) == 0)
    {
        bool ret = glfReadPPM(fp, _width, _height, _nchannels, _data);
        fclose(fp);
        return ret;
    }
    else if (strncmp(suffix, ".jpg", 4) == 0 ||
             strncmp(suffix, ".jpeg", 5) == 0)
    {
        bool ret = glfReadJPG(fp, _width, _height, _nchannels, _data);
        fclose(fp);
        return ret;
    }
    else if (strncmp(suffix, ".png", 4) == 0)
    {
        bool ret = glfReadPNG(fp, _width, _height, _nchannels, _data);
        fclose(fp);
        return ret;
    }
    else if (strncmp(suffix, ".tga", 4) == 0)
    {
        bool ret = glfReadTGA(fp, _width, _height, _nchannels, _data);
        fclose(fp);
        return ret;
    }
    else
    {
        GLF_LOGERROR("Unsupported image format");
        return false;
    }

    return false;
}

bool Image::createFromMemory(GLubyte* data, GLuint width, GLuint height, GLuint nchannels)
{
    GLF_ASSERT(nchannels == 1 || nchannels == 3);

    _width = width;
    _height = height;
    _nchannels = nchannels;
    
    GLuint num = _width * _height * _nchannels;
    if (_data) 
    {
        delete [] _data;
    }

    _data = new GLubyte [num];

    memcpy(_data, data, num);

    return true;
}

const GLubyte* Image::get(GLuint x, GLuint y) const
{
    if (_data && x < _width && y < _height)
    {
        return &_data[(y * _width + x) * _nchannels];
    }

    return NULL;
}

GLubyte* Image::get(GLuint x, GLuint y) 
{
    if (_data && x < _width && y < _height)
    {
        return &_data[(y * _width + x) * _nchannels];
    }

    return NULL;
}


bool Image::write(const char* filename)
{
    const char* suffix = strrchr(filename, '.');

    if (suffix == NULL)
    {
        GLF_LOGERROR("Failed to tell the image type of the file");
        return false;
    }

    FILE* fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        GLF_LOGERROR("Fail to write to %s", filename);
        return false;
    }

    if (strncmp(suffix, ".ppm", 4) == 0)
    {
        bool ret = glfWritePPM(fp, _width, _height, _nchannels, _data);
        fclose(fp);
        return ret;
    }
    else if (strncmp(suffix, ".jpg", 4) == 0 ||
             strncmp(suffix, ".jpeg", 5) == 0)
    {
        bool ret = glfWriteJPG(fp, _width, _height, _nchannels, 90, _data);
        fclose(fp);
        return ret;
    }
    else if (strncmp(suffix, ".png", 4) == 0)
    {
        bool ret = glfWritePNG(fp, _width, _height, _nchannels, _data);
        fclose(fp);
        return ret;
    }
    else if (strncmp(suffix, ".tga", 4) == 0)
    {
        bool ret = glfWriteTGA(fp, _width, _height, _nchannels, _data);
        fclose(fp);
        return ret;
    }
    else
    {
        GLF_LOGERROR("Unsupported image format");
        return false;
    }

    return false;
}
    
void Image::flipVertical()
{
    if (_data == NULL)
    {
        return ;
    }

    int lsize = _width * _nchannels;
    GLubyte *tbuf = new GLubyte [lsize];

    for(int y = 0; y < _height / 2; y++) 
    {
        memcpy(tbuf, &_data[y * lsize], lsize);
        memcpy(&_data[y * lsize], &_data[(_height - y - 1) * lsize], lsize);
        memcpy(&_data[(_height - y - 1) * lsize], tbuf, lsize);
    }

    delete [] tbuf;
}

GLF_NAMESPACE_END
