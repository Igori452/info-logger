project(unit_tests)

file(GLOB_RECURSE UNIT_TESTS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(unit_tests ${UNIT_TESTS_SOURCES})
target_link_libraries(unit_tests PRIVATE logger_lib)

add_test(NAME core_tests COMMAND unit_tests)