
#include <vsys.hpp>
#include <IProcess.hpp>

#include <iostream>
#include <string>

class Proc : public IProcess
{
    public:
        Proc(std::vector<std::string> args)
            :IProcess(args)
        {}

        int32_t run() override
        {
            std::cout << "running shit" << std::endl;

            for (auto& str : args_)
                std::cout << str << std::endl;

            return 42;
        }
};

int main(int argc, char* argv[])
{
    std::cout << "hello world, this is vsys <3\n";

    auto args = IProcess::parseArguments(argc, argv);
    Proc prc(args);
    prc.run();

    return EXIT_SUCCESS;
}

