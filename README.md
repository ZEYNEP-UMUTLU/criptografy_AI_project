# SPHINCS+ Optimization and AI-Assisted Parameter Selection

Final internship project integrating SPHINCS+ SHAKE256 into libsecp256k1.

## Project Summary

This project includes:

- SPHINCS+ SHAKE256-128f integration
- SPHINCS+ SHAKE256-256f integration
- secp256k1 public API wrappers
- CMake module integration
- Benchmark framework
- AI-assisted parameter selection using Random Forest
- 30-iteration benchmark validation
- Signature verification and tamper detection tests

## Implemented Parameter Sets

| Parameter | Security | Signature |
|-----------|----------|-----------|
| 128f-simple | 128-bit | 17088 B |
| 256f-simple | 256-bit | 49856 B |

## AI Results

- Dataset: 180 benchmark observations
- Model: Random Forest Regressor
- R²: 0.9989
- Best parameter: **SPHINCS-SHAKE256-256f-simple**

## Repository Structure

- `include/` Public SPHINCS API
- `src/` Integrated SPHINCS implementations
- `AI/` Machine learning model and benchmark analysis
- `examples/` Example SPHINCS application

## Author

**Zeynep Umutlu**

Computer Engineering Internship Project (2026)
