# helpers/CMakeLists.txt

add_library(helpers STATIC helpers.cpp)
target_include_directories(helpers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})