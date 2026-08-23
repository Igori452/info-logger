#pragma once

#include "ErrorCodes.hpp"

#include <filesystem>
#include <fstream>

class OutputInterface 
{
    public:
        virtual std::error_code write(std::string_view message) = 0;
        virtual ~OutputInterface() = default;
};

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