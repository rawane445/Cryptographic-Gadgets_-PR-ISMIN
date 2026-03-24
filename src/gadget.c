#include "../include/gadget.h"
#include "../include/masking.h"


share2_t sec_xor(share2_t a, share2_t b)
{
    share2_t r;
    r.s0 = a.s0 ^ b.s0;
    r.s1 = a.s1 ^ b.s1;
    return r;
}

share2_t xor_refresh(share2_t a, share2_t b)
{
    share2_t r;
    uint32_t z = rng32();

    r.s0 = (a.s0 ^ b.s0) ^ z;
    r.s1 = (a.s1 ^ b.s1) ^ z;

    return r;
}

share2_t sec_not(share2_t a)
{
    share2_t r;
    r.s0 = ~a.s0;
    r.s1 = a.s1;
    return r;
}

/* ISW masked AND */
share2_t sec_and(share2_t a, share2_t b)
{
    share2_t r;
    uint32_t m = rng32();

    uint32_t t00 = a.s0 & b.s0;
    uint32_t t01 = a.s0 & b.s1;
    uint32_t t10 = a.s1 & b.s0;
    uint32_t t11 = a.s1 & b.s1;

    r.s0 = t00 ^ m;
    r.s1 = t01 ^ t10 ^ t11 ^ m;

    return r;
}

/* DOM-inspired first-order AND */
share2_t dom_and(share2_t a, share2_t b)
{
    share2_t r;
    uint32_t rand = rng32();

    uint32_t p00 = a.s0 & b.s0;
    uint32_t p11 = a.s1 & b.s1;
    uint32_t cross = (a.s0 & b.s1) ^ (a.s1 & b.s0);

    r.s0 = p00 ^ rand;
    r.s1 = p11 ^ cross ^ rand;

    return r;
}

share2_t mult_bool_isw(share2_t a, share2_t b)
{
    return sec_and(a, b);
}

share2_t mult_bool_dom(share2_t a, share2_t b)
{
    return dom_and(a, b);
}

share2_t sec_or(share2_t a, share2_t b)
{
    return sec_not(sec_and(sec_not(a), sec_not(b)));
}

share2_t or_dom(share2_t a, share2_t b)
{
    return sec_not(dom_and(sec_not(a), sec_not(b)));
}

share2_t sec_ref(share2_t a)
{
    share2_t r;
    uint32_t z = rng32();

    r.s0 = a.s0 ^ z;
    r.s1 = a.s1 ^ z;

    return r;
}


ashare2_t arith_ref_q(ashare2_t a, uint32_t q)
{
    ashare2_t r;
    uint32_t z = rng32() % q;

    r.s0 = (a.s0 + z) % q;
    r.s1 = (a.s1 + q - z) % q;

    return r;
}


uint32_t full_xor(share2_t a)
{
    share2_t r = sec_ref(a);
    return r.s0 ^ r.s1;
}

share4_t sec_exp(share2_t a)
{
    share4_t r4;
    share2_t rand_shares;
    share2_t first_half;

    rand_shares.s0 = rng32();
    rand_shares.s1 = rng32();

    first_half = sec_xor(a, rand_shares);

    r4.s0 = first_half.s0;
    r4.s1 = first_half.s1;
    r4.s2 = rand_shares.s0;
    r4.s3 = rand_shares.s1;

    return r4;
}


ashare2_t arith_mult_isw_q(ashare2_t a, ashare2_t b, uint32_t q)
{
    ashare2_t r;
    uint32_t rr = rng32() % q;

    uint32_t p00 = ((uint64_t)a.s0 * b.s0) % q;
    uint32_t p01 = ((uint64_t)a.s0 * b.s1) % q;
    uint32_t p10 = ((uint64_t)a.s1 * b.s0) % q;
    uint32_t p11 = ((uint64_t)a.s1 * b.s1) % q;

    r.s0 = (p00 + rr) % q;
    r.s1 = (p01 + p10 + p11 + q - rr) % q;

    return r;
}

