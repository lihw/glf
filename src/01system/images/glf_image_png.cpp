// -------------------------------------------------------------- 
// glf_image_png_private.cpp
// Read and write PNG image.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_image_png_private.h"

extern "C" {
#include <png.h>
}

#include "../glf_assert.h"
#include "../glf_log.h"

static void glfImagePNGReadFunction(png_structp png_ptr, png_bytep data, png_size_t length)
{
    FILE* fp = (FILE*)png_get_io_ptr(png_ptr);

    fread(data, length, 1, fp);
}

static void glfImagePNGWriteFunction(png_structp png_ptr, png_bytep data, png_size_t length)
{
    FILE* fp = (FILE*)png_get_io_ptr(png_ptr);

    GLuint bytesWritten = fwrite(data, 1, length, fp);
    GLF_ASSERT(bytesWritten == length);
}

static void glfImagePNGFlushFunction(png_structp png_ptr)
{
    /* This function has been intentionally left empty. */
}

static png_voidp glfImagePNGMallocFunction(png_structp png_ptr, png_size_t size)
{
    png_voidp allocatedBlock = new GLubyte [size];
    GLF_ASSERT(allocatedBlock != NULL);

    return allocatedBlock;
}

static void glfImagePNGFreeFunction(png_structp png_ptr, png_voidp data)
{
    GLubyte* p = (GLubyte*)data;
    delete [] p;
}

bool GLF_APIENTRY glfReadPNG(FILE* fp, GLuint& width, GLuint& height, GLuint& nchannels, GLubyte*& data)
{
    GLubyte** rowPointers = NULL;

    // LibPNG specific structures. 
    png_structp png_ptr = NULL;
    png_infop info_ptr  = NULL;
    png_infop end_ptr   = NULL;
    png_byte pbSig[8];
    png_uint_32 png_result;

    if (!fread((GLubyte*)pbSig, 8, 1, fp))
    {
        return false;
    }

    // Check that we're reading valid PNG file. 
    if (!png_sig_cmp(pbSig, 0, 8))
    {
        GLF_LOGERROR("not a valid PNG image (incorrect signature)");
        return false;
    }

    // Our png_ptr is used by the libpng library to maintain, basically, state
    // information about the PNG file as it is being read in by the library.
    // It's used for housekeeping by the libpng library.  The other two
    // pointers, info_ptr and end_ptr, are used to help us extract data from
    // the PNG file. 
    png_ptr = png_create_read_struct_2(
            PNG_LIBPNG_VER_STRING, 
            NULL, // error function pointer 
            NULL, // error printing function
            NULL, // warning function
            NULL, // memory pointer
            glfImagePNGMallocFunction, // memory allocation function pointer 
            glfImagePNGFreeFunction); // memory deallocation function pointer
    if (png_ptr == NULL)
    {
        GLF_LOGERROR("can't create PNG reading structure");
        return false;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) 
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
        delete [] data;
        delete [] rowPointers;
        return false;
    }

    // Create png info structure.
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        GLF_LOGERROR("can't create PNG info structure");
        return false;
    }

    end_ptr = png_create_info_struct(png_ptr);
    if (end_ptr == NULL) 
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        GLF_LOGERROR("can't create PNG info structure");
        return false;
    }

    png_set_read_fn(png_ptr, (png_voidp)fp, glfImagePNGReadFunction);
    png_set_sig_bytes(png_ptr, 8);

    // Read png info. 
    png_read_info(png_ptr, info_ptr);

    // Read image info (width, height, color format) and data. 
    GLint bitDepth;
    GLint colorType;
    GLuint channelCount;

    // Fetch image width, height, bit depth, color type. 
    png_result = png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)&width, 
            (png_uint_32*)&height, &bitDepth, &colorType, NULL, NULL, NULL);
    if (png_result == 0)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
        GLF_LOGERROR("PNG image has invalid header");
        return false;
    }

    // See how many channels this png contains. 
    channelCount = (GLuint)png_get_channels(png_ptr, info_ptr);

    // Non-paletted image. 
    if (colorType != PNG_COLOR_TYPE_PALETTE)
    {
        // Support only for 8 bit depth pixels. 
        if (bitDepth == 8)
        {
            nchannels = channelCount;
        }
        else
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
            GLF_LOGERROR("Unsupported PNG pixel format");
            return false;
        }
    }
    else
    {
        // Paletted image. 
        nchannels = 3;
    }

    if (bitDepth == 16)
    {
        png_set_strip_16(png_ptr);
        bitDepth = 8;
    }

    GLuint rowSize = width * channelCount * (GLuint)bitDepth / 8;

    // Create row pointers for libpng and unpack the data to new buffers, finally assigning to image. 
    data = new GLubyte[rowSize * height];
    GLF_ASSERT(data != NULL);
    if (data == NULL)
    {
        GLF_LOGERROR("new GLubyte [%d] failed", rowSize * height);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
        return false;
    }

    // Assign pointers for each row. 
    rowPointers = new GLubyte*[height];
    GLF_ASSERT(rowPointers != NULL);
    if (rowPointers == NULL)
    {
        delete [] data;
        GLF_LOGERROR("rowPointers = new GLubyte* [%d] failed", height);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
        return false;
    }

    GLubyte* datap = data;
    for (GLuint i = 0; i < height; ++i)
    {
        rowPointers[i] = datap;

        datap += rowSize;
    }

    // Read image data using libpng.
    png_read_image(png_ptr, (png_bytepp)rowPointers);
    png_read_end(png_ptr, NULL);

    delete [] rowPointers;

    /* Convert to RGB data in case of paletted png. */
    if (colorType == PNG_COLOR_TYPE_PALETTE)
    {
        png_colorp palette;
        GLint numPalette;

        png_result = png_get_PLTE(png_ptr, info_ptr, &palette, &numPalette);
        if(png_result == 0)
        {
            delete [] data;
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
            GLF_LOGERROR("invalid PNG palette");
            return false;
        }

        GLubyte* convertedImage = new GLubyte[width * height * 3];
        GLF_ASSERT(convertedImage != NULL);
        if (convertedImage == NULL)
        {
            delete [] data;
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
            GLF_LOGERROR("convertedImage = new GLubyte [%d] failed", height * width * 3);
            return false;
        }

        // 4-bit palette, assign 2 pixels at a time. 
        if (bitDepth == 4)
        {
            GLuint offset = 0;
            GLuint imageSize = width * channelCount * (GLuint)bitDepth * height / 8;

            // If both width and height are odd, no support for loading. 
            if ((width & 1) != 0 && (height & 1) != 0)
            {
                delete [] data;
                delete [] convertedImage;
                png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
                GLF_LOGERROR("4bit paletted png image with width and height having odd values not supported");
                return false;
            }

            for (GLuint i = 0; i < imageSize; ++i)
            {
                GLuint highByte = (GLuint)(data[i] >> 4);
                GLuint lowByte = (GLuint)(data[i] & 0x0F);
                convertedImage[offset++] = (GLubyte)palette[highByte].red;
                convertedImage[offset++] = (GLubyte)palette[highByte].green;
                convertedImage[offset++] = (GLubyte)palette[highByte].blue;
                convertedImage[offset++] = (GLubyte)palette[lowByte].red;
                convertedImage[offset++] = (GLubyte)palette[lowByte].green;
                convertedImage[offset++] = (GLubyte)palette[lowByte].blue;
            }
        }
        // 8 bit palette, assign colors by fetching from palette. 
        else if (bitDepth == 8)
        {
            GLuint offset = 0;
            GLuint imageSize = rowSize * height;
            for (GLuint i = 0; i < imageSize; i++)
            {
                GLubyte colorValue = data[i];
                convertedImage[offset++] = (GLubyte)palette[colorValue].red;
                convertedImage[offset++] = (GLubyte)palette[colorValue].green;
                convertedImage[offset++] = (GLubyte)palette[colorValue].blue;
            }
        }
        else
        {
            delete [] data;
            delete [] convertedImage;
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
            GLF_LOGERROR("Invalid paletted image format");
            return false;
        }

        delete [] data;
        data = convertedImage;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);

    return true;
}

