#include <cassert>
#include <stdexcept>
#include "core/conversation.h"

int main() {
    Conversation conversation;
    assert(conversation.size() == 0);

    assert(conversation.begin() == conversation.end());

    bool caught = false;

    try {
        conversation.at(0);
    } catch (const std::out_of_range&) {
        caught = true;
    }

    assert(caught);

    // Append a message and check size
    conversation.append(Message(Role::System, "Instructions"));
    conversation.append(Message(Role::User, "Hello"));
    conversation.append(Message(Role::Assistant, "Hi there!"));


    Check size after appending messages
    assert(conversation.size() == 3);
    assert(conversation.at(0).content() == "Instructions");
    assert(conversation.at(1).content() == "Hello");
    assert(conversation.at(2).content() == "Hi there!");
    assert(conversation.at(0).role() == Role::System);
    assert(conversation.at(1).role() == Role::User);
    assert(conversation.at(2).role() == Role::Assistant);





        //COMMMENT OUT THE FOLLOWING LINE, USED TO SEE ERROR MESSAGES
    // assert(conversation.size() == 3);
    // assert(conversation.at(0).content() == "Hello");
    // assert(conversation.at(1).content() == "Instructions");
    // assert(conversation.at(2).content() == "Hi there!");
    // assert(conversation.at(0).role() == Role::User);
    // assert(conversation.at(1).role() == Role::System);
    // assert(conversation.at(2).role() == Role::Assistant);

}