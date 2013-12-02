// -------------------------------------------------------------- 
// glf_sdf.cpp
// The signed distance field computation
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_sdf.h"

#include <algorithm>

GLF_NAMESPACE_BEGIN

GLubyte* createSDFBruteForce(const Bitmap& bitmap, GLuint outputWidth, GLuint outputHeight)
{
    GLubyte* ret = new GLubyte [outputWidth * outputHeight];

    GLfloat minDistance = FLT_MAX;
    GLfloat maxDistance = 0;
    GLfloat* tempSDF = new GLfloat [outputWidth * outputHeight];

    for (GLuint i = 0; i < outputHeight; ++i)
    {                         
        for (GLuint j = 0; j < outputWidth; ++j)
        {
            // The corresponding pixel in the source image.
            GLint y = GLint(i * height / outputHeight);
            GLint x = GLint(j * width / outputWidth);

            // For each pixel in the image, we compute its distance to the nearest
            // pixel of opposite state.
            GLint x1 = std::max(x - SPREAD, 0);
            GLint x2 = std::min(x + SPREAD, GLint(width - 1));
            GLint y1 = std::max(y - SPREAD, 0);
            GLint y2 = std::min(y + SPREAD, GLint(height - 1));

            GLint minD = INT_MAX;
            for (GLint yy = y1; yy <= y2; ++yy)
            {
                for (GLint xx = x1; xx <= x2; ++xx)
                {
                    if (image.get(xx, yy)[0] != image.get(x, y)[0])
                    {
                        GLint dy = yy - y;
                        GLint dx = xx - x;
                        GLint d2 = dy * dy + dx * dx;
                        if (d2 < minD)
                        {
                            minD = d2;
                        }
                    }
                }
            }

            if (minD == INT_MAX)
            {
                minD = SPREAD * SPREAD * 2;
            }

            GLfloat sign = 0;
            if (image.get(x, y) > 0)
            {
                // positive distance
                sign = 1.0f;
            }
            else
            {
                // negative
                sign = -1.0f;
            }

            GLfloat d = sign * sqrtf(GLfloat(minD));
            tempSDF[i * outputWidth + j] = d;
            maxDistance = std::max(maxDistance, d);
            minDistance = std::min(minDistance, d);
        }
    }

    for (GLint i = 0; i < outputHeight; ++i)
    {
        for (GLint j = 0; j < outputWidth; ++j)
        {
            GLfloat d = tempSDF[i * outputWidth + j];
            d = 255.0f * (d - minDistance) / (maxDistance - minDistance);
            ret[i * outputWidth + j] = GLubyte(d);
        }
    }

    delete [] tempSDF;

    return ret;
}

