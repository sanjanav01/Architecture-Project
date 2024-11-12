# CMakeLists.txt in utest-imgsoa
cmake_minimum_required(VERSION 3.10)
project(utest-imgsoa)

# Add executable for the SOA test
add_executable(utest-img-soa
        cutfreq_soa_test.cpp
)

# Link libraries
target_link_libraries(utest-img-soa
        PRIVATE imagesoa helpers # Link the imagesoa and helpers libraries
        GTest::gtest_main         # Link Google Test main function
)

# Enable GoogleTest support
include(GoogleTest)
gtest_discover_tests(utest-img-soa)
