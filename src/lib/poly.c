#include "poly.h"
#include <math.h>

#if defined(USE_BIGNUM)

void Polynomial_init(Polynomial *poly, const BIGNUM *modulus) {
    for (int i = 0; i < T + 1; i++) {
        poly->coeff[i] = BN_new();
        BN_rand_range(poly->coeff[i], modulus);
    }
    poly->ctx = BN_CTX_new();
    poly->modulus = BN_dup(modulus);
}

void Polynomial_free(const Polynomial *poly) {
    for (int i = 0; i < T + 1; i++)
        BN_free(poly->coeff[i]);
    BN_CTX_free(poly->ctx);
    BN_free(poly->modulus);
}

Polynomial *Polynomial_gen_n(const int n, const BIGNUM *modulus) {
    Polynomial *poly = malloc(n * sizeof(Polynomial));
    for (int i = 0; i < n; i++)
        Polynomial_init(&poly[i], modulus);
    return poly;
}

void Polynomial_free_n(Polynomial *poly, const int n) {
    for (int i = 0; i < n; i++)
        Polynomial_free(&poly[i]);
    free(poly);
}


BIGNUM *Polynomial_eval_bn(const Polynomial *poly, const BIGNUM *input) {
    BIGNUM *result = BN_new();
    BIGNUM *term = BN_new();
    BIGNUM *exp = BN_new();

    for (int i = 0; i < T + 1; i++) {
        BN_set_word(exp, i);
        BN_exp(term, input, exp, poly->ctx);
        BN_mul(term, poly->coeff[i], term, poly->ctx);
        BN_mod_add(result, result, term, poly->modulus, poly->ctx);
    }

    BN_free(term);
    BN_free(exp);
    return result;
}

BIGNUM *Polynomial_eval_i(const Polynomial *poly, const int input) {
    BIGNUM *input_bn = BN_new();
    BN_set_word(input_bn, input);
    BIGNUM *result = Polynomial_eval_bn(poly, input_bn);
    BN_free(input_bn);
    return result;
}


//todo free memory
void Polynomial_print(const Polynomial *poly) {
    printf("f(x): ");
    for (int i = 0; i < T + 1; i++) {
        if (i > 0)
            printf(" + ");
        printf("%s", BN_bn2dec(poly->coeff[i]));
        if (i > 0)
            printf(" * x^%d ", i);
    }
    printf(" mod %s", BN_bn2dec(poly->modulus));
    printf("\n");
}

#else


void Polynomial_init(Polynomial *poly, const long long modulus) {
    for (int i = 0; i < T + 1; i++) {
        poly->coeff[i] = rand() % modulus;
    }
    poly->modulus = modulus;
}

Polynomial *Polynomial_gen_n(const int n, const long long modulus) {
    Polynomial *poly = malloc(n * sizeof(Polynomial));
    for (int i = 0; i < n; i++)
        Polynomial_init(&poly[i], modulus);
    return poly;
}


long long Polynomial_eval_i(const Polynomial *poly, const long long input) {

    long long result = 0;

    for (int i = 0; i < T + 1; i++) {
        const long long term = (long long)pow(input, i) % poly->modulus;;
        result = (result + term * poly->coeff[i]) % poly->modulus;
    }
    return result;
}

//todo free memory
void Polynomial_print(const Polynomial *poly) {
    printf("f(x): ");
    for (int i = 0; i < T + 1; i++) {
        if (i > 0)
            printf(" + ");
        printf("%lld ", poly->coeff[i]);
        if (i > 0)
            printf(" * x^%d ", i);
    }
    printf(" mod %lld", poly->modulus);
    printf("\n");
}


#endif