/*************************************************************************
	> File Name: test.c
	> Author:longkejie 
	> Mail:1721248012@qq.com 
	> Created Time: 2020年11月07日 星期六 05时13分09秒
 ************************************************************************/

#include<stdio.h>
#include<test.h>
#include <string.h>
#include <stdlib.h>

TestNum my_testnum;
Function func_head,*func_tail = &func_head;
struct LinkNode *temp;
int RUN_ALL_TESTS() {
    for (struct LinkNode *p = func_head.p.next;p != NULL;p = temp) {
        Function *func = Head(p,Function);
        printf(GREEN("[====RUN====]")RED_HL(" %s\n"),func -> str);
        my_testnum.total = 0, my_testnum.success = 0;//清零以便后续测试组测试数据的统计
        func -> func();
       /*释放相应的内存*/
        free(func);
        func = NULL;
        temp = p -> next;
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
    Function *temp = (Function *)calloc(1,sizeof(Function));
    temp -> func = func;
    temp -> str = strdup(str);
    func_tail -> p.next = &(temp -> p);
    func_tail = temp;
}
