#include "dct.h"
#include <stdio.h>
#include <math.h>


void DCTII(int N, float *y, float *x)
{
    static const float Pi = 0x3.243f6a8885a308d313198a2e03707344ap0f;
    int k, n;
    for (k = 0; k < N; ++k)
    {
        float sum = 0;
        for (n = 0; n < N; ++n)
            sum += x[n] * cosf(Pi/N * (n + .5) * k);
        y[k] = sum;
        
    }
}
