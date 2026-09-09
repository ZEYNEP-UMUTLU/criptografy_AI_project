#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "secp256k1_sphincs.h"

#define PK_BYTES 64
#define SK_BYTES 128
#define SIG_BYTES 49856

int main(void) {
    unsigned char *pk = malloc(PK_BYTES);
    unsigned char *sk = malloc(SK_BYTES);
    unsigned char *sig = malloc(SIG_BYTES);
    unsigned char *sig_tampered = malloc(SIG_BYTES);

    const unsigned char message[] =
        "AI-selected SPHINCS+ SHAKE256-256f-simple test message";

    size_t siglen = 0;

    if (!pk || !sk || !sig || !sig_tampered) {
        fprintf(stderr, "ERROR: memory allocation failed\n");
        return 1;
    }

    printf("===== AI-SELECTED SPHINCS+ 256f TEST =====\n");
    printf("Selected parameter : sphincs-shake256-256f-simple\n");
    printf("Public key bytes   : %d\n", PK_BYTES);
    printf("Secret key bytes   : %d\n", SK_BYTES);
    printf("Signature bytes    : %d\n\n", SIG_BYTES);

    /* Key generation */
    printf("[1] Keypair generation...\n");

    if (secp256k1_sphincs_shake256_256f_keypair(pk, sk) != 0) {
        printf("FAIL: keypair\n");
        goto fail;
    }

    printf("PASS: keypair\n");

    /* Signing */
    printf("[2] Signing...\n");

    if (secp256k1_sphincs_shake256_256f_sign(
            sig,
            &siglen,
            message,
            sizeof(message) - 1,
            sk) != 0) {
        printf("FAIL: signing\n");
        goto fail;
    }

    printf("PASS: signing\n");
    printf("Signature length = %zu bytes\n", siglen);

    if (siglen != SIG_BYTES) {
        printf("FAIL: signature size mismatch\n");
        goto fail;
    }

    printf("PASS: signature size\n");

    /* Verification */
    printf("[3] Verification...\n");

    if (secp256k1_sphincs_shake256_256f_verify(
            sig,
            siglen,
            message,
            sizeof(message) - 1,
            pk) != 0) {
        printf("FAIL: verification\n");
        goto fail;
    }

    printf("PASS: verification\n");

    /* Tampered signature */
    printf("[4] Tampered signature rejection...\n");

    memcpy(sig_tampered, sig, SIG_BYTES);
    sig_tampered[0] ^= 0x01;

    if (secp256k1_sphincs_shake256_256f_verify(
            sig_tampered,
            siglen,
            message,
            sizeof(message) - 1,
            pk) == 0) {
        printf("FAIL: tampered signature accepted\n");
        goto fail;
    }

    printf("PASS: tampered signature rejected\n");

    /* Tampered message */
    printf("[5] Tampered message rejection...\n");

    {
        unsigned char tampered_message[sizeof(message)];
        memcpy(tampered_message, message, sizeof(message));
        tampered_message[0] ^= 0x01;

        if (secp256k1_sphincs_shake256_256f_verify(
                sig,
                siglen,
                tampered_message,
                sizeof(message) - 1,
                pk) == 0) {
            printf("FAIL: tampered message accepted\n");
            goto fail;
        }
    }

    printf("PASS: tampered message rejected\n");

    printf("\n===== AI VALIDATION RESULT =====\n");
    printf("AI recommendation   : sphincs-shake256-256f-simple\n");
    printf("secp256k1 parameter : sphincs-shake256-256f-simple\n");
    printf("AI -> secp256k1     : PASS\n");
    printf("API validation      : PASS\n");
    printf("Overall result      : PASS\n");

    free(pk);
    free(sk);
    free(sig);
    free(sig_tampered);

    return 0;

fail:
    free(pk);
    free(sk);
    free(sig);
    free(sig_tampered);

    printf("\n===== AI VALIDATION RESULT =====\n");
    printf("Overall result      : FAIL\n");

    return 1;
}
