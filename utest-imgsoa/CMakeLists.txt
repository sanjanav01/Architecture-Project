add_executable(utest-imgsoa
        test_resize.cpp)
target_link_libraries(utest-imgsoa PRIVATE imgsoa GTest::gtest_main)
