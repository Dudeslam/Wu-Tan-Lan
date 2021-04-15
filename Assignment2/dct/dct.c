#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dct.h"

void DCTII(size_t N, size_t M, float *y, const float *x)
{
	static const float Pi = 0x3.243f6a8885a308d313198a2e03707344ap0f;
	size_t k;
	for (k = 0; k < M; ++k)
	{
		float sum = 0;
		size_t n;
		for (n = 0; n < N; ++n)
			sum += x[n] * cosf(Pi / N * (n + .5) * k);
		y[k] = sum;
	}
	printf("dct done\n");
}