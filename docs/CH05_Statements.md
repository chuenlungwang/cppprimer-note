C++ 定义了表达式语句（expression statement）与声明语句（declaration statement）和控制语句（flow-of-control statement）以及空语句（empty statement）。

表达式语句就是表达式后跟一个分号，其对表达式进行求值并将结果抛弃，通常表达式语句会产生副作用（size effect）如：赋值、输入输出等。

声明语句则是声明各种变量，这在几章中进行过详细的描述。

控制语句包括：if 语句、switch 语句、while 语句、do-while 语句以及 for 语句。这些语句跟 C 、Java 语言中的结构几乎完全一致，一致的部分不再给出。

空语句就是一个分号，本身不具有任何功能，当语言要求必须有一条语句而又没有可执行的操作时就可以写一条空语句。

以上语句加上 {} 大括号构成复合语句（compound statement）或者称为块（block）。块引入一个新的作用域，定义在此作用域中的名字只能在此作用域或其子作用域中可见。名字从声明位置到块结束的右括号为止。这种复合语句与简单语句是可以互换的，通常当逻辑上需要多条语句，而语言只允许一条语句时就会使用复合语句。

# if 语句

if 语句需要注意悬决 else（dangling else） 问题，即 else 只与最近的 if 匹配，如果不是这样，需要使用括号强制匹配。

# switch 语句

case 标签中的值必须是整型常量表达式，并且不能有任何两个 case 具有相同的值。当 switch 选中一个 case 后会一直执行到最后，如果需要只执行一个 case 则需要主动使用 break 。switch 语句中涉及到跳过代码，C++ 语言要求一定不能跳过变量的定义而直接跳到使用的地方。如：
````cpp
int val = 1;
switch(val) {
    case 0:
        int passby = 0;
        break;
    case 1:
        ++passby;  //错误!! 跳过了定义，而直接使用变量 passby
        break;
}
````

# while 语句

注意 while 条件或循环体中定义的变量，在每次循环时会重新定义和销毁。

# do-while 语句

需要注意在 do 循环体中定义的变量，无法在条件中访问。do-while 语句至少会执行一次。

# for 语句

for 分为两种：传统 for 和范围 for。传统 for 就是 C 风格的 for，头部形如：`for(initializer;condition;expression)`其中 for 头部中的三个部分都可以省略，此时循环体就会无限执行。在 C++ 中 initializer 中定义的变量当循环结束时会被销毁，而在 C89 中并不会，C99 行为与 C++ 一样。

范围 for 形如：`for (declaration : expression)` 其中表达式可以是大括号中的初始列表（initializer list）、数组、或者容器类或 string 类等能够被遍历的序列类型。declaration 中定义的变量类型必须与序列中元素类型一致，最常用的方式是用 auto 声明变量，如果希望改变序列中的值，将循环变量类型定义为引用类型。每次循环时，循环变量都会被初始化为下一个元素的值。

# goto 语句

goto 语句用于在同一个函数内跳转。goto 跳转需要给出一个标签，这个标签在一条语句的头部，形成标签语句（labeled statement）。形如：
````cpp
goto label;
label: statement;
````
其中标签独立于语言中的任何别的类别的标识符，即便是与它们重名亦是可以的。与 switch 一样，不可以跳过变量的定义而直接到使用的位置。如果是向后跳转则将跳过部分的变量销毁。

# 异常处理

在 C++ 中进行异常处理是很难做到干净而简单的，其根本原因在于 C++ 中的对象是由程序员管理的。而抛出异常之后需要将申请的资源以及内存回收，C++ 除了会回收局部对象外并不做额外的工作。C++ 的异常机制没有 finally 子句，原因在于 C++ 有 RAII（Resource Acquisition Is Initialization）获取资源即初始化，当对象销毁时将自动释放资源。

C++ 允许抛出任何类型的对象而不仅仅是 exception 及其子类。C++ 抛出的对象不需要 new ，因而，编译器会将其放到一个特殊的位置中，从而可以在不同的调用栈层次进行匹配检查。throw 子句通常构造一个对象，将这个对象放到编译器的特殊位置中。catch 子句中包含一个对象声明，当匹配时编译器将抛出的对象复制到声明对象中，如果声明为引用则将其初始化为抛出对象的引用。当 catch 子句结束时，声明的对象将先被销毁，然后抛出对象也被销毁。重新抛出对象只要 `throw;` 即可，此时会将同一个异常对象再次抛出，如果声明为引用并且改变了此异常对象，那么再次抛出的就是改变后的对象。

查看：[throw.cpp](https://github.com/chuenlungwang/cppprimer-note/blob/master/code/throw.cpp) 了解示例代码。

C++ 的异常匹配与 Java 中的一样，将最匹配最先找到的 catch 子句，而不是精确匹配，所以应当将最具体的异常类放在最前面。另外需要注意的一点是在 try 块中声明的对象是不能在 catch 中访问的。

## 警告：C++ 中很难达到异常安全

当异常发生时将打断程序的正常执行流，此时某些计算还未完成，这样便将一些对象置于非法或者不完全状态，或者资源没有被释放。程序必须正确的清理所有这些才算是异常安全。需要处理异常的程序需要时刻谨记可能出现的异常，以及必须采取的步骤来保证对象处于安全的状态以及资源不会泄露，并且将程序恢复到正确的状态。

## 标准异常

exception 是所有标准异常的基类，定义在 exception 头文件中。stdexcept 头文件中定义了诸如：

- `runtime_error` 运行时错误；
- `range_error` 运行时错误，产生的结果超出有效值范围；
- `overflow_error` 运行时错误，计算上溢；
- `underflow_error` 运行时错误，计算下溢；
- `logic_error` 程序逻辑错误；
- `domain_error` 逻辑错误，在给定参数的情况下无法生成结果；
- `invalid_argument` 逻辑错误，非法参数；
- `length_error` 逻辑错误，试图生成一个操作最大尺寸的独享；
- `out_of_range` 逻辑错误，参数超出合法值的范围；

具体参考网页：[https://en.cppreference.com/w/cpp/error/exception](https://en.cppreference.com/w/cpp/error/exception)

标准库中的异常只有少数操作：创建、复制、赋值，其中 `exception` `bad_alloc` `bad_cast` 只有默认构造函数。其它类型的异常只有一个接收字符串的构造函数用来说明异常信息，这些异常信息可以通过调用异常的 `what()` 成员函数来取得。