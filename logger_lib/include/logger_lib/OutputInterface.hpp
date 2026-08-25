#pragma once

#include "ErrorCodes.hpp"

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
        std::ofstream ofile;

    public:
        OFileInterface() = delete;
        OFileInterface(std::string_view path_);

        std::error_code write(std::string_view message) override;
};

/**
 * Реализация интерфейса вывода для записи в сокет (для Linux).
 */
class OSocketInterface : public OutputInterface
{
    private:
        int socketObject;

    public:
        OSocketInterface() = delete;
        OSocketInterface(int socketObject_);

        std::error_code write(std::string_view message) override;

        OSocketInterface(const OSocketInterface&) = delete;
        OSocketInterface& operator=(const OSocketInterface&) = delete;

        OSocketInterface(OSocketInterface&& other) noexcept;
        OSocketInterface& operator=(OSocketInterface&& other) noexcept;
};