Kyle Zhang

Collaborated With: None

SelectionSort took 76.544 ms while InsertionSort took 36.981 ms. This is an expected result because 
InsertionSort doesn't require the list to be fully scanned while SelectionSort must always scan all remaining
unsorted items of the list. InsertionSort only searches through the already sorted section of items.

The most important part of this lab was definitely the addition of the heap and dynamic memory. While all the
prior labs we completed did not include this concept, this concept is definitely a large one for the c language.
We were required to manage our memory and malloc or free memory when needed. In future projects, memory leaks
will be a big deal especially if your processor is updating very fast. If you don't manage your heap correctly,
you will run out of memory very quickly. In addition, pointers and nodes were a huge part of this lab. This was
also a new topic and required a lot of thinking about. Linked Lists are useful due to them being a dynamic 
data structure. This means that by allocating or deallocating memory, the linked list can grow larger or smaller.
This also means that no size must be defined on initialization. By freeing the memory after it is used, you can
do much more since your memory is only limited by the size of your heap.

My approach to the lab was to first understand what how the heap worked. There is only 1 function that requires
malloc and one that requires free for this lab, but that doesn't mean there will be no memory leaks. After finishing
all the LinkedList.h functions, I was able to complete the two sort functions with not much struggle. I relied
on the header file more than the lab manual, but used the manual for the pseudocode for the sort functions. I
think my approach was good but there were some bugs I couldn't figure out that ended up being really simple 
fixes. In the future, I should really try and make my code simple so it's easier to debug.

I think that this lab was pretty fun. Not only was I able to learn about pointers, the sorting functions were 
pretty interesting. I didn't spend too much time on this lab because I had a good understanding about how nodes
and linked lists worked already. I think it was worthwhile lab to introduce dynamic memory. The hardest part of
the lab was probably still the sorting functions, but they ended up being pretty fun to debug. The examples
shown in class did help quite a bit and were very helpful when I was trying to start the lab. The grading
also seemed appropriate.