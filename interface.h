#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
class interface {
public:
    interface(int argc, char *argv[]);
    int arguments();
    std::string get_file_name() const;
    int get_port() const;
    std::string get_file_error() const;
private:
    void print_help() const;
    int argc_;
    char **argv_;
    std::string file_name;
    int port;
    std::string file_error;
};
#endif
