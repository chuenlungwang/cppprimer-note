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

### 8.1.3 管理输出缓冲

每个输出流都管理着一个缓冲，这个缓冲用于装载程序读写的数据。程序调用输出可能会立即打印出去，也可能操作系统会将数据缓存在 buffer 中，在之后 buffer 积累满了之后再打印出去。使用缓冲允许操作系统将多次输出操作拼接起来，最后调用一次系统级的写操作。这是因为写入到设备中是很耗时的操作，让操作系统将多次输出操作拼接成一次写操作可以提供很大的性能提升。

有多种条件将导致缓冲被刷新——将数据写入到真正的输出设备或者文件中去：

- 程序正常结束，所有的程序中使用到的输出缓冲都会被写入到设备中去；
- 在某些中间时刻如果缓冲已经满了的时候，会在下一次写操作前将其内容刷新到设备中去；
- 通过 endl 操纵子（manipulator）显式将刷新缓冲；
- 使用 unitbuf 操纵子在每次输出操作之后将流的内部状态设为空的缓冲。默认情况下，只有 cerr 的 unitbuf 是设置了的，所以 cerr 的刷新是立即的；
- 一个输出流被绑定到另外一个流上，在这种情况下，被绑定的流将会在绑定的流进行读写时刷新。默认情况下，cout 被绑定到 cin 和 cerr 上，意味着，当任何时候读 cin 或者写 cerr 时都会刷新 cout 中的缓冲；

**刷新输出缓冲**

endl 在缓冲中增加一个换行符并刷新，flush 不添加任何字符进行刷新，ends 添加一个 NULL 字符然后刷新。如：
````cpp
std::cout << "hi!" << std::endl;
std::cout << "hi!" << std::flush;
std::cout << "hi!" << std::ends;
````

**unitbuf 操纵子**

如果想在每次输出之后都进行刷新，就需要用到 unitbuf 操纵子。这个操纵子告知流在每次写入时都调用 flush，nounitbuf 操纵子则将流的状态恢复到正常，由操作系统控制缓冲刷新：如：
````cpp
cout << unitbuf; // all writes will be flushed immediatedly
cout << nounitbuf; // returns to normal buffering
````

**注意：当程序崩溃时缓冲不会被刷新**

当程序意外终止时输出缓冲是不会刷新的，此时程序写入的数据很可能还在缓冲中等待被打印。当你调试一个崩溃的程序时，确保你认为应该输出的数据确实刷新了。由于这种问题已经导致了不计其数的调试时间被浪费了。

**将输入流和输出流绑在一起**

当一个输入流和输出流绑在一起时，当尝试去读取输入流时将会先刷新输出流中的缓冲。ite 函数有两个重载的版本：一个没有参数并返回一个输出流的指针，如果它绑定了一个输出流的话。如果没有绑定则返回一个 nullptr。第二个版本接收一个 ostream 类型的指针，并将其绑定到这个流上，如：`x.tie(&o)` 将流 x 绑定到输出流 o 上，这样 x 的任何操作将导致 o 的输出缓冲被刷新。可以将 istream 或 ostream 对象绑定到另外一个 ostream 上。如：
````cpp
cin.tie(&cout);
ostream *old_tie = cin.tie(nullptr); // cin is no longer tied
cin.tie(&cerr); // reading cin flushes cerr
cin.tie(old_tie);
````
交互式系统应该将其输入流绑定到输出流上，这样做意味着所有的读操作之前都会将缓冲中的数据刷新出去。

传给 tie 一个输出流的指针就可以将调用的流对象绑定到其上，如果传空指针则将其解绑。每个流每次只能绑定到一个输出流上。然而多个流对象可以同时绑定到一个输出流对象上。

## 8.2 文件输入输出

在 fstream 头文件中定义了三个支持文件 IO 的类型：ifstream 来读取给定文件，ofstream 来写入给定文件，fstream 可以同时读写文件。17 章中将介绍如何同时进行同一个文件的读写。

