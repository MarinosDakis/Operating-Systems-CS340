The second project utilizes multi-threading to find the minimum number from 10000 random integers.
A main threads creates 5 smaller threads that are bound within 2000 intervals from the initial 100000 numbers.
Each of the 5 threads creates an additional 20 threads that each find the minimum from an interval of 100 integers and stores them in an array.
