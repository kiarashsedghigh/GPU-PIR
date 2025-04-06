#include "shamir.h"







//
//void key_extract_for_BSs(const EC_GROUP *curve, const EC_POINT *Q_AMF, const BIGNUM *sk_AMF, const BIGNUM *order, BN_CTX *ctx,
//    EC_POINT ***PK_BS_vec, BIGNUM ***sk_BS_vec, BIGNUM **sk_BS_root_out, EC_POINT **QID_BS_out, BIGNUM **h_BS, int *n_out, int *t_out, double *BS_KeyExtraction_time) {
//
//    unsigned char hash[SHA256_DIGEST_LENGTH];
//
//    // printf("Enter number of base stations n = ");
//    // scanf("%d", &n);
//    // printf("Enter threshold t = ");
//    // scanf("%d", &t);
//
//    // if (t > n || t < 1) {
//    //     printf("Invalid threshold. Ensure 1 ≤ t ≤ n.\n");
//    //     return;
//    // }
//    int n = 3;
//    int t = 2;
//
//    *n_out = n;
//    *t_out = t;
//
//    // Step 1: Random alpha_bs and compute r = H1(alpha_bs)
//    BIGNUM *alpha_bs = BN_new();
//    double get_alpha_BS_start_time = get_time_ns();
//    BN_rand_range(alpha_bs, order);
//    double get_alpha_BS_end_time = get_time_ns();
//    double elapsed_time_get_alpha_BS = (get_alpha_BS_end_time - get_alpha_BS_start_time) / 1e9;  // Convert ns to seconds
//
//    BIGNUM *r2 = BN_new();
//    unsigned char alpha_bin[BN_num_bytes(alpha_bs)];
//    BN_bn2bin(alpha_bs, alpha_bin);
//    double hash_alpha_BS_start_time = get_time_ns();
//    SHA256(alpha_bin, BN_num_bytes(alpha_bs), hash);
//    double hash_alpha_BS_end_time = get_time_ns();
//    double elapsed_time_get_hash_alpha_BS = (hash_alpha_BS_end_time - hash_alpha_BS_start_time) / 1e9;  // Convert ns to seconds
//    BN_bin2bn(hash, SHA256_DIGEST_LENGTH, r2);
//
//    // QID_BS = g^r2
//    *QID_BS_out = EC_POINT_new(curve);
//    double Q_BS_start_time = get_time_ns();
//    EC_POINT_mul(curve, *QID_BS_out, r2, NULL, NULL, ctx);
//    double Q_BS_end_time = get_time_ns();
//    double elapsed_time_get_Q_BS = (Q_BS_end_time - Q_BS_start_time) / 1e9;  // Convert ns to seconds
//
//    write_point_to_file(curve, *QID_BS_out, "Q_BS.txt", ctx);
//
//
//    // h_BS = H1(ID_BS || Q_AMF || QID_BS)
//    double H1_BS_start_time = get_time_ns();
//    const char *ID_BS = "BS-000001";
//    char *qamf_hex = EC_POINT_point2hex(curve, Q_AMF, POINT_CONVERSION_UNCOMPRESSED, ctx);
//    char *qbs_hex = EC_POINT_point2hex(curve, *QID_BS_out, POINT_CONVERSION_UNCOMPRESSED, ctx);
//
//    size_t concat_len = strlen(ID_BS) + strlen(qamf_hex) + strlen(qbs_hex);
//    unsigned char *concat = malloc(concat_len + 1);
//    snprintf((char *)concat, concat_len + 1, "%s%s%s", ID_BS, qamf_hex, qbs_hex);
//
//    *h_BS = BN_new();
//    SHA256(concat, strlen((char *)concat), hash);
//    double H1_BS_end_time = get_time_ns();
//    double elapsed_time_H1_BS = (H1_BS_end_time - H1_BS_start_time) / 1e9;  // Convert ns to seconds
//
//    BN_bin2bn(hash, SHA256_DIGEST_LENGTH, *h_BS);
//    print_bn("h_BS", *h_BS);
//    // write_bn_to_file(*h_BS, "h_BS.txt");
//
//    // sk_BS_root = sk_AMF * h_BS + r2
//    BIGNUM *sk_BS_root = BN_new();
//    double sk_BS_start_time = get_time_ns();
//    BN_mod_mul(sk_BS_root, sk_AMF, *h_BS, order, ctx);
//    BN_mod_add(sk_BS_root, sk_BS_root, r2, order, ctx);
//    double sk_BS_end_time = get_time_ns();
//    double elapsed_time_sk_BS = (sk_BS_end_time - sk_BS_start_time) / 1e9;  // Convert ns to seconds
//
//    *sk_BS_root_out = BN_dup(sk_BS_root);
//
//    print_bn("sk_BS_root", sk_BS_root);
//
//    // Step 2: Generate secret sharing polynomial
//    double Lagrange_start_time = get_time_ns();
//
//


