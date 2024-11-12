# CMakeLists.txt in utest-imgaos
cmake_minimum_required(VERSION 3.10)
project(utest-imgaos)

# Add executable for the AOS test
add_executable(utest-img-aos
        cutfreq_aos_test.cpp
)

# Link libraries
target_link_libraries(utest-img-aos
        PRIVATE imageaos helpers # Link the imageaos and helpers libraries
        GTest::gtest_main         # Link Google Test main function
)

# Enable GoogleTest support
include(GoogleTest)
gtest_discover_tests(utest-img-aos)
