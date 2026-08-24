#pragma once

#include "logger_lib/Logger.hpp"

#include <vector>
#include <sstream>

/**
 * Набор юнит-тестов для проверки Logger.
 */
class LoggerLibTests 
{
    public:
        /**
         * Проверка логики логгера через Mock-интерфейс.
         * 
         * Тестирует фильтрацию по уровням, форматирование сообщений
         * и изменение уровня по умолчанию без реального дискового вывода.
         */
        static void test_logger_with_mock();

        /**
         * Проверка записи в реальный файл.
         * 
         * Тестирует создание файла, запись строки и обработку ошибок
         * при открытии несуществующей директории.
         */
        static void test_logger_with_file();
};

namespace TestContext 
{

/**
 * Mock-реализация OutputInterface для тестирования.
 * 
 * Сохраняет все записанные сообщения в вектор строк вместо
 * реального вывода на диск или в сокет.
 */
class MockInterface : public OutputInterface 
{
    public:
        std::vector<std::string> messages;

        std::error_code write(std::string_view message) override;
};

}