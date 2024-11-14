# CMakeLists.txt in utest-imgaos
add_executable(utest-img-aos
       # compress_test_aos.cpp
        compress_test_aos_image.cpp
        ../common/binary.cpp  # Corrected path
        ../helpers/helpers.cpp
)

target_link_libraries(utest-img-aos
        PRIVATE imgaos  # Link the imgaos library if needed
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utest-img-aos)
