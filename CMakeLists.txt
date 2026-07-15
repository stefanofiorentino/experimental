cmake_minimum_required(VERSION 3.14)
project(light_management VERSION 0.1.0)

cmake_policy(SET CMP0057 NEW)
cmake_policy(SET CMP0054 NEW)
add_subdirectory(cmake/check)
add_subdirectory(cmake/mull)

add_subdirectory(src)

include(CTest)
enable_testing()
add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
