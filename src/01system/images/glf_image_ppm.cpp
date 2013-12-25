// -------------------------------------------------------------- 
// glf_image_ppm_private.cpp
// Read and write PPM image.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_image_ppm_private.h"

#include "../glf_assert.h"
#include "../glf_log.h"

bool GLF_APIENTRY glfReadPPM(FILE* fp, GLuint& width, GLuint& height, GLuint& nchannels, GLubyte*& data)
{
    int c,s;

    // Read the first line.
    char line[1024];
    if (!fgets(line, 1024, fp))
    {
        GLF_LOGERROR("Failed to read 1st line");
        return false;
    }

    if (line[0] == 'P' && line[1] == '5')
    {
        nchannels = 1;
    }
    else if (line[0] == 'P' && line[1] == '6')
    {
        nchannels = 3;
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
    n = fscanf(fp, "%d", &width);
    if (n < 1) return false;
    n = fscanf(fp, "%d", &height);
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
    GLuint num = width * height * nchannels;
    if (data) 
    {
        delete [] data;
    }

    data = new GLubyte [num];
    fread(data, 1, num, fp);

    return true;
}

bool GLF_APIENTRY glfWritePPM(FILE* fp, GLuint width, GLuint height, GLuint nchannels, const GLubyte* data)
{
    if (nchannels == 1)
    {
        fprintf(fp, "P5\n");
    }
    else if (nchannels == 3)
    {
        fprintf(fp, "P6\n");
    }
    else
    {
        GLF_LOGERROR("Unsupported pixel channel number");
        return false;
    }

    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");

    fwrite(data, width * height * nchannels, 1, fp);

    return true;
}


