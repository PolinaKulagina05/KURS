#include "calc.h"
using namespace std;

void Authorized::msgsend(int work_sock, const string& mess){
    char *buffer = new char[4096];
    strcpy(buffer, mess.c_str());
    send(work_sock, buffer, mess.length(), 0);
}

string Authorized::MD(const string& sah){
Weak::MD5 hash;
    string digest;
    StringSource(sah, true,  new HashFilter(hash, new HexEncoder(new StringSink(digest))));
      return digest;
 }

Error::Error() {}
 
void Error::errors(string error, string name){
    ofstream file;
    file.open(name, ios::app);
    if(file.is_open()){
        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        file <<error << ':' << asctime(timeinfo) << endl;
        cout << "error: " << error << endl;
    }
}

int Error::er(string file_name, string file_error) {
        fstream file;
        file.exceptions(ifstream::badbit | ifstream::failbit);
        try {
        	file.open(file_name);
        	return 1;
        } catch(const exception & ex) {
        	string error = "Ошибка открытия файла";
        	errors(error, file_error);
        	return 12;
        }
}

void alarm_handler(int signal) {
    cout << "Время ожидания истекло\n";
    exit(EXIT_FAILURE);
}

int Server::self_addr(string& error, string& file_error, int port) {
    if (port < 0 || port > 65535) {
        error = "Некорректное значение порта: " + to_string(port);
        e_error.errors(error, file_error);
        throw std::runtime_error("Invalid port value");
        return -1;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }
    int on = 1;
    int rc = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (rc == -1) {
        throw system_error(errno, generic_category(), "Ошибка установки сокета");
    }
    signal(SIGALRM, alarm_handler);
    alarm(240);
    struct timeval timeout {240, 0};
    rc = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (rc == -1) {
        throw system_error(errno, generic_category(), "Ошибка установки сокета");
    }
    sockaddr_in self_addr;
    self_addr.sin_family = AF_INET;
    self_addr.sin_port = htons(port);
    self_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    cout << "Ожидание подключения клиента...\n";
    int b = bind(sock, reinterpret_cast<sockaddr*>(&self_addr), sizeof(self_addr));
    if (b == -1) {
        cout << "Ошибка привязки\n";
        error = "Ошибка";
        e_error.errors(error, file_error);
        return 1;
    }
    listen(sock, SOMAXCONN);
    return sock;
}

int Server::client_addr(int s, string& error, string& file_error) {
        sockaddr_in * client_addr = new sockaddr_in;
        socklen_t len = sizeof (sockaddr_in);
        int work_sock = accept(s, (sockaddr*)(client_addr), &len);
        if(work_sock == -1) {
            cout << "Ошибка\n";
            error = "Ошибка";
            e_error.errors(error, file_error);
            return 1;
        } else {
        	cout << "Клиент успешно подключился!\n";
            return work_sock;
        }
}

int Authorized::authorized(int work_sock, string file_name, string file_error)
{
    string ok = "OK";
    string err = "ERR";
    string error;
    char msg[255] = {0};

    //АВТОРИЗАЦИЯ
    recv(work_sock, &msg, sizeof(msg), 0);
    string message = msg;
    string login, hashq;
    fstream file;
    file.open(file_name);
    getline (file, login, ':');
    getline (file, hashq);

    //СВЕРКА ЛОГИНА
    if(message.substr(0, login.length()) != login){
        msgsend(work_sock,  err);
        error = "Ошибка логина";
        e_error.errors(error, file_error);
        close(work_sock);
        return 1;
    } else {

        //РАБОТА С СОЛЬЮ
        string salt = message.substr(login.length(), 16);
        string sah = salt + hashq;
        string digest;
        digest = MD(sah);

        //СВЕРКА ПАРОЛЯ
        string received_hash = message.substr(login.length() + 16);
        if (digest != received_hash) {
            msgsend(work_sock, err);
            error = "Ошибка пароля";
            e_error.errors(error, file_error);
            close(work_sock);
            return 1;
        } else {
            msgsend(work_sock, ok);
            return 0;
        }
    }
}

int Calculator::calc(int work_sock)
{
    int Quantity;
    int Length;
    float Vector_numbers;
    recv(work_sock, &Quantity, sizeof(Quantity), 0);
    for(int j = 0; j < Quantity; j++) {
        recv(work_sock, &Length, sizeof(Length), 0);
        float Amount = 0;
        for(int i = 0; i < Length; i++) {
            recv(work_sock, &Vector_numbers, sizeof(Vector_numbers), 0);
            Amount += Vector_numbers;
        }
        send(work_sock, &Amount, sizeof(Amount), 0);
    }
    cout << "Завершение вычислений." << endl;
    close(work_sock);
    return 1;
}
