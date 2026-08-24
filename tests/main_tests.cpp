#include "AppTests/AppTests.hpp"
#include "LoggerLibTests/LoggerLibTests.hpp"

/**
 * Точка входа в программу юнит-тестирования.
 */
int main () 
{
    LoggerLibTests::test_logger_with_mock();
    LoggerLibTests::test_logger_with_file();
    LoggerLibTests::formater_roundtrip_test();

    AppTests::test_thread_manager_queue();
    AppTests::test_thread_manager_stop_operation();
    AppTests::test_thread_manager_error_operation();
    AppTests::test_thread_manager_with_two_thread();
    AppTests::test_thread_manager_with_two_thread_error_operation();

    return 0;
}