#include "User.h"

User::User()
{
    this -> logged_in = false;
}

User::User(zmq::message_t& id)
{
    this -> _id.copy(id);
    this -> logged_in = false;
}

User::User(zmq::message_t& id, std::string username)
{
    this -> _id.copy(id);
    this -> logged_in = false;
    this -> _username = username; 
}

User::User(const User& user)
{
    this -> _id.copy(&user._id);
    this -> _username = user._username;
    this -> logged_in = user.logged_in; 
}

std::string User::get_username()
{
    return _username;
}

zmq::message_t& User::get_id()
{
    return _id;
}

bool User::get_status()
{
    return logged_in;
}

void User::set_username(std::string username)
{
    this -> _username = username;
}

void User::set_id(zmq::message_t& id)
{
    this -> _id.copy(&id);
}

void User::set_status(bool status)
{
    this -> logged_in = status;
}

std::ostream &operator<<(std::ostream &outstream, User user)
{
    outstream << "{ID: " << user.get_id().to_string() << ", Username: " << user.get_username()
    << ", Status: " << user.get_status() << "}\n";

    return outstream;
}
