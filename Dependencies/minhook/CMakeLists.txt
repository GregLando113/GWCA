
add_library(minhook)
file(GLOB SOURCES
    "include/*.h"
    "src/*.h"
    "src/*.c"
    "src/hde/*.h"
    "src/hde/*.c")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
target_sources(minhook PRIVATE ${SOURCES})
target_include_directories(minhook PUBLIC "include/")
