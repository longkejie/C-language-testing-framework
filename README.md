# C-language-testing-framework

**author：longkejie**

**email：1721248012@qq.com**

## 前言：

本项目只是本人学习过程中的一个小的记录，如有错误可联系我更正。（太菜了！）

## 项目环境

**语言：C**

**环境**：**Unbutu** **16.04** **Linux**

**编译器：gcc version 5.4.0**

​										如下：

<img src="https://gitee.com/long_kejie/image/raw/master/image-20201107212821387.png" alt="image-20201107212821387" style="zoom: 67%;" />

**编辑器：vim**

## **项目组成**

### include文件夹

包含所需的所有头文件

### src文件夹

包含所需的所有源文件

### lib文件夹

包含所需的对象文件以及库的打包

### bin文件夹

包含相应的可执行文件

### makefile文件

linux下的编译文件

## 项目成果

仿照谷歌测试框架 **(C++)** 的一个简易的 **C** 语言测试框架

效果如下：

<img src="https://gitee.com/long_kejie/image/raw/master/image-20201107214653372.png" alt="image-20201107214653372" style="zoom:67%;" />

可以看到一共两组测试样例。每组都有一些测试数据。它能判断相应的测试数据是否正确。对与错误的测试数据它会输出相应的错误信息。每组测试样例测试完后会对结果进行相应的统计。

## 项目实行

### 项目需求（基础框架搭建所需要求）

首先我们可以观看一个简单的**调用谷歌测试框架去测试的主文件**

然后我们基于这个文件的功能去实现我们的**C语言的测试框架**。

```c++
#include<stdio.h>
#include <gtest/gtest.h>

int add (int x, int y) {
    return x + y;
}

TEST (testfunc, add) {

    EXPECT_EQ(add(1,0),1);
    EXPECT_EQ(add(2,3),5);
    EXPECT_EQ(add(3,3),5);

}

TEST (testfunc, add2) {
     
    EXPECT_EQ(add(3,0),3);
    EXPECT_EQ(add(2,2),5);
    EXPECT_EQ(add(3,4),5);

}
int main(int argc, char** argv) {
      //初始化 gtest 和 gmock
      ::testing::InitGoogleMock(&argc, argv);
      return RUN_ALL_TESTS();
  }       

```

可以看到它包含一个**TEST(a,b)** 表示一组测试样例。

以及一个**EXPECT_EQ**字样表示每组测试中的一个测试数据(**主要是测试是否相等**）。

在主函数中它还调用了一个RUN_ALL_TESTS()函数去运行每组测试样例。

在这里我们先主要实现这三个功能。

### C语言测试框架（basic_version）

上面我们分析了我们首要执行的三个要求：

#### TEST(a,b)

首先我们判断得知TEST一定是一个宏。

看这样子和函数也完全不搭边。

那我们这就可以开始去实现我们的**TEST**功能了

我们在**include/test.h**中去实现它。

如下：

```c
#define TEST(a,b) void a##_##b()
```

展开相当于定义了一个 void a_b()的函数对应着每一组测试样例。

#### EXPECT_EQ(a,b)

同样这肯定是一个相应的宏定义

我们在**include/test.h**中去实现它。

如下：

```C
#define EXPECT_EQ(a,b) printf("%s == %s ? %s\n",#a,#b,a == b ? "TRUE" : "FALSE");
```

大体的意思就是判断a和b是否相等 相等输出**TRUE**否则输出**FALSE**。

#### RUN_ALL_TESTS()

因为它在主函数中**return** **RUN_ALL_TESTS**(),所以我们大胆猜测它是个函数，而且返回值必须为0（主函数中返回值必须为0)。

同样我们在**include/test.h**中去声明它，在**src/test.c**中定义它。我们先简单的实现一下他。

声明如下：

```C
int RUN_ALL_TESTS();
```

定义如下：

```c
int RUN_ALL_TESTS() {
    return 0;
}
```

此时我们编辑我们的**makefile**如下：

```makefile
.PHONY:clean

all:lib/main.o lib/test.o
	gcc lib/main.o lib/test.o -o bin/test

lib/main.o:include/test.h src/main.c
	gcc -c -I ./include/ src/main.c -o lib/main.o

lib/test.o:include/test.h src/test.c
	gcc -c -I./include src/test.c -o lib/test.o

clean:
	rm -rf lib/test.o lib/main.o bin/test

```

**meke**一下将会在**bin**目录下生成**test**可执行文件。

