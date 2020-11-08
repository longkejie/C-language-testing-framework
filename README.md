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

