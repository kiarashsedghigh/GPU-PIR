#ifndef PIRGB_H
#define PIRGB_H

#include "config.h"
#include "poly.h"


#if defined(USE_BIGNUM)

typedef struct {
    BIGNUM * records[R][S];
}Database;


void database_new(Database * db);

void database_free(const Database * db);

void pirgb_user(int record_index, BIGNUM * Poly_evals [DBS][R], BIGNUM * L_values [DBS]);

void pirgb_server(const Database *db, BIGNUM * request [R], BIGNUM * response [S]);

#else

typedef struct {
    long long records[R][S];
}Database;


void database_new(Database * db);

void pirgb_user(int record_index, long long Poly_evals [DBS][R], long long L_values [DBS]);

void pirgb_server(const Database *db, const long long request [R], long long response [S]);

void database_print(const Database * db);

#endif

#endif