bool GLF_APIENTRY glfWritePNG(FILE* fp, GLuint width, GLuint height, GLuint nchannels, const GLubyte* data)
{
    GLF_ASSERT(nchannels== 3 || nchannels == 4);
    if (nchannels != 3 && nchannels != 4)
    {
        GLF_LOGERROR("unsupported pixel format by PNG");
        return false;
    }

    /* Create the write struct for png. */
    png_structp png_ptr = png_create_write_struct_2(
            PNG_LIBPNG_VER_STRING, 
            NULL, 
            NULL, 
            NULL, 
            NULL, 
            glfImagePNGMallocFunction, 
            glfImagePNGFreeFunction);
    if (png_ptr == NULL)
    {
        GLF_LOGERROR("error in writing PNG image");
        return false;
    }

    // Create the info struct for png. Info struct contains info such as width,
    // height, bit depth and other attributes of image. 
    png_infop info_ptr = png_create_info_struct (png_ptr);
    if(info_ptr == NULL)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        GLF_LOGERROR("Error in writing PNG image");
        return false;
    }

    png_set_write_fn(png_ptr, (png_voidp)fp, glfImagePNGWriteFunction, glfImagePNGFlushFunction);

    // Set png to non-interlaced, non-compressed RGB mode. 
    png_set_IHDR(png_ptr, info_ptr, (png_uint_32)width, (png_uint_32)height, 8,
                 nchannels == 3 ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    // Write the image data. 
    GLuint rowSize = width * nchannels;

    GLubyte** rowPointers = new GLubyte*[height];
    GLF_ASSERT(rowPointers != NULL);
    if (rowPointers == NULL)
    {
        GLF_LOGERROR("rowPointers = new GLubyte* [%d] failed", height);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    GLubyte* datap = const_cast<GLubyte*>(data);

    for (GLuint i = 0; i < height; ++i)
    {
        rowPointers[i] = datap;
        datap += rowSize;
    }

    png_write_image(png_ptr, (png_bytepp)rowPointers);
    png_write_end(png_ptr, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    delete [] rowPointers;

    return true;
}

