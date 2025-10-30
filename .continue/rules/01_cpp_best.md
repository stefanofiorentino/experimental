---
name: C++/CMake Best Practices
globs: ["**/*.cpp", "**/*.hpp", "**/*.h", "**/CMakeLists.txt"]
---
# C++ Best Practices
- Follow the C++ Core Guidelines.
- Use RAII for resource management.
- Prefer smart pointers over raw pointers.
- Keep header files minimal.
# CMake Rules
- Define one target per executable or library.
- Use target-based commands (e.g., target_include_directories).
- Set C++ standard properties per target.