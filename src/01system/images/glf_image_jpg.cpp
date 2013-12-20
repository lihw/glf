// -------------------------------------------------------------- 
// glf_image_jpg_private.cpp
// Read and write JPEG image.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#include "glf_image_jpg_private.h"

#include <stdio.h>
#include <setjmp.h>

extern "C" {
#include <jpeglib.h>
}

#include "../glf_assert.h"
#include "../glf_log.h"

// JPG loading and storing utils 
struct GLFJPGDecodeStruct
{
    jpeg_source_mgr _smgr;          ///< LibJPG source manager
    FILE*           _inputFp;    ///< The input stream.
    JOCTET*         _buffer;                ///< Buffer containing decoded data.
};

// Helper structure used for JPG compressing. 
struct GLFJPGEncodeStruct
{
    jpeg_destination_mgr _dmgr;      ///< LibJPG destination manager. 
    FILE*                _outputFp;   ///< The output stream. 
    JOCTET*              _buffer;                 ///< Buffer containing compressed data. 
};

// JPG error management struct; custom extension stores a setjmp context for returning from exception 
struct GLFJPGErrorManager 
{
    jpeg_error_mgr _pub;    ///< "public" fields 
    jmp_buf _setjmp_buffer; ///< for "try-catch" error handling in C.
};

// Maximum buffer size for JPG compress buffers. 
static const GLuint GLF_IMAGE_JPG_MAXIMUM_BUFFER_SIZE = 256;

// JPGlib decode callback function. 
static void glfImageJPGDecodeInitFunction(j_decompress_ptr cinfo)
{
    GLFJPGDecodeStruct* decodeStruct = (GLFJPGDecodeStruct*)cinfo->src;

    decodeStruct->_buffer = (JOCTET*)(*cinfo->mem->alloc_small)((j_common_ptr)(void*)cinfo, 
        JPOOL_IMAGE, GLF_IMAGE_JPG_MAXIMUM_BUFFER_SIZE * sizeof(JOCTET));
    decodeStruct->_smgr.bytes_in_buffer = 0;
}

// JPGlib call back function for filling input buffer while decoding. Lint error for constant parameter suppressed. 
static boolean glfImageJPGDecodeFillInputBufferFunction(j_decompress_ptr cinfo)
{
    GLuint bytesRead;
    GLFJPGDecodeStruct* decodeStruct = (GLFJPGDecodeStruct*)cinfo->src;
    
    bytesRead = fread((GLubyte*)decodeStruct->_buffer, GLF_IMAGE_JPG_MAXIMUM_BUFFER_SIZE, 1, decodeStruct->_inputFp);
    decodeStruct->_smgr.next_input_byte = decodeStruct->_buffer;
    decodeStruct->_smgr.bytes_in_buffer = bytesRead;

    return TRUE;
}

// Internal callback function for skipping input data while decoding JPG data. Suppress the usage of long and const. 
static void glfImageJPGDecodeSkipInputData(j_decompress_ptr cinfo, long num_bytes)
{
    GLFJPGDecodeStruct* decodeStruct = (GLFJPGDecodeStruct*)cinfo->src;

    if (num_bytes > 0)
    {
        /* Skip buffer until done, avoiding that bytes in buffer never gets below zero. */
        while (num_bytes > (long)decodeStruct->_smgr.bytes_in_buffer)
        {
            num_bytes -= (long)decodeStruct->_smgr.bytes_in_buffer;
            glfImageJPGDecodeFillInputBufferFunction(cinfo); /*lint !e534 Return value ignored. */
        }
    }
    decodeStruct->_smgr.next_input_byte += (size_t)num_bytes;
    decodeStruct->_smgr.bytes_in_buffer -= (size_t)num_bytes;
}

// JPGlib call back function for terminating source while decoding. 
static void glfImageJPGDecodeTerminateSource(j_decompress_ptr cinfo)
{
}

// JPGlib callback function for initializing destination while compressing. 
static void glfImageJPGEncodeInitDestination(j_compress_ptr cinfo)
{
    GLFJPGEncodeStruct* encodeStruct = (GLFJPGEncodeStruct*)cinfo->dest;

    encodeStruct->_buffer = (JOCTET*)(*cinfo->mem->alloc_small)((j_common_ptr)(void*)cinfo, 
        JPOOL_IMAGE, GLF_IMAGE_JPG_MAXIMUM_BUFFER_SIZE * sizeof(JOCTET));
    encodeStruct->_dmgr.next_output_byte = (JOCTET*)encodeStruct->_buffer;
    encodeStruct->_dmgr.free_in_buffer = GLF_IMAGE_JPG_MAXIMUM_BUFFER_SIZE;
}

// JPGlib callback function for emptying buffer while compressing. Constant function parameters suppressed from lint. 
static boolean glfImageJPGEncodeEmptyBuffer(j_compress_ptr cinfo)
{
    GLFJPGEncodeStruct* encodeStruct = (GLFJPGEncodeStruct*)cinfo->dest;
    GLuint bytesWritten = fwrite((GLubyte*)encodeStruct->_buffer, GLF_IMAGE_JPG_MAXIMUM_BUFFER_SIZE, 1, encodeStruct->_outputFp);

    encodeStruct->_dmgr.next_output_byte = encodeStruct->_buffer;
    encodeStruct->_dmgr.free_in_buffer = GLF_IMAGE_JPG_MAXIMUM_BUFFER_SIZE;

    return TRUE;
}

