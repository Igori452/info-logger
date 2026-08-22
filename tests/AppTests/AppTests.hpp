#pragma once

class AppTests 
{
    public:
        static void test_thread_manager_queue();
        static void test_thread_manager_stop_operation();
        static void test_thread_manager_error_operation();

        static void test_thread_manager_with_two_thread();
        static void test_thread_manager_with_two_thread_error_operation();
};