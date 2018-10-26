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

这个类提供了一个虚析构函数，因为其将作为一个基类。当编译期希望提供额外的信息，它通常会在 `type_info` 的派生类中完成。`type_info` 类没有默认构造函数，拷贝和移动构造函数以及赋值操作符都被定义为被删除的。所以只能通过 typeid 来获得 `type_info` 对象，而没有别的方式可以得到。

`type_info` 的 name 成员是由编译器决定的，可能不会与程序中使用的名字相匹配，只保证每个类型的名字是唯一的。如：[RTTI.cc](https://github.com/chuenlungwang/cppprimer-note/blob/master/code/RTTI.cc)

## 19.3 枚举

枚举（enumerations）可以将整数常量集合起来管理。与类一样，每个枚举定义一个新的类型。类是字面类型（literal types）。

C++ 支持两个枚举：带作用域的（scoped）和无作用域的（unscoped）。新标准引入了带作用域的枚举（scoped enumerations）。定义带作用域的枚举使用 enum class 或者 enum struct 关键字，后面跟着枚举名字和逗号分隔的一系列枚举值（enumerators）如：
````cpp
enum class open_modes { input, output, append };
````
如果省略掉 class 或 struct 关键字就是无作用域枚举（unscoped enumeration）。在无作用域 enum 中，枚举名字是可选的。如：
````cpp
enum color {red, yellow, green};
enum {floatPrec = 6, doublePrec = 10, double_doublePrec = 10};
````
如果枚举是没有名字的，那么只能在枚举定义处定义其类型的对象，即在结束的括号处添加逗号分隔的声明列表。

**枚举值（Enumerators）**

在带作用域的枚举中的枚举值的名字需要按照正常的作用域规则进行，并且在枚举作用域之外是不可访问的。无作用域枚举的枚举值放在了与枚举自身相同的作用域中。如：
````cpp
enum color {red, yellow, green};
enum stoplight {red, yellow, green}; // 错误：重复定义的枚举值
enum class peppers {red, yellow, green};

color eyes = green;
peppers p = green; // peppers 的枚举值不在作用域中
color hair = color::red; // 显式指定是可以的
peppers p2 = peppers::red;
````
默认情况下，枚举值从 0 开始，每个枚举值都比之前的那个大 1 ，我们可以给一个或多个枚举值提供初始值，如：
````cpp
enum class intTypes {
    charType = 8, shortType = 16, intType = 16,
    longType = 32, long_longType = 64
};
````
枚举值的值不需要是唯一的，shortType 和 intType 的值就是一样的。如果省略初始值，则其值比之前的枚举值多 1 。枚举值是常量，如果进行初始化，初始值必须是常量表达式。结果就是枚举值自己也是常量表达式，因而可以用于需要常量表达式的场景。如：
````cpp
constexpr intTypes charbits = intTypes::charType;
````
同样可以将枚举用于 switch 语句中，枚举值可以作为 case 标签。可以将枚举类型作为非类型模板参数（nontype template parameter），可以在类定义中初始化枚举类型的静态数据成员。

**与类一样，枚举定义新的类型**

只要枚举是由名字的，就可以定义和初始化这个类型的对象。枚举对象只能由其中一个枚举值或者相同枚举的另外一个对象初始化。如：
````cpp
open_modes om = 2; // 错误：2 不是 open_modes 类型
om = open_modes::input;
````
无作用域的枚举的枚举值及其对象可以自动转为整型值，所以它们可以用于任何需要整数值的场景。如：
````cpp
int i = color::red;
int j = peppers::red;
````

**Specifying the Size of an enum**

如果没有指定枚举值的类型，对于有作用域的枚举来说就是 int 类型，对于无作用域的枚举来说就是足够容纳所有的枚举值的。如果指定了枚举值的类型，那么超出范围将会编译失败。这种方式将保证程序的行为在跨系统时都是一致的。如：
````cpp
enum intValues : unsigned long long {
    charType = 255,
    shortType = 65535,
    intType = 65535,
    longType = 4294967295UL,
    long_longType = 18446744073709551615ULL
};
````

**枚举的前置声明**

在新标准下，可以前置声明枚举。枚举的前置声明必须指定枚举值的类型（显式或隐式）。如：
````cpp
enum intValues : unsigned long long; // 无作用域枚举，必须指定类型
enum class open_modes; // 带作用域的枚举默认是 int 类型
````
所有的枚举定义和声明都必须是完全一致的（枚举值的类型），特别是不能在一个上下文中声明为无作用域的枚举，在另外一个地方声明为带作用域的枚举。

**参数匹配和枚举**

接收枚举的函数不能使用具有相同的值的整型值。如：
````cpp
enum Tokens {INLINE = 128, VIRTUAL = 129};
void ff(Tokens);
void ff(int);
int main() {
    Tokens curTok = INLINE;
    ff(128); // ff(int)
    ff(INLINE); // ff(Tokens)
    ff(curTok); // ff(Tokens)
    return 0;
}
````
但是可以将枚举对象传递给接收整形值的函数，枚举将提升为 int 或者 long 等类型。如:
````cpp
void newf(unsigned char);
void newf(int);
unsigned char uc = VIRTUAL;
newf(VIRTUAL); // newf(int)
newf(uc); // newf(unsigned char)
````

## 19.4 类成员指针（Pointer to Class Member）

指向成员的指针（pointer to member）是一种可以指向类的非静态成员的指针。通常指针可以指向一个对象，但是指向成员的指针表示一个类的成员。静态成员则可以使用常规的指针来操作。

成员指针的类型同时具有类和类的成员的类型。只能将这种指针初始化为类的特定成员，而不指定这个成员属于哪个对象。当我们使用成员指针时，才提供在之上操作的对象。如：[class_member_pointer.cc](https://github.com/chuenlungwang/cppprimer-note/blob/master/code/class_member_pointer.cc)。

### 19.4.1 指向数据成员的指针

定义成员指针必须提供类的名字，如：`const string Screen::*pdata;` 定义 pdata 是“Screen 类的成员指针，其类型是 const string”。初始化的过程如：`pdata = &Screen::contents;` 此处将取地址符作用域类 Screen 的成员，而不是一个内存中的对象。

**使用数据成员指针**

理解当初始化或者赋值成员指针时，其并没有指向任何数据。它表示一个特定的成员，但没有成员所在的对象的信息。当我们解引用成员指针时需要提供对象。成员指针访问符有两个：`.*` 和 `->*` 。如：
````cpp
Screen myScreen, *pScreen = &myScreen;
auto s = myScreen.*pdata;
s = pScreen->*pdata;
````

**函数返回数据成员指针**

对成员指针一样运用常规的访问控制，private 的成员只能在类成员内部或者友元中使用。由于数据成员绝大部分都是私有的，所以应该定义一个共有成员函数返回数据成员指针。如：
````cpp
class Screen {
public:
    static const std::string Screen::*data()
    {
        return &Screen::contents;
    }
};
````
当调用时返回一个成员指针，如：
````cpp
const string Screen::*pdata = Screen::data();
````
pdata 仅仅只是指向类 Screen 的一个成员而不是真正的数据。为了使用 pdata，必须将一个对象绑定到成员指针上。如：
````cpp
auto s = myScreen.*pdata;
````

### 19.4.2 指向成员函数的指针

成员函数指针的定义要加上 `classname::*` 以及正常的函数指针的说明（返回类型与参数列表），如果成员函数是 const 成员或引用成员，必须在成员指针上体现出来。如：
````cpp
char (Screen::*pmf2)(Screen::pos, Screen::pos) const;
pmf2 = &Screen::get;
````
`Screen::*pmf2` 外围的括号是必须的，问题出现在优先级上。没有括号，将被编译器认为是无效的函数声明。

与常规的函数指针不同的是，成员函数和成员指针之间没有直接转换。如：`pmf = Screen::get;` 就是错误的。

**使用成员函数指针**

与使用数据成员一样，使用 `.*` 或 `->*` 操作符来通过成员函数指针调用成员函数。如：
````cpp
Screen myScreen, *pScreen = &myScreen;
char c1 = (pScreen->*pmf)();
char c2 = (myScreen.*pmf2)(0, 0);
````
这里使用括号的原因在函数调用符的优先级比成员指针访问符的优先级要高。

由于函数调用的优先级较高，所以声明和调用成员函数指针都需要加上括号，`(C::*p)(params)` 和 `(obj.*p)(args)`。

**成员指针的类型别名**

成员指针的类型别名如：
````cpp
using Action =
char (Screen::*)(Screen::pos, Screen::pos) const;

Action get = &Screen::get;
````
此处 Action 是“指向类 Screen 的成员函数指针，此成员函数是接收两个 pos 类型的参数返回 char 类型返回值的 const 成员”的别名。

与别的函数指针一样，可以将成员函数指针类型作为返回类型或者参数类型，并且这种类型的参数可以有默认实参，如：
````cpp
Screen& action(Screen&, Action = &Screen::get);
````

**成员函数指针的表（Tables）**

参考：[function_table.cc](https://github.com/chuenlungwang/cppprimer-note/blob/master/code/function_table.cc)

### 19.4.3 将成员函数用作可调用对象

与常规的函数指针不一样的是，成员指针并不是一个可调用对象，这种指针并不支持函数调用操作符。这样就不能将其传递给算法函数了。如：
````cpp
auto fp = &string::empty;
// 错误：.* 或 ->* 才能调用成员函数指针
find_if(svec.begin(), svec.end(), fp);
````
`find_if` 期待一个可调用对象，但是 fp 并不是。

**使用函数来产生可调用对象**

一种从成员函数指针中获取可调用对象的方式是使用 function 模板。如：
````cpp
function<bool (const string&) fcn = &string::empty;
find_if(svec.begin(), svec.end(), fcn);
````
通常，成员函数所在的对象是通过隐式参数 this 传递的，当用 function 来为成员函数产生可调用对象时，this 隐式参数转为显式参数。当 function 对象包含一个成员函数指针时，它会使用成员指针访问符（`.*` 和 `->*`）来对传入的对象进行成员函数指针调用。

当定义 functin 对象时，必须指定函数的签名，这个签名的第一个参数必须是成员函数所在的对象的类型（在之上函数将会执行），并且必须指出所在的对象类型是指针还是引用。如下面就将所在对象定义为了指针：
````cpp
vector<string*> pvec;
function<bool (const string*)> fp = &string::empty;
find_if(pvec.begin(), pvec.end(), fp);
````

**使用`mem_fn`产生可调用对象**

`mem_fn` 可以在不提供函数签名的情况下生成一个可调用对象，这个函数也定义在 functional 头文件中。如：
````cpp
find_if(svec.begin(), svec.end(), mem_fn(&string::empty));
````
由 `mem_fn` 生成的可调用对象可以在指针或对象上调用，而不必显式指出来，如：
````cpp
auto f = mem_fn(&string::empty);
f(*svec.begin()); // 使用 obj.*f()
f(&svec[0]); // 使用 ptr->*f()
````
可以认为 `mem_fn` 生成了一个重载了的调用操作符的可调用对象，其中一个以指针为参数，另一个以对象引用为参数。

**使用 bind 生成可调用对象**

bind 也可以生成一个可调用对象，如：
````cpp
bind(&string::empty, _1)
````
与 `mem_fn` 一样，不需要指定所在的对象是指针还是引用，但需要显式使用占位符告知所在对象在第一个参数的位置。

## 19.5 嵌套类

一个类可以定义在另外一个类中，这样的类成为嵌套类（nested class），或者叫嵌套类型（nested type）。嵌套类最常用于定义实现类。

嵌套类与其外围类是没有关系的，嵌套类型的对象没有外围类中定义的成员，反之亦然。

嵌套类的名字在外围类是可见的，但是外部就不可见了（如果处于外围类的 private 控制下，在 public 的控制下依然是可见的）。

外围类对于嵌套类没有特殊的访问权限，嵌套类对于外围类也没有特殊的访问权限。嵌套类在外围类中定义一个类型成员（type member）。定义在 public 部分中可以被用于任何地方，定义 protected 中则只能被外围类自身、友元和派生类使用，定义在 private 中则只能被外围类自身和友元访问。

**在外围类外部定义嵌套类**

嵌套类必须在外围类的内部声明，但是定义可以放在外围类的外部。当在外围类的外部定义嵌套类时，必须同时用外围类名和嵌套类名进行限定。如：
````cpp
class TextQuery {
public:
    class QueryResult;
};
class TextQuery::QueryResult {
    friend std::ostream&
        print(std::ostream&, const QueryResult&);
public:
    QueryResult(std::string,
        std::shared_ptr<std::set<line_no>>,
        std::shared_ptr<std::vector<std::string>>);
};
TextQuery::QueryResult::QueryResult(string s,
        shared_ptr<set<line_no>> p,
        shared_ptr<vector<string>> f):
    sought(s), lines(p), file(f) { }
````
这里 QueryResult 的构造函数也不是定义在类体内，必须将构造函数用外围类和嵌套类名进行限定。

**嵌套类的静态成员定义**

在类外定义如下：
````cpp
int TextQuery::QueryResult::static_mem = 1024;
````

**嵌套类作用域中的名称查找**

正常的名称查找规则运用于嵌套类。当然，嵌套类由一个额外的外围类作用域可供搜索。嵌套类是外围类的一个类型成员，外围类的成员可以不加限制的使用嵌套类的名字。参考代码：[TextQuery.cc/TextQuery.h](https://github.com/chuenlungwang/cppprimer-note/blob/master/code/TextQuery.cc)

返回值类型需要加以限定 `TextQuery::QueryResult` ，在函数体内则可以直接引用 `QueryResult`。

**嵌套和外围类是独立的**

尽管嵌套类定义在外围类的内部，必须理解的是嵌套类对象与外围类对象之间没有必然的联系。嵌套类对象只包含它自己定义的成员，外围类对象也只包含它自己定义的成员，它不能直接访问嵌套类中的数据成员。

## 19.6 union : 空间节约型的类

union 是一种特殊类型的类。union 可以由多个数据成员，但是在任何一个时间点，只有其中之一的成员是有值的。当 union 的一个成员被赋予值之后，其它所有成员都将是相同的底层二进制，至于如何对这些二进制进行解释则有其它成员本身的类型决定。union 需要的内存大小由最大的数据成员决定，内存大小将足够容纳这个数据类型。

与类一样，union 定义一种新的类型。

union 中的数据成员不能是引用，在 C++ 的早期版本中，数据成员的类型只能是内置类型，现在在新版本的 C++ 中可以是有构造函数和析构函数的类类型。

union 可以设置访问权限标签 public 、private 和 protected，默认情况下 union 是 public 的，这与 struct 是一样的。

union 可以定义成员函数，包括构造函数和析构函数，但是 union 不能继承别的类，也不能作为基类，union 也不能有虚函数。

**定义 union**

union 的定义以 union 关键字开始，后跟随可选的名字，以及一系列在括号中的成员声明。如：
````cpp
union Token {
    char cval;
    int ival;
    double dval;
};
````

**使用 union 类型**

与内置类型一样，默认情况下 union 是不初始化的。用初始化聚合类（aggregate class）一样的方法来初始化 union，如：`Token token = {'a'};` 其中 a 用来初始化第一个成员 cval。union 的成员通过常规的成员访问符进行访问，如：
````cpp
last_token.cval = 'z';
pt->ival = 42;
````
赋值给 union 对象的数据成员将导致别的数据成员的内容是未定义的。

**匿名 union**

匿名 union 是没有名字也没有定义对象的 union，当定义匿名 union 时，编译器自动创建一个匿名对象。如：
````cpp
// Defines an unnamed object, whose members we can access directly
union {
    char cval;
    int ival;
    double dval;
};
cval = 'c';
ival = 42;
````
匿名 union 的成员可以在定义这个匿名 union 的作用域中直接访问。匿名 union 不能有 private 或者 protected 成员，也不能定义成员函数。

**union中有类类型成员**

新标准中允许定义有构造函数和拷贝控制成员的类类型成员，但是使用这种 union 将比只有内置类型成员的 union 要更加复杂。只有内置类型成员的 union 只需要简单的赋值就可以替换其成员的值，对于类类型成员则需要显式地构造和析构了。

当 union 只有内置类型成员时，编译器可以合成默认

## 19.7 本地类

## 19.8 固有的不可移植特性

### 19.8.1 位域（bit-fields）
### 19.8.2 volatile 限定符
### 19.8.3 链接指令：`extern "C"`

## 关键术语
