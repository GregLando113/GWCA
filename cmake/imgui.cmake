if(TARGET imgui)
    return()
endif()

include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.79
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui)
set(SOURCES
    "${imgui_SOURCE_DIR}/imgui.h"
    "${imgui_SOURCE_DIR}/imgui_internal.h"
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/examples/imgui_impl_win32.h"
    "${imgui_SOURCE_DIR}/examples/imgui_impl_win32.cpp"
    "${imgui_SOURCE_DIR}/examples/imgui_impl_dx9.h"
    "${imgui_SOURCE_DIR}/examples/imgui_impl_dx9.cpp")
source_group(TREE "${imgui_SOURCE_DIR}" FILES ${SOURCES})
target_sources(imgui PRIVATE ${SOURCES})
target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")

