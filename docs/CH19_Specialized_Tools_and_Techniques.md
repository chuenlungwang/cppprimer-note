## 19.1 控制内存分配

当对内存分配有特别需求的时候可以重载 new 和 delete 操作符来控制内存分配。

### 19.1.1 重载 new 和 delete

重载 new 和 delete 操作符的方式与重载其它操作符的方式有非常大的不同。当使用 new 表达式时会依次发生三件事：1. 调用库函数 operator new 或者 operator new[] 来分配足够的裸的没有类型的内存以存储对象或数组。2. 调用构造函数构造对象。3. 返回指向这个新分配构建的对象的指针。

当使用 delete 表达式时将发生两件事。1. 调用析构函数进行析构；2. 编译器调用库函数 operator delete 或者 operator delete[] 来释放内存。

定义自己的 operator new 和 operator delete 函数可以 hook 掉内存分配过程。即便是库中已经包含了这些函数的定义，我们依然可以定义自己的版本，而编译器并不会抱怨说重复定义。反而，编译器会使用我们的版本。

当我们定义全局的 operator new 和 operator delete 函数，程序将在任何时候需要分配内存时都调用我们的函数。当编译器看到 new 或 delete 表达式时就会找到对应的操作符函数进行调用。如果被分配的对象是类类型，那么将首先在类作用域内（包括其基类作用域中）查找这两个函数，如果存在就调用它们。否则将从全局作用域中查找，如果编译器找到一个用户定义的版本，将调用此版本，如果还是找不到将调用库中的版本。

使用作用域操作符来指定使用的 new 或 delete 的版本，如：`::new` 和 `::delete`

**operator new 和 operator delete 接口**

库中定义了 8 个重载的 operator new 和 delete 函数，前面四个支持抛出 bad_alloc 异常，后面的则支持不抛出异常。如：
````cpp
//抛出异常的版本
void *operator new(size_t);
void *operator new[](size_t);
void *operator delete(void*) noexcept;
void *operator delete[](void*) noexcept;

//不抛出异常的版本
void *operator new(size_t, nothrow_t&) noexcept;
void *operator new[](size_t, nothrow_t&) noexcept;
void *operator delete(void*, nothrow_t&) noexcept;
void *operator delete[](void*, nothrow_t&) noexcept;
````
`nothrow_t` 是定义在 new 头文件中的 struct，这个类型没有成员。new 头文件中还定义了 nothrow 的常量对象，用户可以将其传递给不抛出异常的 new 。

与析构函数一样，operator delete 是永远不会抛出异常的。

如果应用程序定义以上函数，必须定义在全局作用域或者作为类的成员，如果定义为类的成员，那么这些操作符函数则隐式是静态的。因而，这些函数不能操作类中的数据成员。

当编译器调用 operator new 时，其第一个参数被初始化为对象的大小。调用 operator new[] 则传递存储给定数目元素的大小。

当定义我们自己的 operator new 时，可以传递额外的参数。如果 new 表达式想要调用这种函数的话，就需要使用定位 new 的形式来传递额外的实参。定义的 operator new 一定不会是以下形式，如：
````cpp
void *operator new(size_t, void*);
````
这个特别形式的函数原型是保留的给库使用，是不可以重复定义的。

调用 operator delete 时，编译器会传递待删除的对象的指针给这个函数。当将 operator delete 或 operator delete[] 定义为一个类成员时，函数可以有第一个参数为 `size_t` 类型，如果有的话，那么它将会被初始化为第一个参数所表示的对象的大小。真正调用的 delete 函数由被删除的对象的动态类型决定。

定义 operator new 和 operator delete 函数可以改变内存分配的方式，但是不能改变 new 和 delete 操作符的基本含义。

**malloc 和 free 函数**