// JPGlib callback function for terminating destination buffer while compressing. Constant function parameters suppressed from lint. 
static void glfImageJPGEncodeTermDestination(j_compress_ptr cinfo)
{
    GLFJPGEncodeStruct* encodeStruct = (GLFJPGEncodeStruct*)cinfo->dest;
    GLuint size = GLF_IMAGE_JPG_MAXIMUM_BUFFER_SIZE - encodeStruct->_dmgr.free_in_buffer;

    /* If there are remaining bytes to be written, write them. */
    if (size > 0)
    {
        GLuint bytesWritten = fwrite((GLubyte*)encodeStruct->_buffer, 
            size, 1, encodeStruct->_outputFp);
        GLF_ASSERT(bytesWritten == size);
    }
}

// Replacement for the standard output_message method 
static void glfImageJPGErrorMessage(j_common_ptr cinfo)
{
    char buffer[JMSG_LENGTH_MAX];

    /* Show a clear-text error message */
    (*cinfo->err->format_message) (cinfo, buffer);  
    GLF_LOGWARNING(buffer);
}


// Kiwi replacement for the standard error_exit method 
static void glfImageJPGErrorHandler(j_common_ptr cinfo)
{
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    GLFJPGErrorManager *myerr = (GLFJPGErrorManager*)cinfo->err;

    // Display the message. 
    // TODO: instead of / in addition to logging, pass the message over the 
    //          longjmp so that it can be thrown with the kzsErrorThrow. 
    // (*cinfo->err->output_message) (cinfo); 
    glfImageJPGErrorMessage(cinfo);

    // Return control to the setjmp point.
    longjmp(myerr->_setjmp_buffer, 1);
}

bool GLF_APIENTRY glfReadJPG(FILE* fp, GLuint& width, GLuint& height, GLuint& nchannels, GLubyte*& data)
{
    GLFJPGErrorManager jerr;

    GLFJPGDecodeStruct* decodeStruct = NULL;
    GLubyte** rowPointers            = NULL;
    data                             = NULL;

    // We set up the normal JPG error routines, then override error_exit. 
    jpeg_decompress_struct cinfo;
    cinfo.err                = jpeg_std_error(&jerr._pub);
    jerr._pub.error_exit     = glfImageJPGErrorHandler;
    jerr._pub.output_message = glfImageJPGErrorMessage;
    
    // Establish the setjmp return context; "catch block" for "exceptions" during decompression.
    if (setjmp(jerr._setjmp_buffer))
    {
        // If we get here, the JPG code has signaled an error. 
        // Clean up the JPG object, close the input file, and relay the error forward. 
        // TODO: use error message from JPG 
        delete [] rowPointers;
        delete [] data;
        delete decodeStruct;
        jpeg_destroy_decompress(&cinfo);        
        GLF_LOGERROR("error while loading a JPG image");
        return false;
    }

    // Create decompression info struct.
    jpeg_create_decompress(&cinfo);
    
    // Create and initialize the decode structure with function pointers and pass the input stream. */
    decodeStruct = new GLFJPGDecodeStruct;
    
    decodeStruct->_inputFp                  = fp;
    decodeStruct->_smgr.init_source         = glfImageJPGDecodeInitFunction;
    decodeStruct->_smgr.fill_input_buffer   = glfImageJPGDecodeFillInputBufferFunction;
    decodeStruct->_smgr.skip_input_data     = glfImageJPGDecodeSkipInputData;
    decodeStruct->_smgr.resync_to_restart   = jpeg_resync_to_restart;
    decodeStruct->_smgr.term_source         = glfImageJPGDecodeTerminateSource;

    // Assign our decode struct for source manager of decompress info. 
    cinfo.src = (jpeg_source_mgr*)decodeStruct;

    // Read the header, may "throw exceptions" 
    GLuint jpegError = (GLuint)jpeg_read_header(&cinfo, TRUE);
    if (jpegError != JPEG_HEADER_OK)
    {
        jpeg_destroy_decompress(&cinfo);        
        delete decodeStruct;
        GLF_LOGERROR("invalid JPG header");
        return false;
    }
    if (cinfo.out_color_space != JCS_RGB && cinfo.out_color_space != JCS_GRAYSCALE)
    {
        jpeg_destroy_decompress(&cinfo);        
        delete decodeStruct;
        GLF_LOGERROR("Unsupported JPG input format");
        return false;
    }

    GLuint bytesPerPixel = cinfo.out_color_space == JCS_RGB ? 3 : 1;

    // Assign image characteristics. 
    width     = (GLuint)cinfo.image_width;
    height    = (GLuint)cinfo.image_height;
    nchannels = bytesPerPixel;

    // Calculate row size, since RGB is assumed using value of 3. 
    GLuint rowSize = width * bytesPerPixel;

    data = new GLubyte [rowSize * height];
    GLF_ASSERT(data != NULL);
    if (data == NULL)
    {
        delete decodeStruct;
        jpeg_destroy_decompress(&cinfo);        
        GLF_LOGERROR("new GLubyte [%d] failed", rowSize * height);
        return false;
    }

    // Assign pointers for each row. 
    rowPointers = new GLubyte* [height];
    GLF_ASSERT(rowPointers != NULL);
    if (rowPointers == NULL)
    {
        delete [] data;
        delete [] decodeStruct;
        jpeg_destroy_decompress(&cinfo);        
        GLF_LOGERROR("rowPointers = new GLubyte* [%d] failed", height);
        return false;
    }

    // Assign pointers for each row. 
    GLubyte* datap = data;

    for (GLuint i = 0; i < height; ++i)
    {
        rowPointers[i] = datap;
        datap += rowSize;
    }

    // Prepare jpeg decompression. 
    if (!jpeg_start_decompress(&cinfo))
    {
        delete [] rowPointers;
        delete [] data;
        delete decodeStruct;
        jpeg_destroy_decompress(&cinfo);        
        GLF_LOGERROR("Error in decompressing JPG image", height);
        return false;
    }

    // Read scan lines, data assigned via row pointers. 
    while (cinfo.output_scanline < cinfo.output_height)
    {
        GLint rowsRead = (GLint)jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&rowPointers[cinfo.output_scanline], 1);
        GLF_ASSERT(rowsRead > 0);
        if (rowsRead <= 0)
        {
            delete [] rowPointers;
            delete [] data;
            delete decodeStruct;
            jpeg_destroy_decompress(&cinfo);        
            GLF_LOGERROR("No rows were able to be read from JPG image");
            return false;
        }
    }

    delete [] rowPointers;

    // Finish decompressing. 
    if (!jpeg_finish_decompress(&cinfo))
    {
        delete [] data;
        delete decodeStruct;
        jpeg_destroy_decompress(&cinfo);        
        GLF_LOGERROR("JPG decompression error in finishing");
        return false;
    }

    // Free source manager helper structure. 
    delete decodeStruct;

    // Destroy decompress structure. 
    jpeg_destroy_decompress(&cinfo);

    return true;
}

