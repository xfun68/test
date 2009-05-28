#include "ns2.h"

#include <stdio.h>

/*
ns1::NSB::NSB(void)
{
}

int ns1::NSB::SayAWord(const NSA* nsa)
{
    printf("This is in NSB: ");
    nsa->SayAWord();
    return 1;
}
*/

namespace ns1
{

    NSB::NSB(void)
    {
    }

    int NSB::SayAWord(NSA* const nsa)
    {
        printf("This is in NSB: ");
        nsa->SayAWord();
        return 1;
    }

}
