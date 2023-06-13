#include "DFT.h"
#include <malloc.h>
#include <errno.h>

//////////////////    선언부    //////////////////////////////

static void OnShuffle(complex_num* X, int N, int log2N);
static int OnReverseBitOrder(int index, int LOG2N);
static void relocate(complex_num* X, int N, int radix, complex_num* temp);
static int LOG2N(int Num);

static void OnButterfly_radix_2(complex_num* P, int n,int mode);
static void OnButterfly_radix_k(complex_num* P, int n, int radix, complex_num* temp, int mode);
static void OnButterfly_common_factor(complex_num* P, int N, complex_num* temp, int mode);

//////////////////    정의부    //////////////////////////////


//////// [suffle] ///////
static void OnShuffle(complex_num* X, int N, int log2N)
{
	int i;
	complex_num* temp = (complex_num*)malloc(sizeof(complex_num) * N);

	if (temp == NULL) return;

	for (i = 0; i < N; i++) {
		temp[i].Re = X[OnReverseBitOrder(i, log2N)].Re;
		temp[i].Im = X[OnReverseBitOrder(i, log2N)].Im;
	}

	for (i = 0; i < N; i++) {
		X[i].Re = temp[i].Re;
		X[i].Im = temp[i].Im;
	}

	free(temp);
}

static int OnReverseBitOrder(int index, int LOG2N)
{
	int i, X, Y = 0;

	for (i = 0; i < LOG2N; i++) {
		X = (index & (1 << i)) >> i;
		Y = (Y << 1) | X;
	}

	return Y;
}

static int LOG2N(int Num)
{
	int log2N = 0;

	while (Num >= 2)
	{
		Num >>= 1;
		log2N++;
	}

	return log2N;
}

static void relocate(complex_num* X, int N, int radix, complex_num* temp)
{
	int i, j, pos;

	for (i = 0; i < N; i += radix) {
		pos = i / radix;
		for (j = 0; j < radix; j++) {
			temp[pos + N / radix * j].Re = X[i + j].Re;
			temp[pos + N / radix * j].Im = X[i + j].Im;
		}
	}

	for (i = 0; i < N; i++) {
		X[i].Re = temp[i].Re;
		X[i].Im = temp[i].Im;
	}

}
/////////////////////////


//////// [usable] ///////
void DFT_1d(complex_num* X)
{
	int i, k, n,N;
	if ((N = (int)sizeof(X)) == 8)	N = (int)(_msize(X) / sizeof(complex_num));
	else N /= (int)sizeof(complex_num);

	
	complex_num* temp = (complex_num*)malloc(sizeof(complex_num) * N);

	for (k = 0; k < N; k++)
	{
		for (n = 0; n < N; n++)
		{
			temp[k].Re += X[n].Re * cos(2 * M_PI / N * (k * n % N));
			temp[k].Im += X[n].Re * -1 * sin(2 * M_PI / N * (k * n % N));
		}
	}

	for (i = 0; i < N; i++) {
		X[i].Re = temp[i].Re;
		X[i].Im = temp[i].Im;
	}

	free(temp);
}

double* calcAmplitude(complex_num* X)
{
	//동적 할당된 메모리 반환
	int i;
	int N = (int)(_msize(X)/sizeof(complex_num));

	double* temp = (double*)malloc(sizeof(double) * N);

	for (i = 0; i < N; i++) {
		temp[i] = sqrt(X[i].Re * X[i].Re + X[i].Im * X[i].Im);
	}

	return temp;
}

void FFT_radix_2(complex_num* P)
{
	int N,log2N;

	if ((N = (int)sizeof(P)) == 8)	N = (int)(_msize(P) / sizeof(complex_num));
	else N /= (int)sizeof(complex_num);

	log2N = LOG2N(N);

	OnShuffle(P, N, log2N);

	if (errno = ENOMEM) return;

	OnButterfly_radix_2(P, N,0);
}

void FFT_radix_k(complex_num* P)
{
	int N, radix;

	if ((N = (int)sizeof(P)) == 8)	N = (int)(_msize(P) / sizeof(complex_num));
	else N /= (int)sizeof(complex_num);

	radix = minFactor(N);

	complex_num* temp = (complex_num*)malloc(sizeof(complex_num) * (N + radix - 1));

	if (temp == NULL) return;

	OnButterfly_radix_k(P, N, radix, temp, 0);

	free(temp);
}

void FFT_common_factor(complex_num* P)
{
	int N;
	int cnt;

	if ((N = (int)sizeof(P)) == 8)	N = (int)(_msize(P) / sizeof(complex_num));
	else N /= (int)sizeof(complex_num);

	if (N == minFactor(N)) cnt = 2 * N;
	else cnt = (int)pow(N, 1.5);

	complex_num* temp = (complex_num*)malloc(sizeof(complex_num) * cnt);

	if (temp == NULL) return;

	OnButterfly_common_factor(P, N, temp,0);

	free(temp);
}

