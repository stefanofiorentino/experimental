#
# test based on googletest
#

option(FETCH_3RD_PARTY_REPOS "Fetch 3rd repos" OFF)
include(FetchContent)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if (FETCH_3RD_PARTY_REPOS)
    FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.10.3
        GIT_SHALLOW 1
    )

    FetchContent_GetProperties(entt)

    if(NOT entt_POPULATED)
        FetchContent_Populate(entt)
    endif()

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
        GIT_SHALLOW 1
    )

    FetchContent_GetProperties(googletest)

    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()

    FetchContent_Declare(
        libuv
        GIT_REPOSITORY https://github.com/libuv/libuv.git
        GIT_TAG master
        GIT_SHALLOW 1
    )

    FetchContent_GetProperties(libuv)

    if(NOT libuv_POPULATED)
        FetchContent_Populate(libuv)
    endif()

    FetchContent_Declare(
        libcoap
        GIT_REPOSITORY https://github.com/obgm/libcoap.git
        GIT_TAG main        # or a stable tag/version if you prefer
        GIT_SHALLOW 1
    )

    FetchContent_GetProperties(libcoap)

    if(NOT libcoap_POPULATED)
        FetchContent_Populate(libcoap)
    endif()

else(FETCH_3RD_PARTY_REPOS)
    set(entt_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rd/entt-src")
    set(googletest_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rd/googletest-src")
    set(googletest_BINARY_DIR "${CMAKE_BINARY_DIR}/_deps/googletest-build")
    if(NOT EXISTS  ${googletest_SOURCE_DIR})
        message(FATAL_ERROR "Cannot find the 3rd party repo: ${googletest_SOURCE_DIR}")
    endif()
    set(libuv_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rd/libuv-src")
    set(libuv_BINARY_DIR "${CMAKE_BINARY_DIR}/_deps/libuv-build")
    if(NOT EXISTS  ${libuv_SOURCE_DIR})
        message(FATAL_ERROR "Cannot find the 3rd party repo: ${libuv_SOURCE_DIR}")
    endif()
    set(libcoap_BINARY_DIR "${CMAKE_BINARY_DIR}/_deps/libcoap-build")
endif(FETCH_3RD_PARTY_REPOS)


add_subdirectory(${libuv_SOURCE_DIR} ${libuv_BINARY_DIR} EXCLUDE_FROM_ALL)
add_subdirectory("${googletest_SOURCE_DIR}" "${googletest_BINARY_DIR}" EXCLUDE_FROM_ALL)
add_subdirectory(${libcoap_SOURCE_DIR} ${libcoap_BINARY_DIR})

add_library(GTest::Main ALIAS gtest_main)
add_library(GMock::Main ALIAS gmock_main)

target_compile_features(gtest PUBLIC cxx_std_17)
target_compile_features(gtest_main PUBLIC cxx_std_17)
target_compile_features(gmock PUBLIC cxx_std_17)
target_compile_features(gmock_main PUBLIC cxx_std_17)

if(WIN32)
    set(WINSOCK2 ws2_32)
endif()

function(ADD_UNIT_TEST_IMPL TEST_NAME TEST_SOURCE SHALL_FAIL CXX_STD)
    add_executable(${TEST_NAME} "${TEST_SOURCE}")

    target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    set_target_properties(${TEST_NAME} PROPERTIES POSITION_INDEPENDENT_CODE 1)
    target_link_libraries(
            ${TEST_NAME}
        PRIVATE
            GMock::Main
            Threads::Threads
            ${LIBRT}
            ${WINSOCK2}
            uv_a
    )
    target_include_directories(
        ${TEST_NAME}
            PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${entt_SOURCE_DIR}/src"
    )

    target_include_directories(
        ${TEST_NAME}
            PUBLIC
        "${googletest_SOURCE_DIR}/googlemock/include"
        "${libcoap_SOURCE_DIR}/include"
    )

    if(CI_MODE STREQUAL "COVERAGE")
        target_compile_options(${TEST_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
        target_link_libraries(${TEST_NAME} PUBLIC --coverage)
    elseif(CI_MODE STREQUAL "ASAN" AND NOT ${TEST_NAME} MATCHES "memory_allocation")
        target_compile_options(${TEST_NAME} PUBLIC -fsanitize=address -fno-sanitize-recover=all -fno-omit-frame-pointer)
        target_link_libraries(${TEST_NAME} PUBLIC -fsanitize=address)
    elseif(CI_MODE STREQUAL "UBSAN")
        target_compile_options(${TEST_NAME} PUBLIC -fsanitize=undefined -fsanitize-undefined-trap-on-error -fno-sanitize-recover=all -fno-omit-frame-pointer)
        target_link_libraries(${TEST_NAME} PUBLIC -fsanitize=undefined -fsanitize-undefined-trap-on-error)
    elseif(CI_MODE STREQUAL "THSAN" AND NOT ${TEST_NAME} MATCHES "memory_allocation")
        target_compile_options(${TEST_NAME} PUBLIC -fsanitize=thread -fno-sanitize-recover=all -fno-omit-frame-pointer)
        target_link_libraries(${TEST_NAME} PUBLIC -fsanitize=thread)
    elseif(CI_MODE STREQUAL "MUTATION")
        target_compile_options(
            ${TEST_NAME}
                PUBLIC
            -fexperimental-new-pass-manager
            -fpass-plugin=/usr/lib/mull-ir-frontend-12
        )
    endif()
    if (CI_MODE STREQUAL "THSAN" AND SHALL_FAIL)
        add_test(
            NAME shall_fail_${TEST_NAME}
            COMMAND /bin/bash ${CMAKE_SOURCE_DIR}/scripts/shall_fail.sh $<TARGET_FILE:${TEST_NAME}>
        )
    elseif(CI_MODE STREQUAL "MUTATION")
        add_test(
            NAME mull_${TEST_NAME}
            COMMAND /bin/bash ${CMAKE_SOURCE_DIR}/scripts/mull.sh $<TARGET_FILE:${TEST_NAME}>
        )
    else()
        add_test(
            NAME gtest_${TEST_NAME}
            COMMAND $<TARGET_FILE:${TEST_NAME}>)
    endif()

    target_compile_features(${TEST_NAME} PUBLIC ${CXX_STD})
endfunction()

function(ADD_UNIT_TEST TEST_NAME TEST_SOURCE)
    ADD_UNIT_TEST_IMPL(${TEST_NAME} ${TEST_SOURCE} OFF cxx_std_17)
endfunction()

function(ADD_FAILING_UNIT_TEST TEST_NAME TEST_SOURCE)
    ADD_UNIT_TEST_IMPL(${TEST_NAME} ${TEST_SOURCE} ON cxx_std_17)
endfunction()

function(ADD_UNIT_TEST_CPP20 TEST_NAME TEST_SOURCE)
    ADD_UNIT_TEST_IMPL(${TEST_NAME} ${TEST_SOURCE} OFF cxx_std_20)
endfunction()

function(ADD_FAILING_UNIT_TEST_CPP20 TEST_NAME TEST_SOURCE)
    ADD_UNIT_TEST_IMPL(${TEST_NAME} ${TEST_SOURCE} ON cxx_std_20)
endfunction()

option(BUILD_GOOGLE_BASED_TESTS "Enable building googletest-based unit tests." ON)
if (${BUILD_GOOGLE_BASED_TESTS})
    ADD_UNIT_TEST(brown_bag_mocking_strategies brown_bag_mocking_strategies.cpp)
    ADD_UNIT_TEST(brown_bag_tdd_demo brown_bag_tdd_demo.cpp)
    ADD_UNIT_TEST(callback_check callback_check.cpp)
    ADD_UNIT_TEST(elvis elvis.cpp)
    ADD_UNIT_TEST(from_switch_case_to_polymorphism from_switch_case_to_polymorphism.cpp)
    ADD_UNIT_TEST(integral_type integral_type.cpp)
    ADD_UNIT_TEST(memory_allocation memory_allocation.cpp)
    ADD_UNIT_TEST(not_null_ptr not_null_ptr.cpp)
    ADD_UNIT_TEST(sdk_abstraction sdk_abstraction.cpp)
    ADD_UNIT_TEST(signals signals.cpp)
    ADD_UNIT_TEST(crtp-nvi crtp-nvi.cpp)
    ADD_UNIT_TEST(make_unique make_unique.cpp)
    ADD_UNIT_TEST(test_video_series test_video_series.cpp)
    ADD_UNIT_TEST(visitor_guide_allocators_astring visitor_guide_allocators_astring.cpp)
    ADD_UNIT_TEST(statically_allocated_string statically_allocated_string.cpp)
    ADD_UNIT_TEST(hexagolal hexagonal.cpp)
    ADD_UNIT_TEST(mull mull.cpp)
    if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        ADD_UNIT_TEST(pmr pmr.cpp)
    endif()
    if(NOT USE_UBSAN AND NOT USE_ASAN)
        ADD_UNIT_TEST(pure_virtual_call pure_virtual_call.cpp)
    endif(NOT USE_UBSAN AND NOT USE_ASAN)
    ADD_UNIT_TEST(thread_safe_class thread_safe_class.cpp)

    ADD_UNIT_TEST_CPP20(first_concept first_concept.cpp)
    ADD_UNIT_TEST_CPP20(shared_ptr_or_default shared_ptr_or_default.cpp)
    ADD_UNIT_TEST_CPP20(externally_locked externally_locked.cpp)
    ADD_UNIT_TEST_CPP20(zmq_ctx_new zmq_ctx_new.cpp)
    if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        ADD_FAILING_UNIT_TEST_CPP20(thread_safety thread_safety.cpp)
    endif()
    ADD_UNIT_TEST_CPP20(overload_set_macro overload_set_macro.cpp)
    ADD_UNIT_TEST_CPP20(overload_set_lambda overload_set_lambda.cpp)
    ADD_UNIT_TEST_CPP20(exceptional_stack exceptional_stack.cpp)
    ADD_UNIT_TEST_CPP20(atomic_thread_fence_cppref atomic_thread_fence.cppref.cpp)
    ADD_UNIT_TEST_CPP20(placement_new placement_new.cpp)
    ADD_UNIT_TEST_CPP20(sdk_impl sdk_impl.cpp)
    ADD_UNIT_TEST_CPP20(mockNonVirtualMethod mockNonVirtualMethod.cpp)
    ADD_UNIT_TEST_CPP20(coap coap.cpp)
    # ADD_UNIT_TEST_CPP20(matteo matteo.cpp)
    target_link_libraries(sdk_impl PRIVATE sdk)


    ADD_FAILING_UNIT_TEST(thread_unsafe_class thread_unsafe_class.cpp)
endif (${BUILD_GOOGLE_BASED_TESTS})

add_subdirectory(collezioni_polimorfiche_e_dove_trovarle)