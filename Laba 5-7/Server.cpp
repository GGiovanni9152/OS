#include "CalculationNode.h"
#include "ZmqCommands.h"
#include "Tree.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        exit(EXIT_FAILURE);
    }

    CalculationNode node(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));

    while(1)
    {
        std::string message;
        std::string command;
        message = receive_message(node.parent);
        std::istringstream request(message);

        request >> command;

        if (command == "pid")
        {
            std::string answer = std::to_string(getpid());
            send_message(node.parent, answer);
        }

        else if(command == "ping")
        {
            int child;
            request >> child;
            std::string answer = node.ping(child);
            send_message(node.parent, answer);
        }

        else if (command == "create")
        {
            int child;
            request >> child;
            std::string answer = node.create(child);
            send_message(node.parent, answer);
        }

        else if (command == "send")
        {
            int child;
            std::string string;
            request >> child;
            getline(request, string);
            string.erase(0, 1);
            std::string answer = node.send_message_string(string, child);
            send_message(node.parent, answer);
        }

        else if (command == "exec")
        {
            std::string string;
            getline(request, string);
            std::string answer = node.exec(string);
            send_message(node.parent, answer);
        }

        else if (command == "kill")
        {
            std::string answer = node.kill();
            send_message(node.parent, answer);
            disconnect(node.parent, node.parent_port);
            node.parent.close();
            break;
        }

        else if (command == "clear")
        {
            int child;
            request >> child;
            std::string answer = node.clean_tree(child);
            send_message(node.parent, answer);
        }
    }

    return 0;
}



