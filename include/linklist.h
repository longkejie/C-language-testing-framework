/*************************************************************************
	> File Name: include/linklist.h
	> Author:longkejie 
	> Mail:1721248012@qq.com 
	> Created Time: 2020年11月10日 星期二 22时03分31秒
 ************************************************************************/

#ifndef LINKLIST_H
#define LINKLIST_H


#define offset(T,name) (long long)(&(((T*)(NULL)) -> name)) 

#define Head(p,T)  (T*)((long long)(void *)(p) - offset(T,p))

struct LinkNode {
    struct LinkNode *next;
};


#endif
