#include "complex_num.h"
#include <malloc.h>
#include "string.h"

static int hash_for_type(char type[]);

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

complex_num twiddle_factor(int N, int exp)
{
	complex_num t = { cos(2 * M_PI / N * exp), -1 * sin(2 * M_PI / N * exp) };

	return t;
}

complex_num* data2complex(void* data, size_t size, char datatype[])
{
	//동적 할당 반환
	complex_num* cxtypeData = (complex_num*)malloc(sizeof(complex_num) * size);
	switch (hash_for_type(datatype))
	{
	case 414: {
		for (int i = 0; i < size; i++)
		{
			cxtypeData[i].Re = ((char*)data)[i];
			cxtypeData[i].Im = 0;
		}
		break;
	}
	case 331: {
		for (int i = 0; i < size; i++)
		{
			cxtypeData[i].Re = ((int*)data)[i];
			cxtypeData[i].Im = 0;
		}
		break;
	}
	case 534: {
		for (int i = 0; i < size; i++)
		{
			cxtypeData[i].Re = ((float*)data)[i];
			cxtypeData[i].Im = 0;
		}
		break;
	}
	case 635: {
		for (int i = 0; i < size; i++)
		{
			cxtypeData[i].Re = ((double*)data)[i];
			cxtypeData[i].Im = 0;
		}
		break;
	}
	case 432: {
		for (int i = 0; i < size; i++)
		{
			cxtypeData[i].Re = ((long*)data)[i];
			cxtypeData[i].Im = 0;
		}
		break;
	}
	case 896: {
		for (int i = 0; i < size; i++)
		{
			cxtypeData[i].Re = (double)(((long long*)data)[i]);
			cxtypeData[i].Im = 0;
		}
		break;
	}
	default:
		return NULL;
	}
	
	return cxtypeData;
	
}

static int hash_for_type(char type[])
{
	int len = (int)strlen(type);
	int sum = 0;
	for (int i = 0; i < len; i++)
	{
		sum += type[i];
	}
	return sum;
}

