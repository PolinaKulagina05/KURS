/** @file
 * @author Кулагина П.В.
 * @version 1.0
 * @date 23.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для модуля calc
 */
#include <netinet/in.h>
#include <iostream>
#include <cassert>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <cryptopp/cryptlib.h>
#include <iostream>
#include <vector>
#include <getopt.h>
#include <unistd.h>
#include <getopt.h>
#include <csignal>
#include <cstdlib>
#include <cryptopp/hex.h> 
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
using namespace CryptoPP;
using namespace std;

/** @class Error
 * @brief Класс для обработки и записи ошибок
 */
class Error
{
public:
    Error(); ///< Конструктор по умолчанию
/**
* @brief Метод для записи ошибки в файл.
* @param error Текст ошибки.
* @param name Имя файла для записи ошибки.
*/
    static void errors(std::string error, std::string name);
/**
* @brief Метод для проверки доступности файла.
* @param file_name Имя файла с базой данных.
* @param file_error Имя файла для записи ошибки.
* @return Код ошибки (12, если файл недоступен).
*/
    static int er(std::string file_name, std::string file_error);
};

/**
* @class Server
* @brief Класс для настройки и управления сервером.
*/
class Server
{
private:
    Error e_error; ///< Объект для обработки ошибок.
public:
/**
* @brief Конструктор класса Server.
* @param err Объект класса Error для обработки ошибок.
*/
    Server(Error err) : e_error(err) {
        e_error = err;
    }
/**
* @brief Метод для настройки адреса сервера.
* @param error Ссылка на строку для записи ошибки.
* @param file_error Имя файла для записи ошибки.
* @param port Порт сервера.
* @return Сокет сервера.
* @details Метод начинает с проверки, является ли переданный порт корректным. Порт должен быть в диапазоне от 0 до 65535. Если порт выходит за пределы допустимого диапазона, метод формирует сообщение об ошибке, записывает его в файл (через объект e_error), и выбрасывает исключение std::runtime_error.
@code
if (port < 0 || port > 65535) {
	error = "Некорректное значение порта: " + to_string(port);
	e_error.errors(error, file_error);
	throw std::runtime_error("Invalid port value");
	return -1;
}
@endcode
* @details Метод создает сокет с помощью функции socket. Сокет использует семейство адресов AF_INET (IPv4) и тип сокета SOCK_STREAM (TCP). Если создание сокета завершается с ошибкой (возвращается значение меньше 0), метод выводит сообщение об ошибке с помощью perror и завершает программу с кодом EXIT_FAILURE.
@code
int sock = socket(AF_INET, SOCK_STREAM, 0);
if (sock < 0) {
	perror("Ошибка при создании сокета");
	exit(EXIT_FAILURE);
}
@endcode
* @details Метод устанавливает опцию SO_REUSEADDR для сокета, чтобы разрешить повторное использование адреса и порта после завершения работы сервера. Если установка опции завершается с ошибкой (возвращается -1), метод выбрасывает исключение std::system_error.
@code
int on = 1;
int rc = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
if (rc == -1) {
	throw system_error(errno, generic_category(), "Ошибка установки сокета");
}
@endcode
* @details Метод устанавливает обработчик сигнала SIGALRM с помощью функции signal. Этот сигнал будет использоваться для ограничения времени ожидания подключения клиента. Затем метод устанавливает таймаут на 240 секунд для операций приема данных с помощью опции SO_RCVTIMEO. Если установка таймаута завершается с ошибкой, метод выбрасывает исключение std::system_error.
@code
signal(SIGALRM, alarm_handler);
alarm(240);
struct timeval timeout {240, 0};
rc = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
if (rc == -1) {
	throw system_error(errno, generic_category(), "Ошибка установки сокета");
}
@endcode
* @details Метод настраивает структуру sockaddr_in, которая содержит информацию об адресе сервера.
@code
sockaddr_in self_addr;
self_addr.sin_family = AF_INET;
self_addr.sin_port = htons(port);
self_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
@endcode
* @details Метод вызывает функцию bind, чтобы привязать сокет к указанному адресу и порту. Если привязка завершается с ошибкой (возвращается -1), метод выводит сообщение об ошибке, записывает его в файл (через объект e_error) и возвращает код ошибки 1.
@code
int b = bind(sock, reinterpret_cast<sockaddr*>(&self_addr), sizeof(self_addr));
if (b == -1) {
	cout << "Ошибка привязки\n";
	error = "Ошибка";
	e_error.errors(error, file_error);
	return 1;
}
@endcode
* @details После успешной привязки сокета метод вызывает функцию listen, чтобы начать прослушивание входящих подключений.
*/
    int self_addr(std::string& error, std::string& file_error, int port);
/**
* @brief Метод для обработки подключения клиента.
* @param s Сокет сервера.
* @param error Ссылка на строку для записи ошибки.
* @param file_error Имя файла для записи ошибки.
* @return Сокет клиента.
*/
    int client_addr(int s, std::string& error, std::string& file_error);
};


