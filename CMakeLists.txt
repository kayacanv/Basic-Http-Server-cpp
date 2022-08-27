project(Basic-Http-Server-cpp)

find_package( Threads REQUIRED )

add_executable(server
    ${CMAKE_CURRENT_SOURCE_DIR}/html_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

if(WIN32)
    target_link_libraries(server ws2_32)
endif()

if(UNIX)
    #target_compile_options(server PUBLIC "-pthread")  Replaced with <thread>
endif()

target_link_libraries(server Threads::Threads)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/main.html
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
