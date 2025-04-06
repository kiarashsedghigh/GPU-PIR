#include <stdio.h>
#include "poly.h"
#include <openssl/bn.h>

int main(void) {
    const long long modulus = MODULUS;
    const Polynomial * poly = Polynomial_gen_n(1, modulus);

    /*Evaluating the polynomial at 0 and 1 */
    Polynomial_print(&poly[0]);
    printf("f(0): %lld\n", (Polynomial_eval_i(&poly[0], 0)));
    printf("f(1): %lld\n", (Polynomial_eval_i(&poly[0], 1)));

    return 0;
}