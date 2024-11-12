# CMakeLists.txt in utest-imgaos
add_executable(utest-img-aos
        cutfreq_aos_test.cpp
        resize_aos_test.cpp
)

target_link_libraries(utest-img-aos
        PRIVATE imageaos helpers # Link the imageaos library
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utest-img-aos)