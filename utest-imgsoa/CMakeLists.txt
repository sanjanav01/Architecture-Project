# CMakeLists.txt in utest-imgsoa
add_executable(utest-img-soa
        # Assuming the test file name
        compress_test_soa.cpp  # Assuming the test file name
)

target_link_libraries(utest-img-soa
        PRIVATE imgsoa  # Link the imgsoa library
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utest-img-soa)
