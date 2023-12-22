#include "ZmqCommands.h"
#include "unistd.h"

class CalculationNode
{
    private:
        zmq::context_t context;

    public:
        zmq::socket_t left, right, parent;

        int id, left_id = -2, right_id = -2, parent_id;

        int left_port, right_port, parent_port;

        CalculationNode(int id, int parent_port, int parent_id): id(id), parent_port(parent_port),
            parent_id(parent_id), left(context, ZMQ_REQ), right(context, ZMQ_REQ), parent(context, ZMQ_REP)
        {
            if (id != -1)
            {
                connect(parent, parent_port);
            }
        }

        std::string create(int child_id)
        {
            int port;
            bool is_left = false;

            if (left_id == -2)
            {
                left_port = bind(left, child_id);
                left_id = child_id;
                port = left_port;
                is_left = true;
            }

            else if (right_id == -2)
            {
                right_port = bind(right, child_id);
                right_id = child_id;
                port = right_port;
            }

            else
            {
                return "Can't create node";
            }

            int proccess_id = fork();

            if (proccess_id == 0)
            {
                if (execl("./server", "server", std::to_string(child_id).c_str(), std::to_string(port).c_str(),
                    std::to_string(id).c_str(), (char*)NULL) == -1)
                {
                    std::cout << "Execl error" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            else
            {
                std::string child_pid;
                try
                {
                    if (is_left)
                    {
                        //std::cout << "Ara!!\n";
                        left.setsockopt(ZMQ_SNDTIMEO, 3000);
                        send_message(left, "pid");
                        child_pid = receive_message(left);
                    }

                    else
                    {
                        //std::cout << "Ara2!!\n";
                        right.setsockopt(ZMQ_SNDTIMEO, 3000);
                        send_message(right, "pid");
                        child_pid = receive_message(right);
                    }

                    return "Ok: " + child_pid;
                }

                catch(int) 
                {
                    return "Can't connect to child";
                }
            }
        }

        std::string ping(int id)
        {
            std::string answer = "Ok: 0";

            if (this -> id == id)
            {
                answer = "Ok: 1";
                return answer;
            }

            else if (left_id == id)
            {
                std::string message = "ping " + std::to_string(id);
                send_message(left, message);

                try
                {
                    message = receive_message(left);
                    
                    if (message == "Ok: 1")
                    {
                        answer = message;
                    }   
                }

                catch(int){}
            }

            else if (right_id == id)
            {
                std::string message = "ping " + std::to_string(id);
                send_message(right, message);

                try
                {
                    message = receive_message(right);
                    
                    if (message == "Ok: 1")
                    {
                        answer = message;
                    }   
                }

                catch(int) {}
            }

            return answer;
        }

        std::string send_message_string(std::string string, int id)
        {
            std::string answer = "Parent not found";

            if (left_id == -2 && right_id == -2)
            {
                return answer;
            }

            else if (left_id == id)
            {
                if (ping(left_id) == "Ok: 1")
                {
                    send_message(left, string);
                    try
                    {
                        answer = receive_message(left);
                    }

                    catch(int) {}
                }
            }

            else if (right_id == id)
            {
                if (ping(right_id) == "Ok: 1")
                {
                    send_message(right, string);
                    try
                    {
                        answer = receive_message(right);
                    }

                    catch(int) {}
                }
            }

            else
            {
                if (ping(left_id) == "Ok: 1")
                {
                    std::string message = "send " + std::to_string(id) + " " + string;
                    send_message(left, message);
                    try
                    {
                        message = receive_message(left);
                    }

                    catch(int)
                    {
                        message = "Parent not found";
                    }

                    if (message != "Parent not found")
                    {
                        answer = message;
                    }
                }

                if (ping(right_id) == "Ok: 1")
                {
                    std::string message = "send " + std::to_string(id) + " " + string;
                    send_message(right, message);
                    try
                    {
                        message = receive_message(right);
                    }

                    catch(int)
                    {
                        message = "Parent not found";
                    }

                    if (message != "Parent not found")
                    {
                        answer = message;
                    }
                }
            }

            return answer;
        }

        std::string exec(std::string string)
        {
            std::istringstream input(string);

            int result = 0;
            int amount, number;

            input >> amount;

            for (int str = 0; str < amount; ++str)
            {
                input >> number;
                result += number;
            }

            std::string answer = "Ok: " + std::to_string(id) +  " " + std::to_string(result);

            return answer;
        }

        std::string clean_tree(int child)
        {
            if (left_id == child)
            {
                left_id = -2;
                unbind(left, left_port);
            }

            else
            {
                right_id = -2;
                unbind(right, right_port);
            }

            return "Ok";
        }

        std::string kill()
        {
            if (left_id != -2)
            {
                if (ping(left_id) == "Ok: 1")
                {
                    std::string message = "kill";
                    send_message(left, message);
                    try
                    {
                        message = receive_message(left);
                    }
                    catch(int) {}

                    unbind(left, left_port);
                    left.close();
                }
            }

            if (right_id != -2)
            {
                if (ping(right_id) == "Ok: 1")
                {
                    std::string message = "kill";
                    send_message(right, message);
                    try
                    {
                        message = receive_message(right);
                    }
                    catch(int) {}

                    unbind(right, right_port);
                    right.close();
                }
            }

            return std::to_string(parent_id);
        }

        ~CalculationNode() {}

};