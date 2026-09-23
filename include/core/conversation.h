#ifndef conversation_h
#define conversation_h

#include <cstddef>
#include "core/message.h"

#include <stdexcept>

class Conversation {
public:
    Conversation() = default;
    
    // Destructor to clean up allocated memory
    ~Conversation() {
        delete[] data_;
    }

    void append(Message m) {
        if (size_ == capacity_) {
            // Handle capacity overflow (e.g., reallocate with increased capacity)
            std::size_t new_capacity;

            if (capacity_ == 0) {
                new_capacity = 1; // Start with a capacity of 1
            } else {
                new_capacity = capacity_ * 2; // Double the capacity
            }

            Message* new_data = new Message[new_capacity];

            try {
                for (std::size_t i = 0; i < size_; ++i) {
                    new_data[i] = data_[i]; // Copy existing messages to new array
                }
            } catch (...) {
                delete[] new_data; // Clean up if an exception occurs
                throw; // Rethrow the exception
            }

            delete[] data_; // Free the old array
            data_ = new_data; // Update data_ to point to the new array
            capacity_ = new_capacity; // Update capacity_
        }

        data_[size_] = m; // Add the new message to the end of the conversation
        ++size_; // Increment the size of the conversation
    }

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