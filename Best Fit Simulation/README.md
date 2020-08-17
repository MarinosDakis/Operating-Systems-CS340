### How to compile using Cygwin Terminal:

type: g++ -std=c++11 [drag .cpp file here]   
Then: ./a.exe 1048576 


### Question Provided:

Best Fit simulation.cpp:

Chapter 9 Programming Project - Contiguous Memory Allocation 

1) Do only the Best Fit Strategy.

2) As in past projects you must submit your souce code (a) in one file and (b,c,d) together in another file.
   No zip files to be submitted. 

   Source code file name should be allocator_Initial of First Name_Last Name.c or cpp
   For example: allocator_JSvadlenka.c

   a) Well-documented source code
   b) Compilation instructions 
   c) Write-up of your program design and issues enountered and resolved.
   d) Output according to format in (3) below of the script you executed in (4) below. 
 

3) Please provide output as shown in the sample format below.  This  sample output is similar
   to that in the textbook for the Chapter 9 Programming Project - Contiguous Memory Allocation:

    allocator>STAT

    Addresses [0 : 315000] Process P1

    Addresses [315001 : 512500] Process P3

    Addresses [512501 : 625575] Free

    Addresses [625575 : 725100] Process P6

    Addresses [725001 : END ]  Free



4) You must run the following script of RQ, RL, C, STAT commands  as test input on your own computer
   and submit the output  of execution of the 4 STAT commands. The memory size as input is 1 MB (1048576).
   Remember that Compaction moves processes towards the low end of addresses and free space to the high 
   end of memory addresses. Memory addresses start at 0.
   
   
### Logic of Implementation:


The program utilizes the in-built list and iterator classes in c++ to make use of the next () function (from the iterator class) to aid when comparing different processes.  

Moreover, the program contains two additional classes, i.e. “Process” and “MemoryStorage” that keep track of their object’s top and bottom values; the Process class also contains variables that store the Process’ name and the amount of allocated memory given to it.  

For instance, the MemoryStorage object’s bottom is always set to 0, and the top is determined by the total about of bytes the user wants the program to use; however, in the case of the Process object, its individual top and bottom are based on its allocated memory provided by the user.   



The concept of determining whether a gap exists between two processes is simple and is best conveyed with an example:  

![alt text](https://github.com/MarinosDakis/Operating-Systems-CS340/blob/master/Best%20Fit%20Simulation/gap%20diagram.png)

The benefit of this approach is that if there is a case where a gap exists, then we know the start and end of each gap, and those ranges can also be showed in the STAT function.

THIS APPOACH CAN ONLY BE USED TO CHECK IF A GAP EXISTS BETWEEN PROCESSES

____________________________________________________________________________


But I had to also check if any gap existed below the front process, and above the end process, and the
way I would know that is whether or not the front process' bottom was equal to the main memory's bottom (i.e. 0), 
or whether the end process' top + 1 would be equal to the Maximum allocated storage. And that is how you would know
if there a gap at the end.

Essentially that was how the functions within the program kept track of the positions of the processes and gaps
based on the the process' top and bottom values, and for the best fit algorithm, I would combine the ideas of location
the gaps within the Memory storage, such that the minimum allocatable spot was favored for each inputted process, as
long as there was enough room for the process to fit, or Main memory.

### Sample Output: 

marin@DESKTOP-0QMPIDK ~
$ ./allocator_MDakis 1048576                                                      
The program is running, please choose one of the following commands:  

 RQ [processor name] [memory requested] [type]  
 RL [processor name]  
 C  
 STAT  
 QUIT  

you have selected to use this much memory: 1048576  

RQ P0 262144 B  
RQ P1 262144 B  
RQ P2 262200 B  
STAT  

Addresses [0 : 262143] Process P0  

Addresses [262144 : 524287] Process P1  

Addresses [524288 : 786487] Process P2  

Addresses [786488 : END] FREE  

RL P1  
P1 is going to be removed from memory  
There are now: 524232 bytes remaining  

RQ P3 200000 B  
STAT  

Addresses [0 : 262143] Process P0  

Addresses [262144 : 524287] FREE  

Addresses [524288 : 786487] Process P2  

Addresses [786488 : 986487] Process P3  

Addresses [986488 : END] FREE  

RQ P4 200000 B  
STAT  

Addresses [0 : 262143] Process P0  

Addresses [262144 : 462143] Process P4  

Addresses [462144 : 524287] FREE  

Addresses [524288 : 786487] Process P2  

Addresses [786488 : 986487] Process P3  

Addresses [986488 : END] FREE  

C  
STAT  

Addresses [0 : 262143] Process P0  

Addresses [262144 : 462143] Process P4  

Addresses [462144 : 724343] Process P2  

Addresses [724344 : 924343] Process P3  

Addresses [924344 : END] FREE  

QUIT  
Program Terminating  


