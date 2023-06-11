#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define window_size 2000     //data 개수


char path[260];
int max = 0;
int second = 0;

int main_DFT(void)
{
	FILE* file = fopen("sample1.snd", "r+b");
	if (file == NULL)
	{
		perror("file open error");
	}
	char* data = (char*)malloc(sizeof(char) * window_size);
	double* transform = (double*)malloc(sizeof(double) * window_size);

	//read data
	fread(data, sizeof(char), window_size, file);
	fclose(file);

	//clac DFT
	for (int k = 0; k < window_size; k++)
	{
		double sumA = 0;
		double sumB = 0;
		for (int n = 0; n < window_size; n++)
		{
			sumA += data[n] * cos(2 * M_PI / window_size * (k * n % window_size));
			sumB += data[n] * -1 * sin(2 * M_PI / window_size * (k * n % window_size));
		}

		transform[k] = sqrt(pow(sumA, 2) + pow(sumB, 2));
	}



	//finding freq with max amplitude
	for (int i = 1; i <= window_size / 2; i++)
	{
		if (transform[i] > transform[max])
		{
			second = max;
			max = i;
		}
	}
	printf("\n[최대 크기 주파수: %lfHz,	크기: %lf]\n", (double)max * 8000 / window_size, transform[max]);

	//plot

	FILE* nfile = fopen("DFTres.txt", "w+");
	if (nfile == NULL)
	{
		perror("nfile error");
	}
	for (int i = 0; i < window_size; i++)
	{
		fprintf(nfile, "%lf\n", transform[i]);
	}
	fclose(nfile);

	strcpy(path, "DFTplot.py ");
	char buf[8];
	strcat(path, _itoa(window_size, buf, 10));
	strcat(path, " DFTres.txt ");
	strcat(path, _itoa(max, buf, 10));
	strcat(path, " ");
	strcat(path, _itoa(second, buf, 10));

	system(path);

	//free
	free(transform);
	free(data);
	return 0;
}

