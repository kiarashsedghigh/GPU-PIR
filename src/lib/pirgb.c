#include "pirgb.h"

#if defined(USE_BIGNUM)
void database_new(Database * db) {
  for (int i = 0; i < R; i++)
    for (int j = 0 ; j < S ;j++)
      db->records[i][j] = BN_new();

  //todo for test
  for (int i = 0; i < R; i++)
    for (int j = 0 ; j < S ;j++)
      BN_add_word(db->records[i][j], i + j);
}

void database_free(const Database * db) {
  for (int i = 0; i < R; i++)
    for (int j = 0 ; j < S ;j++)
      BN_free(db->records[i][j]);
}


void pirgb_user(const int record_index, BIGNUM * Poly_evals [DBS][R], BIGNUM * L_values [DBS]) {

  BIGNUM * modulus = BN_new();
  BN_set_word(modulus, MODULUS);

  /* Creating R random degree-t polynomials */
  const Polynomial * R_polys = Polynomial_gen_n(R, modulus);

  /* Set the secret of each polynomial to 0/1 based on the index */
  for (int i=0; i< R; i++)
    BN_set_word(R_polys[i].coeff[0], record_index == i ? 1 : 0);

  /* Compute the polynomials at values 1 - L for each database */
  for (int i = 0; i < DBS; i++)
    for (int j = 0; j < R; j++)
      Poly_evals[i][j] = Polynomial_eval_bn(&R_polys[j], L_values[i]);

  BN_free(modulus);
}


void pirgb_server(const Database *db, BIGNUM * request [R], BIGNUM * response [S]) {

  /* Set the modulus */
  BIGNUM * modulus = BN_new();
  BN_set_word(modulus, MODULUS);

  /* Compute the R * DB */
  for (int i = 0; i < S; i++)
    BN_zero(response[i]);

  /* Response is a linear combination of DB rows */
  BIGNUM * term = BN_new();
  BN_CTX * ctx = BN_CTX_new();
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < S; j++) {
      BN_mul(term , request[i], db->records[i][j], ctx);
      BN_mod_add(response[j], response[j], term, modulus, ctx);
    }
  }

  BN_free(term);
  BN_free(modulus);
  BN_CTX_free(ctx);
}

#else


void database_new(Database * db) {
  //todo for test
  for (int i = 0; i < R; i++)
    for (int j = 0 ; j < S ;j++)
      db->records[i][j] = i + j;
}

void database_print(const Database * db) {
  for (int i = 0; i < R; i++){
    for (int j = 0; j < S; j++) {
      printf("%lld  ", db->records[i][j]);
    }
    printf("\n");
  }
}


void pirgb_user(const int record_index, long long Poly_evals [DBS][R], long long L_values [DBS]) {

  /* Set the modulus */
  const long long modulus = MODULUS;

  /* Creating R random degree-t polynomials */
  Polynomial * R_polys = Polynomial_gen_n(R, modulus);

  /* Set the secret of each polynomial to 0/1 based on the index */
  for (int i=0; i< R; i++)
    R_polys[i].coeff[0] = record_index == i ? 1 : 0;

  /* Compute the polynomials at values 1 - L for each database */
  for (int i = 0; i < DBS; i++)
    for (int j = 0; j < R; j++)
      Poly_evals[i][j] = Polynomial_eval_i(&R_polys[j], L_values[i]);

}


void pirgb_server(const Database *db, const long long request [R], long long response [S]) {

  /* Set the modulus */
  const long long modulus = MODULUS;

  /* Compute the R * DB */
  for (int i = 0; i < S; i++)
    response[i] = 0;

  /* Response is a linear combination of DB rows */
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < S; j++) {
      const long long term = request[i] * db->records[i][j];
      response[j] = (response[j] + term) % modulus;
    }
  }
}


#endif
