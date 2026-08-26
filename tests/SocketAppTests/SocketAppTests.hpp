#pragma once

/**
 * Набор юнит-тестов для проверки MessageStatistics,
 * SocketReceiver и взаимодейтсвие SocketReceiver с Logger,
 * используя интерфейс OSocketInterface.
 */
class SocketAppTests 
{
    public:

        /**
         * Тестирование сбора метрик и подсчета характеристик сообщений лога.
         * Проверяет корректность фильтрации по уровням, расчет длин строк и количество
         * сообщений за час.
         */
        static void message_statistics_test();

        /**
         * Тестирование низкоуровневого чтения пакетов данных из сетевого сокета.
         * Проверяет ожидание сообщения, накопление фрагментов строк в буфер и склейку сообщений.
         */
        static void server_receiver_test();

        /**
         * Интеграционный тест всей цепочки отправки и приема логов.
         * Проверяет сквозную работу компонентов Logger -> OSocketInterface -> SocketReceiver.
         */
        static void server_runner_test();
};