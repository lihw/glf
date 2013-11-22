// -------------------------------------------------------------- 
// main.cpp
// convert freetype font into a distance map.
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include <stdio.h>
#include <stdint.h>

#include <algorithm>
#include <exception>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

const static uint32_t fontHeight = 11;

static bool saveBitmap(const char* filename, const uint8_t* image, uint32_t width, uint32_t height)
{
    FILE* fp;
    fopen_s(&fp, filename, "wb");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open %s\n", filename);
        return false;
    }

    fprintf(fp, "P5\n");
   
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");

    fwrite(image, width * height, 1, fp);

    return true;
}

struct FontBitmap
{
    int32_t width;
    int32_t height;
    int32_t offsetx;
    int32_t offsety;
    uint8_t* data;
};

static void convertFT2Bitmap(FT_Face face, char c, FontBitmap& fontBitmap)
{
    if (FT_Load_Glyph(face, FT_Get_Char_Index(face, c), FT_LOAD_DEFAULT))
    {
        fprintf(stderr, "FT_Load_Glyph failed\n");
        return ;
    }

    // Move The Face's Glyph Into A Glyph Object.
    FT_Glyph glyph;
    if (FT_Get_Glyph(face->glyph, &glyph))
    {
        fprintf(stderr, "FT_Get_Glyph failed\n");
        return ;
    }

    // Convert The Glyph To A Bitmap.
    FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
    FT_BitmapGlyph glyphBitmap = (FT_BitmapGlyph)glyph;

    FT_Bitmap& bitmap = glyphBitmap->bitmap;

    fontBitmap.width = bitmap.width;
    fontBitmap.height = bitmap.rows;
    fontBitmap.offsetx = glyphBitmap->left;
    fontBitmap.offsety = glyphBitmap->top - bitmap.rows;

    fprintf(stdout, "%d\n", c);
    fontBitmap.data = new uint8_t [bitmap.width * bitmap.rows];
    memcpy(fontBitmap.data, bitmap.buffer, bitmap.width * bitmap.rows);

    FT_Done_Glyph(glyph);
}

static void createFontFile(FT_Face face, const char* filename)
{
    FILE* fp = NULL;
    fopen_s(&fp, filename, "wb");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open %s\n", filename);
        return ;
    }
        
    FontBitmap fontBitmaps[95];

    for (char c = 33; c != 127; ++c)
    {
        convertFT2Bitmap(face, c, fontBitmaps[c - 33]);
    }

    fprintf(fp, "static const GLint FONT_HEIGHT = %d;\n", fontHeight);

    // Write the look up table
    fprintf(fp, "static const GLint FONT_LOOKUP_TABLE[] = \n");
    fprintf(fp, "{ \n");
    uint32_t offset = 0;
    for (char c = 33; c != 127; ++c)
    {
        const FontBitmap& fb = fontBitmaps[c - 33];
        fprintf(fp, "   %d, %d, %d, %d, %d\n", 
                fb.width,
                fb.height,
                fb.offsetx,
                fb.offsety,
                offset);

        offset += fb.width * fb.height;
    }
    fprintf(fp, "}; \n");

    // Write the font data
    fprintf(fp, "static const GLubyte FONT_DATA[] = \n");
    fprintf(fp, "{ \n    ");

    uint32_t count = 0;
    for (char c = 33; c != 127; ++c)
    {
        const FontBitmap& fb = fontBitmaps[c - 33];
        for (int32_t i = 0; i < fb.height; ++i)
        {
            for (int32_t j = 0; j < fb.width; ++j)
            {
                fprintf(fp, "0x%x, ", fb.data[i * fb.width + j]);

                // new line
                count++;
                if (count == 20)
                {
                    fprintf(fp, "\n    ");
                    count = 0;
                }
            }
        }

        delete [] fb.data;
    }
    fprintf(fp, "\n}; \n");
}

int main(int argc, const char* argv[])
{
    // Create And Initilize A FreeType Font Library.
    FT_Library library;
    if (FT_Init_FreeType(&library))
    {
        fprintf(stderr, "FT_Init_FreeType failed");
        return EXIT_FAILURE;
    }
 
    // The Object In Which FreeType Holds Information On A Given
    // Font Is Called A "face".
    FT_Face face;
 
    // This Is Where We Load In The Font Information From The File.
    // Of All The Places Where The Code Might Die, This Is The Most Likely,
    // As FT_New_Face Will Fail If The Font File Does Not Exist Or Is Somehow Broken.
    if (FT_New_Face(library, "arial.TTF", 0, &face))
    {
        fprintf(stderr, "FT_New_Face failed (there is probably a problem with your font file)");
        return EXIT_FAILURE;
    }
 
    // For Some Twisted Reason, FreeType Measures Font Size
    // In Terms Of 1/64ths Of Pixels.  Thus, To Make A Font
    // h Pixels High, We Need To Request A Size Of h*64.
    // (h << 6 Is Just A Prettier Way Of Writing h*64)
    FT_Set_Char_Size(face, fontHeight << 6, fontHeight << 6, 96, 96);
 
    createFontFile(face, "font.cpp");
 
    // We Don't Need The Face Information Now That The Display
    // Lists Have Been Created, So We Free The Assosiated Resources.
    FT_Done_Face(face);
 
    // Ditto For The Font Library.
    FT_Done_FreeType(library);
    
    return 0;
}
