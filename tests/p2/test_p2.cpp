// tests/p2/test_p2.cpp
//
// YOUR test suite goes here. At least 12 assert-based test cases — see
// spec §5 for the required categories and the sample test for the
// expected level of rigor.
//
// This file is a stub so the project builds out of the box; replace the
// body of main() with your own tests.

#include "core/conversation.h"
#include "core/message.h"
#include "core/sentinel_scanner.h"
#include "harness/harness.h"
#include "model/replay_client.h"
#include "model/scripted_client.h"

#include <cassert>
#include <stdexcept>

void test_empty_conversation() {
    Conversation local;
    assert(local.size() == 0);      // Ensure that the size of an empty conversation is 0
    assert(local.begin() == local.end());       // The begin and end iterators should be equal for an empty conversation

}

void test_ec_bounds() {       //testing empty conversation bounds
    Conversation converasation;
    bool caught = false;
    
    try {
        converasation.at(0);
    } catch (const std::out_of_range) {
        caught = true;
    }
} 

void test_order_after (){
    Conversation conversation;

    conversation.append(Message(Role::System, "Instructions"));
    conversation.append(Message(Role::User, "Hello"));
    conversation.append(Message(Role::Assistant, "Hi"));

    assert(conversation.size() == 3);       

    assert(conversation.at(0).role() == Role::System);
    assert(conversation.at(0).content() == "Instructions");

    assert(conversation.at(1).role() == Role::User);
    assert(conversation.at(1).content() == "Hello");

    assert(conversation.at(2).role() == Role::Assistant);
    assert(conversation.at(2).content() == "Hi");

}


   


int main() {
    // TODO: write your tests here.
    test_empty_conversation();
    test_ec_bounds();
    test_order_after();

    return 0;
}
