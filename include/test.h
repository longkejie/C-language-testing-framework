/*************************************************************************
	> File Name: test.h
	> Author:longkejie 
	> Mail:1721248012@qq.com 
	> Created Time: 2020年11月07日 星期六 05时13分32秒
 ************************************************************************/

#ifndef _TEST_H
#define _TEST_H

#define TEST(a,b) __attribute__((constructor)) void a##_##b()

#define EXPECT_EQ(a,b) printf("%s == %s ? %s\n",#a,#b,a == b ? "TRUE" : "FALSE");

int RUN_ALL_TESTS() ;

#endif
