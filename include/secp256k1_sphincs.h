#ifndef SECP256K1_SPHINCS_H
#define SECP256K1_SPHINCS_H

#include "secp256k1.h"

#ifdef __cplusplus
extern "C" {
#endif

/** SPHINCS+ SHAKE256-128f-simple public key size in bytes. */
#define SECP256K1_SPHINCS_SHAKE256_128F_PUBLICKEYBYTES 32

/** SPHINCS+ SHAKE256-128f-simple secret key size in bytes. */
#define SECP256K1_SPHINCS_SHAKE256_128F_SECRETKEYBYTES 64

/** SPHINCS+ SHAKE256-128f-simple signature size in bytes. */
#define SECP256K1_SPHINCS_SHAKE256_128F_BYTES 17088

/** SPHINCS+ SHAKE256-128f-simple seed size in bytes. */
#define SECP256K1_SPHINCS_SHAKE256_128F_SEEDBYTES 48

/**
 * Generate a SPHINCS+ SHAKE256-128f-simple key pair.
 *
 * Returns: 0 on success, non-zero on failure.
 *
 * Out: pk: 32-byte public key.
 *      sk: 64-byte secret key.
 */
SECP256K1_API int secp256k1_sphincs_shake256_128f_keypair(
    unsigned char *pk,
    unsigned char *sk
);

/**
 * Generate a SPHINCS+ SHAKE256-128f-simple key pair from a 48-byte seed.
 *
 * Returns: 0 on success, non-zero on failure.
 *
 * Out: pk: 32-byte public key.
 *      sk: 64-byte secret key.
 * In:  seed: 48-byte seed.
 */
SECP256K1_API int secp256k1_sphincs_shake256_128f_seed_keypair(
    unsigned char *pk,
    unsigned char *sk,
    const unsigned char *seed
);

/**
 * Create a SPHINCS+ SHAKE256-128f-simple signature.
 *
 * Returns: 0 on success, non-zero on failure.
 *
 * Out: sig: 17088-byte signature buffer.
 *      siglen: resulting signature length.
 * In:  msg: message to sign.
 *      msglen: message length.
 *      sk: 64-byte secret key.
 */
SECP256K1_API int secp256k1_sphincs_shake256_128f_sign(
    unsigned char *sig,
    size_t *siglen,
    const unsigned char *msg,
    size_t msglen,
    const unsigned char *sk
);

/**
 * Verify a SPHINCS+ SHAKE256-128f-simple signature.
 *
 * Returns: 0 if the signature is valid, non-zero otherwise.
 *
 * In: sig: signature.
 *     siglen: signature length.
 *     msg: original message.
 *     msglen: message length.
 *     pk: 32-byte public key.
 */
SECP256K1_API int secp256k1_sphincs_shake256_128f_verify(
    const unsigned char *sig,
    size_t siglen,
    const unsigned char *msg,
    size_t msglen,
    const unsigned char *pk
);


/** SPHINCS+ SHAKE256-256f-simple public key size in bytes. */
#define SECP256K1_SPHINCS_SHAKE256_256F_PUBLICKEYBYTES 64

/** SPHINCS+ SHAKE256-256f-simple secret key size in bytes. */
#define SECP256K1_SPHINCS_SHAKE256_256F_SECRETKEYBYTES 128

/** SPHINCS+ SHAKE256-256f-simple signature size in bytes. */
#define SECP256K1_SPHINCS_SHAKE256_256F_BYTES 49856

/** SPHINCS+ SHAKE256-256f-simple seed size in bytes. */
#define SECP256K1_SPHINCS_SHAKE256_256F_SEEDBYTES 96

SECP256K1_API int secp256k1_sphincs_shake256_256f_keypair(
    unsigned char *pk,
    unsigned char *sk
);

SECP256K1_API int secp256k1_sphincs_shake256_256f_seed_keypair(
    unsigned char *pk,
    unsigned char *sk,
    const unsigned char *seed
);

SECP256K1_API int secp256k1_sphincs_shake256_256f_sign(
    unsigned char *sig,
    size_t *siglen,
    const unsigned char *msg,
    size_t msglen,
    const unsigned char *sk
);

SECP256K1_API int secp256k1_sphincs_shake256_256f_verify(
    const unsigned char *sig,
    size_t siglen,
    const unsigned char *msg,
    size_t msglen,
    const unsigned char *pk
);

#ifdef __cplusplus
}
#endif

#endif /* SECP256K1_SPHINCS_H */
