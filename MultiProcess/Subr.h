#ifndef SUBS
#define SUBS                // Make sure we don't redecalre the file if it already is
#include <math.h>           // Needed for ceill and sqrt using in the prime callculation

#ifndef O_S                 // Make sure we don't the redecalre the Struct
#define O_S
struct OurStruct{           // A struct which holds the values for the child processes
    pid_t CPID;
    int valueToProcess;
};
#endif // O_S

int IsAPrime(int in);       // Decare the needed Fuctions
void ChildFunct(struct OurStruct *infoin);

int IsAPrime(int in){
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

    for(int v=3;v<=go_to_val;v+=2){                 // check all odd number from 3(First number that we must check, as we have
                                                    // already checked 2 or lower) to the square root of the number
        if(!(in%v))
        {
            return 0;                               // if it devides it is not a prime and thus return 0
        }
    }
    return 1;
}

void ChildFunct(struct OurStruct *infoin){
    struct OurStruct hold = *infoin;
    if(IsAPrime(hold.valueToProcess))   // make sure we see the value given as a struct, and then use its value on the
                                        // prime checker, and then return 1 or 0 respectivly
    {
        exit(1);
    }
    else
    {
        exit(0);
    }
}
#endif //SUBS
