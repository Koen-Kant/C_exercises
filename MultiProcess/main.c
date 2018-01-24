#include <stdio.h>          // Needed for stadart I/O
#include <stdlib.h>         // Needed for among others Malloc
#include <sys/types.h>      // Needed for Pid_t
#include <unistd.h>         // Needed for fork and getpid
#include <sys/wait.h>       // Needed for Waitpid
#include <errno.h>          // Needed for chehing if strtoyl has succeeded
#include "Subr.h"           // inculde the Subroutines

#ifndef O_S
#define O_S
struct OurStruct{           // A struct which holds the values for the child processes
    pid_t CPID;
    int valueToProcess;
};
#endif // O_S

int main(int argc, char *argv[]){
    struct OurStruct *child_list = malloc(sizeof(struct OurStruct)*(argc)); //get an array of Structs by mallocing its size and handing the void* to the childlist as a where a pointer to an Array the size depends on the number of args(argc)
    int temp;               // Some valuesn eeded by the programm
    int buffer;
    pid_t return_val;

    for(int i=1;i<argc;i++)
    {       //loop trough the args and add the value to the child_list
        child_list[i-1].valueToProcess = (int)strtoul(argv[i], NULL, 10);
        if(errno != 0)      //if errno isn't 0 it has been set, probally by strtoul and we assume it didn't work, and exit with status 1
        {
            printf("Strtoul didn't work as it should/n");
            free(child_list);   //clean up the malloc-ed list
            return 1;
        }
    }

    for(int i = 0; i<(argc-1);i++){
        temp = fork();  //fork and store the value
        if(temp == -1)  // if fork did return -1 something has gone wrong, and we exit with status 1
        {
            printf("Fork didn't work as it should/n");
            free(child_list);   //clean the malloc-ed list
            return 1;
        }
        if(!temp){      //if the vaue stored is 0 we are the child process
            //child
            child_list[i].CPID = getpid();  //For completness add the PID of the child's process to its struct
            ChildFunct(&child_list[i]);     //execute the child process fuction with the apporpritate fuction
        }
        else{                               //if parent
        //parent
            child_list[i].CPID = temp;      //store the stored value as the PID of the child
        }
    }

    for(int i=1; i<argc; i++){
        return_val = waitpid(-1, &buffer, 0);   //Wait once for every child, and print output based on the return value
        if(return_val == -1)    //if waitpid retunred -1, womething has gone wrong and we exit with status 1
        {
            printf("Waitpid didn't work as it should/n");
            free(child_list);   //clean the malloc-ed list
            return 1;
        }
        for(int i=0;i<argc;i++)
        {
            if(child_list[i].CPID == return_val) //find the ValueToProcess that was stored in the array list that matches with the child that finished
            {
                temp = child_list[i].valueToProcess;    //store the value if found
                break;
            }
        }
        if(buffer)  //if the Child executed with a value other than 0, the value is a prime
        {
            printf("Child with PID %i found out that value %10i is a prime \n", return_val, temp);
        }
        else    //otherwise it isn't
        {
            printf("Child with PID %i found out that value %10i is not a prime \n", return_val, temp);
        }

    }
    free(child_list);   //free the malloc-ed memory
    return 0;
}
