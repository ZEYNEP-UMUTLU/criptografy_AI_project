#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "secp256k1_sphincs.h"

#define PK_BYTES 64
#define SK_BYTES 128
#define SIG_BYTES 49856
#define ITERATIONS 30

static double elapsed_us(
    const struct timespec *start,
    const struct timespec *end
) {
    return (double)(end->tv_sec - start->tv_sec) * 1000000.0
         + (double)(end->tv_nsec - start->tv_nsec) / 1000.0;
}

static double median(double *values, int n) {
    double tmp[ITERATIONS];

    for (int i = 0; i < n; i++) {
        tmp[i] = values[i];
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (tmp[j] < tmp[i]) {
                double t = tmp[i];
                tmp[i] = tmp[j];
                tmp[j] = t;
            }
        }
    }

    if (n % 2 == 0) {
        return (tmp[n / 2 - 1] + tmp[n / 2]) / 2.0;
    }

    return tmp[n / 2];
}

int main(void) {
    unsigned char *pk = malloc(PK_BYTES);
    unsigned char *sk = malloc(SK_BYTES);
    unsigned char *sig = malloc(SIG_BYTES);

    const unsigned char message[] =
        "AI-selected SPHINCS+ SHAKE256-256f-simple benchmark";

    double keygen_us[ITERATIONS];
    double sign_us[ITERATIONS];
    double verify_us[ITERATIONS];

    struct timespec start, end;
    size_t siglen;

    if (!pk || !sk || !sig) {
        fprintf(stderr, "ERROR: memory allocation failed\n");
        return 1;
    }

    printf("===== AI-SELECTED SPHINCS+ 256f BENCHMARK =====\n");
    printf("Parameter : sphincs-shake256-256f-simple\n");
    printf("n = 32, h = 68, d = 17, b = 9, k = 35, w = 16\n");
    printf("Iterations : %d\n\n", ITERATIONS);

    for (int i = 0; i < ITERATIONS; i++) {

        /* Key generation */
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);

        if (secp256k1_sphincs_shake256_256f_keypair(pk, sk) != 0) {
            fprintf(stderr, "ERROR: keypair failed at iteration %d\n", i + 1);
            return 1;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        keygen_us[i] = elapsed_us(&start, &end);

        /* Signing */
        siglen = 0;

        clock_gettime(CLOCK_MONOTONIC_RAW, &start);

        if (secp256k1_sphincs_shake256_256f_sign(
                sig,
                &siglen,
                message,
                sizeof(message) - 1,
                sk) != 0) {
            fprintf(stderr, "ERROR: signing failed at iteration %d\n", i + 1);
            return 1;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        sign_us[i] = elapsed_us(&start, &end);

        if (siglen != SIG_BYTES) {
            fprintf(stderr,
                    "ERROR: signature size mismatch at iteration %d: %zu\n",
                    i + 1, siglen);
            return 1;
        }

        /* Verification */
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);

        if (secp256k1_sphincs_shake256_256f_verify(
                sig,
                siglen,
                message,
                sizeof(message) - 1,
                pk) != 0) {
            fprintf(stderr, "ERROR: verification failed at iteration %d\n", i + 1);
            return 1;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        verify_us[i] = elapsed_us(&start, &end);
    }

    printf("KEYGEN\n");
    printf("Mean   : %.2f us\n", ({
        double s = 0;
        for (int i = 0; i < ITERATIONS; i++) s += keygen_us[i];
        s / ITERATIONS;
    }));
    printf("Median : %.2f us\n\n", median(keygen_us, ITERATIONS));

    printf("SIGN\n");
    printf("Mean   : %.2f us\n", ({
        double s = 0;
        for (int i = 0; i < ITERATIONS; i++) s += sign_us[i];
        s / ITERATIONS;
    }));
    printf("Median : %.2f us\n\n", median(sign_us, ITERATIONS));

    printf("VERIFY\n");
    printf("Mean   : %.2f us\n", ({
        double s = 0;
        for (int i = 0; i < ITERATIONS; i++) s += verify_us[i];
        s / ITERATIONS;
    }));
    printf("Median : %.2f us\n\n", median(verify_us, ITERATIONS));

    printf("Signature size : %d bytes\n", SIG_BYTES);
    printf("Public key size : %d bytes\n", PK_BYTES);
    printf("Secret key size : %d bytes\n", SK_BYTES);

    printf("\n===== BENCHMARK RESULT =====\n");
    printf("AI-selected parameter : sphincs-shake256-256f-simple\n");
    printf("Benchmark validation   : PASS\n");

    free(pk);
    free(sk);
    free(sig);

    return 0;
}
