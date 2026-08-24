#pragma once

#include "ErrorCodes.hpp"

#include <filesystem>
#include <fstream>

/**
 * Абстрактный интерфейс стратегии вывода (паттерн Стратегия).
 * 
 * Позволяет подменять реализацию вывода (файл, сокет, UI и т.д.)
 * без изменения логики логгера.
 */
class OutputInterface 
{
    public:
        virtual std::error_code write(std::string_view message) = 0;
        virtual ~OutputInterface() = default;
};

/**
 * Реализация интерфейса вывода для записи в файл.
 */
class OFileInterface : public OutputInterface 
{
    private:
        const std::filesystem::path path;
        std::ofstream ofile;

    public:
        OFileInterface() = delete;
        OFileInterface(std::string_view path_);

        std::error_code write(std::string_view message) override;
};

/* РЕАЛИЗАЦИЯ ИНТЕРФЕЙСА ВЫВОДА ДЛЯ СОКЕТА */