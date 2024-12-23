#include <iostream>
#include <string>
#include "interface.h"
#include "calc.h"

int main(int argc, char *argv[]) {
    interface cli(argc, argv);
    int result = cli.arguments();
    if (result != 0) {
        return result;
    }
    
    std::string file_name = cli.get_file_name();
    int port = cli.get_port();
    std::string file_error = cli.get_file_error();

    std::cout << "Данные из interface:" << std::endl;
    std::cout << "Файл: " << file_name << std::endl;
    std::cout << "Порт: " << port << std::endl;
    std::cout << "Файл ошибок: " << file_error << std::endl;

    std::string error;

    Error errors;
    if (errors.er(file_name, file_error) == 12) {
        std::cout << "Ошибка открытия файла" << std::endl;
        return 1;
    }

    Server server(errors);
    Authorized authorized(errors);
    Calculator calculator(errors);
    int s = server.self_addr(error, file_error, port);

    while (true) {
        int work_sock = server.client_addr(s, error, file_error);
        if (work_sock == -1) {
            continue;
        }
        int auth_result = authorized.authorized(work_sock, file_name, file_error);
        if (auth_result == 0) {
            calculator.calc(work_sock);
        }
        close(work_sock);
    }
    return 0;
}
