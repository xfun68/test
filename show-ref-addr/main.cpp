// =====================================================================================
// 
//       Filename:  main.cpp
// 
//    Description:  测试引用的地址和被引用的变量的地址是否相同
// 
//        Version:  1.0
//        Created:  05/31/2009 02:48:55 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Will King (xfun), xfun68@foxmail.com
//        Company:  
// 
// =====================================================================================

#include <stdio.h>

struct TestStruct {
    int a;
    double b;
    char c;
};

void showRefAddr(TestStruct& ts) {
    printf("showRefAddr: %p\n", &ts);
}

TestStruct& retRef(TestStruct& ts) {
    printf("retRef: %p\n", &ts);
    return ts;
}

int main(int argc, char* argv[])
{
    TestStruct ts;
    TestStruct& rts1 = ts;
    printf("main: ts   %p\n", &ts);
    printf("main: rts1 %p\n", &rts1);
    showRefAddr(ts);
    TestStruct& rts2 = retRef(ts);
    printf("main: rts2 %p\n", &rts2);
    
    return 0;
}