ashare2_t arith_mult_double_sni_q(ashare2_t a, ashare2_t b, uint32_t q)
{
    ashare2_t ar = arith_ref_q(a, q);
    return arith_mult_isw_q(ar, b, q);
}


share2_t sec_add(share2_t a, share2_t b)
{
    uint32_t x = unmask_bool(a);
    uint32_t y = unmask_bool(b);
    return mask_bool(x + y);
}

share2_t sec_sub(share2_t a, share2_t b)
{
    uint32_t x = unmask_bool(a);
    uint32_t y = unmask_bool(b);
    return mask_bool(x - y);
}

share2_t sec_add_chain_q(share2_t x, share2_t yprime, uint32_t q, int final_add)
{
    share2_t z = sec_add(x, yprime);

    uint32_t zu = unmask_bool(z);
    uint32_t cc = (zu >> 31) & 1u;

    share2_t cc_share = mask_bool(cc);
    cc_share = sec_ref(cc_share);

    uint32_t cc_val = unmask_bool(cc_share) & 1u;

    uint32_t corr;
    share2_t c_masked;

    if (final_add) {
        corr = cc_val * q;
    } else {
        corr = ((cc_val ^ 1u) & 1u) ? (uint32_t)(-((int32_t)q)) : 0u;
    }

    c_masked = mask_bool(corr);
    return sec_add(z, c_masked);
}

static inline uint32_t psi_u32(uint32_t x, uint32_t r)
{
    return (x ^ r) - r;
}

share2_t a2b_first_order_q(ashare2_t a, uint32_t q)
{
    (void)q;

    uint32_t A = a.s0;
    uint32_t r = a.s1;

    uint32_t gamma = rng32();
    uint32_t T = gamma << 1;
    uint32_t x0 = gamma ^ r;
    uint32_t Omega = gamma & x0;

    x0 = T ^ A;
    gamma = gamma ^ x0;
    gamma = gamma & r;
    Omega = Omega ^ gamma;
    gamma = T & A;
    Omega = Omega ^ gamma;

    for (int j = 1; j < 32; j++) {
        gamma = T & r;
        gamma = gamma ^ Omega;
        T = T & A;
        gamma = gamma ^ T;
        T = gamma << 1;
    }

    x0 = x0 ^ T;

    share2_t out;
    out.s0 = x0;
    out.s1 = r;

    return out;
}

/*
 A2B — Goubin classic linear version (first-order, mod 2^32)
 From theorem 2 recursion recalled in 2014-891.
 */
share2_t a2b_goubin_classic_u32(ashare2_t a)
{
    uint32_t A = a.s0;
    uint32_t r = a.s1;

    uint32_t u = 0;
    uint32_t Ar = A ^ r;
    uint32_t And = A & r;

    for (int i = 0; i < 31; i++) {
        u = 2 * ((u & Ar) ^ And);
    }

    share2_t out;
    out.s0 = A ^ u;
    out.s1 = r;

    return out;
}

//// B2A — GoubinSNI (first-order, mod 2^32)
ashare2_t sec_shareconvert_b2a_classic_q(share2_t b, uint32_t q)
{
    (void)q;

    uint32_t s = rng32();
    uint32_t a1 = b.s0 ^ s;
    uint32_t a2 = b.s1 ^ s;

    uint32_t r = rng32();
    uint32_t u = a1 ^ psi_u32(a1, r ^ a2);

    ashare2_t out;
    out.s0 = u ^ psi_u32(a1, r);
    out.s1 = a2;

    return out;
}

/*
  B2A — Goubin original (first-order, mod 2^32)
  Using the affine Psi decomposition.
 */
ashare2_t b2a_goubin_u32(share2_t b)
{
    uint32_t x0 = b.s0;
    uint32_t r  = b.s1;
    uint32_t r2 = rng32();

    uint32_t A = psi_u32(x0, r ^ r2) ^ psi_u32(x0, r2) ^ x0;

    ashare2_t out;
    out.s0 = A;
    out.s1 = r;

    return out;
}
