/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	Image *ptr = (Image *)malloc(sizeof(Image));
	FILE *fp = fopen(filename, "r");
	char file[20];
	int cols, rows, num;
	fscanf(fp, "%s", file);
	fscanf(fp, "%d %d", &cols, &rows);
	fscanf(fp, "%d", &num);
	ptr->cols = cols;
	ptr->rows = rows;
	ptr->image = (Color **)malloc(sizeof(Color *) * cols * rows);
	Color **p = ptr->image;  
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			*p = (Color *)malloc(sizeof(Color)); 
			fscanf(fp, "%hhu %hhu %hhu   ", &((*p)->R), &((*p)->G), &((*p)->B));
			p += 1;
		}
	}
	fclose(fp);
	return ptr;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");
	Color **ptr = image->image;
	for(int i = 0; i < image->rows; i++)
	{
		for(int j = 0; j < image->cols - 1; j++)
		{
			printf("%3d %3d %3d   ", (*ptr)->R, (*ptr)->G, (*ptr)->B);
			ptr++;
		}			
		printf("%3d %3d %3d\n", (*ptr)->R, (*ptr)->G, (*ptr)->B);
		ptr++;
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	Color **p = image->image;
	for(int i = 0; i < (image->cols) * (image->rows); i++)
	{
		free(*p);
		p++;
	}
	free(image->image);
	free(image);
}
