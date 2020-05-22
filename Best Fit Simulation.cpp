#include <iostream>
#include <sstream>
#include <list> 
#include <iterator>
using namespace std;

//Global Variables
stringstream ss;
string inputString, valuesArray[4], memoryInputString, commandName, processorName, memoryAmountString, strategyType; // to store individual sections of the input string
int requestedMemoryAmount = 0, memoryInput = 0; // stores amount of memory requested
int numberOfProcessors = 0; // keeps track of the total number of processes


class Process { //start Process
    public: 
	string processName;
    int top;
    int bottom;
    int requestedMemory;	
   
    // Constructor 
    Process() { //start parametrized
     
	 processName = ""; //set name of process
	 requestedMemory = 0;
	 bottom = 0;
	 bottom = 0;
	 
    } //end Constructor


    //Parametrized Constructor 
    Process(string name, int requestedMem) { //start parametrized
     
	 processName = name; //set name of process
	 requestedMemory = requestedMem; // set amount of requested memory of process
	 
    } //end parametrized
	
	//Parametrized Constructor 
    Process(string name) { //start parametrized
     
	 processName = name; //set name of process
	 
    } //end parametrized
	
	
	// helper function to override == so that it can be removed from the list
	bool operator == (const Process& p) const {
		return processName == p.processName; 
	}
	
	// helper function to override != to help compare different processes
	bool operator != (const Process& p) const {
		return !operator == (p);
	}
	
}; //end Process


class MemoryStorage { // start MemoryStorage

    public: 
    int memTop = 0;
    int memBottom = 0;
	int remainingMemory = 0;
    
    MemoryStorage(int maxAmount){ // start parametrized Constructor
	
        memTop = maxAmount; // top of storage
        memBottom = 0; // bottom of storage	
        remainingMemory = maxAmount; // remaining memory	

      }// end parametrized Constructor 
		
   
}; // end MemoryStorage

// this function compact the processes such that they pile onto each other at the bottom of memory.
void Compact(list<Process> &list, MemoryStorage &mem){ // start compact

	//check if the first process' bottom != bottom of the storage
	if (list.front().bottom != mem.memBottom){ //start if
		
		list.front().bottom = 0;
		list.front().top = list.front().requestedMemory - 1;
	
	}// end if
		
		
    for (std::list<Process>::iterator i = list.begin(); i != --list.end(); i++){		
		
		//if the current Process' top != next process' bottom - 1
		if ((*i).top != (*next(i)).bottom - 1){ //start if
			
			//set the next Process' new bottom to = to current process' top +1
			(*next(i)).bottom = (*i).top + 1;
			
			//set the next Process' new top = its bottom + the request amount -1
			(*next(i)).top = (*next(i)).bottom + (*next(i)).requestedMemory - 1;
			
		}// end if
	}//end for 
	
} // end compact


