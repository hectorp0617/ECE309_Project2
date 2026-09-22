#ifndef conversation_h
#define conversation_h

#include <cstddef>
#include "core/message.h"

#include <stdexcept>

class Conversation {
public:
    Conversation() = default;

    std::size_t size() const noexcept {
        // Return the number of messages in the conversation
        return size_;
    }

    // Boundary check for accessing messages in the conversation
    const Message& at(std::size_t i) const {
        // Return the message at index i
        if (i >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[i];
    }

    const Message* begin() const noexcept {
        // Return a pointer to the first message in the conversation
        return data_;
    }

    const Message* end() const noexcept {
        if (size_ == 0) {
            return data_;
        }
        // Return a pointer to one past the last message in the conversation
        return data_ + size_;
    }

private:
    Message* data_ = nullptr;
    std::size_t size_ = 0;      // Number of messages in the conversation
    std::size_t capacity_ = 0;      // Capacity of slots allocated for messages in the conversation
};


#endif  // conversation_h