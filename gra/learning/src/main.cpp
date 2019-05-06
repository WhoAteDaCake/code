
#include "Engine.h"

int main(int iArgc, char **cppArgv)
{
    std::unique_ptr<Engine> engine(new Engine(iArgc, cppArgv, "Test window", 400, 400));

    try
    {
        engine->initialize();
    }
    catch (std::string message)
    {
        std::cout << message << "\n";
    }

    return 0;
}