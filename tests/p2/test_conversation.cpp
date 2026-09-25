#include <cassert>
#include <stdexcept>
#include "core/conversation.h"

struct ConversationTest {
    static std::size_t get_capacity(const Conversation& conversation) {
        return conversation.capacity_;
    }
};

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


    //Check size after appending messages
    assert(conversation.size() == 3);
    assert(conversation.at(0).content() == "Instructions");
    assert(conversation.at(1).content() == "Hello");
    assert(conversation.at(2).content() == "Hi there!");
    assert(conversation.at(0).role() == Role::System);
    assert(conversation.at(1).role() == Role::User);
    assert(conversation.at(2).role() == Role::Assistant);


    Conversation copied(conversation);

    assert(copied.size() == conversation.size());
    assert (copied.begin() != conversation.begin());

    for (std::size_t i = 0; i < conversation.size(); ++i) {
        assert(copied.at(i).content() == conversation.at(i).content());
        assert(copied.at(i).role() == conversation.at(i).role());
    }


        //COMMMENT OUT THE FOLLOWING LINE, USED TO SEE ERROR MESSAGES
    // assert(conversation.size() == 3);
    // assert(conversation.at(0).content() == "Hello");
    // assert(conversation.at(1).content() == "Instructions");
    // assert(conversation.at(2).content() == "Hi there!");
    // assert(conversation.at(0).role() == Role::User);
    // assert(conversation.at(1).role() == Role::System);
    // assert(conversation.at(2).role() == Role::Assistant);


    // Test assignment operator
    Conversation assigned;
    assigned.append(Message(Role::User, "This will be replaced"));
    assigned = conversation;

    assert(assigned.size() == conversation.size());
    assert (assigned.begin() != conversation.begin());

    for (std::size_t i = 0; i < conversation.size(); ++i) {
        assert(assigned.at(i).content() == conversation.at(i).content());
        assert(assigned.at(i).role() == conversation.at(i).role());
    }

    // Test move constructor
    Conversation source;
    source.append(Message(Role::User, "Message 1"));
    const Message* original_address = source.begin();

    Conversation moved(std::move(source));

    assert(moved.begin() == original_address);
    assert(moved.size() == 1);
    assert(moved.at(0).content() == "Message 1");

    assert(source.size() == 0);
    assert(source.begin() == nullptr);
    assert(source.begin() == source.end());

    // Test move assignment operator
    Conversation move_source;
    move_source.append(Message(Role::User, "Message 2"));

    Conversation move_assigned;
    move_assigned.append(Message(Role::Assistant, "This will be replaced"));

    const Message* move_original_address = move_source.begin();

    // Move assignment
    move_assigned = std::move(move_source);

    assert(move_assigned.begin() == move_original_address);
    assert(move_assigned.size() == 1);
    assert(move_assigned.at(0).content() == "Message 2");
    assert(move_source.size() == 0);
    assert(move_source.begin() == nullptr);

    move_source.append(Message(Role::User, "Message 3"));
    assert(move_source.size() == 1);
    assert(move_source.at(0).content() == "Message 3");

    //Destination holds transferred date
    assert(move_assigned.size() == 1);
    assert(move_assigned.at(0).content() == "Message 2");
    assert(move_source.begin() != move_assigned.begin());

    //Testing self-assignment for copy assignment operator
    const Message* before_self_assign_address = move_assigned.begin();
    move_assigned = move_assigned; // Self-assignment
    assert(move_assigned.begin() == before_self_assign_address);
    assert(move_assigned.size() == 1);
    assert(move_assigned.at(0).content() == "Message 2");

    //Moving the object to itself should not change its state
    const Message* before_self_move_address = move_assigned.begin();
    move_assigned = std::move(move_assigned); // Self-move assignment
    assert(move_assigned.begin() == before_self_move_address);
    assert(move_assigned.size() == 1);
    assert(move_assigned.at(0).content() == "Message 2");
    
    // Test appending messages to trigger capacity growth
    Conversation growth;
    assert(growth.size() == 0);
    assert(ConversationTest::get_capacity(growth) == 0);

    // Append First message
    growth.append(Message(Role::User, "Message 1"));
    assert(growth.size() == 1);
    assert(ConversationTest::get_capacity(growth) == 1);

    // Append Second Message
    growth.append(Message(Role::User, "Message 2"));
    assert(growth.size() == 2);
    assert(ConversationTest::get_capacity(growth) == 2);

    // Append Third Message
    growth.append(Message(Role::User, "Message 3"));
    assert(growth.size() == 3);
    assert(ConversationTest::get_capacity(growth) == 4); // Capacity should double

    // Append Fourth Message
    growth.append(Message(Role::User, "Message 4"));
    assert(growth.size() == 4);
    assert(ConversationTest::get_capacity(growth) == 4); // Capacity remains the same

    // Append Fifth Message
    growth.append(Message(Role::User, "Message 5"));
    assert(growth.size() == 5);
    assert(ConversationTest::get_capacity(growth) == 8); // Capacity should double

    // Verify the contents of the messages after growth
    assert(growth.at(0).content() == "Message 1");


}