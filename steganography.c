/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	//int a = ((row - 1) * (image->rows) + col - 1);
	Color **ptr = image->image + ((row - 1) * (image->rows) + col - 1);
	int b = (*ptr)->B;
	Color *p = (Color *)malloc(sizeof(Color));
	if(b % 2 == 0)
	{
		p->R = 0;
		p->G = 0;
		p->B = 0;
	}
	else
	{
		p->R = 255;
		p->G = 255;
		p->B = 255;
	}
	return p;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	Image *ptr = (Image *)malloc(sizeof(Image));
	ptr->cols = image->cols;
	ptr->rows = image->rows;
	ptr->image = (Color **)malloc(sizeof(Color *) * (ptr->cols) * (ptr->rows));
	Color **p = ptr->image;
	for(int i = 1; i <= ptr->rows; i++)
	{
		for(int j = 1; j <= ptr->cols; j++)
		{
			*p = evaluateOnePixel(image, i, j);
			p++;
		}
	}
	return ptr;	
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if(argc != 2)
	{
		exit(-1);
	}
	char *filename = argv[1];
	Image *image = readData(filename);
	Image *de_image = steganography(image);
	writeData(de_image);
	freeImage(image);
	freeImage(de_image);
	return 0;	
}
