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

// prototypes
int write_jpg(int jpgcount, BYTE jpg[]);

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
    BYTE chunk[RAWSIZE];
    while(!feof(inptr))
    {
        int result = fread(chunk, sizeof(BYTE), 512, inptr);
        if(result < 0)
            printf("an error occurred\n");
            
        for(int i = index, n = sizeof(chunk); i < n; i++)
        {
            //printf("%x\n", chunk[i]);
            if(((chunk[i] == 0xe0) || (chunk[i] == 0xe1)) && (chunk[i-1] == 0xff) && (chunk[i-2] == 0xd8) && (chunk[i-3] == 0xff))
            {
                jpgcount++;
                printf("%d: %s %d\n",jpgcount, "Found jpg at chunk:", i-3);
                
            }
        }
    }
    

    
    //write_jpg(1,chunk);
    
    // close raw file
    if(inptr != NULL)
    {
        printf ("%s %s", infile, "file opened and closed succesfully!\n");
        fclose(inptr);
    }

    return 0;
}

int
write_jpg(int jpgcount, BYTE jpg[])
{
    // determine filename
    char *outfile[10];
    sprintf(*outfile,"%03d.jpg", jpgcount);
    
    FILE *outptr = fopen(*outfile, "wb");
    if (outptr == NULL)
    {
        printf("Could not open %s.\n", *outfile);
        return 2;
    }
    
    for(int i = 0, n = sizeof(jpg); i < n; i++)
    {
        fwrite(&jpg[i], sizeof(BYTE), 1, outptr);
    }
    
    // close outfile file
    if(outptr != NULL)
    {
        printf ("%s %s", *outfile, "file opened and closed succesfully!\n");
        fclose(outptr);
    }
    return 0;
}
