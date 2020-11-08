/*************************************************************************
	> File Name: test.c
	> Author:longkejie 
	> Mail:1721248012@qq.com 
	> Created Time: 2020年11月07日 星期六 05时13分09秒
 ************************************************************************/

#include<stdio.h>
#include<test.h>
#include <string.h>
Function funcarr[100];
int cnt;

int RUN_ALL_TESTS() {
    for (int i = 0; i < cnt; ++i) {
        printf("RUN TEST : %s\n",funcarr[i].str);
        funcarr[i].func();
        printf("RUN END\n");
    }
    return 0;
}


void add_function(TestFuncT func, const char* str) {
    funcarr[cnt].func = func;
    funcarr[cnt].str = strdup(str);
    cnt++;
}
