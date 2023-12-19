#include <string>
#include <zmq.hpp>

class User
{
    public:
        User();
        User(zmq::message_t &id);
        User(zmq::message_t &id, std::string username);
        User(const User& user);
        std::string get_username();
        zmq::message_t& get_id();
        bool get_status();

        void set_username(std::string username);
        void set_id(zmq::message_t& id);
        void set_status(bool status);

        friend std::ostream& operator<<(std::ostream& outstream, const User user);

    private:
        std::string _username;
        zmq::message_t _id;
        bool logged_in;
};

