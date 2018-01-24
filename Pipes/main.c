#include <stdio.h>          // Needed for stadart I/O
#include <stdlib.h>         // Needed for among others Malloc
#include <sys/types.h>      // Needed for Pid_t
#include <unistd.h>         // Needed for fork and getpid
#include <sys/wait.h>       // Needed for Waitpid
#include <errno.h>          // Needed for chehing if strtoyl has succeeded
#include "Subr.h"           // inculde the Subroutines
#define CHILD_COUNT 3       // declare the number of child processes for this programm

#ifndef O_S
#define O_S
struct OurStruct            // A struct which holds the values for the child processes
{
    pid_t CPID;
    int piperead;           //de pipe waarop child leest
    int pipewrite;          //de pipe waarop child chrijft
    int Ppipewrite;         //de pipe waarop parent schrijft
    int valueToProcess;
    int ReturnValue;
};
#endif // O_S

int main(int argc, char *argv[])
{
    struct OurStruct *child_list = malloc(sizeof(struct OurStruct)*(CHILD_COUNT)); //get an array of Structs by mallocing its size and handing the void* to the childlist as a where a pointer to an Array the size depends on the number of args(argc)
    char Child_count;
    int temp;               // Some values needed by the programm
    int ToParentPipe[2];    //an Int arrey used for the pipe which is shard by the the childeren so the parent only has to lissten on one place
    int *ToChildPipes = malloc(sizeof(int)*CHILD_COUNT*2);  //malloc an int array used for the pipes which the parent shall use to talk to the children.
    struct OurStruct Buff;                                  //The return value from the read, performed by the parent
    int args_index = 1;                                     //an index used to keep track what the next value is to be sent to the children

    pipe(ToParentPipe);                                     //make the pipe which the parent shall use to listen.

    if(CHILD_COUNT < argc)          //determine if we will need the full ammount of childeren indicated by the CHILD_COUNT
    {
        Child_count = CHILD_COUNT;
    }
    else                            //and set the value to the number required
    {
        Child_count = argc-1;
    }

    for(int i =1; i<=Child_count; i++, args_index++)
    {
        //loop trough the args and add the value to the child_list
        child_list[i-1].valueToProcess = (int)strtoul(argv[i], NULL, 10);
        if(errno != 0)      //if errno isn't 0 it has been set, probally by strtoul and we assume it didn't work, and exit with status 1
        {
            printf("Strtoul didn't work as it should/n");
            free(child_list);   //clean up the malloc-ed list
            return 1;
        }
    }

    for(int i = 0; i<Child_count; i++)
    {
        pipe(ToChildPipes+i*2);     //create a pipe in the 1st and 2nd element of the malloced int array.

        child_list[i].piperead      = *(ToChildPipes+i*2);  //figure out which pipes should go where for this child.
        child_list[i].pipewrite     = ToParentPipe[1];      //as an array is almost the same as a pointer we can use +1 instead of [1]
        child_list[i].Ppipewrite    = *(ToChildPipes+i*2+1);
        child_list[i].valueToProcess = (int)strtoul(argv[i+1], NULL, 10);

        temp = fork();  //fork and store the value
        if(temp == -1)  // if fork did return -1 something has gone wrong, and we exit with status 1
        {
            printf("Fork didn't work as it should/n");
            free(child_list);   //clean the malloc-ed list
            return 1;
        }

        if(!temp)       //if the value stored is 0 we are the child process
        {
            //close the two pipes which are reserved for the read and write actions of the parent.
            close(child_list[i].Ppipewrite);

            child_list[i].CPID = getpid();  //For completness add the PID of the child's process to its struct
            ChildLoop(&child_list[i]);      //execute the child process fuction with the apporpritate fuction

            close(child_list[i].pipewrite); //Sluit de pipes we haven't yet
            close(child_list[i].piperead);

            free(child_list);               //when the chil has finiehd its loop, free the malloced memory and
            free(ToChildPipes);             //exit normally
            exit(0);
        }
            //parent
            close(child_list[i].piperead);  //close the pipe on which the childeren are listing, as this is of no importnance to the parent
            child_list[i].CPID = temp;      //store the stored value as the PID of the child
    }
    /*The idea behind this part of the program is, wait until a child (that has recieved a value earlier)
    returns, print output based on the struct they retunred and than, if there is still a value, use the pipe
    that is in that same datastructure to inform the child about their next value, or if there isn't a next value,
    tell the child the work is done */

    for(int i=1; i<argc; i++)       //for every usable value in args*
    {
        read(ToParentPipe[0], &Buff, sizeof(struct OurStruct)); //get a return value from any of the children

        if(Buff.ReturnValue)  //if the Child executed with a value other than 0, the value is a prime
        {
            printf("Child with PID: %i found out that value %10i is a prime \n", Buff.CPID, Buff.valueToProcess);
        }
        else    //otherwise it isn't
        {
            printf("Child with PID: %i found out that value %10i is not a prime \n", Buff.CPID, Buff.valueToProcess);
        }

        if(args_index<argc) //if there are any other value yet to be processed:
        {
            write(Buff.Ppipewrite, argv[args_index],sizeof(unsigned long));
            args_index++;
        }
        else    //otherwise the work is done.
        {
            write(Buff.Ppipewrite, "Bye",sizeof(unsigned long));
            printf("Child with PID: %d has been laid to rest\n", Buff.CPID);
            waitpid(Buff.CPID,NULL,0);  //make sure thre won't be any zombie processens
            close(Buff.pipewrite);      //close the pipe we haven't yet.
            close(Buff.Ppipewrite);
        }
    }
    free(ToChildPipes);
    free(child_list);   //free the malloc-ed memory and exit
    return 0;
}
