/*************************************************************************
	> File Name: main.c
	> Author:longkejie 
	> Mail:1721248012@qq.com 
	> Created Time: 2020年11月07日 星期六 05时13分17秒
 ************************************************************************/

#include<stdio.h>
#include <test.h>

double add (double x, double y) {
    return x + y;
}


TEST (testfunc, add) {

    EXPECT_EQ(add(1,0),1);
    EXPECT_EQ(add(2,3),5);
    EXPECT_EQ(add(3,3),5);
    EXPECT_GT(add(3,3),5);
    EXPECT_LE(add(1,0),1);
    EXPECT_LT(add(3,4),5);
}

TEST (testfunc, add2) {
     
    EXPECT_EQ(add(3,0),3);
    EXPECT_EQ(add(2,2),5);

}
TEST (testfunc, add3) {
    EXPECT_EQ(add(3,4),7);
    EXPECT_LE(add(1,0),2);
}

int main () {
    
    return RUN_ALL_TESTS();
}
