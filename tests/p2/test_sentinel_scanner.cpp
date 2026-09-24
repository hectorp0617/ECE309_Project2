#include <cassert>

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

    return 0;

}