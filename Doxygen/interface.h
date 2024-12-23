/**
* @file interface.h
* @brief Заголовочный файл для класса interface
*/
#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>

/**
* @class interface
* @brief Класс для обработки аргументов командной строки.
*/
class interface {
public:
/**
* @brief Конструктор класса interface.
* @param argc Количество аргументов командной строки.
* @param argv Массив аргументов командной строки.
*/
    interface(int argc, char *argv[]);
/**
* @brief Метод для обработки аргументов командной строки.
* @return 0, если аргументы обработаны успешно, иначе 1.
*/
    int arguments();
/**
* @brief Получить имя файла базы данных.
* @return Имя файла.
*/
    std::string get_file_name() const;
/**
* @brief Получить номер порта.
* @return Номер порта.
*/
    int get_port() const;
/**
* @brief Получить имя файла для записи ошибок.
* @return Имя файла для записи ошибок.
*/
    std::string get_file_error() const;
private:
/**
* @brief Метод для вывода справки.
*/
    void print_help() const;
    int argc_; ///<< Количество аргументов командной строки.
    char **argv_; ///<< Массив аргументов командной строки..
    std::string file_name; ///<< Имя файла с базой данных.
    int port; ///<< Номер порта.
    std::string file_error; ///<< Имя файла для записи ошибок.
};
#endif
