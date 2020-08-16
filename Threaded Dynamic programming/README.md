##### how to compile:  
Using Cygwin terminal:  
open terminal, type "g++ [drag .cpp file onto terminal to create path] -lpthread"  
press enter to let it compile  
type "./a.exe" to run  


##### Logic of program
The program begins initially in the main function (LEVEL1) and generates 10000 random integers and stores them in the array randnums[10000] (code provided in class).
After it finds the minimum and maximum number through an array search, by looking through each element in the array and comparing them to the comparison value set at index 0 of randnums.
 
 Moreover, underneath the random number generation is the creation of the smaller threads (LEVEL2) that invokes Level2Function on creation.
 Within Level2Function, it creates an additional 20 tiny threads (LEVEL3) from a single smaller thread (LEVEL2).
 Each of the tiny threads will search 100 integers progressively through the randnums array indexes, until all 20 tiny threads search through their respected 2000 numbers of the array.
 Overall, there will be a total of 5 smaller threads that create 20 additional tiny threads, leading to a total of 100 tiny threads.
 The tiny threads will then store their minimums in the tinyThdArray[100].
 
 In addition, Level2Function works so that after each creation of the 20 tiny threads, the rest of the function searches for the minimum of those 20 integers
 and stores it in the smallerThdArray[5] at its respected index, and this repeats until all 5 threads have been created with their respected tiny threads,
 and the minimums are stored in their arrays.
 
 After the Level 2 threads are completed, the main function outputs the contents of the arrays

##### illustration of Logic
![alt text](https://github.com/MarinosDakis/Operating-Systems-CS340/blob/master/Threaded%20Dynamic%20programming/Component%201%20%E2%80%93%201.png)

