project(logger_lib)

file(GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(logger_lib ${LIB_SOURCES})

target_include_directories(logger_lib 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    INTERFACE ${CMAKE_SOURCE_DIR})