#
# collezioni_polimorfiche_e_dove_trovarle
#

if (${BUILD_GOOGLE_BASED_TESTS})
    ADD_UNIT_TEST_CPP20(runtime_polymorphism runtime_polymorphism.cpp)
    ADD_UNIT_TEST_CPP20(std_variant std_variant.cpp)
    ADD_UNIT_TEST_CPP20(entt entt.cpp)
    ADD_UNIT_TEST(dod dod.cpp)
endif (${BUILD_GOOGLE_BASED_TESTS})

# ---- Shared function for benchmark targets ----
function(ADD_BENCHMARK_TARGET TARGET_NAME SOURCE)
    add_executable(${TARGET_NAME} ${SOURCE})

    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    set_target_properties(${TARGET_NAME} PROPERTIES POSITION_INDEPENDENT_CODE 1)
    target_link_libraries(
        ${TARGET_NAME}
        PRIVATE
        benchmark::benchmark_main
        Threads::Threads
    )
    target_include_directories(
        ${TARGET_NAME}
        PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${entt_SOURCE_DIR}/src"
        "${benchmark_SOURCE_DIR}/include"
    )
    target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
endfunction()

# ---- Separate benchmark executables (one per approach) ----
if (${BUILD_GOOGLE_BASED_TESTS})
    ADD_BENCHMARK_TARGET(benchmark_dod benchmark_dod.cpp)
    ADD_BENCHMARK_TARGET(benchmark_entt benchmark_entt.cpp)
    ADD_BENCHMARK_TARGET(benchmark_runtime benchmark_runtime.cpp)
    ADD_BENCHMARK_TARGET(benchmark_variant benchmark_variant.cpp)
endif (${BUILD_GOOGLE_BASED_TESTS})
