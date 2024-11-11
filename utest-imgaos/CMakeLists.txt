# CMakeLists.txt in utest-imgaos
add_executable(utest-img-aos
        cutfreq_aos_test.cpp
        cutfreq_aos_test.cpp  # Assuming the test file name
        imageaos_test.cpp
        aosresize_test.cpp  # Assuming the test file name
)

target_link_libraries(utest-img-aos
        PRIVATE imgaos  # Link the imgaos library
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utest-img-aos)
