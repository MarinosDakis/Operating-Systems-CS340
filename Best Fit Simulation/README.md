### How to compile using Cygwin Terminal:

type: g++ -std=c++11 [drag .cpp file here]   
Then: ./a.exe 1048576 


### Question provided:

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
   
### Logic of implementation:

I simulated a memory Storage with the inbuilt stl list in c++  | BOTTOM | GAPS & PROCESSES | TOP |  
which uses the  "-std=c++11" extension when compiling the program.  
I used it because of some of the useful in-built features of the next() function, which
was extremely useful when using iterators to do my comparisons between processes.

I have two separate classes of Process and MemoryStorage that will keep track of their individual top and bottom values
which signify what position their ends meet at  

For the memoryStorage, the bottom is always 0, whereas its top depends on the value of the inputtted byte request  

For Process, it also has a value to store its requested memory amount and name that will help keep track of each process  
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

### Output Example: 

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


