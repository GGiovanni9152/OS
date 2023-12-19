#include <iostream>
#include <string>
#include <zmq.hpp>
#include <thread>
#include "User.h"

class Client
{
    public:
        Client();
        Client(std::string IP);

        void connect();

        void send(std::string text);
        void read();
        void event_processing();
        
        void register_socket();
        
        void disconnect();

        ~Client() = default;

    private:
        std::string IP;
        zmq::socket_t socket;
        User user;
        bool logged_in = false;
};
