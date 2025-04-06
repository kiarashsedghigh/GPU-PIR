#ifndef POLY_H
#define POLY_H

#include <openssl/ec.h>
#include "config.h"

#if defined(USE_BIGNUM)
typedef struct {
    BIGNUM * coeff[T + 1];
    BN_CTX * ctx;
    BIGNUM * modulus;
}Polynomial;


void Polynomial_init(Polynomial *poly, const BIGNUM * modulus);

void Polynomial_free(const Polynomial *poly);

void Polynomial_free_n(Polynomial *poly, int n);

Polynomial * Polynomial_gen_n(int n, const BIGNUM * modulus);

BIGNUM * Polynomial_eval_i(const Polynomial *poly, int input);

BIGNUM * Polynomial_eval_bn(const Polynomial *poly, const BIGNUM *input);

void Polynomial_print(const Polynomial *poly);

#else

typedef struct {
    long long coeff[T + 1];
    long long modulus;
}Polynomial;


void Polynomial_init(Polynomial *poly, const long long modulus);

Polynomial * Polynomial_gen_n(int n, long long modulus);

long long Polynomial_eval_i(const Polynomial *poly, long long input);

void Polynomial_print(const Polynomial *poly);


#endif




#endif //POLY_H
