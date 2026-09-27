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
#include <utility>
#include <sstream>      //string input streams
#include <memory>

//Helper Function to test
struct ConversationTest {
    static std::size_t get_capacity(const Conversation& conversation) {
        return conversation.capacity_;
    }
};

//assert pending_ never exceeds sentinel.size() -1...
struct SentinelScannerTest { 
    static std::size_t get_pending_size(const SentinelScanner& scanner) { // buffer length
        return scanner.pending_.size(); //return char count
    } 
};

class TestInput : public InputSource { // provide input to harness
public:
    explicit TestInput(const std::string& text)
        : input_(text) {} // store input

    std::string read_line() override { // read next line
        std::string line; 
        if (!std::getline(input_, line)) { 
            eof_ = true; // keep record of the end of the input
        } 
        return line; // return input
    } 

    bool is_eof() const override { // input state
        return eof_; // return eof
    } 

private:
    std::istringstream input_; // read stored text
    bool eof_ = false; 
}; 

class TestOutput : public OutputSink { //harness output
public:
    std::string text; //store what was written

    void write(std::string_view chunk) override { 
        text.append(chunk); // add to stored text
    } 
}; 


//END OF HELPER FUNCTIONS





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
    } catch (const std::out_of_range&) {        //Catch by refrence
        caught = true;
    }

    assert(caught);

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

void test_cc() {        //Test copy constructor
    Conversation orig;
    orig.append(Message(Role::User, "Hello"));
    orig.append(Message(Role::Assistant, "Hi"));

    Conversation copied(orig);

    assert(copied.size() == orig.size());       //verify size
    assert(copied.begin() != orig.begin());

    for(std::size_t i = 0; i < orig.size(); i++) {
        assert(copied.at(i).role() == orig.at(i).role());
        assert(copied.at(i).content() == orig.at(i).content());
    }
}

void test_copy_assignment() {
    Conversation original;      //original conversation
    original.append(Message(Role::User, "Hello"));
    original.append(Message(Role::Assistant, "Hi"));

    Conversation assigned;      
    assigned.append(Message(Role::System, "Old contents"));

    assigned = original;

    assert(assigned.size() == original.size());     //veriy size
    assert(assigned.begin() != original.begin());

    //Cycle through roles and contents to verify equal to 
    for (std::size_t i = 0; i < original.size(); ++i) {
        assert(assigned.at(i).role() == original.at(i).role());
        assert(assigned.at(i).content() == original.at(i).content());
    }
}

void test_move_constructor() {
    Conversation source;
    source.append(Message(Role::User, "Moving"));

    const Message* original_address = source.begin();

    Conversation moved(std::move(source));

    assert(moved.begin() == original_address);
    assert(moved.size() == 1);
    assert(moved.at(0).content() == "Moving");
    assert(moved.at(0).role() == Role::User);

    assert(source.size() == 0);
    assert(source.begin() == nullptr);
    assert(source.begin() == source.end());
}

void test_move_assign() {
    Conversation source;
    source.append(Message(Role::User, "Check"));        //append message

    //Destination
    Conversation destination;
    destination.append(Message(Role::User, "old"));       //Append message

    const Message* orig_addr = source.begin();      //Save Address

    destination = std::move(source);        //Store contents here

    // ensure destination
    assert(destination.begin() == orig_addr);
    assert(destination.size() == 1);
    assert(destination.at(0).content() == "Check");
    assert(destination.at(0).role() == Role::User);

    //Source Check
    assert(source.size() == 0);
    assert(source.begin() == nullptr);
    assert(source.begin() == source.end());
}


//Capacity growth test function
void test_cap_growth() {

// Create conversation
    Conversation conversation;
    assert(conversation.size() == 0);
    assert(ConversationTest::get_capacity(conversation) == 0);

    // Anticipated capacities
    const std::size_t expected[] = {1, 2, 4, 4, 8};

    // Insert messages
    for (std::size_t i = 0; i < 5; ++i) {
        conversation.append(
            Message(Role::User, std::to_string(i))
        );

        // Ensure size and capacity
        assert(conversation.size() == i + 1);
        assert(ConversationTest::get_capacity(conversation) == expected[i]);

        // Check stored messages
        for (std::size_t j = 0; j <= i; ++j) {
            assert(conversation.at(j).content() == std::to_string(j));
            assert(conversation.at(j).role() == Role::User);
        }
    }
}

//text scanner
void text_scanner() {
    //Scanner
    SentinelScanner scanner("|<end_conversation|>");
    const std::string input = "Ordinary text";

    //Do something with the text
    auto result = scanner.feed(input);
    auto remaining = scanner.flush();

    //Verify results
    assert(result.sentinel_found == false);
    assert(remaining.sentinel_found == false);
    assert(result.safe_text + remaining.safe_text == input);
}

//Split boundary tests
void split_boundary_test () {
    //Stop marker
    const std::string sentinel = "<|end_conversation|>";
    const std::string text = "Goodbye." + sentinel;      //Input

    //test for every possible split point
    for(std::size_t i = 0; i <= text.size(); i++){
        SentinelScanner scanner(sentinel);      //new scanner

        auto one = scanner.feed(text.substr(0,i));      //input text prior to split
        auto two = scanner.feed(text.substr(i));        //split onward

        //assertions
        assert(one.sentinel_found || two.sentinel_found);       //either found marker
        assert(one.safe_text + two.safe_text == "Goodbye.");     //combined output (ENSURE EQUAL)
    }
}

