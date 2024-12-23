#include "UnitTest++/UnitTest++.h"
#include "interface.h"
#include "calc.h"
#include <filesystem>

// Тест на вывод справки с коротким параметром
TEST(TestShortHelp) {
    const char* argv[] = { "program", "-h" };
    int argc = sizeof(argv) / sizeof(argv[0]);
    std::ostringstream captured_output;
    std::streambuf* original_cout = std::cout.rdbuf();
    std::cout.rdbuf(captured_output.rdbuf());
    interface iface(argc, const_cast<char**>(argv));
    int result = iface.arguments();
    std::cout.rdbuf(original_cout);
    std::string expected_output = "Калькулятор\n-h --help Помощь\n-f --file Название файла\n-p --port Порт\n-e --error Файл ошибок\n";
    CHECK_EQUAL(expected_output, captured_output.str());
}

// Тест на вывод справки с длинным параметром
TEST(TestLongHelp) {
	optind = 1;
    const char* argv[] = { "program", "--help" };
    int argc = sizeof(argv) / sizeof(argv[0]);
    std::ostringstream captured_output;
    std::streambuf* original_cout = std::cout.rdbuf();
    std::cout.rdbuf(captured_output.rdbuf());
    interface iface(argc, const_cast<char**>(argv));
    int result = iface.arguments();
    std::cout.rdbuf(original_cout);
    std::string expected_output = "Калькулятор\n-h --help Помощь\n-f --file Название файла\n-p --port Порт\n-e --error Файл ошибок\n";
    CHECK_EQUAL(expected_output, captured_output.str());
}

// Тест на проверку работы длинного параметра --file
TEST(TestLongOptionsF) {
    optind = 1;
    const char* argv[] = { "program", "--file", "base.txt" };
    std::size_t argc = std::size(argv);
    interface cli(argc, const_cast<char**>(argv));
    std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    int result = cli.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL("base.txt", cli.get_file_name());
}

// Тест на проверку работы длинного параметра --port
TEST(TestLongOptionsP) {
    optind = 1;
    const char* argv[] = { "program", "--port", "12345" };
    std::size_t argc = std::size(argv);
    interface cli(argc, const_cast<char**>(argv));
    std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    int result = cli.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL(12345, cli.get_port());
}

// Тест на проверку работы длинного параметра --error
TEST(TestLongOptionsE) {
    optind = 1;
    const char* argv[] = { "program", "--error", "error.txt" };
    std::size_t argc = std::size(argv);
    interface cli(argc, const_cast<char**>(argv));
    std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    int result = cli.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL("error.txt", cli.get_file_error());
}

// Тест на проверку работы короткого параметра -f
TEST(TestShortOptionsF) {
    optind = 1;
    const char* argv[] = { "program", "-f", "base.txt" };
    std::size_t argc = std::size(argv);
    interface cli(argc, const_cast<char**>(argv));
    std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    int result = cli.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL("base.txt", cli.get_file_name());
}

// Тест на проверку работы короткого параметра -p
TEST(TestShortOptionsP) {
    optind = 1;
    const char* argv[] = { "program", "-p", "12345" };
    std::size_t argc = std::size(argv);
    interface cli(argc, const_cast<char**>(argv));
    std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    int result = cli.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL(12345, cli.get_port());
}

// Тест на проверку работы короткого параметра -e
TEST(TestShortOptionsE) {
    optind = 1;
    const char* argv[] = { "program", "-e", "error.txt" };
    std::size_t argc = std::size(argv);
    interface cli(argc, const_cast<char**>(argv));
    std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    int result = cli.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL("error.txt", cli.get_file_error());
}

// Тест для проверки параметров по умолчанию
TEST(TestNoneOptions) {
    optind = 1;
    const char* argv[] = { "program" };
    std::size_t argc = std::size(argv);
    interface cli(argc, const_cast<char**>(argv));
    std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    int result = cli.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL("/etc/vcalc.conf", cli.get_file_name());
    CHECK_EQUAL(33333, cli.get_port());
    CHECK_EQUAL("/var/log/vcalc.log", cli.get_file_error());
}

// Тест на проверку работы с интерфейсом с длинным неправильным параметром
TEST(TestInterfaceInvalidArg) {
	optind = 1;
	opterr = 0;
	std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    const char* argv[] = { "program", "--invalid" };
    int argc = sizeof(argv) / sizeof(argv[0]);
    interface cli(argc, const_cast<char**>(argv));
    int result = cli.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL(1, result);
    CHECK_EQUAL("Неверно введен параметр. Используйте -h для помощи.\n", output_buffer.str().c_str());
}

// Тест на проверку работы с интерфейсом с коротким неправильным параметром
TEST(NonExistOptions) {
    optind = 1;
    opterr = 0;
    const char* argv[] = { "program", "-j" };
    std::size_t argc = std::size(argv);
    interface cli(argc, const_cast<char**>(argv));
    std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    int result = cli.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL(1, result);
    CHECK_EQUAL("Неверно введен параметр. Используйте -h для помощи.\n", output_buffer.str().c_str());
}

// Тест на успешное открытие файла c базой данных
TEST(TestError_SuccesOpenFile) {
    Error error;
    int result = error.er("base.txt", "error.txt");
    CHECK_EQUAL(1, result);
}

// Тест на ошибку открытия файла c базой данных
TEST(TestError_FailOpenFile) {
	opterr = 0;
	std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    Error error;
    int result = error.er("test_file.txt", "error.txt");
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL(12, result);
}

// Тест на успешное создание файла для записи ошибок
TEST(TestError_SuccesCreateErrorFale) {
    opterr = 0;
    std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    Error error;
    std::string error_file = "NONerror.txt";
    int result = error.er("test_file.txt", error_file);
    std::cout.rdbuf(original_cout);
    CHECK(std::filesystem::exists(error_file));
}

// Тест на ошибку открытия файла без раширения
TEST(TestError_FileWithoutExtension) {
	opterr = 0;
	std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    Error error;
    int result = error.er("base", "error.txt");
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL(12, result);
}

// Тест на длину соли
TEST(TestLenghtSalt) {
    Authorized auth{Error()};
    std::string message = "test_login7EDAD0BA80203B87";
    std::string salt = message.substr(message.length() - 16);
    CHECK_EQUAL(16, salt.length());
}

// Тест на проверку хэша
TEST(TestHash) {
    Authorized auth{Error()};
    std::string input = "test_string";
    std::string hash = auth.MD(input);
    CHECK_EQUAL("3474851A3410906697EC77337DF7AAE4", hash);
}

// Тест на недопустимое значение порта
TEST(TestNoInvalidPort) {
	opterr = 0;
	std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    Error errors;
    Server server(errors);
    std::string error;
    std::string file_error = "error.txt";
    int invalid_port_low = -1;
    CHECK_THROW(server.self_addr(error, file_error, invalid_port_low), std::runtime_error);
    int invalid_port_high = 9999999;
    CHECK_THROW(server.self_addr(error, file_error, invalid_port_high), std::runtime_error);
    std::cout.rdbuf(original_cout);
}

int main() {
    return UnitTest::RunAllTests();
}
