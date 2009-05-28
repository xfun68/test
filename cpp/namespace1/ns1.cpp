#include "ns1.h"

#include <stdio.h>
#include <string.h>

ns1::NSA::NSA(void)
{
    memcpy(word_, "in nsa\0", 7);
    word_[sizeof(word_)-1] = '\0';
}

int ns1::NSA::SayAWord(void)
{
    puts(word_);
    return 1;
}
