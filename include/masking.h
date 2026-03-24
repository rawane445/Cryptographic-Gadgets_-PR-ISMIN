#pragma once
#include <stdint.h>

///i represent a secret value x as: x = x0 XOR x1

///masked value with 2 shares (for 1st order security) 
typedef struct { 
    uint32_t s0;   //random mask, share0
    uint32_t s1;   //masked value, share1
    } share2_t;
    
typedef struct {
    uint32_t s0;
    uint32_t s1;
} ashare2_t;

typedef struct {
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
} share4_t;

void rng_init(void);///Initializes the pseudo random generator , called just when the program start
uint32_t rng32(void);//returns a random 32bit value (used as mask/randomness)

share2_t mask_bool(uint32_t x);///creates shares(x0, x1) such that x = x0 XOR x1
uint32_t unmask_bool(share2_t a);///reconstructs x by xoring the two shares

uint32_t unmask_bool4(share4_t a);

ashare2_t mask_arith_q(uint32_t x, uint32_t q);
uint32_t unmask_arith_q(ashare2_t a, uint32_t q);