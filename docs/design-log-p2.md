# Design Log — Project 2

(500–800 words total. See spec §5 for what each section must cover.)

## Growth factor and amortized cost

Within Conversations, the messages are stored within a dynamically allocated array, beginning with both size zero and capacity zero, and with no allocated storage. If the array were to be empty, which at the start it is, the first append creates one slot. Furthermore, whenever the array were to become full, the capacity doubles in capacity. Lastly, size counts the store's messages, while capacity provides the available slots. 

Doubling the capacity whenever the array is to become full proves to be more efficient than continuously growing by one slot with every message. Furthermore, grouping by one slot on every append would require copying the already existing messages each time, resulting in O(n^2) total copying for n messages. On the other hand, with doubling the copying cost, follow a geometric series, meaning their sum is less than 2n. With new insertions, the total amount of element operations is O(n), resulting in an amortized cost of O(1) per append.


## Rule of Five evidence



## Sentinel scanner: bounded pending_ proof



## What I would change differently
