# CMakeLists.txt in utest-imgaos
add_executable(utest-img-soa
        cutfreq_soa_test.cpp
        cutfreq_soa_test.cpp
        maxlevel_test_soa.cpp
        compress_test_soa.cpp
        resize_test.cpp
        # Assuming the test file name
)

target_link_libraries(utest-img-soa
        PRIVATE imagesoa  # Link the imageaos library
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utest-img-soa)
