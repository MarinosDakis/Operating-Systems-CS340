#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <time.h>
using namespace std;

// how to compile:
// Using Cygwin terminal:
// open terminal, type "g++ [drag .cpp file onto terminal to create path] -lpthread"
// press enter to let it compile
// type "./a.exe" to run


// Design process:
// Main thread contains 10000 numbers
// split into 5 smaller threads (level 2) dealing with 2000 numbers each
// Each of those (level 2) threads contains 20 tiny threads (level 3) that will process 100 numbers each 

//           10000             5 x 2000 = 10000         5(20 tiny threads x 100 numbers) = 10000 
//      [1 main thread]        [5 small threads]              [100 tiny threads]
// 

// Requirements: [10000 array] [5 array to store smaller threads mins] [100 array to store tiny thread mins]

// smaller thd 1 :    0 - 1999 --> tiny thd 1  [0-99] [99-199] [199-299] [299-399] [399-499] [499-599] [599-699] [699-799]...
// smaller thd 2 : 1999 - 3999 --> tiny thd 21 [1999-2099] [2099-2199] [2199-2299] [2299-2399] [2399-2499] [2499-2599]...
// smaller thd 3 : 3999 - 5999 --> tiny thd 41 [3999-4099]...
// smaller thd 4 : 5999 - 7999 --> tiny thd 61 [5999-6099]...
// smaller thd 5 : 7999 - 9999 --> tiny thd 81 [7999-8099]...

//     0        1        2        3       4
// [small 1][small 2][small 3][small 4][small 5]
//     
// [tiny 1] [tiny 2] [tiny 3] [tiny 4] [tiny 5] [tiny 6] [tiny 7] [tiny 8] [tiny 9]...[tiny 20] [iterate 5 total]
//    

/*
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

*/

// required global variables (shared) for thread processes & random number generator:
// tiny = LEVEL 3, smaller = LEVEL2, main = LEVEL1

int randnums[10000], smallerThdArray[5], tinyThdArray[100], grandMin = 0, count = 0; //tinyThdArray stores min values via Level3Function, and smallerThdArray via Level2Function  
#define NUM_THREADS_SMALL 5
#define NUM_THREADS_TINY 20 //(iterate 5 times)

// thd IDS
pthread_t smallThds[NUM_THREADS_SMALL]; // array of threads for level 2 threads
pthread_t tinyThds[NUM_THREADS_TINY]; // array of threads for level 3 threads

// counts
int COUNT = 0; // the index to store mins in [100] array 
int COUNT2 = 0; // the index to store the mins in [5] array


/////////////////////////////////////////////////////////////////////////////////////////////

void *Level3Function(void *threadInput){ //start level 3 Function

// represent current thread id as parsed for loop index, so that thread calculates correct range
long thdID = (long)threadInput;

// index variables
int start, end;

// locating correct positions
// COUNT = range [0,99]
// thdID = range [0,20]
// (COUNT/20) = range [0-4] (dividing COUNT by 20 signifies the correct level 2 thread created at the time [0-4] by using characteristics of int division)
// E.g. [0,19]/20 = 0 (first smaller thread), [20,39]/20 = 1 (second smaller thread), [40,59]/20 = 2 (third smaller thread), and so on to 4 (fifth smaller thread)

// finding the end index
end = (2000 * (COUNT/20)) + 100 * (thdID + 1);

// Some examples to visualize how the end index is found:

// first case
// if count = 0, then 0/20 = 0 (int division), therefore it shows that the first smaller thread is passing through the function
// now there are 20 tiny threads that are being created, so let us assume thdID is 0 to show that the first tiny thread is going to search through its respected 100 integers
// in this case we know (count/20) = 0, thdID = 0
// inside the formula: (2000 * 0) + 100 * (0 + 1) = 100 
// so the thread will search indexes: [0 - 100]

// last case
// if count = 99, then 99/20 = 4 (int division), therefore it shows that the fourth smaller thread is passing through the function
// now there are 20 tiny threads that are being created, so let us assume thdID is 19 to show that the 20th tiny thread is going to search through its respected 100 integers
// in this case we know (count/20) = 4, thdID = 19
// inside the formula: (2000 * 4) + 100 * (19 + 1) = 10000
// // so the thread will search indexes:  [9900 - 10000]

// finding the start index 
start = end - 100; // the start in all cases is just the end value and subtract 100, since each tiny thread only searches for the minimum in 100 increments

// locating the MIN based on calculated start & end bounds, and storing at current COUNT index in [100] array
int currentMin = randnums[start]; // setting the current min to first element of interval
for (int i = start + 1; i < end; i++){ // start + 1 because we have temp min in [start]
if (randnums[i] < currentMin) currentMin = randnums[i]; //replace currentMin if smaller number found
}
tinyThdArray[COUNT] = currentMin; //store min value inside array of mins
//cout << "MIN stored in tinyThdArray at index [" << COUNT << "]: " << tinyThdArray[COUNT] << endl;

COUNT++; // increment counter 
pthread_exit(NULL); // exit thread

} // end level 3 function

