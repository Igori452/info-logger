project(logger_lib)

add_library(${PROJECT_NAME} 
    src/ErrorCodes.cpp
    src/Logger.cpp
    src/LoggerMessage.cpp
    src/OutputInterface.cpp)

target_include_directories(logger_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)