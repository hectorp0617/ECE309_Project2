#include <cassert>
#include <string>
#include "core/message.h"

int main() {
    // Test default constructor
    Message defaultMessage;
    assert(defaultMessage.role() == Role::System);
    assert(defaultMessage.content() == "");

    // Test parameterized constructor
    Message userMessage(Role::User, "Hello, Assistant!");
    assert(userMessage.role() == Role::User);
    assert(userMessage.content() == "Hello, Assistant!");

    Message assistantMessage(Role::Assistant, "Hello, User!");
    assert(assistantMessage.role() == Role::Assistant);
    assert(assistantMessage.content() == "Hello, User!");

    return 0;
}