// this method applies the Best fit alogorithm by separating the different cacases that can occur.
void Bestfit(Process P, list<Process> &list, MemoryStorage &mem){ // start best fit
	
	// default min value that
	int min = 0;
	
	// storing values to see which side of the list is smaller, so that we can add a new process to it, 
	int belowLowestProces = list.front().bottom; 
	int aboveHighestProcess = list.back().top;
     
	int bottomGap = (belowLowestProces - mem.memBottom); // we take the lowest process' bottom - 0
	int topGap = (mem.memTop - aboveHighestProcess); // we take the highest process in list, and then subtract that from the top of the memory (1MB)
	
	
	// compares and finds the smaller end of the memory and set it as the default memory value   |BOTTOM| Bottom GAP  |Processes| Top GAP  |ABOVE|
	if (topGap > bottomGap && bottomGap >= P.requestedMemory){
		
		min = bottomGap; // store min to the smallest value

	} //end if
	
	
	// otherwise if the top gap has enough space to fit the process then
	else if (topGap >= P.requestedMemory){
		
		min = topGap; // store min to the smallest value

	} // end else if
	
 
 int minCounter = 0; // keeps track of iterations to get to smallest gap (so add the process process at that spot)
 int iterationCounter = 0; // keeps track of the number of iterations done in total
 
  for (std::list<Process>::iterator i = list.begin(); i != --list.end(); i++){ //start for
	  
	  int currentTop = (*i).top; // keeps track of current top 
	  int nextBottom = (*next(i)).bottom; // keeps track of next bottom
	  iterationCounter++; // keeps track of the total number of iterations
	  
	  
	  // if the gap is smaller than the current min value, and that the process can fit in the gap
	  // and check if min is 0, that means that the default value wasn't found yet at the point, so it sets the default calculated
	  if((nextBottom - currentTop < min || min == 0)&& nextBottom - currentTop >= P.requestedMemory){ //start if
          
          // min = smallest gap size		  
		  min = nextBottom - currentTop;
		  
		  // keeps track of the number of iterations taken to reach the smallest gap
		  minCounter = iterationCounter;
		  
	  
	  } // end if
	  
	 // and if no min was found, min is still 0, and so there is no space
	if (min == 0) {
	
		cout << "Cannot add process due to insufficient available space." << endl;
		return;
				
	}  
	  
	  
  } // end for
  
	  // if the minCounter == 0 that means the min gap either between the bottom and the first process, or the top and the last process
	  if(minCounter == 0){ //start minCounter == 0
		  
		  if(min == bottomGap){ //start if
			  
			// adding process to the bottom		
		       P.bottom = 0; // the bottom is equal to the bottom of memory, i.e. 0
		       P.top = (P.requestedMemory - 1); // the top is determined by the amount of memory requested 
		       mem.remainingMemory -= P.requestedMemory; // remove the total available space in memory, since the process is taking up space
		       list.push_front(P); // add process to list
		       numberOfProcessors++; // incremement the number of existing processors 
			  
		  }//end if
		  
		  else if(min == topGap) { //start if

			// adding process to the top				  
		       P.bottom = (aboveHighestProcess + 1); // we set the bottom on the process on top of the (previous last process + 1)	
		       P.top = (P.bottom + P.requestedMemory) - 1; //hence the process top would be its new bottom + the request amounted
		       mem.remainingMemory -= P.requestedMemory; // remove the total available space in memory, since the process is taking up space
		       list.push_back(P); // add process to list
		       numberOfProcessors++; // incremement the number of existing processors  
			    
		  } // end if
		  
	  } // start minCounter == 0
	  
	  
	  else{ // otherwise loop through the processes and their gaps until the smallest gap is found and then insert it inside
		  
		 iterationCounter = 0;
		 int currentTop = 0;
		 
		  // keep iterating until it reaches currentTop
		  for (std::list<Process>::iterator i = list.begin(); i != list.end(); i++){
			 
			 iterationCounter++; // keeps track of processes cycled through
			
			 // once you reach the process before the smallest gap, add the new process after it
			 if (iterationCounter == minCounter + 1){ 
				 
	              P.bottom = (currentTop + 1); // we set the bottom on the process on top of the (previous last process + 1)	
		          P.top = (P.bottom + P.requestedMemory) - 1; //hence the process top would be its new bottom + the request amounted
		          mem.remainingMemory -= P.requestedMemory; // remove the total available space in memory, since the process is taking up space
		          list.insert(i, P); // otherwise put the process in the smallest gap 	
		          numberOfProcessors++; // incremement the number of existing processors
				
				 break;
				 
			 } //end if
			 
			   // this is below because it caused an issue when setting the new top of the new process
			   currentTop = (*i).top; // keeps track of current top
	     

	  } //end for
	  
	  
	 } // end else
	  
	  
  }// end best fit


// the following function tells you whether a gap exists between the processes and does not consider if any gaps appear below the first process or after the last process
static bool GapExist(list<Process> list){ //start GapExist
	
bool gapExist = false;

  for (std::list<Process>::iterator i = list.begin(); i != --list.end(); i++){ //start for
	  
	  int current = (*i).top; // stores current process top value
	  int nextProcess = (*next(i)).bottom; // stores next process bottom value

	  // if the processes are stacked one after another | P1 | P2 | P3 | P4 | P5 |
	  // then the current process will be equal to the next process' bottom + 1
	  // otherwise we can assume a gap Exists somewhere throughout the list
	  if (current != nextProcess - 1) gapExist = true;
	  
  } // end for
	
    // otherwise no gap exists
	return gapExist;
	
} // end GapExist


