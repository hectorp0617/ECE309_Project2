#include <cassert>
#include <string>

#include "core/sentinel_scanner.h"

int main() {
    // Test the SentinelScanner with a sentinel string
    SentinelScanner scanner("END");

    // Feed a chunk that does not contain the sentinel
    auto result1 = scanner.feed("Hello, this is a test. ");
    assert(result1.safe_text == "Hello, this is a test");
    assert(result1.sentinel_found == false);

    // Feed a chunk that contains the sentinel
    auto result2 = scanner.feed("This is another test. END This part should not be included.");
    assert(result2.safe_text == ". This is another test. ");
    assert(result2.sentinel_found == true);

    // Flush any remaining text
    auto result3 = scanner.flush();
    assert(result3.safe_text.empty());
    assert(result3.sentinel_found == false);


    SentinelScanner scanner2("END");

    auto result4 = scanner2.feed("Goodbye.EN");
    assert(result4.safe_text == "Goodbye.");
    assert(result4.sentinel_found == false);

    auto result5 = scanner2.feed("D");
    assert(result5.safe_text.empty());
    assert(result5.sentinel_found == true);

    const std::string sentinel = "<|end_conversation|>";
    const std::string text = "Goodbye." + sentinel;

    // Test feeding the text in various ways to ensure the sentinel is detected correctly
    for (std::size_t i = 0; i <= text.size(); ++i) {
        SentinelScanner boundary_scanner(sentinel);

        auto first_part = boundary_scanner.feed(text.substr(0, i));          // Feed the first part of the text  
        auto second_part = boundary_scanner.feed(text.substr(i));     // Everything after the first part of the text

        
        assert(second_part.sentinel_found || first_part.sentinel_found);        // Ensure that the sentinel was found in either part

        assert(first_part.safe_text + second_part.safe_text == "Goodbye.");     // Ensure that the safe text from both parts matches the expected safe text
    }

    // Test feeding the text one character at a time to ensure the sentinel is detected correctly
     SentinelScanner between_scanner(sentinel);
     std::string collected;
     bool found = false;

     for  (std::size_t i = 0; i < text.size(); ++i) {
            auto result = between_scanner.feed(text.substr(i, 1));        // Feed one character at a time
    
            collected += result.safe_text;        // Collect the safe text
            found = found || result.sentinel_found;        // Update the sentinel found status
        }


    assert(found);        // Ensure that the sentinel was found
    assert(collected == "Goodbye.");        // Ensure that the collected safe text matches the expected safe text
    //End of test cases

    //Partial marker test

    SentinelScanner partial_scanner("END");
    auto part1 = partial_scanner.feed("Hello, EN");

    auto flush_result = partial_scanner.flush();

    assert(part1.sentinel_found == false);
    assert(flush_result.sentinel_found == false);\

    assert(part1.sentinel_found + flush_result.sentinel_found == false);

    auto second_flush_result = partial_scanner.flush();

    assert(second_flush_result.sentinel_found == false);




    return 0;
    }





