# CMakeLists.txt in utest-imgaos
add_executable(utest-img-aos
        ../common/binary.cpp  # Corrected path
        ../helpers/helpers.cpp
        compress_test_aos.cpp
)

target_link_libraries(utest-img-aos
        PRIVATE imgaos  # Link the imgaos library if needed
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utest-img-aos)