//the following function will add proccess to the list with their respected top and bottom values
void addProcess(Process P, MemoryStorage &mem, list<Process> &list){ // start add process
	
	// if the inputted process requires more space than what exists in memory
	if(P.requestedMemory > mem.remainingMemory){
		
		cout << "can not add process due to insufficient available space" << endl;
		return;
		
	} //end if
	
	// if there are currently no processors already existing in memory
	else if(numberOfProcessors == 0){
	
      P.bottom = mem.memBottom; // the bottom is equal to the bottom of memory, i.e. 0
      P.top = (P.requestedMemory - 1); // the top is determined by the amount of memory requested [-1 because the bottom starts from 0]
      mem.remainingMemory -= P.requestedMemory; // remove the total available space in memory, since the process is taking up space
	  list.push_back(P); // add process to list
	  numberOfProcessors++; // incremement the number of existing processors
	  return;
		
	}//end else if
	
	
	// in the case that there is only 1 element in the list, or that there are no gaps inbetween the processes (does not include if any exist around processes)
	else if(numberOfProcessors == 1 ||GapExist(list) == false){ 
	
	// storing values to see which side of the list is smaller, so that we can add a new process to it, 
	int belowLowestProces = list.front().bottom; 
	int aboveHighestProcess = list.back().top;
     
	int bottomGap = (belowLowestProces - mem.memBottom); // we take the lowest process' bottom - 0
	int topGap = (mem.memTop - aboveHighestProcess); // we take the highest process in list, and then subtract that from the top of the memory (1MB)
	
	// if the gap on the top is greater then the gap on the bottom, and the requested amount from the process is able to fit in the bottom gap then
	if (topGap > bottomGap && bottomGap >= P.requestedMemory){
		
		P.bottom = 0; // the bottom is equal to the bottom of memory, i.e. 0
		P.top = (P.requestedMemory - 1); // the top is determined by the amount of memory requested [-1 because the bottom starts from 0]
		mem.remainingMemory -= P.requestedMemory; // remove the total available space in memory, since the process is taking up space
		list.push_front(P); // add process to list
		numberOfProcessors++; // incremement the number of existing processors
		return;
		
	} //end if
	
	
	// otherwise if the top gap has enough space to fit the process then
	else if (topGap >= P.requestedMemory){
		
	    P.bottom = (aboveHighestProcess + 1); // we set the bottom on the process on top of the (previous last process + 1)	
		P.top = (P.bottom + P.requestedMemory) - 1; //hence the process top would be its new bottom + the request amounted
		mem.remainingMemory -= P.requestedMemory; // remove the total available space in memory, since the process is taking up space
		list.push_back(P); // add process to list
		numberOfProcessors++; // incremement the number of existing processors
		return;
		
	} // end else if
		
		
		else {	
		
		cout << "can not add process due to insufficient available space" << endl;
		return;
		
	} // end else	
	
    } //end else if	
	
	
	// there is gaps scattered throughout the memory, and we apply the best fit strategy
	else
		Bestfit(P, list, mem);

} //end addProcess method

// this function removes the process that is inserted
// and calculates the total remaining memory for the memory storage
void RemoveProcess(Process &processName, list<Process> &list, MemoryStorage &mem){
	
	int newRemainingSpace = 0;
	
	    // caclulate the previous total memory by calculating the requested memory of each process in the list 
        for (std::list<Process>::iterator i = list.begin(); i != list.end(); i++){//start for
		
		newRemainingSpace += (*i).requestedMemory;
		int x = (*i).requestedMemory;
		
		} // end for
	
	// remove the process from list
	list.remove(processName);
	
	    // and then iterate through the loop again by subtracting the processes' requested memory without the one that gets removed
		// hence to find new remaining memory of storage
        for (std::list<Process>::iterator i = list.begin(); i != list.end(); i++){ // start
		
		newRemainingSpace -= (*i).requestedMemory;
		int x = (*i).requestedMemory;
		
		}// end for
	
	// decrease the amount of processors
	numberOfProcessors--;
	
	// set the new remaining memory
	mem.remainingMemory += newRemainingSpace;
	
}


// this function outputs all of the processes' data and it finds the gaps
void Stats(list <Process> &list, MemoryStorage &mem){ //start stats
	
		// if the process at the bottom of the list is not equal to the bottom of memory
		// then that means that there is a gap under the first process of memory
		if (list.front().bottom != mem.memBottom){ //start bottom if
			
			int gapTop = list.front().bottom; // stores top of gap
			int gapBottom = mem.memBottom; // stores bottom of gap
			
         cout << "Addresses [" << gapBottom << " : " << gapTop << "] FREE " << endl << endl;
			
		} // end bottom if	 
		 
		 // iterating through all the processes and outputting them and any gaps between them
		for (std::list<Process>::iterator i = list.begin(); i != list.end(); i++){ //start for	 
		
		     // if the last process is at the end of the MemoryStorage - 1
			 if ((*i).top == mem.memTop - 1){
		     cout << "Addresses [" << (*i).bottom << " : END" << "] Process " << (*i).processName << endl << endl;

	 } // end if
	 	 
        //otherwise output the process ranges	 
	    else  cout << "Addresses [" << (*i).bottom << " : " << (*i).top << "] Process " << (*i).processName << endl << endl;		
	
		 // if the top of the current process' top + 1 is not equal to the next process' bottom, and the current process' top + 1 is less than the next process' bottom
		 // then output the gap's size
		  if ((*i).top + 1 != (*next(i)).bottom && (*i).top + 1 < (*next(i)).bottom) { 
			 
			 int gapBottom = (*i).top + 1;
			 int gapTop = (*next(i)).bottom - 1;
	     
         cout << "Addresses [" << gapBottom << " : " << gapTop << "] FREE " << endl << endl;
		 
		 }// end else if
	 
	} // end for
		 
		 
		// if the process at the top of the list is not equal to the top of memory
		// then that means that there is a gap above the last process of memory
		if (list.back().top != mem.memTop){ //start top if
			
			int gapTop = mem.memTop; // stores top of gap
			int gapBottom = list.back().top + 1; // stores bottom of gap
			
         cout << "Addresses [" << gapBottom << " : END] FREE " << endl << endl;
			
		} // end top if
		 
	 

    } //end Stats


