add_executable(utest-imgaos
        imageaos_test.cpp
        aosresize_test.cpp)
target_link_libraries(utest-imgaos PRIVATE imgaos GTest::gtest_main)
