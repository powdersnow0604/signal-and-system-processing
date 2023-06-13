#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include "DFT.h"
#define window_size	 5400     //data 개수 

static char path[260];
static int max = 0;
static int second = 0;

int main(void)
{
	FILE* file = fopen("sample1.snd", "r+b");
	if (file == NULL)
	{
		perror("file open error");
	}
	char* data = (char*)malloc(sizeof(char) * window_size);
	complex_num* FFTRes;
	double* amplitude;

	//read data
	fread(data, sizeof(char), window_size, file);
	fclose(file);

	
	//change type to complex_num
	FFTRes = data2complex(data, window_size, "char");
	free(data);


	//calc FFT
	FFT_common_factor(FFTRes);  //change here for another version of FFT

	//calc amplitude
	amplitude = calcAmplitude(FFTRes);
	free(FFTRes);

	//finding freq with max amplitude
	for (int i = 1; i <= window_size / 2; i++)
	{
		if (amplitude[i] > amplitude[max])
		{
			second = max;
			max = i;
		}
	}
	printf("\n[최대 크기 주파수: %lfHz,	크기: %lf]\n", (double)max * 8000 / window_size, amplitude[max]);


	//store
	FILE* nfile = fopen("DFTres.txt", "w+");
	if (nfile == NULL)
	{
		perror("nfile error");
	}
	for (int i = 0; i < window_size; i++)
	{
		fprintf(nfile, "%lf\n", amplitude[i]);
	}
	fclose(nfile);

	//plot
	plot(path, "DFTplot.py", "DFTres.txt", window_size, 8000, 2, max, second);


	//free
	free(amplitude);

	return 0;
}