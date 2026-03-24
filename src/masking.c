#include "../include/masking.h"
#include <stdlib.h>
#include <time.h>

//Initializes the pseudo-random generator , called just when the program start
void rng_init(void) 
{
    srand((unsigned)time(NULL));///srand pour ne donnera pas tjs la meme suite de nb à chaque exécution
    ////time(null) pour prend l’heure actuelle comme graine
}

//returns a random 32bit value (used as mask/randomness
uint32_t rng32(void) 
{
///Parce que rand seul ne donne pas tjs 32 bits utiles selon le compilateur donc on combine 2 appels
///un décalé à gauche de 16 bits et un autre XOR avec le premier
    return ((uint32_t)rand() << 16) ^ (uint32_t)rand();
}

///creates shares(x0, x1) such that x = x0 XOR x1
share2_t mask_bool(uint32_t x) 
{
///s0XORs1= s0 XOR (x XOR s0)=x c’est sécurisé car s0 seul est aléatoire et s1 seul ressemble aussi à une val aléatoire
    share2_t a;
    a.s0 = rng32();///// random mask
    a.s1 = x ^ a.s0;///// masked share
    return a;
}

////reconstructs x by xoring the two shares
uint32_t unmask_bool(share2_t a)
 {
    return a.s0 ^ a.s1;
}

uint32_t unmask_bool4(share4_t a)
{
    return a.s0 ^ a.s1 ^ a.s2 ^ a.s3;
}

ashare2_t mask_arith_q(uint32_t x, uint32_t q)
{
    ashare2_t a;
    a.s0 = rng32() % q;
    a.s1 = (x + q - a.s0) % q;
    return a;
}

uint32_t unmask_arith_q(ashare2_t a, uint32_t q)
{
    return (a.s0 + a.s1) % q;
}