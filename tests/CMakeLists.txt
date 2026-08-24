project(unit_tests)

add_executable(${PROJECT_NAME} 
    AppTests/AppTests.cpp
    LoggerLibTests/LoggerLibTests.cpp
    main_tests.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE logger_lib app_core)

add_test(NAME core_tests COMMAND ${PROJECT_NAME})