int main(int argc, char *argv[]){ // start main


memoryInputString = argv[1]; //total amount of memory allowed [1MB] 
cout << "The program is running, please choose one of the following commands:\n\n RQ [processor name] [memory requested] [type] \n RL [processor name] \n C \n STAT \n QUIT\n" << endl;

//convert allocated amount into string
stringstream ss(memoryInputString);

ss >> memoryInput; // store the amount of 1MB

cout << "you have selected to use this much memory: " << memoryInput << endl << endl;

//create storage object
MemoryStorage mem(memoryInput);

// list that will store the nodes
list <Process> memorySimulation;


while(true){ //start of true
//////////////////////////////////////////////////////////////////////////////////////////

//Getting the information from Console:

getline(cin, inputString); //gets the input from the user 
cin.clear(); // clear input buffer

stringstream ss(inputString); // creating a string stream to separate the input values

int i = 0;
while (i < 4 && ss.good()){
	
    ss >> valuesArray[i]; // store individual strings as array elements
	
	switch(i){
	
	case 0: commandName = valuesArray[i]; // store command name
	break;
	
	case 1: processorName = valuesArray[i];  // store processorName 
	break;
	
	case 2: memoryAmountString = valuesArray[i];  // store memoryAmountString
	break; 
 
	case 3: strategyType = valuesArray[i];  // store strategyType
	break;
}
	i++;
}

stringstream ss2(memoryAmountString); // creating a string stream to convert string to int
 ss2 >> requestedMemoryAmount; // stores the requested memory from user as int

//////////////////////////////////////////////////////////////////////////////////////////

	
	if (commandName.compare("RQ") == 0){ // if the value via console = RQ [REQUEST CASE]
	
	// if the strategy type isn't B, make user input again
	if (strategyType.compare("B") != 0){
		cout << "Please select the type of allocation strategy you would like to do [B] " << endl;
		continue;
	}
	
	Process p = {processorName, requestedMemoryAmount}; //create a process from inputted data
	addProcess(p, mem, memorySimulation); //store inside the list [process name, memory object, list name]
	
	} //end RQ if
	
	else if(commandName.compare("RL") == 0){// if the value via console = RL [RELEASE CASE]
	
	Process p = {processorName}; // create a process with the same name as the one to be removed
    //(help function works by overriding == and != such that it only depends on the name to delete)
	cout << processorName << " is going to be removed from memory" << endl;	
	
    RemoveProcess(p, memorySimulation, mem); // removes process with same name in list
	
	cout << "There are now: " << mem.remainingMemory << " bytes remaining" << endl << endl;	
	
	} //end RL if
	
	else if(commandName.compare("C") == 0){ // if the value via console = C [COMPACT CASE]
	
	// if there are no processes in memory, then there is nothing to compact, hence make user enter again
	if (numberOfProcessors == 0) {
		cout << "There are currently no processes stored in memeory" << endl;
		continue;
	}
	
	Compact(memorySimulation, mem);
	
	} //end C if
	
	else if(commandName.compare("STAT") == 0){ // if the value via console = STAT [STATISTICS CASE]
	cout << endl;
	
	// if there is 0 processes then show that the whole memory is free, and re loop
		if (numberOfProcessors == 0){
			cout << "Addresses [0 : END] FREE" << endl << endl;
			continue;
		}	     
		 
	     else Stats(memorySimulation, mem); // display statistics of memory
		 
	} //end Stat if
	
	
	else if(commandName.compare("QUIT") == 0){ // if the value via console = QUIT [QUIT CASE]
	
	cout << "Program Terminating" << endl;
	exit(0); // exit out of the program
	
	} //end STAT if
		
		
	else cout << "please make sure you use spelled a command correctly: RQ | RL | C | STAT | QUIT " << endl;	

  }// end of true

} //end main
