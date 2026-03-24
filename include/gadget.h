#pragma once
#include <stdint.h>
#include "masking.h"

/* Main literature references used in the project:
1) ISW03 Ishai, Sahai, Wagner=>Private Circuits: Securing Hardware Against Probing Attacks
2) DOM16 Gross, Mangard, Korak=>Domain-Oriented Masking
3) Goubin01=> A Sound Method for Switching Between Boolean and Arithmetic Masking
4) Coron14=>Conversion from Arithmetic to Boolean Masking with Logarithmic Complexity
5) GoubinSNI17=>High-Order Conversion From Boolean to Arithmetic Masking
*/

/// 8.1 Linear Boolean gadgets (section 8.1 dans le rapport)
// secure XOR on Boolean shares
share2_t sec_xor(share2_t a, share2_t b);
///Boolean NOT on masked shares
share2_t sec_not(share2_t a);

// 8.2 Non-linear Boolean gadgets  (section 8.2 dans le rapport) 
//ISW-style masked AND
share2_t sec_and(share2_t a, share2_t b);
//DOM-inspired masked AND 
share2_t dom_and(share2_t a, share2_t b);
//aliases used for benchmarking Boolean multiplication
share2_t mult_bool_isw(share2_t a, share2_t b);
share2_t mult_bool_dom(share2_t a, share2_t b);
///OR derived from masked AND + NOT
share2_t sec_or(share2_t a, share2_t b);
share2_t or_dom(share2_t a, share2_t b);


//// 8.3 Refresh and helper gadgets (section 8.3 dans le rapport)
// XOR followed by refresh
share2_t xor_refresh(share2_t a, share2_t b);

/// Boolean share refresh
share2_t sec_ref(share2_t a);

/// arithmetic share refresh modulo q
ashare2_t arith_ref_q(ashare2_t a, uint32_t q);

/// recombine Boolean shares
uint32_t full_xor(share2_t a);

/// expand 2 Boolean shares into 4 Boolean shares 
share4_t sec_exp(share2_t a);


/// 8.4 Experimental arithmetic gadgets   
//// experimental arithmetic multiplication modulo q 
ashare2_t arith_mult_isw_q(ashare2_t a, ashare2_t b, uint32_t q);

/// experimental refresh-then-multiply variant 
ashare2_t arith_mult_double_sni_q(ashare2_t a, ashare2_t b, uint32_t q);


/// 8.5 Conversions between Boolean and arithmetic masking
/*
  Exact conversions modulo 2^32
  a2b_first_order_q:   Coron14 logarithmic A2B (first order, mod 2^32)
  a2b_goubin_classic_u32:Goubin01 linear A2B (first order, mod 2^32)
  sec_shareconvert_b2a_classic_q:GoubinSNI17 first-order B2A (mod 2^32)
  b2a_goubin_u32:Goubin01 original first-order B2A (mod 2^32)
  The parameter q is kept only for compatibility with the rest
  of the project, but the exact conversions below work modulo 2^32.
 */

//// A2B conversions 
share2_t a2b_first_order_q(ashare2_t a, uint32_t q);
share2_t a2b_goubin_classic_u32(ashare2_t a);

/// B2A conversions 
ashare2_t sec_shareconvert_b2a_classic_q(share2_t b, uint32_t q);
ashare2_t b2a_goubin_u32(share2_t b);


/// 8.6 Baseline functions that must be presented carefully

/*
 * These functions are kept for structure testing, experimentation,
 * or simple baselines. They should not be overclaimed as faithful
 * secure gadgets from the literature without additional justification.
 */

/// baseline add 
share2_t sec_add(share2_t a, share2_t b);

//// baseline subtract
share2_t sec_sub(share2_t a, share2_t b);

//// structural approximation for modular chained correction 
share2_t sec_add_chain_q(share2_t x, share2_t yprime, uint32_t q, int final_add);