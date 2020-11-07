# C-language-testing-framework

**author：longkejie**

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

我们在**./include/test.h**中去实现它。

如下：

```c
#define TEST(a,b) void a##_##b()
```

展开相当于定义了一个 void a_b()的函数对应着每一组测试样例。

#### EXPECT_EQ(a,b)

同样这肯定是一个相应的宏定义

我们在**./include/test.h**中去实现它。

如下：

```C
#define EXPECT_EQ(a,b) printf("%s == %s ? %s\n",#a,#b,a == b ? "TRUE" : "FALSE");
```

大体的意思就是判断a和b是否相等 相等输出**TRUE**否则输出**FALSE**。

#### RUN_ALL_TESTS()

因为它在主函数中**return** **RUN_ALL_TESTS**(),所以我们大胆猜测它是个函数，而且返回值必须为0（主函数中返回值必须为0)。

同样我们在**./include/test.h**中去声明它，在**./src/test.c**中定义它。我们先简单的实现一下他。

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

