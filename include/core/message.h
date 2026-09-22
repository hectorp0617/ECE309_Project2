#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

enum class Role {System, User, Assistant};

class Message {
public:    
    Message()
    : role_(Role::System), content_("") {}

    Message(Role role, std::string content)
    : role_(role), content_(content) {}


//Return Role of the message
Role role() const noexcept {
    return role_;
}

//Return content of the message
const std::string& content() const noexcept {
    return content_;
}

private:
    Role role_;
    std::string content_;
};

#endif // MESSAGE_H