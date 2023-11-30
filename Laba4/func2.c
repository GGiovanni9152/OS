#include <stdio.h>


double PI(int k)
{
    double result = 1;
    
    for (double factor = 1; factor <= k; ++factor)
    {
        result *= ((4 * factor * factor) / (4 * factor * factor - 1));
    }
    
    return 2 * result;
}

double factorial(int n)
{
    double result = 1;
    
    for (int factor = 2; factor <= n; ++factor)
    {
        result *= factor;
    }
    
    return result;
}

double E(int x)
{
    double result = 0;
    
    for (int step = 0; step <= x; ++step)
    {
        result += (1 / factorial(step));
    }
    
    return result;
}

