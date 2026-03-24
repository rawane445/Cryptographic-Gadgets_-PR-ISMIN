#pragma once

#include <time.h>
#include "gadget.h"
#include "masking.h"

/* Binary gadgets: share2_t f(share2_t, share2_t) */
double perf_gadget2(int iterations, share2_t (*gadget)(share2_t, share2_t), share2_t a, share2_t b);

/* Unary gadgets: share2_t f(share2_t) */
double perf_gadget1(int iterations, share2_t (*gadget)(share2_t), share2_t a);

/* Gadget returning an unmasked 32-bit value */
double perf_fullxor(int iterations, uint32_t (*gadget)(share2_t), share2_t a);

/* Gadget expanding to 4 shares */
double perf_exp(int iterations, share4_t (*gadget)(share2_t), share2_t a);


double perf_agadget2_q(int iterations,ashare2_t (*gadget)(ashare2_t, ashare2_t, uint32_t),ashare2_t a, ashare2_t b, uint32_t q);