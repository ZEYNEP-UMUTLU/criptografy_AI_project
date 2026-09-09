#include "secp256k1_sphincs.h"
#include "sphincs_shake256_128f/api.h"

SECP256K1_API int secp256k1_sphincs_shake256_128f_keypair(
    unsigned char *pk,
    unsigned char *sk
) {
    return spx_128f_keypair(pk, sk);
}

SECP256K1_API int secp256k1_sphincs_shake256_128f_seed_keypair(
    unsigned char *pk,
    unsigned char *sk,
    const unsigned char *seed
) {
    return spx_128f_seed_keypair(pk, sk, seed);
}

SECP256K1_API int secp256k1_sphincs_shake256_128f_sign(
    unsigned char *sig,
    size_t *siglen,
    const unsigned char *msg,
    size_t msglen,
    const unsigned char *sk
) {
    return spx_128f_signature(sig, siglen, msg, msglen, sk);
}

SECP256K1_API int secp256k1_sphincs_shake256_128f_verify(
    const unsigned char *sig,
    size_t siglen,
    const unsigned char *msg,
    size_t msglen,
    const unsigned char *pk
) {
    return spx_128f_verify(sig, siglen, msg, msglen, pk);
}
