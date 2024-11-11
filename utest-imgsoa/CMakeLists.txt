# CMakeLists.txt in utest-imgsoa
add_executable(utest-img-soa
        imagesoa_test.cpp
        test_resize.cpp  # Assuming the test file name
        cutfreq_soa_test.cpp
        cutfreq_soa_test.cpp  # Assuming the test file name
)

target_link_libraries(utest-img-soa
        PRIVATE imgsoa  # Link the imgsoa library
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utest-img-soa)
