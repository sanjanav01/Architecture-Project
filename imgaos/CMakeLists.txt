# Define the imageaos library without directly including helpers.cpp
add_library(imageaos STATIC imageaos.cpp)
target_include_directories(imageaos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link the helpers library to imageaos
target_link_libraries(imageaos PUBLIC helpers)