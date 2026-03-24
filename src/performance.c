#include "../include/performance.h"

double perf_gadget2(int iterations, share2_t (*gadget)(share2_t, share2_t), share2_t a, share2_t b)
{
    if (iterations <= 0) {
        return 0.0;
    }

    share2_t result;

    clock_t start = clock();

    for (int i = 0; i < iterations; i++) 
    {
        result = gadget(a, b);
        a = result;   /* avoid simple compiler optimization */
    }

    clock_t end = clock();

    return (double)(end - start) / CLOCKS_PER_SEC;
}

double perf_gadget1(int iterations, share2_t (*gadget)(share2_t), share2_t a)
{
    if (iterations <= 0) {
        return 0.0;
    }

    share2_t result;

    clock_t start = clock();

    for (int i = 0; i < iterations; i++) 
    {
        result = gadget(a);
        a = result;
    }

    clock_t end = clock();

    return (double)(end - start) / CLOCKS_PER_SEC;
}

double perf_fullxor(int iterations, uint32_t (*gadget)(share2_t), share2_t a)
{
    if (iterations <= 0) {
        return 0.0;
    }

    uint32_t result = 0;

    clock_t start = clock();

    for (int i = 0; i < iterations; i++) 
    {
        result = gadget(a);

        a = mask_bool(result);
    }

    clock_t end = clock();

    return (double)(end - start) / CLOCKS_PER_SEC;
}

double perf_exp(int iterations, share4_t (*gadget)(share2_t), share2_t a)
{
    if (iterations <= 0) {
        return 0.0;
    }
    share4_t result;
    clock_t start = clock();
    for (int i = 0; i < iterations; i++) 
    {
        result = gadget(a);
        uint32_t x = unmask_bool4(result);
        a = mask_bool(x);
    }
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}


double perf_agadget2_q(int iterations,ashare2_t (*gadget)(ashare2_t, ashare2_t, uint32_t),ashare2_t a, ashare2_t b, uint32_t q)
{
    if (iterations <= 0) {
        return 0.0;
    }

    ashare2_t result;

    clock_t start = clock();

    for (int i = 0; i < iterations; i++)
    {
        result = gadget(a, b, q);
        a = result;   /* avoid simple compiler optimization */
    }

    clock_t end = clock();

    return (double)(end - start) / CLOCKS_PER_SEC;
}