# Project Architecture

This is a CMake-based Cpp experimental project:
- Include files in `/include`
- Source files in `/src`
- Test files in `/test`
- CMake files in every folder where code resides

## Coding Standards
- Use C++ for all new files (with .cpp extensions)
- Follow the existing naming conventions
- Write tests for all new features using GoogleTest and GoogleMock
- Use `#ifdef __cplusplus` to wrap extern "C" blocks
- Use `extern "C"` when calling C functions from C++
- Use `const char*` instead of `char*` whenever possible
