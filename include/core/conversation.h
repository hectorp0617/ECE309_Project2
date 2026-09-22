#ifndef conversation_h
#define conversation_h

#include <cstddef>
#include "core/message.h"


class Conversation {
public:
    Conversation() = default;

    std::size_t size() const noexcept {
        // Return the number of messages in the conversation
        return size_;
    }

private:
    Message* data_ = nullptr;
    std::size_t size_ = 0;      // Number of messages in the conversation
    std::size_t capacity_ = 0;      // Capacity of the conversation (maximum number of messages it can hold)
};


#endif  // conversation_h