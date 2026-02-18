#include "gmurman/inc/gmurman.hpp"
#include <iostream>
#include <string>

int main() {
    std::srand(std::time(0));
    try
    {
        //_6gmurman(1000000000);
        //_8gmurman(1000000000);
        //_13gmurman(1000000000);
        //_15gmurman(1000000000);
        //_19gmurman(1000000000);
        //_20gmurman(1000000000);
        //_21gmurman(1000000000);
        //_22gmurman(1000000000);
        //_26gmurman(1000000000);
        //_27gmurman(1000000000);
        //_28gmurman(1000000000);
        //_29gmurman(1000000000);
        //_30gmurman(1000000000);
        //_32gmurman(1000000000);
        //_45gmurman(1000000000);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error : " << e.what() << "\n";
    }
    catch (...)
    {
        std::cerr << "Unknown error" << "\n";
    }

    return 0;
}
