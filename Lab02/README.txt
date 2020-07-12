Kyle Zhang

Collaborated with: None

Question 1: What happens if the line “scanf(" ");” executes? Why?
Because the functions in C are pass by value, if you don't pass a pointer to a variable to scanf, there will be
no variable for scanf to modify so nothing will happen.

Question 2: In this lab, we forbid you from using printf() or scanf() inside of certain
functions. Explain why a rule like this is useful.
This rule is useful for making it clear to those reading your code how it works. It would be confusing if you
prompted the user for a operator but then called the function and then prompted them for the operands. This also
condenses the code needed for the program. Instead of needing to write a separate printf and scanf line for each
operator function, you can just put it once at the beginning of the infinite loop and it will work fine.

Question 3: How long did this lab take you? Was it harder or easier than you expected?
I thought this lab was pretty easy but there were some weird bugs with Coolterm or MPLabX that made it difficult
for me to know whether it was a bug with my code or with the software. Other than that the lab was pretty easy
and I finished it in reasonable time.