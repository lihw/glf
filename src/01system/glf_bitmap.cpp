// -------------------------------------------------------------- 
// glf_bitmap.cpp
// Read and write bitmaps
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_bitmap.h"

#include "glf_log.h"
#include "glf_assert.h"

#include <stdio.h>

GLF_NAMESPACE_BEGIN

Bitmap::Bitmap()
{
    _width = 0;
    _height = 0;
    _data = NULL;
    _nchannels = 1;
}

Bitmap::~Bitmap()
{
    delete [] _data;
}

bool Bitmap::createFromFile(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) 
    { 
        GLF_LOGERROR("Failed to read %s", filename);
        return GL_FALSE; 
    }

    int c,s;

    // Read the first line.
    char line[1024];
    if (!fgets(line, 1024, fp))
    {
        GLF_LOGERROR("Failed to read 1st line of %s", filename);
        return false;
    }

    if (line[0] == 'P' && line[1] == '5')
    {
        _nchannels = 1;
    }
    else if (line[0] == 'P' && line[1] == '6')
    {
        _nchannels = 3;
    }
    else
    {
        GLF_LOGERROR("Unsupported PPM pixel format");
        return false;
    }

    // Ignore the comments
    while ((c = fgetc(fp)) == '#') 
    { 
        do 
        { 
            s = fgetc(fp); 
        } while (s != '\n');
    } 

    ungetc(c, fp);

    // Read the width and height
    int n, tmp;
    n = fscanf(fp, "%d", &_width);
    if (n < 1) return false;
    n = fscanf(fp, "%d", &_height);
    if (n < 1) return false;
    n = fscanf(fp, "%d", &tmp);
    if (n < 1 || tmp != 255) return false;

    // Ignore any new lines
    c = fgetc(fp);
    while (c == '\r' || c == '\n') 
    {
        c = fgetc(fp);
    } 

    ungetc(c, fp);

    // Read the rasterization data
    GLuint num = _width * _height * _nchannels;
    if (_data) 
    {
        delete [] _data;
    }

    _data = new GLubyte [num];
    fread(_data, 1, num, fp);

    fclose(fp);

    return true;
}

bool Bitmap::createFromMemory(GLubyte* data, GLuint width, GLuint height, GLuint nchannels)
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

const GLubyte* Bitmap::get(GLuint x, GLuint y) const
{
    if (_data && x < _width && y < _height)
    {
        return &_data[(y * _width + x) * _nchannels];
    }

    return NULL;
}

GLubyte* Bitmap::get(GLuint x, GLuint y) 
{
    if (_data && x < _width && y < _height)
    {
        return &_data[(y * _width + x) * _nchannels];
    }

    return NULL;
}


bool Bitmap::write(const char* filename)
{
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        GLF_LOGERROR("Fail to open %s for writing", filename);
        return false;
    }

    if (_nchannels == 1)
    {
        fprintf(fp, "P5\n");
    }
    else if (_nchannels == 3)
    {
        fprintf(fp, "P6\n");
    }
    else
    {
        fclose(fp);
        GLF_LOGERROR("Unsupported pixel channel number");
        return false;
    }

    fprintf(fp, "%d %d\n", _width, _height);
    fprintf(fp, "255\n");

    fwrite(_data, _width * _height * _nchannels, 1, fp);

    return true;
}

GLF_NAMESPACE_END