malloc 和 free 是继承自 C 的库函数，我们的 operator new 和 operator delete 函数可以将底层工作交给这两个函数。其中 malloc 的参数是 `size_t` 乃是要分配的字节数，返回内存指针或者在失败时返回 0 。而 free 则以 malloc 的返回值作为参数，释放相关的内存，调用 free(0) 是合法的但是没有任何效果。
````cpp
void *operator new(size_t size) {
    if (void *mem = malloc(size))
        return mem;
    else
        throw bad_alloc();
}
void operator delete(void *mem) noexcept {
    free(mem);
}
````

### 19.1.2 定位（placement）new 表达式

operator new 和 operator delete 是库中的常规函数，意味着可以直接调用这些函数。在语言的早期版本，应用程序为了分离分配内存和初始化工作，会调用 operator new 和 operator delete，这与现在的 allocator 中的 allocate 和 deallocate 成员函数的效果是一致的，它们只负责分配和回收内存。

与 allocator 不同的，早期版本无法直接调用构造函数对内存进行对象构造，相反，我们必须使用定位 new 的方式进行构造对象。定位 new 将提供额外的地址信息，如：
````cpp
new (place_address) type; (1)
new (place_address) type (initializers); (2)
new (place_address) type [size]; (3)
new (place_address) type [size] { braced initializer list } (4)
````
其中 `place_address` 就是希望在此处构建的内存地址。当我们使用以上形式时，调用的就是 `operator new(size_t, void*)` 的函数，将返回我们给出的指针实参，这个函数是编译器不允许我们重载的。定位 new 的工作就是在我们指定的地方进行对象初始化。

传递给 placement new 的指针可以不是之前 operator new 分配的内存的指针，甚至可以不是动态内存的指针。

**显式调用析构函数**

虽然不能直接调用构造函数，但是可以直接调用析构函数，与调用任何其它的成员函数一样的方式去调用析构函数。如：
````cpp
string *sp = new string("a values");
sp->~string();
````
与调用 allocator.destroy 一样，析构函数将清理对象但是不会释放其内存，我们可以重用此内存。

## 19.2 运行时类型识别

运行时类型识别（Runtime type identification, RTTI）通过两个操作符提供：1. typeid 操作符返回给定表达式的类型；2. `dynamic_cast` 操作符将一个基类指针或者引用转为派生类的指针或引用；

在有些时候我们想通过基类指针或引用去调用派生类的函数是不可能的，原因在于我们无法定义这个虚函数。如果不能使用虚函数，我们可以使用 RTTI 操作符。另一方面，使用这些操作符将比使用虚成员函数更加易错：程序员必须知道对象的真实类型，并且必须检查转型是否成功。

RTTI 应该被限制使用在有限的范围内，更好的方式是定义虚函数而不是直接管理这些类型。

### 19.2.1 `dynamic_cast` 操作符

动态转型具有如下形式：
````cpp
dynamic_cast<type*>(e) // (1)
dynamic_cast<type&>(e) // (2)
dynamic_cast<type&&>(e) // (3)
````
其中 type 必须是具有虚函数的类类型。(1) 中 e 必须是有效的指针；(2) 中 e 必须是左值；(3) 中 e 必须不是左值；

在以上所有情形中，e 必须是 type 的共有派生子类、共有基类或者与 type 一致的类型。只有这样才能转型成功，否则转型就会失败。如果动态转型为指针类型失败结果将是 0，如果动态转型为引用类型失败，结果将会抛出 `bad_cast` 异常。

**指针类型动态转换**

常用的形式如：
````cpp
if (Derived *dp = dynamic_cast<Derived*>(bp))
{
} else {  // bp 指向一个真正的基类对象
}
````
如果 bp 指向一个派生对象，那么 dp 将会初始化为指向bp 所指向的 Derived 类型的对象的指针，那么使用 Derived 的操作就是正常的。否则，dp 的值将是 0，此时 if 条件将会失败。

可以在空指针上执行 `dynamic_cast` ，其结果是一个空指针。

**引用类型的动态转换**

