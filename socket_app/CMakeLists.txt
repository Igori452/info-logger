project(socket_app)

set(MESSAGE_STATISTICS_LIB message_statistic_lib)
set(SOCKET_RECEIVER_LIB socket_receiver_lib)
set(SERVER_RUNNER server_runner)

add_library(${MESSAGE_STATISTICS_LIB} STATIC MessageStatistics/MessageStatistics.cpp)
target_include_directories(${MESSAGE_STATISTICS_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${MESSAGE_STATISTICS_LIB} PUBLIC logger_lib)

add_library(${SOCKET_RECEIVER_LIB} STATIC SocketReceiver/SocketReceiver.cpp)
target_include_directories(${SOCKET_RECEIVER_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${SERVER_RUNNER} STATIC ServerRunner/ServerRunner.cpp)
target_include_directories(${SERVER_RUNNER} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${SERVER_RUNNER} PUBLIC 
    logger_lib
    ${MESSAGE_STATISTICS_LIB} 
    ${SOCKET_RECEIVER_LIB}
)

add_executable(${PROJECT_NAME} server_app.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${MESSAGE_STATISTICS_LIB} 
    ${SOCKET_RECEIVER_LIB} 
    ${SERVER_RUNNER}
)