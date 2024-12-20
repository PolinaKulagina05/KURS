#include "UnitTest++/UnitTest++.h"
#include "interface.h"
#include "calc.h"
#include <sstream>
#include <string>
#include <iostream>

TEST(TestHelpOutput) {
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

int main() {
    return UnitTest::RunAllTests();
}
