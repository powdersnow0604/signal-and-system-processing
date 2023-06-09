#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define window_size 2187     //data 개수

typedef struct {
	double real;
	double imaginary;
}complex_num;

static complex_num* FFT(complex_num* P, int radix);
static complex_num twiddle_factor(int N, int exp);
static complex_num complexAdd(complex_num A, complex_num B);
static complex_num complexMul(complex_num A, complex_num B);

static char path[260];
static int max = 0;


int main_FFT_radix_k(void)
{
	FILE* file = fopen("sample1.snd", "r+b");
	if (file == NULL)
	{
		perror("file open error");
	}
	char* data = (char*)malloc(sizeof(char) * window_size);
	complex_num* cxtypeData = (complex_num*)malloc(sizeof(complex_num) * window_size);
	double* amplitude = (double*)malloc(sizeof(double) * window_size);

	//read data
	fread(data, sizeof(char), window_size, file);
	fclose(file);


	//change type to complex_num
	for (int i = 0; i < window_size; i++)
	{
		cxtypeData[i].real = data[i];
		cxtypeData[i].imaginary = 0;
	}
	free(data);


	//calc FFT
	complex_num* FFTres = FFT(cxtypeData,3);


	//calc amplitude
	for (int i = 0; i < window_size; i++)
	{
		amplitude[i] = sqrt(pow(FFTres[i].real, 2) + pow(FFTres[i].imaginary, 2));
	}
	free(FFTres);

	//finding freq with max amplitude
	for (int i = 1; i <= window_size / 2; i++)
	{
		if (amplitude[i] > amplitude[max])
		{
			max = i;
		}
	}
	printf("\n[최대 크기 주파수: %lfHz,	크기: %lf]\n", (double)max * 8000 / window_size, amplitude[max]);


	//plot
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

	strcpy(path, "DFTplot.py ");
	char buf[8];
	strcat(path, _itoa(window_size, buf, 10));
	strcat(path, " DFTres.txt ");
	strcat(path, _itoa(max, buf, 10));

	system(path);

	//free
	free(amplitude);
	free(cxtypeData);

	return 0;
}

static complex_num* FFT(complex_num* P, int radix)
{
	//setting
	int n = _msize(P) / sizeof(complex_num);
	if (n == 1)
	{
		return P;
	}


	complex_num** Terms = (complex_num**)malloc(sizeof(complex_num*) * radix);
	complex_num** eachTermRes = (complex_num**)malloc(sizeof(complex_num) * radix);

	for (int i = 0; i < radix; i++)
	{
		Terms[i] = (complex_num*)malloc(sizeof(complex_num) * (n / radix));
	}

	for (int i = 0; i < radix; i++)
	{
		for (int j = 0; j < n / radix; j++)
		{
			Terms[i][j] = P[radix * j + i];
		}
	}
	

	//decompose
	for (int i = 0; i < radix; i++)
	{
		eachTermRes[i] = FFT(Terms[i], radix);
	}
	

	//conquer
	complex_num* FFTRes = (complex_num*)calloc(n,sizeof(complex_num));
	for (int i = 0; i < n/radix; i++)
	{
		for (int j = 0; j < radix; j++)
		{

			FFTRes[i + n / radix * j] = complexAdd(FFTRes[i + n / radix * j], eachTermRes[0][i]);
			for (int k = 1; k < radix; k++)
			{

				FFTRes[i + n / radix * j] = complexAdd(FFTRes[i + n / radix * j], complexMul(eachTermRes[k][i],twiddle_factor(n,((i+n/radix*j)*k)%n)));
			}
		}
	}


	//free
	for (int i = 0; i < radix; i++)
	{
		free(Terms[i]);
	}
	free(Terms);

	if (n != radix)
	{
		for (int i = 0; i < radix; i++)
		{
			free(eachTermRes[i]);
		}
		free(eachTermRes);
	}

	return FFTRes;
}

static complex_num complexAdd(complex_num A, complex_num B)
{
	complex_num res = { A.real + B.real, A.imaginary + B.imaginary };
	return res;
}

static complex_num complexMul(complex_num A, complex_num B)
{
	complex_num res = { A.real * B.real - B.imaginary * A.imaginary, A.real * B.imaginary + A.imaginary * B.real };
	return res;
}

static complex_num twiddle_factor(int N, int exp)
{
	complex_num t = { cos(2 * M_PI / N * exp), -1 * sin(2 * M_PI / N * exp) };
	
	return t;
}
