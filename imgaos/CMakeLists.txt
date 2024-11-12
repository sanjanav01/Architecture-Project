add_library(imgaos
        imageaos.cpp
        imageaos.hpp
)
# imgaos/CMakeLists.txt

# Define the imageaos library
add_library(imageaos STATIC imageaos.cpp)
target_include_directories(imageaos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link both helpers and common libraries to imageaos
target_link_libraries(imageaos PUBLIC helpers common)  # Add common here
