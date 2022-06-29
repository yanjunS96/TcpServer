#include "Server.h"
#include <unistd.h>
using namespace SelfServer;

int main() {
    Server::Instance()->runStart();
    //std::cout << "Hello, World!" << std::endl;

    while(1)
    {
        pause();
    }

    return 0;
}
