// =====================================================================================
// 
//       Filename:  main.cc
// 
//    Description:  测试文本协议
// 
//        Version:  1.0
//        Created:  07/13/2009 05:38:46 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Wang Xiaofeng (xfun68), xfun68@gmail.com
//        Company:  Linekong
// 
// =====================================================================================

#include <stdio.h>
#include <inttypes.h>
#include <AGIP.h>

int main(int argc, char* argv[])
{
    AGIPPfBalanceQueryRes pfBalanceRes('<', '>', ';', '=', '|');

    AGIPPfBalanceQueryRes::BalanceInfo* balanceArray =
        new AGIPPfBalanceQueryRes::BalanceInfo[2];
    memset(balanceArray, 0, 2*sizeof(AGIPPfBalanceQueryRes::BalanceInfo));

    balanceArray->nSubjectID = 3;
    balanceArray->nAmount = 1200;
    (balanceArray+1)->nSubjectID = 4;
    (balanceArray+1)->nAmount = 800;

    pfBalanceRes.setBalanceDetail(2, balanceArray);

    pfBalanceRes.showInfo();

    int resCount = 2;

    AGIPPfBalanceQueryRes::BalanceInfo* resDetailAry =
        new AGIPPfBalanceQueryRes::BalanceInfo[resCount];
    memset(resDetailAry, 0, resCount*sizeof(AGIPPfBalanceQueryRes::BalanceInfo));

    pfBalanceRes.getBalanceDetail(&resCount, resDetailAry);

    for (int32_t i = 0; i < 2; ++i) {
        printf("[%03d] Subject: %d;Amount: %d\n",
            i,
            resDetailAry[i].nSubjectID,
            resDetailAry[i].nAmount);
    }

    pfBalanceRes.showInfo();

    return 0;
}

