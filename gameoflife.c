/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	int rows = image->rows, cols = image->cols;
	Color *cell = (Color *)malloc(sizeof(Color));
	Color *ptr;
	int R[9], G[9], B[9];
	int row_orig = (rows + row - 2) % rows + 1;
	int col_orig = (cols + col - 2) % cols + 1;
	int row_s = row_orig, col_s;
	for(int i = 0; i < 3; i++)
	{	
		col_s = col_orig;
		for(int j = 0; j < 3; j++)
		{
			ptr = *(image->image + (row_s - 1) * cols + col_s - 1); 
			R[3 * i + j] = ptr->R;
			G[3 * i + j] = ptr->G;
			B[3 * i + j] = ptr->B;
			col_s = col_s % cols + 1;
		}
		row_s = row_s % rows + 1;
	}
	
	int A[18] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};
	int sum_R, sum_G, sum_B;
	int R_v = 0, G_v = 0, B_v = 0;
	int base = 1;
	for(int i = 0; i < 8; i++) //i represents the ith bit
	{
		int k_R = (A[i] & R[4])/A[i], k_G = (A[i] & G[4])/A[i], k_B = (A[i] & B[4])/A[i];
		sum_R = 0, sum_G = 0, sum_B = 0;
		for(int j = 0; j < 9; j++)  //j represents the jth pixel
		{
			if(j != 4)
			{
				sum_R += (A[i] & R[j])/A[i];
				sum_G += (A[i] & G[j])/A[i];
				sum_B += (A[i] & B[j])/A[i];
			}
		}
		if(k_R == 0)
			if(A[sum_R] != 0) sum_R = (A[sum_R] & rule)/A[sum_R];
	       	if(k_G == 0) 
			if(A[sum_G] != 0) sum_G = (A[sum_G] & rule)/A[sum_G]; 
		if(k_B == 0) 
			if(A[sum_B] != 0) sum_B = (A[sum_B] & rule)/A[sum_B];
		if(k_R == 1) 
			if(A[sum_R+9] != 0)sum_R = (A[sum_R + 9] & rule)/A[sum_R + 9];
		if(k_G == 1) 
			if(A[sum_G+9] != 0)sum_G = (A[sum_G + 9] & rule)/A[sum_G + 9];
		if(k_B == 1) 
			if(A[sum_B+9] != 0)sum_B = (A[sum_B + 9] & rule)/A[sum_B + 9];
		R_v += base * sum_R;
		G_v += base * sum_G;
		B_v += base * sum_B;
		base = base * 2;
	}
	cell->R = R_v;
	cell->G = G_v;
	cell->B = B_v;
	return cell;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image *image_trans = (Image *)malloc(sizeof(Image));
	image_trans->image = (Color **)malloc(sizeof(Color *) * image->cols * image->rows);
	Color **ptr = image_trans->image;
	image_trans->rows = image->rows;
	image_trans->cols = image->cols;
	for(int i = 1; i <= image->rows; i++)
	{
		for(int j = 1; j <= image->cols; j++)
		{
			*(ptr) = evaluateOneCell(image, i, j, rule);
			ptr += 1;
		}
	}
	return image_trans;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if(argc != 3)
        {
		printf("usage: ./gameOfLife filename rule\n");
		printf("filename is an ASCII PPM file (type P3) with maximum value 255\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
        char *filename = argv[1];
	char *rule_str = argv[2];
	uint32_t rule = strtol(rule_str, NULL, 16);
	Image *image = readData(filename);
	if(image == NULL) exit(-1);
	Image *image_trans = life(image, rule);
	writeData(image_trans);
	freeImage(image_trans);
	freeImage(image);
	return 0;	
}
