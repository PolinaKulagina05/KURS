#include "interface.h"
#include <iostream>
#include <getopt.h>
interface::interface(int argc, char *argv[])
    : argc_(argc), argv_(argv),
      file_name("/etc/vcalc.conf"),
      port(33333),
      file_error("/var/log/vcalc.log") {}
      
int interface::arguments() {
    const struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"file", required_argument, 0, 'f'},
        {"port", required_argument, 0, 'p'},
        {"error", required_argument, 0, 'e'},
        {0, 0, 0, 0}
    };
    int option;
    int option_index = 0;
    while ((option = getopt_long(argc_, argv_, "hf:p:e:", long_options, &option_index)) != -1) {
        switch (option) {
            case 'h':
                print_help();
                return 1;
            case 'f':
                file_name = std::string(optarg);
                break;
            case 'p':
                port = std::stoi(std::string(optarg));
                break;
            case 'e':
                file_error = std::string(optarg);
                break;
            case '?':
                std::cout << "Неверно введен параметр. Используйте -h для помощи." << std::endl;
                return 1;
            default:
                std::cout << "Неизвестный параметр. Используйте -h для помощи." << std::endl;
                return 1;
        }
    }
    return 0;
}
std::string interface::get_file_name() const {
    return file_name;
}
int interface::get_port() const {
    return port;
}
std::string interface::get_file_error() const {
    return file_error;
}
void interface::print_help() const {
    std::cout << "Калькулятор" << std::endl;
    std::cout << "-h --help Помощь" << std::endl;
    std::cout << "-f --file Название файла" << std::endl;
    std::cout << "-p --port Порт" << std::endl;
    std::cout << "-e --error Файл ошибок" << std::endl;
}
