*/+--# Design Log — Project 2

(500–800 words total. See spec §5 for what each section must cover.)

## Growth factor and amortized cost

Within Conversations, the messages are stored within a dynamically allocated array, beginning with both size zero and capacity zero, and with no allocated storage. If the array were to be empty, which at the start it is, the first append creates one slot. Furthermore, whenever the array were to become full, the capacity doubles in capacity. Lastly, size counts the store's messages, while capacity provides the available slots. 

Doubling the capacity whenever the array is to become full proves to be more efficient than continuously growing by one slot with every message. Furthermore, grouping by one slot on every append would require copying the already existing messages each time, resulting in O(n^2) total copying for n messages. On the other hand, with doubling the copying cost, follow a geometric series, meaning their sum is less than 2n. With new insertions, the total amount of element operations is O(n), resulting in an amortized cost of O(1) per append.


## Rule of Five evidence

The conversation class has its own dynamically allocated array, allowing me to implement the rule of five to control operations upon storage, containing the destructor, copy constructor, copy assignment operator, move constructor, and move assignment operator. Since the Conversation class explicitly defines or deletes any of the five special resource-management functions, it should also define or delete all five.

Firstly, the destructor destroys the array's message once the conversation's duration is over, releasing the allocation. Secondly, the copy constructor allocates sufficient capacity for the source's messages, then copies each message. Following the operation, there are now two identical conversations yet in different arrays, meaning either one can be destroyed without the worry of affecting the other. Third, copy assignment safely replaces existing contents by first creating a deep copy of the names of replacements followed by swapping the destinations pointer, size, and capacity with that temporary deep copy. Furthermore, the detination owns the now copied contents, while the temporary owns the old arrays detincation, which its constructor releases when the functions terminate. Fourth, the move constructor takes the source's pointer, size, and capacity and then resets the source to nullptr, containing both size and capacity zero. Fifth, the move assignment replaces storage by deleting the array in which a destination may already own, followed by taking the source's pointer and counts and resetting them to empty.


## What I would change differently

If I were to approach this project again, I would first have allowed myself to have sufficient time to organize my tests through a named function file, as when first initially implementing the tests, I found it difficult to distinguish within them and ensure I approached them correctly. Furthermore, to continue adding to the abstraction and ease of completing the program, I would separate the conversation interface and its implementation. Throughout the program the constructors, assignment operators, destructor, and append function were placed directly inside the conversation. h. Though this made it convenient to work through each function, eventually the header grew to become too long and harder to navigate as the class grew. Ultimately, I would have approached this program differently by allowing myself the time to improve the organization of the things listed above to improve readability and efficiency.