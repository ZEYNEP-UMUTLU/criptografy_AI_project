# SPHINCS+ Optimization, AI-Assisted Parameter Selection, and secp256k1 Integration

## 1. Project Overview

This internship project focused on optimizing the SPHINCS+ post-quantum digital signature scheme, benchmarking multiple parameter sets, integrating selected implementations into secp256k1, and developing an AI-assisted parameter selection workflow.

The project covered:

- SPHINCS+ implementation optimization
- OpenMP-based parallelization
- SHAKE256/AVX2 optimization
- Benchmarking of six SPHINCS+ parameter sets
- AI-assisted parameter ranking
- Integration of SPHINCS+ SHAKE256-128f-simple
- Integration and validation of SPHINCS+ SHAKE256-256f-simple
- Cryptographic API validation
- Negative/tampering tests
- Clean-environment build and regression testing
- Final GitHub repository validation

## 2. Optimization Process

The optimization process was performed incrementally.

The main optimization stages were:

1. Compiler optimization with `-O3`
2. Native architecture optimization with `-march=native`
3. Link-time optimization with `-flto`
4. Function inlining and frame-pointer optimization
5. OpenMP parallelization
6. Parallel WOTS+ and Merkle-tree operations
7. AVX2-based SHAKE256/Keccak optimization
8. Final integration and controlled benchmarking

Profiling identified Keccak/Shake-related operations as major computational hotspots.

OpenMP experiments showed that parallelization does not automatically provide linear speedup because thread-management and synchronization overhead can dominate relatively small workloads. Treehash and WOTS+ parallelization were therefore applied selectively.

## 3. Benchmark Dataset

A final benchmark dataset containing 180 observations was generated.

Six SPHINCS+ parameter sets were evaluated with 30 outer runs per parameter set:

| Parameter set | Security | n | h | d | FORS h | FORS trees | Signature |
|---|---:|---:|---:|---:|---:|---:|---:|
| SHAKE256-128s-simple | 128-bit | 16 | 63 | 7 | 12 | 14 | 7,856 B |
| SHAKE256-128f-simple | 128-bit | 16 | 66 | 22 | 6 | 33 | 17,088 B |
| SHAKE256-192s-simple | 192-bit | 24 | 63 | 7 | 14 | 17 | 16,224 B |
| SHAKE256-192f-simple | 192-bit | 24 | 66 | 22 | 8 | 33 | 35,664 B |
| SHAKE256-256s-simple | 256-bit | 32 | 64 | 8 | 14 | 22 | 29,792 B |
| SHAKE256-256f-simple | 256-bit | 32 | 68 | 17 | 9 | 35 | 49,856 B |

## 4. Benchmark Results

The 180-run benchmark produced the following mean results:

| Parameter | KeyGen (us) | Sign (us) | Verify (us) | Signature |
|---|---:|---:|---:|---:|
| 128f | 2,289.14 | 52,165.63 | 2,892.75 | 17,088 B |
| 128s | 134,701.54 | 1,016,206.73 | 973.03 | 7,856 B |
| 192f | 3,293.89 | 82,850.32 | 4,186.73 | 35,664 B |
| 192s | 193,353.58 | 1,734,065.98 | 1,401.90 | 16,224 B |
| 256f | 7,873.21 | 146,868.97 | 3,692.86 | 49,856 B |
| 256s | 133,020.40 | 1,567,028.38 | 2,184.92 | 29,792 B |

The 256f raw benchmark contained two execution-performance regimes. Later controlled validation runs stabilized around approximately 79–81 ms signing time. These observations were retained rather than arbitrarily removing them as statistical outliers.

## 5. AI-Assisted Parameter Selection

An AI-assisted parameter selection workflow was developed using Python, pandas, NumPy, and scikit-learn.

The model was:

`RandomForestRegressor(n_estimators=300, random_state=42)`

The dataset contained 180 observations.

The machine-learning target was:

`sign_us`

The feature set was:

- n
- h
- d
- b
- k
- w
- signature_bytes

Leave-One-Out Cross-Validation was used during model evaluation.

