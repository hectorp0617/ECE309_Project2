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
}