如果对引用类型进行 `dynamic_cast` 时，其错误处理方式将会不一样，因为这种方式的转换在无法完成时会抛出异常。如：
````cpp
void f(const Base &b)
{
    try {
        const Derived &d = dynamic_cast<const Derived&>(b);
    } catch (bad_cast) {}
}
````

### 19.2.2 typeid 操作符

RTTI 提供第二个操作符是 typeid 操作符，其可以知道对象的类型是什么。typeid 表达式以 typeid(e) 的形式存在，其中 e 是任何表达式或者类型名字，结构是 `type_info` 库类或者其公有派生类的一个常量对象引用。`type_info` 类被定义在 typeinfo 头文件中。

typeid 操作符可以被运用于任何表达式，顶层 const 将会被忽略，如果表达式是引用那么 typeid 将返回其绑定的对象类型。当表达式是数组或函数时，其不会被转为指针，结果将是数组类型或函数类型。

如果操作数不是类类型或者是一个没有虚函数的类，那么结果是操作数的静态类型。当操作数是一个至少有一个虚函数的类类型左值，那么类型将在运行时被求值。

**使用 typeid 操作符**

通常使用 typeid 来比较两个表达式的类型或者将一个表达式的类型与特定的类型进行比较。如：
````cpp
Derived *dp = new Derived;
Base *bp = dp;
if (typeid(*bp) == typeid(*dp)) {
// bp 和 dp 指向相同类型的对象
}
if (typeid(*bp) == typeid(Derived)) {
// bp 实际指向一个 Derived
}
````
注意这里 typeid 的参数不是指针本身，如果是指针的话将在编译期返回指针的静态类型。

仅当类型具有虚函数时，才需要 typeid 在运行时求得其动态类型，而此时将必须对表达式求值。如果类型没有虚函数，那么 typeid 将在编译期返回表达式的静态类型；编译器不需要对表达式求值就可以知道其静态类型。这使得 `typeid(*p)` 如果 p 所指向的类型没有虚函数的话，p 可以不是有效的指针（如空指针）。

### 19.2.3 使用 RTTI

使用 RTTI 的一个场景是定义 equal 函数，如果使用在基类中将 equal 函数定义为虚函数，那么其参数将不得不是基类类型，导致函数将无法使用派生类中的特有成员。而且 equal 函数应该是类型不同时返回 false。所以我们的 equal 函数定义如下：
````cpp
class Base {
    friend bool operator==(const Base&, const Base&);
protected:
    virtual bool equal(const Base&) const;
};
class Derived : public Base {
protected:
    bool equal(const Base&) const;
};

// 相等操作符
bool operator==(const Base &lhs, const Base &rhs)
{
    return typeid(lhs) == typeid(rhs) && lhs.equal(rhs);
}
bool Derived::equal(const Base &rhs) const
{
    auto r = dynamic_cast<const Derived&>(rhs);
    // do the work to compare two Derived objects
}
bool Base::equal(const Base &rhs) const
{
    // do whatever is required to compare to Base objects
}
````

### 19.2.4 `type_info` 类

`type_info` 类的精确定义在不同的编译器实现之间有所不同，但是标准保证这个类定义在 typeinfo 头文件中，并且提供如下函数：

- `t1 == t2` 当 t1 和 t2 是 `type_info` 类型对象且表示相同的类型时返回 true，否则返回 false；
- `t1 != t2` 与上一条款相反；
- `t.name()` 返回类型名字的可打印 C 风格字符串，类型名字是与系统相关的；
- `t1.before(t2)` 当类型 t1 比 t2 出现的早时返回 true，顺序是编译器相关的；

## 19.3 枚举

## 19.4 类成员指针

### 19.4.1 指向数据成员的指针
### 19.4.2 指向成员函数的指针
### 19.4.3 将成员函数用作可调用对象

## 19.5 嵌套类

## 19.6 union : 空间节约型的类

## 19.7 本地类

## 19.8 固有的不可移植特性

### 19.8.1 位域（bit-fields）
### 19.8.2 volatile 限定符
### 19.8.3 链接指令：`extern "C"`

## 关键术语
