
add_library(directx INTERFACE)
target_include_directories(directx INTERFACE "include")
target_link_libraries(directx INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/Lib/x86/d3d9.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lib/x86/d3dx9.lib")

