#
# collezioni_polimorfiche_e_dove_trovarle
#

if (${BUILD_GOOGLE_BASED_TESTS})
    ADD_UNIT_TEST_CPP20(runtime_polymorphism runtime_polymorphism.cpp)
    ADD_UNIT_TEST_CPP20(std_variant std_variant.cpp)
    ADD_UNIT_TEST(entt entt.cpp)
endif (${BUILD_GOOGLE_BASED_TESTS})
