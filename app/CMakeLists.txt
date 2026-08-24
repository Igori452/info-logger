project(app)

set(APP_LIBRARY app_core)

add_library(${APP_LIBRARY} STATIC ThreadManager/ThreadManager.cpp)

target_include_directories(${APP_LIBRARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${APP_LIBRARY} PUBLIC logger_lib)

add_executable(${PROJECT_NAME} main_app.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${APP_LIBRARY})
