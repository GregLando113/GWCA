cmake_minimum_required(VERSION 3.16)

project(GWCA)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "You are configuring a 64bit build, this is not supported. Run cmake with `-A Win32`")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(GWCA_FOLDER "${CMAKE_CURRENT_LIST_DIR}/")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/")

add_subdirectory(Dependencies)

add_library(gwca)
file(GLOB SOURCES
    "source/stdafx.h"
    "source/*.cpp"
    "include/gwca/constants/*.h"
    "include/gwca/context/*.h"
    "include/gwca/gamecontainers/*.h"
    "include/gwca/gameentities/*.h"
    "include/gwca/managers/*.h"
    "include/gwca/packets/*.h"
    "include/gwca/utilities/*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
target_sources(gwca PRIVATE ${SOURCES})
target_compile_features(gwca PUBLIC cxx_std_17)

target_precompile_headers(gwca PRIVATE "source/stdafx.h")
target_include_directories(gwca PUBLIC "include/")

target_link_libraries(gwca PUBLIC
	directx
    minhook)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	include(imgui)
	add_subdirectory(Examples)
endif()
