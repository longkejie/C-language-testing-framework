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
TestNum my_testnum;
int cnt;

int RUN_ALL_TESTS() {
    for (int i = 0; i < cnt; ++i) {
        printf(GREEN("[====RUN====]")RED_HL(" %s\n"),funcarr[i].str);
        my_testnum.total = 0, my_testnum.success = 0;//清零以便后续测试组测试数据的统计
        funcarr[i].func();
        printf(GREEN("[  "));
        if (my_testnum.total == my_testnum.success) {
            printf(BULE_HL("%6.2lf%%"),100.0);
            printf(GREEN("  ]"));
            printf(" total : %d success : %d\n",my_testnum.total,my_testnum.success);
        }else {
            printf(RED_HL("%6.2lf%%"),(double)my_testnum.success/my_testnum.total * 100.0);
            printf(GREEN("  ]"));
            printf(" total : %d success : %d\n",my_testnum.total,my_testnum.success);
        }
    }
    return 0;
}


void add_function(TestFuncT func, const char* str) {
    funcarr[cnt].func = func;
    funcarr[cnt].str = strdup(str);
    cnt++;
}
