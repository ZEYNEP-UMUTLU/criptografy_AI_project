#include "secp256k1_sphincs.h"

/*
 * The 256f implementation is namespace-isolated in CMake.
 * Keep the public secp256k1 wrapper API unchanged while mapping
 * the internal SPHINCS+ API to its prefixed symbols.
 */
#define crypto_sign_keypair sphincs256f_crypto_sign_keypair
#define crypto_sign_seed_keypair sphincs256f_crypto_sign_seed_keypair
#define crypto_sign_signature sphincs256f_crypto_sign_signature
#define crypto_sign_verify sphincs256f_crypto_sign_verify

#include "sphincs_shake256_256f/api.h"

SECP256K1_API int secp256k1_sphincs_shake256_256f_keypair(
    unsigned char *pk,
    unsigned char *sk
) {
    return crypto_sign_keypair(pk, sk);
}

SECP256K1_API int secp256k1_sphincs_shake256_256f_seed_keypair(
    unsigned char *pk,
    unsigned char *sk,
    const unsigned char *seed
) {
    return crypto_sign_seed_keypair(pk, sk, seed);
}

SECP256K1_API int secp256k1_sphincs_shake256_256f_sign(
    unsigned char *sig,
    size_t *siglen,
    const unsigned char *msg,
    size_t msglen,
    const unsigned char *sk
) {
    return crypto_sign_signature(sig, siglen, msg, msglen, sk);
}

SECP256K1_API int secp256k1_sphincs_shake256_256f_verify(
    const unsigned char *sig,
    size_t siglen,
    const unsigned char *msg,
    size_t msglen,
    const unsigned char *pk
) {
    return crypto_sign_verify(sig, siglen, msg, msglen, pk);
}
