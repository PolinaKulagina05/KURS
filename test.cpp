#include "UnitTest++/UnitTest++.h"
#include "interface.h"
#include "calc.h"

TEST(TestShortHelp) {
    const char* argv[] = { "program", "-h" };
    int argc = sizeof(argv) / sizeof(argv[0]);
    std::ostringstream captured_output;
    std::streambuf* original_cout = std::cout.rdbuf();
    std::cout.rdbuf(captured_output.rdbuf());
    interface iface(argc, const_cast<char**>(argv));
    int result = iface.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL(1, result);
    std::string expected_output = "Калькулятор\n-h --help Помощь\n-f --file Название файла\n-p --port Порт\n-e --error Файл ошибок\n";
    CHECK_EQUAL(expected_output, captured_output.str());
}

TEST(TestLongHelp) {
    const char* argv[] = { "program", "--help" };
    int argc = sizeof(argv) / sizeof(argv[0]);
    std::ostringstream captured_output;
    std::streambuf* original_cout = std::cout.rdbuf();
    std::cout.rdbuf(captured_output.rdbuf());
    interface iface(argc, const_cast<char**>(argv));
    int result = iface.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL(1, result);
    std::string expected_output = "Калькулятор\n-h --help Помощь\n-f --file Название файла\n-p --port Порт\n-e --error Файл ошибок\n";
    CHECK_EQUAL(expected_output, captured_output.str());
}

TEST(TestLongOptions) {
    optind = 1;
    const char* argv[] = { "program", "--file", "base.txt", "--port", "12345", "--error", "error.txt" };
    std::size_t argc = std::size(argv);
    interface cli(argc, const_cast<char**>(argv));
    std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    int result = cli.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL(0, result);
    CHECK_EQUAL("base.txt", cli.get_file_name());
    CHECK_EQUAL(12345, cli.get_port());
    CHECK_EQUAL("error.txt", cli.get_file_error());
}

TEST(TestShortOptions) {
    optind = 1;
    const char* argv[] = { "program", "-f", "base.txt", "-p", "12345", "-e", "error.txt" };
    std::size_t argc = std::size(argv);
    interface cli(argc, const_cast<char**>(argv));
    std::ostringstream output_buffer;
    auto original_cout = std::cout.rdbuf(output_buffer.rdbuf());
    int result = cli.arguments();
    std::cout.rdbuf(original_cout);
    CHECK_EQUAL(0, result);
    CHECK_EQUAL("base.txt", cli.get_file_name());
    CHECK_EQUAL(12345, cli.get_port());
    CHECK_EQUAL("error.txt", cli.get_file_error());
}

int main() {
    return UnitTest::RunAllTests();
}
