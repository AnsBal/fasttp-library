
include(arch/${ARCH}/arch_src.cmake)

add_library(fasttp STATIC
    ${${ARCH}_src}
    code_ptr.hpp
    common.cpp
    arch/tracepoint.hpp
    context.cpp context.hpp
    fasttp.cpp
    reclaimer.cpp reclaimer.hpp)
set_target_properties(fasttp PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
target_link_libraries(fasttp process pthread ${${ARCH}_lib})
target_include_directories(fasttp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
