#ifndef  __DFT_H__
#define  __DFT_H__

#undef SPEED

#include "complex_num.h"
#include "factorization.h"
#include "plot.h"

void DFT_1d(complex_num* X);
double* calcAmplitude(complex_num* X);

void FFT_radix_2(complex_num* P);
void FFT_radix_k(complex_num* P);
void FFT_common_factor(complex_num* P);

void IFFT_radix_2(complex_num* P);
void IFFT_radix_k(complex_num* P);
void IFFT_common_factor(complex_num* P);

#endif