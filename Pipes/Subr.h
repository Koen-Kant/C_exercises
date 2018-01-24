#ifndef SUBS
#define SUBS                // Make sure we don't redeclare the file if it already is
#include <math.h>           // Needed for ceill and sqrt we use in the prime callculation
#include <string.h>         // Cheking for the return value for the Subr

#ifndef O_S                 // Make sure we don't the redeclare the Struct
#define O_S
struct OurStruct            // A struct which holds the values for the child processes, mostly pipes
{
    pid_t CPID;
    int piperead;       //de pipe waarop child lees
    int pipewrite;      //de pipe waarop child chrijft
    int Ppipewrite;     //de pipe waarop parent schrijft
    int valueToProcess;
    int ReturnValue;
};
#endif // O_S

int IsAPrime(int in);       // Decare the needed Fuctions
void ChildLoop(struct OurStruct *infoin);

int IsAPrime(int in)
{
    if(in==2)           //make sure that we take out two as it would mess with the other checks
    {
        return 1;
    }

    else if(in<2 || !(in&1))    // if the number is 1 or smaller the number isn't a prime
    {
        return 0;               // in addition if the number isn't odd(LSB is 0) it is dividable by 2
    }

    int go_to_val = (int)ceil(sqrt((double)in));    // as we'll be dicinding, it will only be of use if we devide until the
    // root of the number, as we'l be repeating the devision

    for(int v=3; v<=go_to_val; v+=2)                // check all odd number from 3(First number that we must check, as we have
    {
        // already checked 2 or lower) to the square root of the number
        if(!(in%v))
        {
            return 0;                               // if it devides it is not a prime and thus return 0
        }
    }
    return 1;
}

void ChildLoop(struct OurStruct *infoin)
{
    char Buff[20];                          //declare the buffer for the read
    struct OurStruct hold = *infoin;        //make sure we have a Struct Ourstruct
    while(1){
        hold.ReturnValue = IsAPrime(hold.valueToProcess);   //determine if the vakue we got was a prime and store it
        write(hold.pipewrite, &hold, sizeof(struct OurStruct)); //stuff the entire data structre trough the pipe so the parent can easly find who had sent it.
        read(hold.piperead, &Buff, sizeof(unsigned long));                         //read whatever the reply was,
        if(!strcmp(Buff,"Bye"))                             //and get out of the loop when we are done.
        {
            break;
        }
        hold.valueToProcess = strtoul(Buff,NULL, 10);           //otherwise use strtoul to get the next value and begin again.
    }
}
#endif //SUBS
