add_executable(multithread multithread.cpp)
target_link_libraries(multithread fasttp pthread)

add_executable(bm bm_tracepoint.cpp)
target_link_libraries(bm benchmark fasttp)

add_executable(recursive recursive.cpp)
target_link_libraries(recursive fasttp)
