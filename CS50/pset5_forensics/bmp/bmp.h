/****************************************************************************
 * bmp.h
 *
 * Computer Science 50
 * Problem Set 5
 *
 * BMP-related data types based on Microsoft's own.
 ***************************************************************************/

#include <stdint.h>


/* 
 * Common Data Types 
 *
 * The data types in this section are essentially aliases for C/C++ 
 * primitive data types.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/cc230309(PROT.10).aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */

typedef uint8_t  BYTE;  // 8-bit unsigned, i.e, 1 byte
typedef uint32_t DWORD; // 32-bit unsigned, i.e., 4 bytes
typedef int32_t  LONG;  // 32-bit signed, i.e., 4 bytes
typedef uint16_t WORD;  // 16-bit unsigned, i.e., 2 bytes


/*
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183374(VS.85).aspx.
 */

typedef struct 
{ 
    WORD   bfType;              // The file type; must be BM.
    DWORD  bfSize;              // The size, in bytes, of the bitmap file
    WORD   bfReserved1; 
    WORD   bfReserved2; 
    DWORD  bfOffBits; 
} __attribute__((__packed__)) 
BITMAPFILEHEADER; 


/*
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the 
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183376(VS.85).aspx.
 */
       
typedef struct
{
    DWORD  biSize;              // The number of bytes required by the structure.
    LONG   biWidth;             // The width of the bitmap, in pixels. 
    LONG   biHeight;            // The height of the bitmap, in pixels.
    WORD   biPlanes; 
    WORD   biBitCount; 
    DWORD  biCompression; 
    DWORD  biSizeImage;         // The size, in bytes, of the image. This may be set to zero for BI_RGB bitmaps.
    LONG   biXPelsPerMeter; 
    LONG   biYPelsPerMeter; 
    DWORD  biClrUsed; 
    DWORD  biClrImportant; 
} __attribute__((__packed__))
BITMAPINFOHEADER; 


/* 
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/aa922590.aspx.
 */

typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;