bool GLF_APIENTRY glfWriteJPG(FILE* fp, GLuint width, GLuint height, 
                     GLuint quality, GLuint nchannels, const GLubyte* data) 
{
    GLF_ASSERT(nchannels == 3);
    GLF_ASSERT(quality <= 100);

    if (quality > 100)
    {
        quality = 100;
    }

    if (nchannels != 3)
    {
        GLF_LOGERROR("unsupported JPG format");
        return false;
    } 

    jpeg_error_mgr jerr;

    // TODO: do not use default jpeg error handler since it automatically calls exit() internally upon failure 
    // Set the error manager. 
    jpeg_compress_struct cinfo;
    cinfo.err = jpeg_std_error(&jerr);
    // Create compression structure. 
    jpeg_create_compress(&cinfo);

    // Allocate memory for destination manager. 
    GLFJPGEncodeStruct* encodeStruct = new GLFJPGEncodeStruct;

    // Set helper struct for compression structure. 
    cinfo.dest = (jpeg_destination_mgr*)encodeStruct;

    // Setup write parameters for image. 
    encodeStruct->_outputFp                 = fp;
    encodeStruct->_dmgr.init_destination    = glfImageJPGEncodeInitDestination;
    encodeStruct->_dmgr.empty_output_buffer = glfImageJPGEncodeEmptyBuffer;
    encodeStruct->_dmgr.term_destination    = glfImageJPGEncodeTermDestination;
    
    // Prepare for compression. Ignore alpha channel.
    cinfo.image_width      = (JDIMENSION)width;
    cinfo.image_height     = (JDIMENSION)height;
    cinfo.input_components = 3;
    cinfo.in_color_space   = JCS_RGB;

    // Set JPEG default after the color space has been defined. 
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, (GLint)quality, TRUE);
    jpeg_start_compress(&cinfo, 1);
    
    // Write the JPEG scan lines, one by one. 
    JSAMPROW sampleRow[1];
    JDIMENSION rowsWritten;

    while(cinfo.next_scanline < cinfo.image_height)
    {
        GLubyte* imageRow;
        imageRow = (GLubyte*)data + width * 3 * (GLuint)cinfo.next_scanline;

        sampleRow[0] = (JSAMPROW)&imageRow[0];
        rowsWritten = jpeg_write_scanlines(&cinfo, (JSAMPARRAY)sampleRow, 1);
        if (rowsWritten != 1)
        {
            delete encodeStruct;
            jpeg_destroy_compress(&cinfo);
            GLF_LOGERROR("invalid amount of rows written when outputting JPG image");
            return false;
        }
    }

    // Finish compression. 
    jpeg_finish_compress(&cinfo);
    // Destroy compression structure. 
    jpeg_destroy_compress(&cinfo);

    // Release helper structure from memory. 
    delete encodeStruct;

    return true;
}


