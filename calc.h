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

class Error
{
public:
    Error();
    static void errors(std::string error, std::string name);
    static int er(std::string file_name, std::string file_error);
};

class Server
{
private:
    Error e_error;
public:
    Server(Error err) : e_error(err) {
        e_error = err;
    }
    int self_addr(std::string& error, std::string& file_error, int port);
    int client_addr(int s, std::string& error, std::string& file_error);
};

class Authorized
{
private:
    Error e_error;
    void msgsend(int work_sock, const std::string& mess);
    friend class TestTestSaltGenerator;
public:
    std::string MD(const std::string& sah);
    Authorized(Error err) : e_error(err) {
        e_error = err;
    }
	int authorized(int work_sock, string file_name, string file_error);
};

class Calculator
{
private:
    Error e_error;
public:
    Calculator(Error err) : e_error(err) {
        e_error = err;
    }
	int calc(int work_sock);
};

