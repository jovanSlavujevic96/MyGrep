#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>

#include "my_grep_api/MyGrep.h"
#include "my_grep_api/Printer.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: $ ./my_grep \"KEYWORD\" \"PATH\"" << std::endl;
        return 1;
    }

    const std::string keyword = argv[1];
    const std::string path = argv[2];
    if (!std::filesystem::exists(path))
    {
        std::cerr << "Cannot find path: \"" << path << "\"" << std::endl;
        return 1;
    }

    MyGrep myGrep(new Printer());


    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    auto t1 = high_resolution_clock::now();

    myGrep.grep(keyword, path);

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() << "ms" << std::endl;

    return 0;
}
