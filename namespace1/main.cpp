#include "ns2.h"

using namespace ns1;

int main(int argc, char* argv[])
{
    NSA nsa;
    NSB nsb;

    nsa.SayAWord();
    nsb.SayAWord(&nsa);

    return 0;
}
