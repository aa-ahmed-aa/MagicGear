/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[1];
    char* outfile = argv[2];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
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

    
    //reading pixels in the array of triple[1000000]
    int padding =  (4 - ((bi.biWidth * sizeof(RGBTRIPLE)) % 4)) % 4;
    RGBTRIPLE triple[1000000];
    int counter=0;
    for(int i=0;i<abs(bi.biHeight);i++)
    {
        for(int j=0;j<bi.biWidth;j++)
        {
            fread(&triple[counter],sizeof(RGBTRIPLE),1,inptr);   
            counter++;
        }
        fseek(inptr, padding, SEEK_CUR);
    }//end reading
    
    for(int i=0;i<abs(bi.biHeight);i++)
    {
        for(int j=0;j<bi.biWidth;j++)
        {
             //write to the file in text 
            if (triple[counter].rgbtRed == 0xff && triple[counter].rgbtGreen == 0xff && triple[counter].rgbtBlue == 0xff )
            {
                printf(" ");  
                fprintf(outptr," ");    
            }    
            else 
            {
                printf("*");
                fprintf(outptr,"*");
            }
            counter--;
        }
        
        printf("\n");  
        fprintf(outptr,"\n");
    }//end reading
    
    

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