void single_char_test() {
    const std::string sentinel = "<|end_conversation|>";        //Stop marker
    const std::string text = "Goodbye." + sentinel;     //Build input
    SentinelScanner scanner(sentinel);      //scanner for stream
    std::string collected;      // returned text
    bool found = false;     // Track whether marker detected

    //iterate thorugh each char
    for (std::size_t i = 0; i < text.size(); i++){
        auto result = scanner.feed(text.substr(i,1));       //insert one char
        collected += result.safe_text;      //gather safe text
        found = found || result.sentinel_found;     //recall detection
    }

    assert(found);      //Check marker detected 
    assert(collected == "Goodbye.");      //check initial text remains
}

void test_partial_flush(){
    SentinelScanner scanner("END");     //stop marker
    const std::string input = "Hello, EN";      //Ending with incomplete marker

    //Input and release
    auto result = scanner.feed(input);
    auto remaining = scanner.flush();

    assert(!result.sentinel_found);     //no incomplete marker was found
    assert(!remaining.sentinel_found);      //no marker in regards to flush
    assert(result.safe_text + remaining.safe_text == input);
}

//False alarm
void test_false_alarm() { // Ensure scanner dosent trigger on partial matches

    SentinelScanner scanner("<|end_conversation|>"); // Real marker
    const std::string input = "Hello <|end_world|> goodbye."; // duplicat marker

    //proccess and release
    auto result = scanner.feed(input); 
    auto remaining = scanner.flush(); 

    assert(result.sentinel_found == false); // No marker is matched
    assert(!remaining.sentinel_found); // no mathc from the flush
    assert(result.safe_text + remaining.safe_text == input); // all text was preserved

} 

//assert pending_ never exceeds sentinel.size() -1...

void test_bounded_memory() { 
    const std::string sentinel = "<|end_conversation|>"; // marker
    const std::string pattern = "<|end_"; // repeating input
    const std::size_t total = 4 * 1024 * 1024; 

    SentinelScanner scanner(sentinel); //scanner

    //iterate through e/a char
    for (std::size_t i = 0; i < total; ++i) { 
        auto result = scanner.feed(pattern.substr(i % pattern.size(), 1));// one char read
        //deny any false matches found
        assert(!result.sentinel_found); 
        //bound check
        assert(SentinelScannerTest::get_pending_size(scanner) <= sentinel.size() - 1); 
    } 
}

//Safe to destroy or reassign
void test_moved_from_reuse() { 
    Conversation source; // source.
    source.append(Message(Role::User, "Original")); // append message

    Conversation destination(std::move(source)); // move message
    source.append(Message(Role::User, "New")); // RECYCLE 

    //assertions
    assert(source.size() == 1); // message count
    assert(source.at(0).content() == "New"); // Check contents
    assert(destination.size() == 1); 

    assert(destination.at(0).content() == "Original");
    assert(source.begin() != destination.begin()); 
} 

//Confirm provided loop stops with TurnLimi when conversation is used underneath it
void test_turn_limit() {
    HarnessConfig config;       //configuration
    config.max_turns = 1;       //one turn

    //scripted replies
    auto model = std::make_unique<ScriptedModelClient>("scripts/test_turn_limit.script");

    Harness harness(std::move(model), config); // Transfer to harness
    TestInput input("Hello\nAnother question\n"); // two user messages.
    TestOutput output; // Capture the output

    auto reason = harness.run(input, output); 

    //assertions
    assert(reason.kind == StopReason::Kind::TurnLimit); // Check why it stopped.
    assert(harness.conversation().size() == 2); // one user and one assistant message, total of 2

    //check messages and reply
    assert(harness.conversation().at(0).content() == "Hello"); 
    assert(harness.conversation().at(1).content() == "First reply."); 
    assert(output.text.find("Second reply.") == std::string::npos);
}

//Loop halts when SentinelScanner reports the sentinel found
void test_sentinel_found() { 
    HarnessConfig config; 
    config.max_turns = 5; // 5 turns max
    
    //LOAD REPLIES
    auto model = std::make_unique<ScriptedModelClient>("scripts/test_sentinel_stop.script"); 

    Harness harness(std::move(model), config); // harness created
    TestInput input("Bye\nAnother question\n"); // provide two inputs
    TestOutput output; // get output

    auto reason = harness.run(input, output); // Proceed with conversation

    //ASSERTIONS
    assert(reason.kind == StopReason::Kind::Sentinel); // Check stop reason
    assert(harness.conversation().size() == 2); // only ONE turn completed
    assert(harness.conversation().at(1).content() == "Goodbye.<|end_conversation|>"); // check stored reply
    assert(output.text.find("Goodbye.") != std::string::npos); 
    assert(output.text.find("<|end_conversation|>") == std::string::npos); // marker is hidden?
    assert(output.text.find("SHOULD_NOT_APPEAR") == std::string::npos); // trailing text is removed
    assert(output.text.find("SECOND_REPLY") == std::string::npos); 
} 

   


int main() {
    // TODO: write your tests here.
    //All function calls to the above tests
    //TODO: Finish remaining tests (Capacity growth, plain text scanner, split, single char,... )


    test_empty_conversation();
    test_ec_bounds();
    test_order_after();
    test_cc();
    test_copy_assignment(); 
    test_move_constructor();
    test_move_assign();
    test_cap_growth();
    text_scanner();
    split_boundary_test();
    single_char_test();
    test_partial_flush();
    test_false_alarm();
    test_bounded_memory();
    test_moved_from_reuse();
    test_turn_limit();
    test_sentinel_found();


    return 0;
}