void IFFT_radix_2(complex_num* P)
{
	int i,N,log2N;

	if ((N = (int)sizeof(P)) == 8)	N = (int)(_msize(P) / sizeof(complex_num));
	else N /= (int)sizeof(complex_num);

	log2N = LOG2N(N);

	OnShuffle(P, N, log2N);
	OnButterfly_radix_2(P, N, 1);

	for (i = 0; i < N; i++) {
		P[i].Re /= N;
		P[i].Im /= N;
	}
}

void IFFT_radix_k(complex_num* P)
{
	int i,N,radix;

	if ((N = (int)sizeof(P)) == 8)	N = (int)(_msize(P) / sizeof(complex_num));
	else N /= (int)sizeof(complex_num);

	radix = minFactor(N);

	complex_num* temp = (complex_num*)malloc(sizeof(complex_num) * (N + radix - 1));

	if (temp == NULL) return;

	OnButterfly_radix_k(P, N, radix, temp, 1);

	free(temp);

	for (i = 0; i < N; i++) {
		P[i].Re /= N;
		P[i].Im /= N;
	}
}

void IFFT_common_factor(complex_num* P)
{
	int i,N;
	int cnt;

	if ((N = (int)sizeof(P)) == 8)	N = (int)(_msize(P) / sizeof(complex_num));
	else N /= (int)sizeof(complex_num);

	if (N == minFactor(N)) cnt = 2 * N;
	else cnt = (int)pow(N, 1.5);

	complex_num* temp = (complex_num*)malloc(sizeof(complex_num) * cnt);

	if (temp == NULL) return;

	OnButterfly_common_factor(P, N, temp, 1);

	free(temp);
	
	for (i = 0; i < N; i++) {
		P[i].Re /= N;
		P[i].Im /= N;
	}
}
//////////////////////////


//////// [butterfly] /////
static void OnButterfly_radix_2(complex_num* P, int n, int mode)
{
	if (n == 1)
		return;
	int i, s = 1-2*mode;
	complex_num temp;

	OnButterfly_radix_2(P, n / 2,mode);
	OnButterfly_radix_2(P + n / 2, n / 2,mode);

	for (i = 0; i < n / 2; i++)
	{
		temp = complexMul(twiddle_factor(n, s*i), (P + n / 2)[i]);

		P[i+n/2] = complexSub(P[i], temp);
		P[i] = complexAdd(P[i], temp);
	}
}

static void OnButterfly_radix_k(complex_num* P, int n, int radix, complex_num* temp, int mode)
{
	//setting
	if (n == 1)
		return;

	int i,j,k;

	//relocate
	if(n != radix)
		relocate(P, n, radix,temp);
	
	//decompose
	for (i = 0; i < radix; i++) {
		OnButterfly_radix_k(P + i * n / radix, n / radix, radix,temp,mode);
	}

	//conquer
	if (mode) {
		for (i = 0; i < n; i++) {
			temp[i + radix - 1] = twiddle_factor(n, -i);
		}
	}
	else {
		for (i = 0; i < n; i++) {
			temp[i + radix - 1] = twiddle_factor(n, i);
		}
	}

	for (i = 0; i < n / radix; i++)
	{
		for (j = 0; j < radix-1; j++) {
			temp[j] = P[i + n / radix * (j+1)];
		}

		for (j = radix-1; j >= 0; j--)
		{
			P[i + n / radix * j] = P[i];
			for (k = 1; k < radix; k++) {
				P[i + n / radix * j] = complexAdd(P[i + n / radix * j], complexMul(temp[k-1], temp[radix-1+((i + n / radix * j) * k) % n]));
			}
		}
	}

}

static void OnButterfly_common_factor(complex_num* P, int N, complex_num* temp, int mode)
{
	//setting
	if (N == 1)
		return;
	
	int i,j,k,radix = minFactor(N);

	relocate(P, N, radix, temp);

	//decompose
	for (i = 0; i < radix; i++)
	{
		OnButterfly_common_factor(P + N / radix * i, N / radix, temp,mode);
	}

	//conquer
	if (mode) {
		for (i = 0; i < N; i++) {
			temp[i + radix - 1] = twiddle_factor(N, -i);
		}
	}
	else {
		for (i = 0; i < N; i++) {
			temp[i + radix - 1] = twiddle_factor(N, i);
		}
	}

	for (i = 0; i < N / radix; i++)
	{
		for (j = 0; j < radix-1; j++) {
			temp[j] = P[i + N / radix * (j + 1)];
		}

		for (j = radix - 1; j >= 0; j--)
		{
			
			P[i + N / radix * j] = P[i];
			for (k = 1; k < radix; k++) {
				P[i + N / radix * j] = complexAdd(P[i + N / radix * j], complexMul(temp[k - 1], temp[radix-1 + ((i + N / radix * j) * k) % N]));
			}
		}
	}
}
//////////////////////////