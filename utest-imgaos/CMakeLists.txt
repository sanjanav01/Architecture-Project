# CMakeLists.txt in utest-imgaos
add_executable(utest-img-aos
        # Assuming the test file name
        compress_test_aos.cpp  # Assuming the test file name
        cutfreq_aos_test.cpp
        cutfreq_aos_test.cpp
        compress_test_aos_image.cpp  # Assuming the test file name
)

target_link_libraries(utest-img-aos
        PRIVATE imgaos  # Link the imgaos library
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utest-img-aos)
