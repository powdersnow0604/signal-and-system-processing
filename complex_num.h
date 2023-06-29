#ifndef  __COMPLEX_NUM_H__
#define  __COMPLEX_NUM_H__

#define _USE_MATH_DEFINES
#include <math.h>

typedef struct {
	double Re;
	double Im;
}complex_num;

extern inline complex_num complexAdd(complex_num A, complex_num B);

extern inline complex_num complexMul(complex_num A, complex_num B);

extern inline complex_num complexSub(complex_num A, complex_num B);

extern inline complex_num complexDiv(complex_num A, complex_num B);

extern inline double complexAbs(complex_num A);

extern inline complex_num conjugate(complex_num A);

extern inline double phaseAngle(complex_num A);

extern inline complex_num twiddle_factor(int N, int exp);

extern complex_num* data2complex(void* data, size_t size, char datatype[]);

extern complex_num* mkCdata(size_t size, double (*func)(int));

#pragma region checkType
#define checkType(x) _Generic((x),	char : "char", short : "short", \
									int : "int", long : "long", \
									float : "float", long long : "long long", \
									double : "double", default : "Nan")
#pragma endregion

extern const complex_num I;

#endif