The resulting metrics were:

- MAE: 11,298.22 us
- RMSE: 23,417.53 us
- R2: 0.9989

The R2 value is reported as a dataset/model evaluation metric and should not be interpreted as proof of generalization to unseen SPHINCS+ parameter configurations.

### Feature Importance

| Feature | Importance |
|---|---:|
| b | 0.343460 |
| k | 0.222194 |
| h | 0.212264 |
| d | 0.176216 |
| signature_bytes | 0.023082 |
| n | 0.022785 |
| w | 0.000000 |

The AI workflow was used primarily as a parameter ranking and selection aid rather than as a precise latency predictor.

## 6. AI Parameter Ranking

The final multi-criteria ranking was:

| Rank | Parameter | Security | Predicted Sign (us) | KeyGen (us) | Verify (us) | Signature | Score |
|---:|---|---:|---:|---:|---:|---:|---:|
| 1 | SHAKE256-256f-simple | 256 | 147,423.4 | 7,873.21 | 3,692.86 | 49,856 B | 0.738328 |
| 2 | SHAKE256-128f-simple | 128 | 52,165.36 | 2,289.14 | 2,892.75 | 17,088 B | 0.707293 |
| 3 | SHAKE256-192f-simple | 192 | 82,849.25 | 3,293.90 | 4,186.73 | 35,664 B | 0.692599 |
| 4 | SHAKE256-128s-simple | 128 | 1,016,142 | 134,701.54 | 973.03 | 7,856 B | 0.466782 |
| 5 | SHAKE256-256s-simple | 256 | 1,567,425 | 133,020.40 | 2,184.92 | 29,792 B | 0.420936 |
| 6 | SHAKE256-192s-simple | 192 | 1,734,028 | 193,353.58 | 1,401.90 | 16,224 B | 0.306769 |

The AI-selected parameter set was:

`SPHINCS+-SHAKE256-256f-simple`

The selection considered signing performance, key-generation performance, verification performance, signature size, and security level through the multi-criteria evaluation.

Public and secret key sizes were also recorded and reported as part of the benchmark and validation data.

## 7. Selected 256f Parameter Set

The selected parameter set uses:

n = 32
h = 68
d = 17
b = 9
k = 35
w = 16

Derived sizes:

Public key = 64 bytes
Secret key = 128 bytes
Signature = 49,856 bytes
Seed = 96 bytes

The parameter set provides a 256-bit security level.

## 8. AI Prediction Versus Controlled Execution

The AI model predicted approximately 147.45 ms for signing the selected 256f configuration.

Controlled integrated execution produced approximately 79–81 ms depending on the benchmark environment and run.

This difference demonstrates an important limitation of the current AI model: the model is useful for ranking the tested parameter configurations, but its absolute latency prediction should not be interpreted as a precise execution-time estimator for a different optimized implementation or execution regime.

The final decision therefore relied on the combination of AI ranking and direct controlled cryptographic benchmarking.

## 9. secp256k1 Integration

Both SPHINCS+ SHAKE256-128f-simple and SHAKE256-256f-simple were integrated into the secp256k1 project.

Separate source trees and namespaces were used to prevent symbol collisions between the two SPHINCS+ implementations.

The 256f integration includes:

- SPHINCS+ API wrapper
- parameter definitions
- address handling
- FORS
- WOTS+
- signing and verification
- SHAKE256/FIPS202 implementation
- hash functions
- treehash
- optimized SHAKE offsets
- namespace-prefixed symbols

The public secp256k1-facing API includes:

- secp256k1_sphincs_shake256_256f_keypair
- secp256k1_sphincs_shake256_256f_seed_keypair
- secp256k1_sphincs_shake256_256f_sign
- secp256k1_sphincs_shake256_256f_verify

## 10. Cryptographic API Validation

The AI-selected 256f implementation was validated inside secp256k1.

The following tests passed:

1. Keypair generation
2. Signature generation
3. Signature length validation
4. Signature verification
5. Tampered signature rejection
6. Tampered message rejection

Final result:

