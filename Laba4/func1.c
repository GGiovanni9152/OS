#include <stdio.h>
#include <math.h>

const int E_ACCURACY = 200000000;
const int PI_ACCURACY = 10000000;

double bpow(double base, int power)
{
    double result = 1.0;
    while (power > 0)
    {
        if (power % 2 != 0)
        {
            result *= base;
        }
        
        base *= base;
        power /= 2;
    }
    return result;
}

double E(int x)
{
    if (x < 0)
    {
        return -1;
    }
    
    double result;
    result = (1.0 + 1.0 / x);
    return bpow(result, x);
}

double PI(int k)
{
    double result = 0;
    
    for (int step = 0; step <= k; ++step)
    {
        result += (bpow(-1, step % 2) / (2 * step + 1));
    }
    
    return 4 * result;
}

