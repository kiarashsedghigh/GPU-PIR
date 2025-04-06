#include "pirgb.h"
#include <stdio.h>
#include "poly.h"
#include <openssl/bn.h>

int main(void) {
    /* Set seed for the randomness */
    srand(time(NULL));

#if defined(USE_BIGNUM)
    BIGNUM * modulus = BN_new();
    BN_set_word(modulus, MODULUS);

    /* Creating a database */
    Database db;
    database_new(&db);

    /* Generating l distinct indices */
    BIGNUM * L_values [DBS];
    for (int i = 0; i < DBS; i++) {
        L_values[i] = BN_new();
        BN_rand_range(L_values[i], modulus);
    }

    /* Generating the 2D request/response arrays */
    BIGNUM * request_matrix [DBS][R];
    BIGNUM * response_matrix [DBS][S];

    for (int i = 0; i < DBS; i++){
        for (int j = 0; j < R; j++)
            request_matrix[i][j] = BN_new();
        for (int j = 0; j < S; j++)
            response_matrix[i][j] = BN_new();
    }

    /* Run PIR user */
    const int record_index = 0;
    pirgb_user(record_index, request_matrix, L_values);

    /* Run PIR server */
    for (int i = 0; i < DBS; i++)
        pirgb_server(&db, request_matrix[i], response_matrix[i]);

    for (int i = 0; i < DBS; i++){
        for (int j = 0; j < S; j++) {
            BN_print_fp(stdout, response_matrix[i][j]);
            printf("   ");
        }
        printf("\n");
    }

#else
    /* Creating a database */
    Database db;
    database_new(&db);
    database_print(&db);


    /* Generating l distinct indices */
    long long L_values[DBS];
    for (int i = 0; i < DBS; i++)
        L_values[i] = rand() % MODULUS;

    /* Generating the 2D request/response arrays */
    long long request_matrix[DBS][R];
    long long response_matrix[DBS][S];

    /* Run PIR user */
    const int record_index = 0;
    pirgb_user(record_index, request_matrix, L_values);

    /* Run PIR server */
    for (int i = 0; i < DBS; i++)
        pirgb_server(&db, request_matrix[i], response_matrix[i]);

#endif

    return 0;
}
