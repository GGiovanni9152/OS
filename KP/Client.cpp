#include "Client.h"

static zmq::context_t context;


Client::Client()
{
    IP = "tcp://127.0.0.1:/5555";
}

Client::Client(std::string IP)
{
    this -> IP = IP;
}

void Client::connect()
{
    std::cout << "Client connecting...\n";
    socket = zmq::socket_t(context, zmq::socket_type::dealer);
    socket.connect(IP);
    register_socket();
}

void Client::register_socket()
{
    std::string message = "REG";
    zmq::message_t registration(message);
    socket.send(registration);
    zmq::message_t id;
    socket.recv(id);
    user.set_id(id);
}

void Client::read()
{
    while(1)
    {
        zmq::message_t response;
        socket.recv(response);
        std::cout << response.to_string() << "\n";

        std::string answer = response.to_string();

        if (answer == "Login successful")
        {
            user.set_status(true);
            logged_in = true;
        }

        if (answer == "Login error")
        {
            user.set_username("");
            logged_in = false;
        }

        if (answer == "Logout successful")
        {
            user.set_status(false);
            logged_in = false;
        }

    }
}

void Client::event_processing()
{
    std::thread recv(&Client::read, this);

    while(1)
    {
        std::string command;

        if (logged_in)
        {
            std::cout << user.get_username() << ": ";
        }

        else
        {
            std::cout << "Please, log in: ";
        }

        std::cin >> command;

        if (command == "LOGIN")
        {
            std::string username;
            std::cin >> username;
            command = command + " " + username;
            user.set_username(username);
            send(command);

        }

        else if (command == "SEND")
        {
            if (logged_in)
            {
                std::string username, message;
                std::cin >> username;
                getline(std::cin, message);
                command = command + " " + username + " " + message;
                send(command);
            }

            else
            {
                std::cout << "You must log in first!\n";
            }

        }

        else if (command == "MAKEGROUP")
        {
            if (logged_in)
            {
                std::string groupname, members, creator;
                std::cin >> groupname;
                getline(std::cin, members);
                creator = user.get_username();
                //std::cout << members;
                command = command + " " + groupname + " " + creator + " " +members;
                send(command);
            }

            else
            {
                std::cout << "You must log in first!\n";
            }
        }
        
        else if (command == "GROUPSEND")
        {
            if (logged_in)
            {
                std::string groupname, message;
                std::cin >> groupname;
                getline(std::cin, message);
                command = command + " " + groupname + " " + message;
                send(command);
            }

            else
            {
                std::cout << "You must log in first!\n";
            }
        }

        else if (command == "LOGOUT")
        {
            if (logged_in)
            {
                send(command);
            }

            else
            {
                std::cout << "You must log in first\n";
            }
        }

        else
        {
            std::cout << "Wrong input\n";
        }
    }
}

void Client::disconnect()
{
    socket.disconnect(IP);
}

void Client::send(std::string text)
{
    zmq::message_t message(text);
    socket.send(message);
}