//
//
//
///*============================================
//        Test Key Extraction Function (BSs)
//==============================================*/
//void test_BS_reconstruction(const BIGNUM **sk_BS_vec, const int *indices, int t, const BIGNUM *order, BN_CTX *ctx, const BIGNUM *expected_root) {
//    BIGNUM *reconstructed = BN_new();
//    BN_zero(reconstructed);
//
//    for (int i = 0; i < t; i++) {
//        BIGNUM *lambda_i = BN_new();
//        BN_one(lambda_i);
//
//        BIGNUM *xi = BN_new();
//        BN_set_word(xi, indices[i]);  // xi = x_i (1-based)
//
//        for (int j = 0; j < t; j++) {
//            if (i == j) continue;
//
//            BIGNUM *xj = BN_new(); BN_set_word(xj, indices[j]);
//
//            BIGNUM *num = BN_new();  // -xj mod order
//            BIGNUM *den = BN_new();  // xi - xj mod order
//            BIGNUM *inv = BN_new();
//            BIGNUM *temp = BN_new();
//
//            // num = -xj mod order
//            BN_mod_sub(num, BN_value_one(), xj, order, ctx);
//            BN_mod_sub(num, order, xj, order, ctx);  // safer
//
//            // den = xi - xj mod order
//            BN_mod_sub(den, xi, xj, order, ctx);
//
//            // inv = den^-1 mod order
//            if (!BN_mod_inverse(inv, den, order, ctx)) {
//                fprintf(stderr, "Error: no modular inverse for denominator in Lagrange basis\n");
//                exit(EXIT_FAILURE);
//            }
//
//            // temp = num * inv mod order
//            BN_mod_mul(temp, num, inv, order, ctx);
//
//            // lambda_i *= temp mod order
//            BN_mod_mul(lambda_i, lambda_i, temp, order, ctx);
//
//            // Clean up
//            BN_free(xj); BN_free(num); BN_free(den); BN_free(inv); BN_free(temp);
//        }
//
//        // Debug print λ_i
//        printf("λ_%d (Lagrange coeff for x = %d): ", indices[i], indices[i]);
//        print_bn("", lambda_i);
//
//        // Multiply λ_i with sk_BS_i
//        BIGNUM *term = BN_new();
//        BN_mod_mul(term, sk_BS_vec[indices[i] - 1], lambda_i, order, ctx);
//
//        // Add to reconstruction
//        BN_mod_add(reconstructed, reconstructed, term, order, ctx);
//
//        BN_free(lambda_i);
//        BN_free(xi);
//        BN_free(term);
//    }
//
//    BN_mod(reconstructed, reconstructed, order, ctx);
//
//    printf("\nReconstructed root secret key from shares:\n");
//    print_bn("sk_BS_reconstructed", reconstructed);
//
//    if (BN_cmp(reconstructed, expected_root) == 0) {
//        printf("\n✅ Reconstruction SUCCESS: matches original sk_BS_root.\n");
//    } else {
//        printf("\n❌ Reconstruction FAILED: does not match original sk_BS_root.\n");
//    }
//
//    BN_free(reconstructed);
//}
//
//
