#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
using namespace std;

// How to compile using Cygwin64 Terminal: 
// Open the terminal and type g++, then drag the .cpp file onto the terminal and it should add the
// path to the program to it
// Then press enter and it should create a "a.exe" file
// In the terminal type ./a.exe "command you want to test" to execute the program

int main(int argc,char* argv[]){

//for pipe
#define read_pipe_P1 0  
#define write_pipe_P1 1

int fd[2]; //allocate space for the pipe
pid_t pid; //to create child process
struct timeval tv; //to get time

// variable initializations
int child_seconds = 0, parent_seconds = 0;
double child_microseconds = 0, child_microseconds_in_seconds = 0, start_of_child = 0, parent_microseconds = 0, parent_microseconds_in_seconds = 0, end_of_parent = 0, start_of_parent = 0, elapsed_time = 0;
string name_of_command = "null";

 //store program name 
 name_of_command = argv[1]; 

 //creation of pipe
   if (pipe(fd) == 1){
	   perror("Pipe failed");
	   return 1;
   }
   
 //creation of child process
   pid = fork();
   
 //the following code determines where there was an error in forking the process
   if (pid < 0) { //error occurred
     perror("Fork Failed");
     return 1;
   } //end error occurred
   
   
     else if (pid == 0) { // child process start
	 cout << "child process created" << endl << endl;
     gettimeofday(&tv, NULL);
	 child_seconds = tv.tv_sec; //get time in seconds 
	 child_microseconds = tv.tv_usec; //get time in microseconds 
	 child_microseconds_in_seconds = child_microseconds / 1000000; // turn micro into seconds
	 start_of_child = (child_seconds + child_microseconds_in_seconds); //start of child contains total number of seconds
     
	 //close unused read side of pipe
	 close(fd[read_pipe_P1]);
	 
	 //write to pipe (child's starting time)
	 write(fd[write_pipe_P1], &start_of_child, sizeof(start_of_child)); //store total number of seconds of child in pipe
	
	 //close write to pipe
	 close(fd[write_pipe_P1]);
	 
	 // outputting variable info
	 cout << "Child starting time: " << start_of_child << endl;	 //start of child
	 cout << "The name of the command: " << name_of_command << endl; //name of command
	 cout << "the output of " << name_of_command << " is:" << endl; // output of said command
	 
	 //using execlp command which overrides current child's process
	 string output_of_commmand; // to store value of output
	 output_of_commmand = execlp(argv[1], argv[1], NULL); //stores output of execlp
	 perror("output failed"); //outputs error if path not found 
	 cout << output_of_commmand << endl;

	 //kill child
	 exit(0);
	 
   } // end child process
   
   else { // parent process start
   
     //wait for child process to terminate
     wait(NULL);
     cout << "\nChild termination complete" << endl << endl;
	 
	 //getting times of parent
	 gettimeofday(&tv, NULL);
	 parent_seconds = tv.tv_sec; //parent time in seconds
	 parent_microseconds = (tv.tv_usec); //parent time in microseconds
	 parent_microseconds_in_seconds = parent_microseconds / 1000000; // turn micro into seconds
	 end_of_parent = (parent_seconds + parent_microseconds_in_seconds); //end of parent now contains total number of seconds for parent side
	 
	 //close unused write side of pipe
	 close(fd[write_pipe_P1]);
	 
	 //reading from pipe
	 read(fd[read_pipe_P1], &start_of_parent, sizeof(start_of_parent)); //read total number of seconds on child side
	
     //close read from pipe
	 close(fd[read_pipe_P1]);
   
     //Now that all the values are stored, we want to subtract the total time of parent from total time from child
     // calculating elapsed time using pipe value
     // we take value from parent, and subtract from time taken from pipe
     elapsed_time = (end_of_parent - start_of_parent);
    
	 // outputting rest of data about variables
	 //cout << "Child starting time: " << start_of_child << endl; (always outputs initialized value of 0)
	 cout << "The parent's starting time from pipe: " << start_of_parent << endl;
	 cout << "The parent's ending time: " << end_of_parent << endl;
	 cout << "Total elapsed time: " << elapsed_time << endl;
   
  } // end parents process
   
}//end main
   
   
   
   
   
   
   
   
   
   
   
