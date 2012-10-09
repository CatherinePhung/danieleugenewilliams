/****************************************************************************
 * resize.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Resizes a BMP piece by piece by [n], just because.
 ***************************************************************************/
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"


int
main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: resize [n] infile.bmp outfile.bmp\n");
        return 1;
    }
    
    // resize multiple
    int resize_multiple = atoi(argv[1]);

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
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
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
    
    // START UPDATE HEADER INFO
    
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (bi.biSizeImage * resize_multiple);
    
    int biHeight     = bi.biHeight;
    int biWidth      = bi.biWidth;
    
    bi.biHeight     *= resize_multiple;
    bi.biWidth      *= resize_multiple;
    bi.biSizeImage  *= resize_multiple;
    
    // END UPDATE HEADER INFO

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // determine padding for outfile
    int outpadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, absbiHeight = abs(biHeight); i < absbiHeight; i++)
    {
        fpos_t position;
        fgetpos (inptr, &position);
        //write scanline [resize_multiple] times
        for(int m = 0; m < resize_multiple; m++) 
        {
            // fseek back to beginning of scanline if m < resize_multiple
            if(m < resize_multiple)
                fsetpos (inptr, &position);
            
            // iterate over pixels in scanline
            for (int j = 0; j < biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                //write RGB [resize_multiple] times
                for(int n = 0; n < resize_multiple; n++) 
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
                
            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < outpadding; k++)
                fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
