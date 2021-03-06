/*************************************************************************
	> File Name: test.h
	> Author:longkejie 
	> Mail:1721248012@qq.com 
	> Created Time: 2020年11月07日 星期六 05时13分32秒
 ************************************************************************/

#ifndef _TEST_H
#define _TEST_H
#include <linklist.h>
#define COLOR(s,a) "\033["#a"m"s"\033[0m"
#define COLOR_HL(s,a) "\033[1;"#a"m"s"\033[0m"
#define RED(s) COLOR(s,31)
#define RED_HL(s) COLOR_HL(s,31)
#define GREEN(s) COLOR(s,32)
#define GREEN_HL(s) COLOR_HL(s,32)
#define BULE(s) COLOR(s,34)
#define BULE_HL(s) COLOR_HL(s,34)
#define YELLOW(s) COLOR(s,33)
#define YELLOW_HL(s) COLOR_HL(s,33)


#define Type(a) _Generic((a),\
                         int : "%d",\
                         char : "%c",\
                         double : "%lf",\
                         const char * : "%s",\
                         float : "%f"\
                        )


#define PUT(a) {\
                char temp[80];\
                sprintf(temp,YELLOW_HL("%s"),Type(a));\
                printf(temp,a);\
}

#define TEST(a,b)\
    void a##_##b();\
        __attribute__((constructor)) void add##_##a##_##b() { add_function(a##_##b,#a"."#b);}\
    void a##_##b()

#define EXPECT(a,b,comp){\
                         __typeof(a) _a = (a);\
                         __typeof(b) _b = (b);\
                         if (!(_a comp _b)) {\
                             printf(YELLOW_HL("\n              %s:%d:failure\n"),__FILE__,__LINE__);\
                             printf(YELLOW_HL("                   expect : %s %s %s\n"),#a, #comp, #b);\
                             printf(YELLOW_HL("                   actual : "));\
                             PUT(_a);\
                             printf(YELLOW_HL(" vs "));\
                             PUT(_b);\
                             printf(YELLOW_HL("\n\n"));\
                        } else {\
                             my_testnum.success++;\
                        }\
                        my_testnum.total++;\
     printf(GREEN("[-----------]")" %s %s %s %s\n",#a,#comp,#b,_a comp _b ? GREEN_HL("TRUE") : RED_HL("FALSE"));\
                        }


#define EXPECT_EQ(a,b) EXPECT(a,b,==)
#define EXPECT_NE(a,b) EXPECT(a,b,!=)
#define EXPECT_GE(a,b) EXPECT(a,b,>=)
#define EXPECT_LE(a,b) EXPECT(a,b,<=)
#define EXPECT_GT(a,b) EXPECT(a,b,>)
#define EXPECT_LT(a,b) EXPECT(a,b,<)


int RUN_ALL_TESTS() ;

typedef void(*TestFuncT)();

typedef struct Function{
    TestFuncT func;
    const char * str;
    struct LinkNode p;
}Function;

typedef struct testnum{
    int total;
    int success;
}TestNum;

extern TestNum my_testnum;

extern Function funcarr[100];

void add_function(TestFuncT , const char *);

#endif
