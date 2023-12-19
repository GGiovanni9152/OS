#include <iostream>
#include "Client.h"

int main()
{
    Client client("tcp://127.0.0.1:5555");
    client.connect();
    client.event_processing();
    client.disconnect();

    return 0;
}