/**
* @class Authorized
* @brief Класс для авторизации клиента.
*/
class Authorized
{
private:
    Error e_error; ///< Объект для обработки ошибок.
/**
* @brief Метод для отправки сообщения клиенту.
* @param work_sock Сокет клиента.
* @param mess Сообщение для отправки.
*/
    void msgsend(int work_sock, const std::string& mess);
public:
/**
* @brief Метод для вычисления MD5 хеша.
* @param sah Строка для хеширования.
* @return Хеш в формате строки.
*/
    std::string MD(const std::string& sah);
/**
* @brief Конструктор класса Authorized.
* @param err Объект класса Error для обработки ошибок.
*/
    Authorized(Error err) : e_error(err) {
        e_error = err;
    }
/**
* @brief Метод для авторизации клиента.
* @param work_sock Сокет клиента.
* @param file_name Имя файла с данными для авторизации.
* @param file_error Имя файла для записи ошибки.
* @return Результат авторизации (0 - успех, 1 - ошибка).
* @details Метод принимает сообщение от клиента через сокет work_sock. Это сообщение содержит данные для авторизации (логин, соль и хеш пароля).
@code
recv(work_sock, &msg, sizeof(msg), 0);
string message = msg;
@endcode
* @details Метод открывает файл file_name, который содержит данные авторизации (логин и хеш пароля).
@code
fstream file;
file.open(file_name);
getline(file, login, ':');
getline(file, hashq);
@endcode
* @details Метод сравнивает логин, полученный от клиента, с логином, хранящимся в файле. Если логины не совпадают, метод отправляет клиенту сообщение ERR, записывает сообщение об ошибке в файл (через объект e_error) и закрывает соединение.
@code
if (message.substr(0, login.length()) != login) {
    msgsend(work_sock, err);
    error = "Ошибка логина";
    e_error.errors(error, file_error);
    close(work_sock);
    return 1;
}
@endcode
* @details Если логин корректен, метод извлекает соль из сообщения клиента (16 символов после логина). Затем метод формирует строку sah, которая состоит из соли и хеша пароля из файла. Далее метод вычисляет MD5-хеш строки sah с помощью функции MD.
@code
string salt = message.substr(login.length(), 16); // Извлекаем соль из сообщения
string sah = salt + hashq;
string digest = MD(sah);
@endcode
* @details Метод сравнивает вычисленный хеш digest с хешем, полученным от клиентa. Если хеши не совпадают, метод отправляет клиенту сообщение ERR, записывает сообщение об ошибке в файл и закрывает соединение.
@code
string received_hash = message.substr(login.length() + 16);
if (digest != received_hash) {
    msgsend(work_sock, err);
    error = "Ошибка пароля";
    e_error.errors(error, file_error);
    close(work_sock);
    return 1;
}
@endcode
* @details Если логин и пароль корректны, метод отправляет клиенту сообщение OK и возвращает 0, что означает успешную авторизацию.
*/
	int authorized(int work_sock, string file_name, string file_error);
};


/**
* @class Calculator
* @brief Класс для выполнения вычислений.
*/
class Calculator
{
private:
    Error e_error; //< Объект для обработки ошибок.
public:
/**
* @brief Конструктор класса Calculator.
* @param err Объект класса Error для обработки ошибок.
*/
    Calculator(Error err) : e_error(err) {
        e_error = err;
    }
/**
* @brief Метод для выполнения вычислений.
* @param work_sock Сокет клиента.
* @return Результат вычислений (1 - успех).
* @details Метод принимает от клиента значение Quantity, которое указывает, сколько наборов чисел будет отправлено.
@code
recv(work_sock, &Quantity, sizeof(Quantity), 0);
@endcode
* @details Метод выполняет цикл по количеству наборов чисел (Quantity). Для каждого набора: Принимает значение Length, которое указывает, сколько чисел содержится в текущем наборе. Инициализирует переменную Amount для хранения суммы чисел в текущем наборе.
@code
for (int j = 0; j < Quantity; j++) {
    recv(work_sock, &Length, sizeof(Length), 0);
    float Amount = 0;
@endcode
* @details Внутри цикла по Length метод принимает числа от клиента и суммирует их в переменной Amount.
@code
for (int i = 0; i < Length; i++) {
    recv(work_sock, &Vector_numbers, sizeof(Vector_numbers), 0);
    Amount += Vector_numbers;
}
@endcode
* @details После суммирования всех чисел в наборе метод отправляет результат (Amount) обратно клиенту.
@code
send(work_sock, &Amount, sizeof(Amount), 0);
@endcode
* @details После завершения всех вычислений метод выводит закрывает соединение с клиентом.
*/
	int calc(int work_sock);
};

