
#include <vsys.hpp>
#include <ServerProcess.hpp>

#include <iostream>
#include <string>

StringVector parseArguments(int argc, char* argv[])
{
    std::vector<std::string> args(argc);

    for (std::size_t i = 0; i < argc; ++i)
        args[i] = std::string(argv[i]);

    return args;
}

int main(int argc, char* argv[])
{
    std::cout << "hello world, this is vsys <3\n";

    auto args = parseArguments(argc, argv);
    ServerProcess prc(args);
    prc.run();

    return EXIT_SUCCESS;
}

