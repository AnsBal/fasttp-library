
add_library(process STATIC
    ${${ARCH}_src}
    elf.cpp
    memmap.cpp
    process.cpp)
set_target_properties(process PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(process elf util)
