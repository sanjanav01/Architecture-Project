# CMakeLists.txt in utest-imgsoa
add_executable(utest-img-soa
        ../common/binary.cpp  # Corrected path
        ../helpers/helpers.cpp
        compress_test_soa.cpp
)

target_link_libraries(utest-img-soa
        PRIVATE imgsoa  # Link the imgsoa library
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utest-img-soa)