AI recommendation: sphincs-shake256-256f-simple
secp256k1 parameter: sphincs-shake256-256f-simple
AI -> secp256k1: PASS
API validation: PASS
Overall result: PASS

## 11. Final Controlled 256f Benchmark

The final AI-selected secp256k1 benchmark used:

OMP_NUM_THREADS=1
OMP_DYNAMIC=FALSE
taskset -c 0

The benchmark used 30 iterations.

| Operation | Mean (us) | Median (us) |
|---|---:|---:|
| KeyGen | 3,916.05 | 3,858.42 |
| Sign | 80,754.57 | 79,917.05 |
| Verify | 2,088.45 | 2,077.43 |

Sizes:

Signature size = 49,856 bytes
Public key = 64 bytes
Secret key = 128 bytes

Benchmark validation result: PASS

## 12. SPHINCS+ Functional Testing

The final SPHINCS+ integration tests covered the major cryptographic components and tampering behavior.

The final validation included successful tests for:

- WOTS+
- FORS
- keypair generation
- signing
- verification
- invalid/tampered signatures
- modified messages

The implementation rejected modified signatures and modified messages as expected.

## 13. secp256k1 Regression Testing

The final repository was cloned into a clean temporary directory and built independently.

The clean-clone procedure successfully completed:

CMake configuration: PASS
Compilation: PASS
CTest: 223/223 PASS
Failed tests: 0

Therefore:

100% tests passed, 0 tests failed out of 223.

This provides independent evidence that the committed repository can be configured, compiled, and tested successfully outside the original working directory.

## 14. Repository Integrity

The final repository was checked for:

- tracked source files
- AI source code
- AI dataset
- AI outputs
- benchmark outputs
- final delivery documents
- SPHINCS+ 128f source tree
- SPHINCS+ 256f source tree
- public headers
- integration wrappers

The repository audit found no tracked:

- build directories
- .o files
- .so files
- .a files
- .pyc files
- virtual-environment files

The final repository therefore contains source code and reproducibility-related artifacts without accidentally committing generated build products.

## 15. Final Deliverables

### AI

AI/parameter_selector.py
AI/day19_ai_dataset.csv
AI/bench_ai_selected_256f.c
AI/test_ai_selected_256f.c
AI/results/ai_predictions.csv
AI/results/ai_feature_importance.csv
AI/results/ai_parameter_ranking.csv
AI/results/ai_parameter_selection_report.txt
AI/results/final_ai_parameter_selection.txt
AI/results/final_256f_benchmark.txt
AI/results/final_api_validation.txt

### Final Delivery

FINAL_DELIVERY/README.md
FINAL_DELIVERY/FINAL_TECHNICAL_REPORT.md
FINAL_DELIVERY/ai_feature_importance.csv
FINAL_DELIVERY/ai_parameter_ranking.csv
FINAL_DELIVERY/ai_parameter_selection_report.txt
FINAL_DELIVERY/benchmark_sphincs_day17_final.txt
FINAL_DELIVERY/final_256f_benchmark.txt
FINAL_DELIVERY/final_ai_parameter_selection.txt
FINAL_DELIVERY/final_api_validation.txt

## 16. Final Status

The project has completed the following stages:

- SPHINCS+ optimization: COMPLETE
- Benchmarking of six parameter sets: COMPLETE
- 180-run benchmark dataset: COMPLETE
- AI-assisted parameter ranking: COMPLETE
- AI-selected parameter validation: COMPLETE
- SHAKE256-256f integration: COMPLETE
- secp256k1 integration: COMPLETE
- Cryptographic API validation: COMPLETE
- Negative/tampering tests: COMPLETE
- Final controlled benchmark: COMPLETE
- Clean-clone build: COMPLETE
- 223/223 secp256k1 tests: PASS
- Repository integrity audit: PASS
- Final technical documentation: COMPLETE

The final selected configuration is:

`SPHINCS+-SHAKE256-256f-simple`

The implementation has been integrated into secp256k1, cryptographically validated, benchmarked under controlled conditions, and verified in a clean environment.
