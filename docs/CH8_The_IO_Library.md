## 8.1 IO 类
### 8.1.1 不能拷贝或赋值 IO 对象

IO 类型的对象是不可以拷贝或赋值的，函数只能传递或者返回流对象的引用。读取或写入 IO 对象会改变起状态，所以引用必须不是 const 的。

### 8.1.2 条件状态

IO 操作不可避免地会出现错误，有些错误是可以恢复的，如格式错误；有些错误则深入到系统中，并且超出了程序可以修正的范围。IO 类定义函数和标记来访问和修改流的条件状态（condition state）。如：

- strm::iostate strm 是一个 IO 类型，iostate 是一个机器相关的整形类型用于表示 stream 的条件状态；
- strm::badbit 常量值，可以赋值给 strm::iostate，用于表示流被损坏了；
- strm::failbit 常量值，可以赋值给 strm::iostate 用于表示 IO 操作失败了；
- strm::eofbit 常量值，可以赋值给 strm::iostate 用于表示流到了 end-of-file；
- strm::goodbit 常量值，可以赋值给 strm::iostate，用于表示流没有遇到错误，这个值保证是 0；
- s.eof()  当流对象的 eofbit 被设置时返回 true；
- s.fail() 如果流对象的 failbit 或 badbit 被设置时返回 true；
- s.bad() 如果流对象的 badbit 被设置时返回 true；
- s.good() 如果流对象状态没有任何错误（valid state）时返回 true；
- s.clear() 将流对象状态设置为没有错误，返回 void；
- s.clear(flags) 将流对象的状态设置为 flags 所表示的值，它是 strm::iostate 类型；
- s.setstate(flags) 在 s 上添加特定的条件状态 flags，flags 的类型时 strm::iostate，返回 void；
- s.rdstate() 返回 s 的当前条件状态，返回类型是 strm::iostate；

如以下代码：
````cpp
int ival;
std::cin >> ival;
````
如果输入 Boo 将会读取失败，输入操作符想要获取一个 int 类型的值，但是遇到了字符 B，这时 cin 对象中将会产生一个错误状态。同样，如果输入 end-of-file 时，cin 也会处于错误状态。

一旦发生了错误，此流对象接下来的 IO 操作将会全部失败，只有当流对象没有错误时才能对其进行读取和写入。由于流可能处于错误的状态，通常需要在使用前检查流是否处于正确状态。检查流对象的状态的最简单的方式就是将其放到一个条件中去：
````cpp
while (cin >> word)
    // ok: read operation successful
````
while 条件检查 `>>` 表达式返回的流的状态，如果操作成功，这个状态将保持有效，并且条件检查将会返回 true。

**检查流的状态**

将流放到条件中只是告知我们流是否是有效的，但是并没有告知发生了什么。有时需要知道更详细的信息。IO 对象使用一个机器相关的整形 iostate 来承载状态信息。这个类型是一个 bit 的集合。IO 类同时定义了 4 个常量值来表示不同的 bit 模式，它们用于表示不同的 IO 状态。这些值可以通过位操作符来测试和设置 iostate 值。

badbit 表示系统级的错误，如不可恢复的读写错误。通常如果 badbit 被设置的话，这个流对象就不再可用了。failbit 在一个可恢复的错误后设置，如，当需要一个数字时读取一个字符。通常是可以修正错误，并且继续使用流的。当到达文件尾部时会同时设置 eofbit 和 failbit。goodbit 则保证位 0，用于表示流没有错误。如果 badbit，failbit 或 eofbit 中任何一个被设置，那么对流进行条件求值将会失败。

标准库还定义了一系列函数来检查这些标记的状态。如果 good 函数返回 true 则表示没有任何一个错误位被设置，bad，fail 和 eof 函数返回 true 则是当对应的位被设置时。另外，fail 在 bad 位被设置时返回 true。

查看流的整体状态使用 good 或 fail 函数，将流对象用于条件中时，相当于调用 `!stream.fail()` ，eof 和 bad 操作则仅仅只揭示特定的错误是否发生。

**管理条件状态**

rdstate 成员返回流的当前状态的 iostate 值，setstate 在流上设置给定的状态位来表示发生了某些问题。而 clear 成员中不带参数则清除所有的失败位，在这之后调用 good 将返回 true。如：
````cpp
auto old_state = cin.rdstate();
cin.clear();
process_input(cin);
cin.setstate(old_state);
````

clear 还有一个有一个实参的版本，这个实参的类型是 iostate 值用于表示新的流状态，它是将参数中的值全部替换掉流中的状态值。为了关闭一个单独的状态，使用 rdstate 成员函数读取状态值，并用位操作符生成想要的新状态。如以下代码将 failbit 和 badbit 关掉，但是不改变 eofbit 的值：
````cpp
cin.clear(cin.rdstate() & ~cin.failbit & ~cin.badbit);
````

### 8.1.3 管理输出缓存
## 8.2 文件输入输出
### 8.2.1 使用文件流对象
### 8.2.2 文件模式
## 8.3 string 流
### 8.3.1 使用 istringstream 对象
### 8.3.2 使用 ostringstream 对象
## 关键概念