void *Level2Function(void *threadInput){ // start level 2 function

//make level 3 threads and store into [100] array of MINS
for (intptr_t j = 0; j < NUM_THREADS_TINY; j++){ // 20 threads (level 3)
pthread_create(&tinyThds[j], NULL, Level3Function, (void*)j); // creating LEVEL3 threads
pthread_join(tinyThds[j],NULL); //stops overlapping
}

// find the smallest mins of 100 array in 20 element increments

// represent current thread id as parsed for loop index, so that thread calculates correct range
long thdID = (long)threadInput;

// index variables
int start, end;

// finding the end index
end = (COUNT2 + 1) * 20;

// Some examples to visualize how the end index is found:

// COUNT2 reprents the current smaller thread that is created at the time
// hence COUNT2 will only reach up to 4 (representing a total of 5 smaller threads)

// First case
// Assume COUNT2 = 0
// then based on formula: (0+1) * 20 = 20
// index: [0 - 20]

// Last case
// Assume COUNT2 = 4
// then based on formula: (4+1) * 20 = 100
// index: [80 - 100]


// finding the start index 
start = end - 20; // the start in all cases is just the end value and subtract 20, since each smaller thread only searches for the minimum in 20 increments

// locating the MIN based on calculated start & end bounds, and storing at current COUNT index in [5] array
int currentMin = tinyThdArray[start]; // setting the current min to first element of interval
for (int i = start + 1; i < end; i++){
if (tinyThdArray[i] < currentMin) currentMin = tinyThdArray[i]; //replace currentMin if smaller number found
}
smallerThdArray[COUNT2] = currentMin; //store min value inside array of mins
//cout << "MIN stored in SmallerThdArray at index [" << COUNT2 << "]: " << smallerThdArray[COUNT2] << endl << endl;

COUNT2++; // increment counter  
pthread_exit(NULL); // exit thread

} // end level 2 function

/////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){ // start main

// CREATING THE RANDOM NUMBERS
time_t t;
// seed random generator
srand((unsigned)time(&t));
// generate random nums
for(int i = 0; i < 10000; i++){
	randnums [i] = rand();
}

/////////////////////////////////////////////////////////////////////////////////////////////

// finding the min/max values through brute force and printing them
int controlMax = randnums[0], controlMin = randnums[0]; // control variables to store Min & Max

for (int i = 1; i < 10000; i++){
	if (randnums[i] < controlMin) controlMin = randnums[i]; // stores control Min
	if (randnums[i] > controlMax) controlMax = randnums[i]; // stores control Max
}
cout << "The control max is: " << controlMax << endl; // output max
cout << "The control min is: " << controlMin << endl << endl; // output min

/////////////////////////////////////////////////////////////////////////////////////////////

// make level 2 threads,that create level 3 threads
for (intptr_t i = 0; i < NUM_THREADS_SMALL; i++){ 
pthread_create(&smallThds[i], NULL, Level2Function, (void*)i); //creates LEVEL2 threads
pthread_join(smallThds[i], NULL);  //stops overlapping
}

/////////////////////////////////////////////////////////////////////////////////////////////

// Printing out all data

//output [100] array
for(int i = 0; i < 100; i++)
	cout << "MIN stored in tinyThdArray at index [" << i << "]: " << tinyThdArray[i] << endl;
	
	cout << endl;

//output [5] array	
for(int i = 0; i < NUM_THREADS_SMALL; i++)
	cout << "MIN stored in SmallerThdArray at index [" << i << "]: " << smallerThdArray[i] << endl;
		
	cout << endl;	

// finding the Grand min value 
grandMin = smallerThdArray[0]; // store temp min value as first index of [5] array
for (int i = 1; i < NUM_THREADS_SMALL; i++)
	if (smallerThdArray[i] < grandMin) grandMin = smallerThdArray[i]; // replace grand min if smaller value is found at higher index

//output Grandmin	
cout << "The Grand min value is: " << grandMin << endl;

} // end main
