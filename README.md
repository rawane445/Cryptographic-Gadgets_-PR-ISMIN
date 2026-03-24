# Cryptographic-Gadgets_-PR-ISMIN : Performance Evaluation of Masked Cryptographic Gadgets
This project presents the implementation and evaluation of masked cryptographic gadgets designed to protect against side-channel attacks.

## Context

While cryptographic algorithms are mathematically secure, their implementations may leak sensitive information through physical effects such as power consumption or timing. Masking is a widely used countermeasure that protects secrets by splitting them into multiple random shares.

## Project Overview

In this project, we implement and analyze several families of masked gadgets:

- Boolean masking (first-order, 2 shares)
- Arithmetic masking modulo q
- Conversion algorithms between Boolean and arithmetic masking
- Performance evaluation of each gadget

The goal is to bridge the gap between theoretical constructions and practical implementations.

## Implemented Gadgets

### 1. Linear Boolean Gadgets
- `sec_xor`
- `sec_not`

### 2. Non-linear Boolean Gadgets
- `sec_and` (ISW)
- `dom_and` (DOM)
- `sec_or`, `or_dom`

### 3. Refresh and Helper Gadgets
- `xor_refresh`
- `sec_ref`
- `arith_ref_q`
- `full_xor`
- `sec_exp`

### 4. Experimental Arithmetic Gadgets
- `arith_mult_isw_q`
- `arith_mult_double_sni_q`

### 5. Conversion Gadgets (A2B / B2A)
- `a2b_first_order_q` (Coron et al.)
- `a2b_goubin_classic_u32`
- `sec_shareconvert_b2a_classic_q`
- `b2a_goubin_u32`

### 6. Baseline Functions (for testing purposes)
- `sec_add`
- `sec_sub`
- `sec_add_chain_q`

## Performance Evaluation

Each gadget is executed up to 10 million iterations to obtain stable timing measurements.

The results highlight key insights:
- Linear operations are extremely fast
- Non-linear operations require additional randomness
- Arithmetic gadgets are more expensive
- Conversion algorithms are the most costly operations

##Technologies

- Language: C
- Standard libraries: `stdint`, `time`, `stdlib`
- Platform: Tested on Windows (MinGW / PowerShell)

## References

- Ishai, Sahai, Wagner (ISW03) – Private Circuits  
- Gross et al. (DOM16) – Domain-Oriented Masking  
- Goubin (2001) – Boolean and Arithmetic Masking  
- Coron et al. (2014) – A2B Conversion  
- GoubinSNI (2017) – B2A Conversion  

## Author

**Rawane AL ZOHBI**  
---

This project aims to provide both a practical implementation and a deeper understanding of masking techniques used in modern cryptographic systems.
