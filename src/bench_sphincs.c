#include <stdio.h>
#include <string.h>

#include "bench.h"
#include "secp256k1_sphincs.h"

typedef struct {
    unsigned char pk[SECP256K1_SPHINCS_SHAKE256_128F_PUBLICKEYBYTES];
    unsigned char sk[SECP256K1_SPHINCS_SHAKE256_128F_SECRETKEYBYTES];
    unsigned char sig[SECP256K1_SPHINCS_SHAKE256_128F_BYTES];
    unsigned char msg[32];
    size_t siglen;
} sphincs_bench_data;

static void bench_sphincs_keygen(void *arg, int iters) {
    sphincs_bench_data *data = (sphincs_bench_data *)arg;
    int i;

    for (i = 0; i < iters; i++) {
        if (secp256k1_sphincs_shake256_128f_keypair(data->pk, data->sk) != 0) {
            fprintf(stderr, "SPHINCS+ key generation failed\n");
            return;
        }
    }
}

static void bench_sphincs_sign_setup(void *arg) {
    sphincs_bench_data *data = (sphincs_bench_data *)arg;

    if (secp256k1_sphincs_shake256_128f_keypair(data->pk, data->sk) != 0) {
        fprintf(stderr, "SPHINCS+ key generation failed\n");
    }

    memset(data->msg, 0xA5, sizeof(data->msg));
    data->siglen = 0;
}

static void bench_sphincs_sign(void *arg, int iters) {
    sphincs_bench_data *data = (sphincs_bench_data *)arg;
    int i;

    for (i = 0; i < iters; i++) {
        if (secp256k1_sphincs_shake256_128f_sign(
                data->sig,
                &data->siglen,
                data->msg,
                sizeof(data->msg),
                data->sk) != 0) {
            fprintf(stderr, "SPHINCS+ signing failed\n");
            return;
        }
    }
}

static void bench_sphincs_verify_setup(void *arg) {
    sphincs_bench_data *data = (sphincs_bench_data *)arg;

    if (secp256k1_sphincs_shake256_128f_keypair(data->pk, data->sk) != 0) {
        fprintf(stderr, "SPHINCS+ key generation failed\n");
        return;
    }

    memset(data->msg, 0xA5, sizeof(data->msg));
    data->siglen = 0;

    if (secp256k1_sphincs_shake256_128f_sign(
            data->sig,
            &data->siglen,
            data->msg,
            sizeof(data->msg),
            data->sk) != 0) {
        fprintf(stderr, "SPHINCS+ signing failed\n");
    }
}

static void bench_sphincs_verify(void *arg, int iters) {
    sphincs_bench_data *data = (sphincs_bench_data *)arg;
    int i;

    for (i = 0; i < iters; i++) {
        if (secp256k1_sphincs_shake256_128f_verify(
                data->sig,
                data->siglen,
                data->msg,
                sizeof(data->msg),
                data->pk) != 0) {
            fprintf(stderr, "SPHINCS+ verification failed\n");
            return;
        }
    }
}

int main(int argc, char **argv) {
    sphincs_bench_data data;
    int iters = 1;

    if (argc > 1) {
        sscanf(argv[1], "%d", &iters);
        if (iters < 1) {
            iters = 1;
        }
    }

    printf("SPHINCS+ SHAKE256-128f-simple benchmark\n");
    printf("Results are in microseconds per operation.\n");
    printf("Iterations: %d\n\n", iters);

    run_benchmark("sphincs_keygen", bench_sphincs_keygen,
                  NULL, NULL, &data, 10, iters);

    run_benchmark("sphincs_sign", bench_sphincs_sign,
                  bench_sphincs_sign_setup, NULL, &data, 10, iters);

    run_benchmark("sphincs_verify", bench_sphincs_verify,
                  bench_sphincs_verify_setup, NULL, &data, 10, iters);

    return 0;
}
