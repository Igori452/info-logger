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
        std::filesystem::path path;
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
        std::string ip;
        uint16_t port;

        int socketObject;

        std::error_code connectToServer();
        void disconnect();

    public:
        OSocketInterface() = delete;
        OSocketInterface(std::string_view ip_, uint16_t port_);

        std::error_code write(std::string_view message) override;

        OSocketInterface(const OSocketInterface&) = delete;
        OSocketInterface& operator=(const OSocketInterface&) = delete;

        OSocketInterface(OSocketInterface&& other) noexcept;
        OSocketInterface& operator=(OSocketInterface&& other) noexcept;

        ~OSocketInterface();
};