这几个对象提供了 cin 和 cout 提供的操作，如：`<<` `>>` 用于读写，可以用 getline 来读一个 ifstream，并且 8.1 节介绍的内容都适用于这几个类型。

除了从 iostream 类型继承来的行为之外，fstream 头文件中的类型还定义了管理文件相关的成员，这些操作可以被 fstream 、ifstream 或 ofstream 调用，但不能在其它 IO 类型上调用。如：

- `fstream fstrm;` 创建一个没有关联文件的文件流，fstream 是定义在 fstream 头文件中的一个类型；
- `fstream fstrm(s);` 创建一个 fstream 并打开名为 s 的文件，s 可以是 string 类型或者是一个 C 风格字符串指针，这个构造函数是 explicit 的，默认的文件模式取决于 fstream 的类型；
- `fstream fstrm(s, mode);` 与上一个构造函数类似，但是以给定的模式 mode 打开 s 文件；
- `fstrm.open(s)` 打开名为 s 的文件，将其关联到 fstrm 对象上，s 可以是 string 或者 C 风格字符串指针，默认的文件模式取决于 fstrm 的类型，返回 void；
- `fstrm.open(s.mode)` 与上面一样，但是由 open 方法提供文件开打模式 mode；
- `fstrm.close()` 关闭与 fstrm 关联的文件，返回 void；
- `fstrm.is_open()` 返回一个 bool 值告知是否此 fstrm 关联的文件已经成功打开，并且没有被关闭；

### 8.2.1 使用文件流对象

当我们想要读写文件时，我们通常会定义个文件流对象，并且将之关联到一个此文件上。每个文件流对象都定义了 open 成员函数，它会做与系统相关的操作来定位给定，并且打开它进行读或写操作。

当创建文件流对象时可以选择性的提供一个文件名，如果提供了文件名，那么 open 就会自动调用。如：
````cpp
ifstream in(ifile); // construct an ifstream and open the given file
ofstream out; // output file stream that is not associated with any file
````
在新标准下，文件名字可以是 string 类型或者 C 风格字符数组。之前的版本只支持 C 风格字符数组。

**open 和 close 成员**

当定义一个空的文件流对象时，可以接着在后面通过 open 将其关联到一个文件上。如：
````cpp
ifstream in(ifile);
ofstream out;
out.open(ifile + ".copy");
````
当调用 open 失败时，会设置 failbit，由于 open 可能会失败，所以最好需要验证一下 open 是否成功。如：
````cpp
if (out) // check that the open succeeded
    // the open succeeded, so we can use the file
````
如果打开失败，条件将会失败，我们就不能使用 out 对象。

一旦一个文件流对象被打开，它将与给定的文件持续关联。如果在一个已经打开的文件流对象上调用 open 将会失败，并且设置 failbit。接下来尝试使用这个文件流将会失败。为了将文件流对象关联到一个不同的文件上，需要选将之前的文件关闭，才能打开新的文件。如：
````cpp
in.close();
in.open(ifile + "2");
````
如果 open 成功的话，open 会将流的状态设置为 0 ，这样 good() 将返回 true。

**自动构建和析构**

代码如：
````cpp
for (auto p = argv + 1; p != argv + argc; ++p) {
    ifstream input(*p);
    if (input) {
        process(input);
    } else
        cerr << "couldn't open: " + string(*p);
}
````
每次迭代时都会自动创建一个新的 ifstream 对象并打开给定文件。由于 input 是 while 中的本地对象，它将在每次迭代时自动的创建和销毁。当 fstream 对象离开作用域之后，与其关联的文件会自动关闭。在下一次迭代时会创建一个新的。

### 8.2.2 文件模式
## 8.3 string 流
### 8.3.1 使用 istringstream 对象
### 8.3.2 使用 ostringstream 对象
## 关键概念