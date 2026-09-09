#include <stdio.h>
#include <string.h>

#include "secp256k1_sphincs.h"

int main(void) {
    unsigned char pk[SECP256K1_SPHINCS_SHAKE256_128F_PUBLICKEYBYTES];
    unsigned char sk[SECP256K1_SPHINCS_SHAKE256_128F_SECRETKEYBYTES];
    unsigned char sig[SECP256K1_SPHINCS_SHAKE256_128F_BYTES];
    const unsigned char msg[] = "SPHINCS+ integration test";
    size_t siglen = 0;

    if (secp256k1_sphincs_shake256_128f_keypair(pk, sk) != 0) {
        fprintf(stderr, "SPHINCS+ key generation failed\n");
        return 1;
    }

    if (secp256k1_sphincs_shake256_128f_sign(
            sig, &siglen, msg, sizeof(msg) - 1, sk) != 0) {
        fprintf(stderr, "SPHINCS+ signing failed\n");
        return 1;
    }

    if (siglen != SECP256K1_SPHINCS_SHAKE256_128F_BYTES) {
        fprintf(stderr, "Unexpected signature length: %lu\n", (unsigned long)siglen);
        return 1;
    }

    if (secp256k1_sphincs_shake256_128f_verify(
            sig, siglen, msg, sizeof(msg) - 1, pk) != 0) {
        fprintf(stderr, "SPHINCS+ verification failed\n");
        return 1;
    }

    /* Test 1: modified signature must be rejected */
    sig[0] ^= 0x01;

    if (secp256k1_sphincs_shake256_128f_verify(
            sig, siglen, msg, sizeof(msg) - 1, pk) == 0) {
        fprintf(stderr, "Tampered signature was incorrectly accepted\n");
        return 1;
    }

    /* Restore original signature */
    sig[0] ^= 0x01;

    /* Test 2: modified message must be rejected */
    {
        unsigned char tampered_msg[sizeof(msg) - 1];

        memcpy(tampered_msg, msg, sizeof(msg) - 1);
        tampered_msg[0] ^= 0x01;

        if (secp256k1_sphincs_shake256_128f_verify(
                sig, siglen, tampered_msg, sizeof(tampered_msg), pk) == 0) {
            fprintf(stderr, "Tampered message was incorrectly accepted\n");
            return 1;
        }
    }

    printf("SPHINCS+ SHAKE256-128f-simple integration test: PASS\n");
    printf("Original signature: ACCEPTED\n");
    printf("Tampered signature: REJECTED\n");
    printf("Tampered message:   REJECTED\n");
    printf("Public key: %d bytes\n",
           SECP256K1_SPHINCS_SHAKE256_128F_PUBLICKEYBYTES);
    printf("Secret key: %d bytes\n",
           SECP256K1_SPHINCS_SHAKE256_128F_SECRETKEYBYTES);
    printf("Signature:  %lu bytes\n", (unsigned long)siglen);

    return 0;
}
