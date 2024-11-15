# CMakeLists.txt in utest-imgaos
add_executable(utest-img-aos
        cutfreq_aos_test.cpp
        maxlevel_test.cpp  # Assuming the test file name
        ../common/binary.cpp  # Corrected path
        ../helpers/helpers.cpp
        compress_test_aos.cpp
        resize_test.cpp
)

target_link_libraries(utest-img-aos
        PRIVATE imageaos  # Link the imageaos library
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utest-img-aos)
