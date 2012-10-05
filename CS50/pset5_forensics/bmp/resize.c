/****************************************************************************
 * resize.c
 *
 * Computer Science 50
 * Problem Set 5
 * Daniel E. Williams
 * danieleugenewilliams@gmail.com
 *
 * Resizes image files.
 * Created:  September 2, 2012
 * Modified: September 11, 2012
 *
 * Notes: new, resized image means new header info!
 *      : file size, image size, width, and height must change
 *      : need to change both structs
 *
 * TODO: update bi.biWidth: "The width of the bitmap, in pixels."
 * TODO: update bi.biHeight: "The height of the bitmap, in pixels."
 * TODO: update bi.biSizeImage: "The size, in bytes, of the image. This may be set to zero for BI_RGB bitmaps."
 * TODO: write each pixel [n] times
 * TODO: store rgb in array...malloc
 * TODO: write each line [n] times 
 ***************************************************************************/
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

// Protoypes

void update_biWidth(LONG resize_multiple);
void update_biHeight(LONG resize_multiple);
void update_biSizeImage(LONG resize_multiple);
void resize_image(LONG resize_multiple);

// infile's BITMAPFILEHEADER
BITMAPFILEHEADER bf;

// infile's BITMAPINFOHEADER
BITMAPINFOHEADER bi;

int
main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: resize [int] infile outfile\n");
        return 1;
    }
    
    // resize multiple
    LONG resize_multiple = atoi(argv[1]);

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    //BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    //BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // BEGIN UPDATE OF HEADER DATA
    
    // update bi.biWidth
    update_biWidth(resize_multiple);
    
    // update bi.biHeight
    update_biHeight(resize_multiple);
    
    // update bi.SizeImage
    update_biSizeImage(resize_multiple);

    // END UPDATE OF HEADER DATA
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            //fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            
            // write RGB triple to outfile
            // write pixel [resize_multiple] times across
            for (int i = 0; i <= resize_multiple; i++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
            fputc(0x00, outptr);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}


// update bi.biWidth
void
update_biWidth(LONG resize_multiple)
{
    // multiply width by resize multiple
    bi.biWidth *= resize_multiple;
}


// update bi.biHeight
void
update_biHeight(LONG resize_multiple)
{
    // multiply height by resize multiple
    bi.biHeight *= resize_multiple;
}


// update bi.biSizeImage
void
update_biSizeImage(LONG resize_multiple)
{
    // multiply height by resize multiple
    bi.biSizeImage *= resize_multiple;
}

