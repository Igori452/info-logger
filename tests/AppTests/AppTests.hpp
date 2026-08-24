#pragma once

/**
 * Набор юнит-тестов для проверки ThreadManager.
 */
class AppTests 
{
    public:
        /**
         * Базовая проверка работы очереди FIFO.
         * Проверяет корректность push/pop в однопоточном режиме.
         */
        static void test_thread_manager_queue();

        /**
         * Проверка штатной остановки.
         * 
         * После вызова stop() очередь дочитывается,
         * новые push игнорируются.
         */
        static void test_thread_manager_stop_operation();

        /**
         * Проверка аварийного режима.
         * 
         * Критические ошибки (FILE_NOT_OPEN) переводят менеджер
         * в состояние остановки.
         */
        static void test_thread_manager_error_operation();

        /**
         * Стресс-тест Producer-Consumer.
         * 
         * Проверяет отсутствие гонок и потерь сообщений
         * при параллельной работе двух потоков.
         */
        static void test_thread_manager_with_two_thread();

        /**
         * Проверка обработки ошибок в многопоточной среде.
         * 
         * Consumer имитирует критическую ошибку после обработки
         * части сообщений.
         */
        static void test_thread_manager_with_two_thread_error_operation();
};