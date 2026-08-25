project(unit_tests)

add_executable(${PROJECT_NAME} 
    AppTests/AppTests.cpp
    LoggerLibTests/LoggerLibTests.cpp
    SocketAppTests/SocketAppTests.cpp
    main_tests.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    logger_lib
    app_core
    message_statistic_lib
    socket_receiver_lib
    server_runner
)

add_test(NAME core_tests COMMAND ${PROJECT_NAME})