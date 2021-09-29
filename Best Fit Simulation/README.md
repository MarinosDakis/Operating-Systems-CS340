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

The benefit of this approach is that if there is a case where a gap exists, then we know the start and end of each gap, and those ranges can also be displayed in the STAT function.

**THIS APPOACH CAN ONLY BE USED TO CHECK IF A GAP EXISTS BETWEEN PROCESSES**

__________________________________________________________________________________________________________________

To check whether gaps existed below the front process and above the back process a different approach was required.  
Once again a diagram will be used to describe the concept: 

![alt text](https://github.com/MarinosDakis/Operating-Systems-CS340/blob/master/Best%20Fit%20Simulation/front%20and%20back%20gap%20diagram.png)

__________________________________________________________________________________________________________________

Essentially these concepts of gap tracking were used within functions in the program to apply the best fit algorithm with respect to the allocated byte size, such that the minimum allocatable memory location was favored for each new inputted process (if there was were sufficient bytes available).  

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

_______________________________________________________________________________________
Issue:

I had an issue trying to get the values to input in the different cases of the commands:
For example, if the user wanted to enter the QUIT option initially, I had it so that
cin required 4 inputs before it could continue, i.e. cin >> a >> b >> c >> d;
Hence, the user wouldn't have been able to use the program as stated in the instructions.

Solution:

The user enters a string and then I separate each string into separate elements of an array via an string stream,
and each of those values gets assosicated with its own value. 

For instance:

[0] stores the name of the command
[1] stores the processor name
[2] stores the amount of memory requested
[3] stores the memory allocation strategy (best fit in all cases)

I would then convert the the memory amount into an integer value to be able to allocate the correct ranges in memory
and the beneficial part of this way is that single instance commands like C/STAT/QUIT work.

Issue:
Issue with the removing a process from the list because of how the process object was created using the process
name and the allocated memory; however, since the assignment stated that the user must only type "RL P"
I couldn't just pass in the same memory requested.

Also, since the inbuilt remove() command used the == operator to match which element to delete in the list
I had to find a solution to that.

Solution: 
I overrided the == operator to be able to make it so if another process with the same name is passed through
the function, it would be able to still delete the process in the list.

For example, assume Process p = {processorName, requestedMemoryAmount};
we notice how p contains the processor name AND the memory amount, so that is the information that is
put into the list

Now, the issue as stated above is that I couldn't use the requested memory as a parameter, so I created
another Parameterized constructor that only used the name to create the process and made a process with
the same name that is to be deleted, such that when it it passed into the function, the overwritten == operator
will see that they contain the same name; hence, it will think that they are equal, but in reality the only thing
that is the same with then is their name. Afterwards, I pass the process into the removal function and when the
remove function is called, the dummy process will act as though its resembling what process is to be deleted from the
list.

Issue:
minor logical errors

Solution: 
changing the order in which certain statements were executed, such that they executed in a way that made
the program work.
For example, the order in which functions are displayed above the main.
Also an error message was executing too early, so I rearranged some lines, and added a condition
that checks the uses the default min value initally.

Issue:
The program sometimes wouldn't compile well, and I couldn't figure out why until after a while.

Solution: 
The Rename() function makes the compilation of the program a bit weird, and sometimes you have to
restart the terminal and recompile the program before any changes could be in effect.
Also, commenting the function until I had everything working helped as well

(If you have issues compiling the program it's best to try restarting your terminal)

________________________________________________________________________________________________
Thought process:

I simulated a memory Storage with the inbuilt stl list in c++  | BOTTOM | GAPS & PROCESSES | TOP |
which uses the  "-std=c++11" extension when compiling the program.
I used it because of some of the useful in-built features of the next() function, which
was extremely useful when using iterators to do my comparisons between processes.

I have two separate classes of Process and MemoryStorage that will keep track of their individual top and bottom values
which signify what position their ends meet at

For the memoryStorage, the bottom is always 0, whereas its top depends on the value of the inputtted byte request

For Process, it also has a value to store its requested memory amount and name that will help keep track of which process is which
For the process, its top and bottom depends on what the inputted requested memory is and based on how the process is added its values are calculated differently

The main idea in figuring out whether a gap exists between processes is by comparing the top and bottom values for each Process by 1
For example, in assuming the first process is allocated 1000 bytes of memory, we set the bottom = to 0, and its top = 999
therefore, if the next process' bottom is not equal to the current process' top + 1, then we can assume a gap exists
This is also how the gaps were calculated, and they were assigned their own individual tops and bottoms, hence how
their ranges showed in the STAT function

Moreover, the above idea only focuses on whether a process contains a gap in between them; for example,

| BOTTOM | P1 | GAP | P2 | TOP |

But I had to also check if any gap existed below the front process, and above the end process, and the
way I would know that is whether or not the front process' bottom was equal to the main memory's bottom (i.e. 0), 
or whether the end process' top + 1 would be equal to the Maximum allocated storage. And that is how you would know
if there a gap at the end.

Essentially that was how the functions within the program kept track of the positions of the processes and gaps
based on the the process' top and bottom values, and for the best fit algorithm, I would combine the ideas of location
the gaps within the Memory storage, such that the minimum allocatable spot was favored for each inputted process, as
long as there was enough room for the process to fit, or Main memory.
