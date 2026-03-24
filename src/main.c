#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include "../include/masking.h"
#include "../include/gadget.h"
#include "../include/performance.h"
///QQ HELPERSSS
static uint32_t unmask_arith_u32(ashare2_t a)
{
    return a.s0 + a.s1;
}

static ashare2_t mask_arith_u32(uint32_t x)
{
    ashare2_t a;
    a.s0 = rng32();
    a.s1 = x - a.s0;
    return a;
}


int main(void)
{
    rng_init();

    uint32_t x = 12;
    uint32_t y = 7;
    uint32_t q = 3329;

    share2_t a = mask_bool(x);
    share2_t b = mask_bool(y);

    ashare2_t aa_q = mask_arith_q(x, q);
    ashare2_t bb_q = mask_arith_q(y, q);

    ashare2_t aa_2k = mask_arith_u32(x);
    ashare2_t bb_2k = mask_arith_u32(y);

    int N = 10000000;

    printf("Performance results for masked gadgets\n");
    printf("Iterations: %d\n", N);

    printf("    Correctness checks \n");

    printf("unmask_bool(a) = %u\n", unmask_bool(a));
    printf("unmask_bool(b) = %u\n", unmask_bool(b));

    printf("unmask_arith_q(aa_q,q) = %u\n", unmask_arith_q(aa_q, q));
    printf("unmask_arith_q(bb_q,q) = %u\n", unmask_arith_q(bb_q, q));

    printf("unmask_arith_u32(aa_2k) = %u\n", unmask_arith_u32(aa_2k));
    printf("unmask_arith_u32(bb_2k) = %u\n\n", unmask_arith_u32(bb_2k));

    {
        share2_t r1 = a2b_first_order_q(aa_2k, 0);
        share2_t r2 = a2b_goubin_classic_u32(aa_2k);

        printf("A2B (Coron/Goubin log) : %u\n", unmask_bool(r1));
        printf("A2B (Goubin classic)   : %u\n\n", unmask_bool(r2));
    }

    {
        ashare2_t r1 = sec_shareconvert_b2a_classic_q(a, 0);
        ashare2_t r2 = b2a_goubin_u32(a);

        printf("B2A (GoubinSNI)        : %u\n", unmask_arith_u32(r1));
        printf("B2A (Goubin original)  : %u\n\n", unmask_arith_u32(r2));
    }

    printf("    Published gadgets \n");

    printf("sec_xor        : %f\n", perf_gadget2(N, sec_xor, a, b));
    printf("xor_refresh    : %f\n\n", perf_gadget2(N, xor_refresh, a, b));

    printf("sec_and (ISW)  : %f\n", perf_gadget2(N, sec_and, a, b));
    printf("dom_and (DOM)  : %f\n\n", perf_gadget2(N, dom_and, a, b));

    printf("mult_bool_isw  : %f\n", perf_gadget2(N, mult_bool_isw, a, b));
    printf("mult_bool_dom  : %f\n\n", perf_gadget2(N, mult_bool_dom, a, b));

    printf("sec_or         : %f\n", perf_gadget2(N, sec_or, a, b));
    printf("or_dom         : %f\n\n", perf_gadget2(N, or_dom, a, b));

    printf("sec_not        : %f\n", perf_gadget1(N, sec_not, a));
    printf("sec_ref        : %f\n", perf_gadget1(N, sec_ref, a));
    printf("full_xor       : %f\n\n", perf_fullxor(N, full_xor, a));

    printf("sec_exp        : %f\n\n", perf_exp(N, sec_exp, a));

    printf("    Experimental gadgets \n");

    printf("arith_mult_isw_q        : %f\n",
           perf_agadget2_q(N, arith_mult_isw_q, aa_q, bb_q, q));

    printf("arith_mult_double_sni_q : %f\n\n",
           perf_agadget2_q(N, arith_mult_double_sni_q, aa_q, bb_q, q));

    printf("sec_add (baseline)      : %f\n", perf_gadget2(N, sec_add, a, b));
    printf("sec_sub (baseline)      : %f\n\n", perf_gadget2(N, sec_sub, a, b));

    {
        clock_t start = clock();

        share2_t tmp = a;
        for (int i = 0; i < N; i++)
            tmp = sec_add_chain_q(tmp, b, q, 1);

        double t = (double)(clock() - start) / CLOCKS_PER_SEC;
        printf("sec_add_chain_q         : %f\n", t);
    }

    printf("\n    A2B comparison (mod 2^32)\n");

    {
        clock_t start = clock();

        ashare2_t tmp = aa_2k;
        for (int i = 0; i < N; i++) {
            share2_t r = a2b_first_order_q(tmp, 0);
            tmp = mask_arith_u32(unmask_bool(r));
        }

        double t = (double)(clock() - start) / CLOCKS_PER_SEC;
        printf("A2B Coron (log)        : %f\n", t);
    }

    {
        clock_t start = clock();

        ashare2_t tmp = aa_2k;
        for (int i = 0; i < N; i++) {
            share2_t r = a2b_goubin_classic_u32(tmp);
            tmp = mask_arith_u32(unmask_bool(r));
        }

        double t = (double)(clock() - start) / CLOCKS_PER_SEC;
        printf("A2B Goubin (classic)   : %f\n", t);
    }

    printf("\n   B2A comparison (mod 2^32) \n");

    {
        clock_t start = clock();

        share2_t tmp = a;
        for (int i = 0; i < N; i++) {
            ashare2_t r = sec_shareconvert_b2a_classic_q(tmp, 0);
            tmp = mask_bool(unmask_arith_u32(r));
        }

        double t = (double)(clock() - start) / CLOCKS_PER_SEC;
        printf("B2A GoubinSNI          : %f\n", t);
    }

    {
        clock_t start = clock();

        share2_t tmp = a;
        for (int i = 0; i < N; i++) {
            ashare2_t r = b2a_goubin_u32(tmp);
            tmp = mask_bool(unmask_arith_u32(r));
        }

        double t = (double)(clock() - start) / CLOCKS_PER_SEC;
        printf("B2A Goubin (original)  : %f\n", t);
    }
    return 0;
}