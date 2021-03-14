# Exercise 3
---
## a) What is the thread-specific data needed for? Why are the facilities described in the course book (Ch. 26.5) and lecture slides needed? Would it be enough if the required memory allocation is done separately and dynamically for each thread (using malloc)?
* Thread specific data is needed for synchronising access to global static varibales. Another thread can alter the state of the variable or a static buffer and the functions aren't thread safe.
* Pthread_key-structure helps to create specific key store for each thread and these are shared in function scopes.
* Malloc or free aren't reentrant and can mannipulate static varibales if called simultaneosly
* pthread_key lets the programmmer to refer to every value the same way while having a thread specific verison of the data. So there's no need for pointers