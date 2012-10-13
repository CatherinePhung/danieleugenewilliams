/****************************************************************************
 * recover.c
 * Daniel Williams
 * danieleugenewilliams@gmail.com
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

int
main(int argc, char *argv[])
{    
    // open raw file 
    FILE *inptr = NULL;
    inptr = fopen(RAWCARD, "rb");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", RAWCARD);
        return 1;
    }
    
    int index = 0;
    int jpgcount = 0;
    
    // jpg filename
    char *outfile;
    outfile = calloc (10,sizeof(char));
    if(outfile == NULL)
    {
        printf("Could not allocate memory to outfile...exiting.");
        return 2;
    }
    
    // output file handle
    FILE *outptr = NULL;
    
    // get file size
    fseek(inptr, 0L, SEEK_END);
    int RAWSIZE = ftell(inptr);
    fseek(inptr, 0L, SEEK_SET);
    
    BYTE *chunk;
    chunk = calloc (RAWSIZE,sizeof(BYTE));
    if(chunk == NULL)
    {
        printf("Could not allocate memory to chunk...exiting.");
        return 3;
    }
    while(!feof(inptr))
    {
        fread(chunk, sizeof(BYTE), 512, inptr);
            
        for(int i = index, n = sizeof(chunk); i < n; i++)
        {
            // if jpg magic numbers found
            if(((i+3 < n) && ((chunk[i+3] == 0xe0) || (chunk[i+3] == 0xe1))) && ((i+2 < n) && (chunk[i+2] == 0xff)) && ((i+1 < n) && (chunk[i+1] == 0xd8)) && (chunk[i] == 0xff))
            {
                // increment jpg count
                jpgcount++;
                
                // print msg to console...found jpg
                printf("%03d: %s\n",jpgcount, "Found jpg");
                
                free(outfile);
                outfile = calloc (10,sizeof(char));
                if(chunk == NULL)
                {
                    printf("Could not allocate memory to chunk...exiting.");
                    return 3;
                }
                
                // set jpg filename
                sprintf(outfile,"%03d.jpg", (char)jpgcount);
                
                // close outfile file, if open
                if(outptr != NULL)
                {
                    printf ("%s %s", outfile, "file closed succesfully!\n");
                    fclose(outptr);
                }
                
                // open new jpg file
                outptr = fopen(outfile, "wb");
                if (outptr == NULL)
                {
                    printf("Could not open %s.\n", outfile);
                    return 4;
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
        printf ("%s %s", outfile, "file closed succesfully!\n");
        fclose(outptr);
    }
    
    // close raw file
    if(inptr != NULL)
    {
        printf ("%s %s", RAWCARD, "file opened and closed succesfully!\n");
        fclose(inptr);
    }
    
    free(chunk);
    free(outfile);

    return 0;
}
