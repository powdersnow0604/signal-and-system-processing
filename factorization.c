#include "prime_factor.h"

int return_1_factor(int num)
{
	int k = 2;
	double sqrtnum = sqrt(num);
	while (k <= sqrtnum && num != 1)
	{
		if (num % k == 0)
		{
			return k;
		}
		k++;
	}
	return num;
}