我们使用**./bin/test**运行一下该可执行文件

![image-20201107224230352](https://gitee.com/long_kejie/image/raw/master/image-20201107224230352.png)

发现什么也没有执行。

这是因为我们的主函数中只执行了**RUN_ALL_TESTS**()函数，而我们并没有给它实现任何功能，只是返回了一个0，所以该可执行程序将不会执行任何操作。

接下来我们就应该来实现我们的第二版程序了。

### C语言测试框架（basic_version2)

因为我们刚才实现的版本没有执行任何操作，所以我们就想如何在执行主函数前执行我们的测试函数呢？

经过老师的指点我得知在**C语言中有如下关键字**

```c
__attribute__((constructor))
```

它可以设置函数属性，使该函数优于主函数先执行。

有了该属性，我们可以使我们的每组测试函数优先于主函数先执行。

我们将**TEST宏**修改如下：

```c
#define TEST(a,b) __attribute__((constructor)) void a##_##b()
```

这样我们的测试函数将会优于主函数先被执行。

我们再次**make**一下,然后执行可执行程序。

结果如下：

![image-20201107225830089](https://gitee.com/long_kejie/image/raw/master/image-20201107225830089.png)

可以看到我们的每组测试用例都被执行了，而且对于**==**判断也都正确。

这样我们就实现了我们测试框架的基础版本。

### C语言测试框架（Mid-level version1）

前面我们实现了一个基本的测试框架。

它能完成对每组测试数据进行测试。

但我们的程序只是单纯的在测试，它并没有对每组测试数据进行分类和统计。这样似乎显得我们的程序有点笨重。

并且我们的**RUA_ALL_TESTS**()函数并没有实现任何功能。

#### Mid-level 1要求

1.我们应该实现对每组测试数据进行分类。

2.我们应该优化我们的**RUN_ALL_TESTS**()函数，在该函数中调用我们的所有测试数据，并输出结果。

#### 实现过程

##### a.对每组测试数据进行分类

对于这个要求，我们可以利用C语言当中的一个结构体来保存我们的每组测试数据函数的函数指针变量，以及函数名字变量。

我们在**include/test.h**中去定义该结构体如下：

```c
typedef void(*TestFuncT)();//定义一个函数指针类型，方便保存每个测试函数的信息。

typedef struct Function{
    TestFuncT func;
    const char * str;
}Function;//定义一个Function结构体类型，来保存我们相应的数据。

```

然后我们就应该去实现一个函数，在每次调用TEST宏的时候，都将该测试数据保存起来，**这里我们先用一个结构体数组来对我们的每组测试数据进行保存**

我们在**include/test.h**中对该数组进行声明，使用**extern**即可

```c
extern Function funcarr[100];
```

这里我们定义了我们的数组大小为100，这样的设定似乎有点臃肿，它只能支持100组测试数据？what？。这里我们暂且不纠结，我们先来实现我们**Mid-level**的基本要求。

现在我们就需要来实现这一个将我们的测试数据保存到结构体数组中的函数了。

首先该函数可以没有返回类型，但他必须拥有我们每组测试数据的信息（函数指针，函数名称）。

我们先在include/test.h中来声明它：

```c
void add_function(TestFuncT , const char* );
```

然后我们在src/test.c去实现它：

首先我们要在**test.c**中定义该结构体数组，以及一个**int**类型的**cnt**（初始为0）来索引我们的下标。

```c
Funticon funcarr[100];
int cnt;//都是全局变量，初始值为0

void add_function(TestFuncT func, const char* str) {
    funcarr[cnt].func = func;
    funcarr[cnt].str = strdup(str);
    cnt++;
}

```

接下来我们就应该想，我们什么时候去调用该函数呢？是不是应该联想到我们上面的TEST宏，我们每次测试的时候都会调用它，这就是我们将该组测试信息添加到结构体数组中保存的最佳时机。

我们修改我们的TEST宏如下：

```c
#define TEST(a,b)\
    void a##_##b();\
        __attribute__((constructor)) void add##_##a##_##b() { add_function(a##_##b,#a"."#b);}\
    void a##_##b()
```

大体的意思是，先声明我们的每组测试函数，然后调用add_function函数将该测试函数添加到结构体数组（该操作是在主函数前执行的），然后再定义我们的测试函数。

这样我们就对每组测试数据分好类了。



##### b.优化RUN_ALL_TESTS()

前面我们对每组测试数据都分好类了，这里我们需要实现的就是调用所用测试数据，并将它们的结果输出。

实现如下：

```c
int RUN_ALL_TESTS() {
    for (int i = 0; i < cnt; ++i) {
        printf("RUN TEST : %s\n",funcarr[i].str);
        funcarr[i].func();
        printf("RUN END\n");
    }
    return 0;
}
```

我们一看就知道它干了什么事。

现在我们**make**一下，然后运行它。

结果如下：

![image-20201108100911250](https://gitee.com/long_kejie/image/raw/master/image-20201108100911250.png)

可以看到它对每组测试数据都分好了类，并且在我们调用了**RUN_ALL_TESTS**()它才进行输出

如果我们在主函数中，不调用它，结果将如下：

![image-20201108101241841](https://gitee.com/long_kejie/image/raw/master/image-20201108101241841.png)

什么也没发生。

至此我们的Mid-level1实现完成。

### C语言测试框架（Mid-level version2）

前面我们实现了我们的Mid-level 1，它能基本完成对于==的判断，以及对每组测试数据进行了分类。

但我们应该不满足于此，首先我们的测试框架是不是太丑了点。我们应该来美化一下它。

并且它只能满足于==的判断？我们还应该完成包括（>=, <=, >, !=, < ）的判断

#### Mid-level 2要求

1.对我们的程序进行美化（包含颜色

2.实现更多的测试种类

#### 实现过程

##### a.输出颜色和格式信息

在C语言中我们要输出彩色字体可以使用以下格式：

**使用格式：样式开始+被修饰字符串+样式结束**

样式开始：\033[参数1；参数2；参数3  其中参数1，参数2，参数3为可选组合

被修饰字符串：需要输出的内容

样式结束：\033[0m

根据以上的格式，我们可以声明一个颜色的宏，将相应的字符串改成具有颜色的字符串。

如下（因为我们的每种颜色只是参数不同，我们可以先定义一个不具体颜色的宏COLOR，然后在每种颜色的宏内，嵌套COLOR，只要我们改变具体的参数即可。这里我们实现红，绿，蓝，黄四种颜色，以及它们的高亮颜色：

```C
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
```

这样我们就声明好了对字符串进行四种颜色（以及高亮）渲染的宏。

接下来我们就在RUN_ALL_TESTS()中去实现相应的格式信息输出，以及给它们添加颜色。

修改如下：

```c
int RUN_ALL_TESTS() {
    for (int i = 0; i < cnt; ++i) {
        printf(GREEN("[====RUN====]")RED_HL(" %s\n"),funcarr[i].str);//贴加了相应的颜色
        funcarr[i].func();
        printf(GREEN("[==RUN END==]\n"));//贴加了相应的颜色
    }
    return 0;
}

```

**make**然后运行它，结果如下：

![image-20201108112841833](https://gitee.com/long_kejie/image/raw/master/image-20201108112841833.png)

是不是有点意思了。

然后我们还需要对每组测试数据的每个测试数据进行一下格式化，我们是在EXPECT_EQ宏对它进行输出的，这里我们就应该去修改一下该宏的输出。

如下：

```c
#define EXPECT_EQ(a,b) printf(GREEN("[-----------]")"%s == %s %s\n",#a,#b,a == b ? GREEN_HL("TRUE") : RED_HL("FALSE"));
```

**make**运行如下：

![image-20201108114338134](https://gitee.com/long_kejie/image/raw/master/image-20201108114338134.png)

可以看到我们的整体框架格式是搭建完成了。

##### b.完善EXPECT宏

上面的整体框架我们是搭建好了，那么现在我们就需要来完善我们的功能了。

首先，我们的测试框架只能对EXPECT_EQ进行测试（它只能判断两个表达式是否相等），我们需要多加一点功能，使他能做的不只是判等。

我们可以实现一个EXPECT宏，它传入两个表达式，以及这两个表达式之间进行比较的字符串，如（==，<=,等等）。

然后对于每个测试，我们只要改变它的符号即可。

总体实现如下：

```c
/*EXPECT宏，实现对不同comp进行判断*/
#define EXPECT(a,b,comp){\
    printf(GREEN("[-----------]")"%s %s %s %s\n",#a,#comp,#b,a comp b ? GREEN_HL("TRUE") : RED_HL("FALSE"));\
                        }
/*嵌套一个EXPECT宏，传入不同的COMP*/
#define EXPECT_EQ(a,b) EXPECT(a,b,==)
#define EXPECT_NE(a,b) EXPECT(a,b,!=)
#define EXPECT_GE(a,b) EXPECT(a,b,>=)
#define EXPECT_LE(a,b) EXPECT(a,b,<=)
#define EXPECT_GT(a,b) EXPECT(a,b,>)
#define EXPECT_LT(a,b) EXPECT(a,b,<)
```



接下来我们读我们完善后的EXPECT宏进行测试。我们修改我们的main.c ,使它包含多种测试种类。

然后**make**后，运行观看结果如下：

![image-20201108120911145](https://gitee.com/long_kejie/image/raw/master/image-20201108120911145.png)

这样我们的EXPECT宏就基本完善好了。

至此我完成了我们的Mid - level version2。

### C语言测试框架（Top version）

在Mid level中，我们搭建好了我们的基本框架，贴加了字体颜色以及格式化输出。最后我们还完善了我们的EXPECT宏，使它能完成更多的功能。

现在我们要来更加完善一下我们的程序，修改一些不那么好的设计，以及寻找相应的bug并修改它。

#### Top version要求

1.统计每组测试数据的总测试量，以及其中测试正确的数量，打印出相应信息**（总数量，成功的数量，成功百分比）**

2.打印错误提示信息（对每组测试数据中测试错误的相关测试的信息的打印）

3.优化相应可能会导致错误的设计

#### 实现过程

##### a.对每组测试数据进行统计并输出

这个我们可以利用一个结构体，该结构体包含两个信息（一组测试数据的测试数量以及它正确的数量）

我们在**include/test.h**中声明并定义如下：

```c#
typedef struct testnum{
    int total;
    int success;
}TestNum;

extern TestNum my_testnum;

```

然后我们就可以利用它来统计了，我们应该在什么时候进行统计呢？我们每组测试都是对EXPECT宏的调用，所以我们每次调用EXPECT宏的时候，都将改组测试数据的**total总量**加一，如果测试正确，**success**加一。

EXPECT宏修改如下：

```c
#define EXPECT(a,b,comp){\
                         my_testnum.total++;\
                         if (a comp b ) my_testnum.success++;\
    printf(GREEN("[-----------]")"%s %s %s %s\n",#a,#comp,#b,a comp b ? GREEN_HL("TRUE") : RED_HL("FALSE"));\
                        }
```

值得注意的是，my_testnum这个变量必须在**src/test.c**中去定义它，我们前面的**extern**只是对它进行一个声明。

这样我们就统计好我们的相应信息了，就下来我们就在**RUN_ALL_TESTS**()函数中对我们的统计信息进行输出。

RUN_ALL_TESTS()修改如下：

```c
int RUN_ALL_TESTS() {
    for (int i = 0; i < cnt; ++i) {
        printf(GREEN("[====RUN====]")RED_HL(" %s\n"),funcarr[i].str);
        my_testnum.total = 0, my_testnum.success = 0;//清零以便后续测试组测试数据的统计
        funcarr[i].func();
        printf(GREEN("[  "));
        if (my_testnum.total == my_testnum.success) {
            printf(BULE_HL("%6.2lf%%"),100.0);//全部正确，输入百分百（为蓝色）
            printf(GREEN("  ]"));
            printf(" total : %d success : %d\n",my_testnum.total,my_testnum.success);
        }else {
            printf(RED_HL("%6.2lf%%"),(double)my_testnum.success/my_testnum.total * 100.0);//有错误的话，输入的为红色
            printf(GREEN("  ]"));
            printf(" total : %d success : %d\n",my_testnum.total,my_testnum.success);
        }
    }
    return 0;
}

```

展示结果如下：

<img src="https://gitee.com/long_kejie/image/raw/master/image-20201109161016926.png" alt="image-20201109161016926" style="zoom:67%;" />

这样我们就完成了对每组测试数据的统计。

##### b.打印错误提示信息

我们的错误信息中，要包含错误所在文件，以及行数，以及错误提示。

我们先来实现打印错误所在的文件，和行数。

这里我们使用C的两个内置宏即可

在我们的EXPECT宏最前面加一个错误判断即可

如下:

```c
#define EXPECT(a,b,comp){\
                         if (!(a comp b)) printf(YELLOW_HL("\n              %s:%d:failure\n\n"),__FILE__,__LINE__);\
                         my_testnum.total++;\
                         if (a comp b ) my_testnum.success++;\
    printf(GREEN("[-----------]")" %s %s %s %s\n",#a,#comp,#b,a comp b ? GREEN_HL("TRUE") : RED_HL("FALSE"));\
                        }

```

这样我们就可以打印出我们测试错误的信息所在的文件以及行数，成果如下：

<img src="https://gitee.com/long_kejie/image/raw/master/image-20201109162256229.png" alt="image-20201109162256229" style="zoom:67%;" />

接下来我们还需打印相关的错误提示信息，这对我们的测试框架特别重要，可以提示我们如何去修改我们错误。

我们的错误提示信息如何设置才能通俗易懂呢？

首先它需要一个期盼值，然后再输出我们的实际情况，这样两个一对比，就能见分晓。

我们先来输出我们的期盼值，这不难，我们是期盼我们的测试是正确的，所以将我们的测试输出即可。

```c
#define EXPECT(a,b,comp){\
                         if (!(a comp b)) {\
                             printf(YELLOW_HL("\n              %s:%d:failure\n"),__FILE__,__LINE__);\
  \\相关期盼值的打印          	printf(YELLOW_HL("                   expect : %s %s %s\n\n"),#a, #comp, #b);\
                         }\
                         my_testnum.total++;\
                         if (a comp b ) my_testnum.success++;\
    printf(GREEN("[-----------]")" %s %s %s %s\n",#a,#comp,#b,a comp b ? GREEN_HL("TRUE") : RED_HL("FALSE"));\
                        }

```

结果如下：

<img src="https://gitee.com/long_kejie/image/raw/master/image-20201109163238101.png" alt="image-20201109163238101" style="zoom:67%;" />

对于第一个错误，我们实际上是期盼add(3,3) == 5的 ，但是实际上不是的，我们接下来应该将我们的实际情况打印出来。

接下来就是实现我们这个功能的最难部分了，我们应该如何获得我们测试数据的类型呢。

接下来就是我们的泛型编程的用处了，C语言中有一个**_Generic**((a),....)关键字，它能根据表达式a的类型。映射出我们想要的表达式。

官网解释如下：

![image-20201109163949643](https://gitee.com/long_kejie/image/raw/master/image-20201109163949643.png)

可以了解到的是，它能根据我们控制表达式的类型，获取到关联列表中相应的表达式。

有了这个，我们的问题就迎刃而解了。

我们可以定义一个Type宏，可以通过我们传入进去的变量的相应类型得到相应表达式，我们获取到的表达式内容应该是一个字符串,来控制我们printf输出，如（int对应的是“%d”，double对应的是“%lf”）

Type宏定义如下：

```c
#define Type(a) _Generic((a),\
                         int : "%d",\
                         char : "%c",\
                         double : "%lf",\
                         const char * : "%s",\
                         long long : "%lld",\
                         char * : "%s",\
                         float : "%f"\
                        )
```

接下來我们就可以通过Type(a) 来获得相应的类型了。

值得注意的是，**_Generic**返回的是一个表达式，不是一个字符串，我们不能直接用来作为printf的控制格式字符串，我们可以通过

**sprintf**来将该表达式的内容读入到相应的字符串， 然后利用该字符串进行输出。

我们可以定义一个PUT宏，来原子性的完成我们下面想要的操作。

1.获取变量a所对应的表达式

2.将该表达式的内容（字符串）输入到我们定义的一个临时变量temp中存起来

3.利用temp进行格式化输出。

代码如下：

```c
#define PUT(a) {\
                char temp[80];\
                sprintf(temp,YELLOW_HL("%s"),Type(a));\
                printf(temp,a);\
}
```

下面我们就可以在我们的EXPECT宏中调用PUT宏来进行相应的输出了。

```c
printf(YELLOW_HL("                   actual : "));\
PUT(a);printf(YELLOW_HL(" vs "));PUT(b);printf(YELLOW_HL("\n\n"));\

```

打印结果如下：

<img src="https://gitee.com/long_kejie/image/raw/master/image-20201109171700447.png" alt="image-20201109171700447" style="zoom:67%;" />

对应第一个错误，我们期望add(3,3) == 5

实际是 6 == 5，这肯定是错误的。

我们将这两边的类型改一下，看是否能输出正确。

<img src="https://gitee.com/long_kejie/image/raw/master/image-20201109171900240.png" alt="image-20201109171900240" style="zoom:67%;" />

可以看到它能识别出double类型的表达式。

这样我们的打印错误提示信息就完成了。

##### c.优化Top version

在我们的EXPECT宏中，我们是直接利用a comp b进行比较，这可能在宏的展开过程中由于符号的优先级问题出现问题，所以我们需要将两个表达式a，b封装起来，先计算再比较。

改动如下：

```c
#define EXPECT(a,b,comp){\
                         __typeof(a) _a = (a);\
                         __typeof(b) _b = (b);\
                         ......................\
                         ......................\
                         ......................\
                         ......................\
                         }
```

...为我们后续省略的内容，详细可见前面，这里我们主要实现的是，利用_a来封装a, _b来封装b，后续的所有比较操作，都是它两在进行，这样就不会出现任何因为优先级的问题而发生的错误。

至此我们就实现了我们的Top version。

### C语言测试框架（Final version）

截止现在我们将我们的功能基本完成了，以及进行了优化。那我们怎么还有个最终版本呢？**（Final version)**

在刚开始设计框架的时候，为了简便，我们对我们的每组测试样例用数组保存起来了。数组开了100的大小。

你这是可能就会想到当我们的测试样例增加到100组以上时怎么办呢？我们的程序这时就显得有点无能为力了。

那我们应该如何来实现动态添加测试样例呢？

我们可以使用支持动态扩容的顺序表，以及链表来实现。

但是我们的顺序表每次扩容的时候都要进行大量的数组数据迁移，不能达到灵活的进行添加测试样例的功能。

鉴于上述原因，我们使用链表来实现

#### Finished version要求

a.使用链表来完成信息的存储，并在使用后有效的释放内存

#### 实现过程

##### a.建立链表外骨骼

我们首先建立一个和链表相关的头文件include/linklist.h，定义一个链表结构体。

如下：

```c
struct LinkNode {
    struct LinkNode *next;
};
```

它只包含一个指向下一节点的next指针域。

然后我们就需要在我们Function结构体中去定义一个**struct LinkNode** 类型的变量p，用来寻求每个Function类型变量（得到相应的测试信息）所在的地址。

如下：

```c
typedef struct Function{
    TestFuncT func;
    const char * str;
    struct LinkNode p;
}Function;
```

我们每次可以通过p的地址以及偏移量得到相应的结构体所在的首地址，然后就可以索引到func函数，以及str函数的名字。

然后我们就需要在我们的include/linklist.h中去定义相应的宏来实现寻找首地址的功能。

如下：

```c
#define offset(T,name) (long long)(&(((T*)(NULL)) -> name)) //得到结构体中相应变量的偏移量。

#define Head(p,T)  (T*)((void *)(p) - offset(T,p)) //得到T类型结构体的首地址

```

现在我们的链表外骨骼就搭建起来了。

##### b.利用链表保存相应数据

我们首先在我们的src/test.c中定义一个Function类型的全局变量func_head（不保存然后信息）,以及一个指向链表尾结点的指针（Function *）类型的全局变量func_tail（初始化为func_head的地址）。

然后在我们的add_function函数定义如下：

```c
void add_function(TestFuncT func, const char* str) {
    Function *temp = (Function *)calloc(1,sizeof(Function));
    temp -> func = func;
    temp -> str = strdup(str);
    func_tail -> p.next = &(temp -> p);
    func_tail = temp;
}

```

至此我们的所有测试信息都保存在链表中了，因为链表是支持扩容的，只要我们的系统内存还够，我们的测试信息就能一直被保存起来。

##### c.在RUN_ALL_TESTS()函数中，利用链表进行信息的输出。

如下：
中间的内容没有改变，就省略了。

主要改变的是我们每次都是地址去访问相应的信息了，不是简单的数组直接访问了。

```c
int RUN_ALL_TESTS() {
    for (struct LinkNode *p = func_head.p.next;p != NULL;p = p->next) {
        Function *func = Head(p,Function);
        printf(GREEN("[====RUN====]")RED_HL(" %s\n"),func -> str);
        my_testnum.total = 0, my_testnum.success = 0;//清零以便后续测试组测试数据的统计
        func -> func();
        ..........................................................................
         ..........................................................................
            ..........................................................................
            ..........................................................................
    return 0;
}

```

至此我们的C语言的简易测试框架就完成了。

但我们的设计肯定不可能在此止步，后续还会进行一步步的优化，这里我就不详谈了。

如果本说明有什么技术上不懂的（第一次写这种整个项目的说明文档，可能写的比较简陋！！！）

可邮箱联系。

联系方式详见本文开头。