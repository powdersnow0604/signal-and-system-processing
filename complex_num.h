#ifndef  __COMPLEX_NUM_H__
#define  __COMPLEX_NUM_H__

#define _USE_MATH_DEFINES
#include <math.h>

typedef struct {
	double Re;
	double Im;
}complex_num;

extern complex_num complexAdd(complex_num A, complex_num B);

extern complex_num complexMul(complex_num A, complex_num B);

extern complex_num complexSub(complex_num A, complex_num B);

extern complex_num twiddle_factor(int N, int exp);

extern complex_num* data2complex(void* data, size_t size, char datatype[]);

#endif