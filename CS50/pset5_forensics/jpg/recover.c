/****************************************************************************
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 *
 * TODO: open card.raw
 * TODO: determine start of new image
 *       0xff 0xd8 0xff 0xe0
 *       0xff 0xd8 0xff 0xe1
 * TODO: determine filename
 * TODO: write all bytes of image to the same file
 ***************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../bmp/bmp.h"

#define RAWCARD "card.raw"

#define RAWSIZE 2421760

int
main(int argc, char *argv[])
{
    // remember raw file
    char *infile = RAWCARD;
    
    // open raw file 
    FILE *inptr = fopen(infile, "rb");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }
    
    int index = 0;
    int jpgcount = 0;
    
    // jpg filename
    char *outfile[10];
    
    // output file handle
    FILE *outptr = NULL;
    
    BYTE chunk[RAWSIZE];
    while(!feof(inptr))
    {
        int result = fread(chunk, sizeof(BYTE), 512, inptr);
        if(result < 0)
            printf("an error occurred\n");
            
        for(int i = index, n = sizeof(chunk); i < n; i++)
        {
            //printf("%x\n", chunk[i]);
            if(((chunk[i+3] == 0xe0) || (chunk[i+3] == 0xe1)) && (chunk[i+2] == 0xff) && (chunk[i+1] == 0xd8) && (chunk[i] == 0xff))
            {
                // increment jpg count
                jpgcount++;
                
                // print msg to console...found jpg
                printf("%03d: %s\n",jpgcount, "Found jpg");
                
                // set jpg filename
                sprintf(*outfile,"%03d.jpg", jpgcount);
                
                // close outfile file, if open
                if(outptr != NULL)
                {
                    printf ("%s %s", *outfile, "file closed succesfully!\n");
                    fclose(outptr);
                }
                
                // open new jpg file
                outptr = fopen(*outfile, "wb");
                if (outptr == NULL)
                {
                    printf("Could not open %s.\n", *outfile);
                    return 2;
                }
            }
        }
        
        // if jpg found, write 512 bytes to current file
        if(outptr != NULL)
        {
            fwrite(chunk, sizeof(BYTE), 512, outptr);
        }
    }
    
    // close outfile file, if open
    if(outptr != NULL)
    {
        printf ("%s %s", *outfile, "file closed succesfully!\n");
        fclose(outptr);
    }
    
    // close raw file
    if(inptr != NULL)
    {
        printf ("%s %s", infile, "file opened and closed succesfully!\n");
        fclose(inptr);
    }

    return 0;
}
