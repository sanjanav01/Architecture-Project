# CMakeLists.txt in utest-imgsoa
add_executable(utest-img-soa
        test_resize.cpp
)

target_link_libraries(utest-img-soa
        PRIVATE imgsoa helpers common# Link the imgsoa library
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utest-img-soa)
