#include "CalculationNode.h"
#include "ZmqCommands.h"
#include "Tree.h"

int main()
{
    std::string command;
    CalculationNode node(-1, -1, -1);
    std::string answer;

    std::cout << "Commands:\n";
    std::cout << "\t" << "create id: creates new calculatin node" << std::endl;
    std::cout << "\t" << "exec id n n1 n2 ... nk: calculating sum" << std::endl;
    std::cout << "\t" << "kill id: killing a calculation node" << std::endl;
    std::cout << '\t' << "ping id: checking node availability" << std::endl;

    Tree tree;

    while ((std::cout << "Please enter command: ") && (std::cin >> command))
    {
        if (command == "create")
        {
            int child;
            std::cin >> child;

            if (tree.Exist(child))
            {
                std::cout << "Already exists" << std::endl;
            }

            else
            {
                while (1)
                {
                    int parent_id = tree.FindID();
                    
                    if(parent_id == node.id)
                    {
                        answer = node.create(child);
                        tree.Add(child, parent_id);
                        break;
                    }

                    else
                    {
                        std::string message = "create " + std::to_string(child);
                        answer = node.send_message_string(message, parent_id);
                        if (answer == "Parent not found")
                        {
                            tree.Availability_check(parent_id);
                        }
                        else
                        {
                            tree.Add(child, parent_id);
                            break;
                        }
                    }
                }

                std::cout << answer << std::endl;
            }
        }

        else if (command == "exec")
        {
            std::string string;
            int child;
            std::cin >> child;
            getline(std::cin, string);

            if (!tree.Exist(child))
            {
                std::cout << "Parent not exist" << std::endl;
            }

            else
            {
                std::string message = "exec " + string;
                answer = node.send_message_string(message, child);
                std::cout << answer << std::endl;
            }
        }

        else if (command == "ping")
        {
            int child;
            std::cin >> child;

            if(!tree.Exist(child))
            {
                std::cout << "Parent is not exist" << std::endl;
            }

            else if (node.left_id == child || node.right_id == child)
            {   
                answer = node.ping(child);
                std::cout << answer << std::endl;
            }

            else
            {
                std::string message = "ping " + std::to_string(child);
                answer = node.send_message_string(message, child);

                if (answer == "Parent not found")
                {
                    answer = "Ok: 0";
                }

                std::cout << answer << std::endl;
            }
        }

        else if (command == "kill")
        {
            int child;
            std::cin >> child;
            std::string message = "kill";

            if (!tree.Exist(child))
            {
                std::cout << "Parent is not exist" << std::endl;
            } 

            else
            {
                answer = node.send_message_string(message, child);

                if (answer != "Parent not found")
                {
                    tree.Remove_from_tree(child);

                    if (child == node.left_id)
                    {
                        unbind(node.left, node.left_port);
                        node.left_id = -2;
                        answer = "Ok";
                    }

                    else if (child == node.right_id)
                    {
                        node.right_id = -2;
                        unbind(node.right, node.right_port);
                        answer = "Ok";
                    }

                    else
                    {
                        message = "clear " + std::to_string(child);
                        answer = node.send_message_string(message, std::stoi(answer));
                    }

                    std::cout << answer << std::endl;
                }
            }
        }

        else
        {
            std::cout << "Wrong command" << std::endl;
        }
    }

    node.kill();

    return 0;
}