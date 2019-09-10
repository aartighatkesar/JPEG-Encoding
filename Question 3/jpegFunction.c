#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Htables.h"

/*******************************************/
int BitSize(int value)
{
	// Function to find the size 'm'
	int bitsize;
	bitsize = ceil(log2(abs(value )+ 1));
	return(bitsize);

}

/*******************************************/
void VLI_encode(int bitsz, int value, char *block_code)
{
	// Function to perform Variable Length Integer Encoding

	printf("In VLI_Encode\n");
	int i;
	
	char *temp = malloc(sizeof(char)* (bitsz + 1));

	for (i = 0; i <= bitsz; i++)
	{
		temp[i] = '\0';
	}

	if (value < 0)
	{
		value = value - 1;
	}

	for (i = bitsz - 1; i >= 0; i--)
	{
		if (value & 1)
			temp[i] = '1';
		else
		temp[i] = '0';

		value = value >> 1;
	}
	strcat(block_code, temp);
	free(temp);
}

/*******************************************/

void ZigZag(int ** img, int y, int x, int *zigline)
{
	int i, j;

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			zigline[Zig[i][j]] = img[i + y][j + x];
		}
	}

}

/*******************************************/
void DC_encode(int dc_value, int prev_value, char *block_code)
{
	int diff,bitsize;
	diff = dc_value - prev_value;
	bitsize = BitSize(diff);
    strcat(block_code, dcHuffman.code[bitsize]);
	VLI_encode(bitsize, diff, block_code);

}

/*******************************************/
void AC_encode(int *zigzag, char *block_code)
{
	/*Init Variables*/

	int idx = 1; // starts from 1 as idx = 0  is DC
	int zerocnt = 0; // Counts runs of zeros
	int bitsize;

	while (idx < 64)
	{
		if (zigzag[idx] == 0)
		{
			zerocnt++;
		}
		else
		{
			/* ZRL coding - enters this when encounters first non zero AC coeffient. So perfom Run length coding with obtained zerocount*/
			for (; zerocnt > 15; zerocnt=zerocnt - 16) // take 16 at a time to use ZRL for code
			{
				strcat(block_code, acHuffman.code[15][0]); // Append code for ZRL : VLC ac for ZRL
			}

			bitsize = BitSize(zigzag[idx]); // Get size required for value
			strcat(block_code, acHuffman.code[zerocnt][bitsize]); // VLC ac for pair (run,size) for AC
			VLI_encode(bitsize, zigzag[idx], block_code);
			zerocnt = 0; // Resetting zero count for next run length
		}
		idx++;
	}
	// EOB coding -  End of block run length of zeros
	if (zerocnt)
	{
		strcat(block_code, acHuffman.code[0][0]);
	}
}

/*******************************************/
void Block_encode(int prev_value, int *zigzag, char *block_code)
{
	DC_encode(zigzag[0], prev_value, block_code);
	AC_encode(zigzag, block_code);
	// NULL character taken care by strcat
}

/*******************************************/
int Convert_encode(char *block_code, unsigned char *byte_code)
{

	// Converts Block_code to individual byte code
	// Takes care of byte stuffing 
	// Final block_code contains less than 8 number of elements

	char tempArr[8] = { '\0' };
	int length=0;
	int len; // Holds number of bytes
	int i,j,k,temp,totLen;
	int value = 0;
	int rem = 0;

	totLen = strlen(block_code);
	len = strlen(block_code);
	rem = len % 8; // Remaining number of elements in block_code
	len = len / 8;

	for (i = 0; i <= 8 * len - 1; i = i + 8)
	{
		for (j = 0; j < 8; j++)
		{
			// 0 in ASCII is 48 i.e  0011 0000
			// 1 in ASCII is 49 i.e  0011 0001 Notice the last bit only is different

			temp = block_code[j + i] & 1;
			value = pow(2, 7 - j) * temp + value;
		}

		byte_code[length] = value;
		length = length + 1;

		if (value == 0xFF)
		{
			byte_code[length] = 0x00;
			length = length + 1;
		}

		value = 0;
	}

	// Get remaining bits in block_code

	for (k = 0; k < 8; k++)
	{
		tempArr[k] = block_code[k + 8*len];
	}
	
	// Set all elements in block_code to NULL so that only the reamining elements can be copied
	memset(block_code, '\0', strlen(block_code));
	strcat(block_code, tempArr);

	return(length);

}

/*******************************************/
unsigned char Zero_pad(char *block_code)
{

	// Zeropadding
	unsigned char byte_value;
	int length = 0; 
	int i,value=0,temp;
	length = strlen(block_code);

	if (length > 8)
	{
		printf("Exiting From Zero Pad");
		exit(-1);
	}

	for(i = length; i < 8;i++)
	{
		block_code[i] = '0';
	}


	for (i = 0; i < 8; i++)
	{

		temp = block_code[i] & 1;
		value = pow(2, 7 - i) * temp + value;
	}

	byte_value = value;

}

