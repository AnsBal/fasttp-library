add_executable(multithread multithread.cpp)
target_link_libraries(multithread fasttp pthread)

add_executable(recursive recursive.cpp)
target_link_libraries(recursive fasttp)
