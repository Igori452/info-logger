#pragma once

#include <system_error>
#include <optional>

class SocketReceiver 
{
    private:
        int socketObject;
        std::string streamBuffer;

    public:
        SocketReceiver() = delete;
        SocketReceiver(int socketObject_);

        std::error_code waitData(size_t imeoutSeconds) const;

        /**
         * Чтение доступных данных из сетевого сокета во внутренний буфер.
         * 
         * true, если данные успешно прочитаны и добавлены в streamBuffer.
         * false при ошибке сети или если клиент штатно отключился.
         */

        bool readFromNetwork();

        /**
         * Извлечение следующего полного сообщения из накопленного буфера.
         * 
         * Извлеченная строка лога (включая разделитель), 
         * или std::nullopt, если символ разделителя (separator) еще не найден в буфере.
         */
        std::optional<std::string> getNextMessage(const char separator);
};