GLubyte* createSDF8SSEDT(const Bitmap& bitmap, GLuint outputWidth, GLuint outputHeight)
{
    struct Point
    {
        GLuint dx;
        GLuint dy;

        GLint distance() { return dx * dx + dy * dy; };
    };

    Point* pos = new Point [bitmap.getWidth() * bitmap.getHeight()];
    Point* neg = new Point [bitmap.getWidth() * bitmap.getHeight()];

    // Initialize the grid
    for (GLuint i = 0; i < bitmap.getHeight(); i++)
    {
        for (GLuint j = 0; j < bitmap.getWidth(); j++)
        {
            GLuint index = i * bitmap.getWidth() + j;

            if (bitmap.get(j, i)[0] > 0)
            {
                pos[index].dx = 0;
                pos[index].dy = 0;
                neg[index].dx = INT_MAX;
                neg[index].dy = INT_MAX;
            }
            else
            {
                pos[index].dx = INT_MAX;
                pos[index].dy = INT_MAX;
                neg[index].dx = 0;
                neg[index].dy = 0;
            }
        }
    }

    GLfloat minDistance = FLT_MAX;
    GLfloat maxDistance = - FLT_MAX;

    // Generate the SDF in the same size of input image.
#define COMPARE(img, p, x, y, offsetx, offsety) \
    {\
        Point o = img[image.getWidth() * (y + offsetx) + (x + offsety)]; \
        o.dx += offsetx; \
        o.dy += offsety; \
        if (o.distance() < p.distance()) \
        { \
            p = o; \
        } \
    }

    // Pass 0
    for (GLint i = 1; i < bitmap.getHeight() - 1; ++i)
    {
        for (GLint j = 1; j < bitmap.getWidth() - 1; ++j)
        {
            GLint index = i * bitmap.getWidth() + j;

            Point p;
            
            p = pos[index];
            COMPARE(pos, p, j, i, -1, 0);
            COMPARE(pos, p, j, i,  0, -1);
            COMPARE(pos, p, j, i, -1, -1);
            COMPARE(pos, p, j, i,  1, -1);
            pos[index] = p;

            p = neg[index];
            COMPARE(neg, p, j, i, -1, 0);
            COMPARE(neg, p, j, i,  0, -1);
            COMPARE(neg, p, j, i, -1, -1);
            COMPARE(neg, p, j, i,  1, -1);
            pos[index] = p;
        }
        
        for (GLint j = bitmap.getWidth() - 2; j >= 1; --j)
        {
            GLint index = i * bitmap.getWidth() + j;

            Point p;

            p = pos[index];
            COMPARE(pos, p, j, i, 1, 0);
            pos[index] = p;
            
            p = neg[index];
            COMPARE(neg, p, j, i, 1, 0);
            neg[index] = p;
        }
    }
    
    // Pass 1
    for (GLint i = bitmap.getHeight() - 1; i >= 1; --i)
    {
        for (GLint j = bitmap.getWidth() - 1; j >= 1; --j)
        {
            GLint index = i * bitmap.getWidth() + j;

            Point p;
            
            p = pos[index];
            COMPARE(pos, p, j, i,  1, 0);
            COMPARE(pos, p, j, i,  0,  1);
            COMPARE(pos, p, j, i, -1,  1);
            COMPARE(pos, p, j, i,  1,  1);
            pos[index] = p;

            p = neg[index];
            COMPARE(neg, p, j, i,  1, 0);
            COMPARE(neg, p, j, i,  0,  1);
            COMPARE(neg, p, j, i, -1,  1);
            COMPARE(neg, p, j, i,  1,  1);
            pos[index] = p;
        }
        
        for (GLint j = 1; j < bitmap.getWidth() - 1; ++j)
        {
            GLint index = i * bitmap.getWidth() + j;

            Point p;

            p = pos[index];
            COMPARE(pos, p, j, i, -1, 0);
            pos[index] = p;
            
            p = neg[index];
            COMPARE(neg, p, j, i, -1, 0);
            neg[index] = p;

            d[index] = sqrtf(GLfloat(pos[index].distance())) - 
                sqrtf(GLfloat(neg[index].distance()));
            minDistance = std::min(d[index], minDistance);
            maxDistance = std::max(d[index], maxDistance);
        }
    }

#undef COMPARE

    delete [] pos;
    delete [] neg;

    GLubyte* ret = new GLubyte [outputWidth * outputHeight];
    // Downsize the distance field to the output size.
    for (GLint i = 0; i < outputHeight; ++i)
    {
        for (GLint j = 0; j < outputWidth; ++j)
        {
            GLint y = i * image.getHeight() / outputHeight;
            GLint x = j * image.getWidth() / outputWidth;

            GLint y1 = std::max(y - 1, 0);
            GLint y2 = std::min(y + 1, image.getHeight());
            GLint x1 = std::max(x - 1, 0);
            GLint x2 = std::min(x + 1, image.getWidth());
            
            GLfloat sum = 0;
            GLsizei count = 0;
            for (GLint xx = x1; xx < x2; ++xx)
            {
                for (GLint yy = y1; yy < y2; ++yy)
                {
                    GLint index = yy * image.getWidth() + xx;
                    GLfloat s = (d[index] - minDistance) / (maxDistance - minDistance);
                    sum += s;
                    count++;
                }
            }
            
            GLint index = y * outputWidth + x;
            ret[index] = uint8_t((sum / (GLfloat)(count)) 255.0f)
        }
    }

    delete [] d;

    return ret;
}

GLF_NAMESPACE_END
