#include <iostream>
#include "Server.h"

int main()
{
    Server server("tcp://127.0.0.1:5555");
    server.run();
    server.event_processing();
    server.stop();
    return 0;
}