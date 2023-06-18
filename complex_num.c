#include "complex_num.h"
#include <malloc.h>
#include "string.h"

complex_num complexAdd(complex_num A, complex_num B)
{
	complex_num res = { A.Re + B.Re, A.Im + B.Im };
	return res;
}

complex_num complexMul(complex_num A, complex_num B)
{
	complex_num res = { A.Re * B.Re - B.Im * A.Im, A.Re * B.Im + A.Im * B.Re };
	return res;
}

complex_num complexSub(complex_num A, complex_num B)
{
	complex_num res = { A.Re - B.Re, A.Im - B.Im };
	return res;
}

double complexAbs(complex_num A)
{
	return sqrt(A.Im * A.Im + A.Re * A.Re);
}

complex_num conjugate(complex_num A)
{
	complex_num temp = { A.Re,A.Im * -1 };
	return temp;
}

double phaseAngle(complex_num A)
{
	return atan2(A.Im, A.Re);
}

complex_num twiddle_factor(int N, int exp)
{
	complex_num t = { cos(2 * M_PI / N * exp), -1 * sin(2 * M_PI / N * exp) };

	return t;
}

complex_num* data2complex(void* data, size_t size, size_t type)
{
	//동적 할당 반환
	complex_num* cxtypeData = (complex_num*)malloc(sizeof(complex_num) * size);
	switch (type)
	{
	case 1: {
		for (int i = 0; i < size; i++)
		{
			*(((long long*)cxtypeData)+i*2) = ((char*)data)[i];
			cxtypeData[i].Im = 0;
		}
		break;
	}
	case 2: {
		for (int i = 0; i < size; i++)
		{
			*(((long long*)cxtypeData) + i*2) = ((short*)data)[i];
			cxtypeData[i].Im = 0;
		}
		break;
	}
	case 4: {
		for (int i = 0; i < size; i++)
		{
			*(((long long*)cxtypeData) + i*2) = ((int*)data)[i];
			cxtypeData[i].Im = 0;
		}
		break;
	}
	case 8: {
		for (int i = 0; i < size; i++)
		{
			*(((long long*)cxtypeData) + i*2) = ((long long*)data)[i];
			cxtypeData[i].Im = 0;
		}
		break;
	}
	default:
		return NULL;
	}

	return cxtypeData;

}
