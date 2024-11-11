# Add to this list all files related to common library
add_library(common
        progargs.cpp
        binary.cpp
        metadata.cpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Use this line only if you have dependencies from this library to GSL
target_link_libraries(common PRIVATE Microsoft.GSL::GSL)