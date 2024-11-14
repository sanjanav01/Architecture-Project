# helpers/CMakeLists.txt

add_library(helpers STATIC helpers.cpp)
target_include_directories(helpers PUBLIC common ${CMAKE_CURRENT_SOURCE_DIR})