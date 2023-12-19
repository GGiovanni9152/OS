#include "Server.h"
#include <sstream>

static zmq::context_t context;

Server::Server()
{
    IP = "tcp://127.0.0.1:/5555";
}

Server::Server(std::string ip)
{
    this -> IP = ip;
}

void Server::run()
{
    std::cout << "Server starts...\n";
    socket = zmq::socket_t(context, zmq::socket_type::router);
    socket.bind(IP);
}

bool Server::isOnline(std::string username)
{
    for (int user = 0; user < users.size(); ++user)
    {
        if (users[user].get_username() == username)
        {
            return users[user].get_status();
        }
    }

    return false;
}

User Server::search_username(std::string username)
{
    for (int user = 0; user < users.size(); ++user)
    {
        if (users[user].get_username() == username)
        {
            return users[user];
        }
    }
}

User Server::search_id(zmq::message_t& id)
{
    for (int user = 0; user < users.size(); ++user)
    {
        if (users[user].get_id().to_string() == id.to_string())
        {
            return users[user];
        }
    }
}

void Server::registration(zmq::message_t& id)
{
    User user(id);
    this -> users.push_back(user);
    std::string message = id.to_string();
    send(id, message);
}

void Server::login(zmq::message_t& id, std::stringstream& input)
{
    std::string username;
    std::string answer;

    input >> username;

    if (!isOnline(username))
    {
        for (int user = 0; user < users.size(); ++user)
        {
            if (users[user].get_id().to_string() == id.to_string() && users[user].get_status() == false)
            {
                users[user].set_username(username);
                users[user].set_status(true);
            }
        }

        answer = "Login successful";
    }
    else
    {
        answer = "Login error";
    }

    send(id, answer);
}

void Server::logout(zmq::message_t& id)
{
    std::string answer;

    for (int user = 0; user < users.size(); ++user)
    {
        if (users[user].get_id().to_string() == id.to_string())
        {
            users[user].set_status(false);
            users[user].set_username("");
        }
    }

    answer = "Logout successful";
    send(id, answer);
}

void Server::send(zmq::message_t& id, std::string message)
{
    zmq::message_t zmq_message(message);
    socket.send(id, ZMQ_SNDMORE);
    socket.send(zmq_message);
}

void Server::send_message(zmq::message_t& sender_id, std::stringstream& input)
{
    std::string username, message, input_word;
    input >> username;
    while(input >> input_word) message += input_word + " ";

    if (isOnline(username))
    {
        User getter = search_username(username);
        User sender = search_id(sender_id);

        zmq::message_t getter_id;
        getter_id.copy(getter.get_id());

        std::string sending = sender.get_username() + ": " + message;

        send(getter_id, sending);
        send(sender_id, "Sending successful");
    }

    else
    {
        send(sender_id, "User is not online");
    }
}

void Server::send_group(zmq::message_t& sender_id, std::stringstream& input)
{
    std::string groupname, message, input_word;
    input >> groupname;
    while(input >> input_word) message += input_word + " ";
    
    if (groups.contains(groupname))
    {
        for (std::string getter : groups[groupname])
        {
            if (isOnline(getter))
            {
                User getter_account = search_username(getter);
                User sender = search_id(sender_id);

                zmq::message_t getter_id ;
                getter_id.copy(getter_account.get_id());

                std::string sending = groupname + "(" + sender.get_username() + "): " + message;
                
                send(getter_id, sending);
            }
        }

        send(sender_id, "Sending successful");

    }

    else
    {
        send(sender_id, "Group doesn't exits");
    }

}

void Server::make_group(zmq::message_t& id, std::stringstream& input)
{
    std::string group_name, username;

    std::vector<std::string> usernames;

    input >> group_name;

    while(input >> username) {usernames.push_back(username);}

    groups.insert(std::make_pair(group_name, usernames));

    send(id, "Group created");
}

void Server::users_list()
{
    std::cout << "==========Users List==========\n";
    std::cout << "[";

    for (User user: users)
    {
        std::cout << user;
    }

    std::cout << "]\n";
    std::cout << "===========Group List==========\n";
    std::cout << "[";

    for (auto group : groups)
    {
        std::cout << "Groupname: "<< group.first << ", Members: ";

        for (std::string user : group.second)
        {
            std::cout << user << ' '; 
        }

        std::cout << "\n";

    }

    std::cout << "]\n";
}

void Server::event_processing()
{
    while (1)
    {
        users_list();

        zmq::message_t id;
        zmq::message_t message;

        socket.recv(id);
        socket.recv(message);

        std::string string_message = message.to_string();
        std::stringstream input(string_message);
        std::string command;
        input >> command;

        std::cout << "[" << command << "] " << id.to_string() << " | " << message.to_string() << "\n";

        if (command == "REG")
        {
            registration(id); 
        }
        
        else if (command == "LOGIN")
        {
            login(id, input);
        }

        else if (command == "SEND")
        {
            send_message(id, input);
        }

        else if (command == "MAKEGROUP")
        {
            make_group(id, input);
        }

        else if (command == "GROUPSEND")
        {
            send_group(id, input);
        }

        else if (command == "LOGOUT")
        {
            logout(id);
        }

        else
        {
            std::string answer;
            answer = "Wrong command";
            send(id,answer);
            command = answer;
        }
    }
}

void Server::stop()
{
    socket.unbind(IP);
}














