面向对象（OOP）和泛型编程（generic programming）都是处理在书写程序时未知的类型，所不同的是 OOP 处理直到运行时才知道的类型，而泛型编程则处理知道编译时才知道的类型。

当书写泛型程序时，写出来的代码与特定类型是独立的。当使用泛型程序时则需要提供类型或者值（作为泛型实参）给泛型代码。

模板（Template）是泛型编程的基石，使用模板并不需要太多关于模板的知识。在本章将学习如何定义自己的模板。

模板是 C++ 中的泛型编程的基石。模板是编译器生成类和函数的蓝本（blueprint）或公式（formula），当使用泛型类型（generic type）或者泛型函数（generic function）时，必须提供必要的信息从而将蓝本转换为特定的类或函数，转换过程发生在编译期间。

## 16.1 定义模板

为每个类型都定义相同的操作是十分繁琐的，而且需要知道所有操作的类型，更合理的方式是定义函数模板，然后在使用时提供类型即可。参考代码：[template.cc](https://github.com/chuenlungwang/cppprimer-note/blob/master/code/template.cc)。

### 16.1.1 函数模板

相比于为每个类型定义一个新的函数，可以定义函数模板（function template）。函数模板是合成特定类型版本的蓝本。如：
````cpp
template <typename T>
int compare(const T &v1, const T &v2)
{
    if (std::less<T>()(v1, v2)) return -1;
    if (std::less<T>()(v2, v1)) return 1;
    return 0;
}
````
模板定义从关键字 template 开始，后面跟着模板参数列表（template parameter list），这是放在尖括号中的一个逗号分隔的一个或多个模板参数（template parameters）。

在模板定义中，模板参数列表不能是空的。

模板参数列表与函数参数列表类似，模板参数表示在类或函数定义中使用到类型或值。当使用模板时，通过隐式或显式的方式将模板实参（template argument）绑定到模板参数。

以上的 compare 函数声明了一个类型参数 T，在 compare 内部，使用 T 来表示一个类型，T 真正表示什么类型将在编译期间由其是如何使用决定的。

**实例化函数模板**

当调用函数模板时，编译器使用调用实参来推断模板实参。编译器使用实参的类型来决定绑定到模板参数 T 上的类型。通常函数模板是通过推断得到的，不需要显式提供，而且可能推断出来的类型与调用实参的类型并不一样。如：
````cpp
cout << compare(1, 0) << endl;
````
这里实参的类型是 int，编译器将 int 推断为模板实参，并且将此实参绑定到模板参数 T 上。编译器使用推断的模板参数来实例化（instantiate）一个特定版本的函数。当编译器实例化一个模板时，乃是使用模板实参替换对应的模板参数来创建一个模板的新“实例”。如还可以做一下调用：
````cpp
vector<int> vec1{1,2,3}, vec2{4,5,6};
cout << compare(vec1, vec2) << endl;
````
将实例化一个版本，其中 T 被 `vector<int>` 替换。这些编译器生成的函数被统称为模板的实例（instantiation）。

**模板类型参数**

compare 函数具有一个模板类型参数（template type parameter），通常，可以将类型参数当作一个类型说明符（type specifier）来使用，这与使用内置类型或类类型是一样的。特别是，类型参数可以被使用于返回类型或函数参数类型，或者变量声明以及强转时的类型。如：
````cpp
template <typename T> T foo(T* p)
{
    T tmp = *p;
    return tmp;
}
````
每个类型参数都可以被放在关键字 class 或 typename 之后。class 与 typename 在这种情况下是完全一样的，且可以互换。而使用 typname 关键字是一个更加直观的方式，毕竟可以使用内置类型作为模板类型实参，而且 typename 更加清晰地告知后面跟随的名字是类型名字。然而，当 typename 被添加到 C++ 语言时，模板已经被广泛运用了；一些程序员依然在继续使用 class 关键字。

**非类型模板参数**

除了可以定义类型参数，还是可以定义带有非类型参数（nontype parameters）的模板。非类型参数表示一个值而不是类型。非类型参数通过使用特定类型名字而不是 class 或 typename 来指定。当模板被实例化时，非类型参数将被一个用户提供的值或者编译器推断的值替代。这些值必须是常量表达式（constant expressions），只有这样编译器才能在编译期间实例化模板。如：
````cpp
template <unsigned N, unsigned M>
int compare(const char (&p1)[N], const char (&p2)[M])
{
    cout << N << std::endl;
    cout << M << std::endl;
    return strcmp(p1, p2);
}
compare("hi", "mom");
````
编译器将会使用字面量的长度替换 N 和 M 非类型参数来实例一个模板版本。上面将被实例化为：
````cpp
int compare(const char (&p1)[3], const char (&p2)[4]);
````
一个非类型参数可能是整数类型、函数或对象的指针或左值引用。绑定到整型的实参必须是常量表达式；绑定到指针或引用的实参必须具有静态生命周期（static lifetime），不能使用常规本地对象或动态对象的地址或引用作为实参，指针参数还可以被实例化为 nullptr 或零值常量表达式；

模板的非类型参数在模板定义中是一个常量值。一个非类型参数可以在需要常量表达式的时候使用，如：作为数组的长度。

提供给非类型模板参数的模板实参必须是常量表达式。

**内联（inline）和 constexpr 函数模板**

函数模板可以与常规函数一样被声明为 inline 或 constexpr 的，inline 或 constexpr 关键字放在模板参数列表后，在返回类型前：
````cpp
template <typename T> inline T min(const T &, const T &);
````

**书写类型无关（Type-Independent）的代码**

如 compare 函数中使用 `const T &` 作为参数，那么可以传递任何类型的参数进去，扩大了函数的使用范围。而内部都使用 `less<T>` 可调用对象，less 对象可以处理好指针不是指向同一个数组的情况，若用 `<` 得到的结果将是未定义的（undefined）。

模板程序应该尽可能的减少对实参类型的要求。

**模板编译**

编译器并不是遇到模板定义时生成代码，而是在实例化特定版本的模板时生成代码。当使用模板时才生成代码将影响到如何组织源代码和错误何时被发现。

通常，调用函数时编译器只要看到函数的声明即可。使用类对象时，编译器需要知道类的定义（class definition）和成员函数的声明，而不需要成员函数的定义，因而，将类定义和函数声明放在头文件中，而函数定义或类成员函数定义则放在源文件中。

不过模板却有不同：为了生成实例，编译器需要知道函数模板的定义以及类模板成员函数（class template member function）的定义。因而，与非模板代码不同，模板代码的头文件中通常包含声明和定义。

函数模板和类模板的成员函数定义通常放在头文件中。

**关键概念：模板和头文件**

模板包含两种类型的名字：与模板参数无关的、与模板参数有关的。

模板提供者需要保证所有与模板参数无关的名字在模板被使用时是可见的。另外，模板提供者必须保证模板的定义（包括类模板成员定义）在模板被实例化时是可见的；

保证声明所有与实例化模板的类型参数相关的函数、类型和操作符是可见的，如：模板类型参数的成员函数、操作符重载以及内部定义的类型。

模板提供者应该将所有模板定义和定义模板时用到的名字声明都放在头文件中。模板的用户使用时必须包含模板头文件以及用于实例化模板的类型的头文件。

**编译错误大部分在实例化期间发现**

代码直到模板被实例化时才生成影响了何时才能知道模板代码中的编译错误。通常，有三个阶段编译器可能会发现错误。

第一，当编译器模板本身时，可以发现一些语法错误；
第二，当编译器发现模板被使用时，会检查是否参数数目与模板定义一致；
第三，与第二个阶段几乎是在一起发生的，在实例化期间，可以发现类型相关的错误（不存在的操作），取决于编译器如何管理实例，这些错误可能在链接期间被发现。

当书写模板时，不应该让类型过度具体（限制过多），但模板通常都会对其使用的类型做出一些假设。

将由调用者保证传给模板的实参符合模板对类型的要求，即所有的操作都是存在的，并且表现的是符合要求的。

### 16.1.2 类模板

类模板（class template）是合成类的蓝本。与函数模板不同的是编译器不能推断出类模板的模板参数的类型。相反，使用类模板时必须提供额外的信息，这些信息将放在类模板名后的尖括号中。这些额外信息是模板实参列表（template arguments list），用于替换模板参数列表（template parameters list）。

**定义类模板**

参考代码：[template_Blob.cc](https://github.com/chuenlungwang/cppprimer-note/blob/master/code/template_Blob.cc)

与函数模板一样，类模板以关键字 template 跟随模板参数列表。在类模板的定义中（包括成员定义），可以如使用常规类型或值一样使用模板参数，这些模板参数将在使用时被提供实参。

如在 Blob 类模板中，返回值类型是 T&&，当实例化 Blob 时，T 将被替换为指定的模板实参类型。

**实例化类模板**

当使用类模板时，必须提供额外的信息，这个额外信息就是显式模板实参（explicit template arguments），它们将被绑定到模板的参数上。编译器使用这些模板实参来实例化从模板中实例化一个特定的类。如：
````cpp
Blob<int> ia;
Blob<int> ia2 = {0,1,2,3,4};
````
编译器为每一个不同的元素类型生成不同的类，如：
````cpp
Blob<string> names;
Blob<double> prices;
````
以上将实例化两个完全不同的类：以 string 为元素的 Blob 和以 double 为元素的 Blob。类模板的每个实例都构成完全独立的类。类型 `Blob<string>` 和其它的 Blob 类型之间没有任何关系，亦没有任何特殊的访问权限。

**在模板作用域中实例化一个模板类型**

为了便于阅读模板的类代码，谨记类模板名字不是类型的名字。一个类模板被用于实例化类型，而实例化的类型总是包含模板的实参。可能会令人疑惑的是，在类模板中通常不使用真实存在的类型或值作为模板的实参，而是使用模板自己的参数作为模板实参。比如：Blob 中的 data 被定义为 `std::shared_ptr<std::vector<T>> data;` 便是这个道理。当实例化 `Blob<int>` 时，data 将被实例化为 `shared_ptr<vector<int>>`。

**类模板的成员函数**

与常规函数一样，可以给类模板在类体内或类体外定义成员函数。同样，定义在类体内的成员函数是隐式内联的。类模板的成员函数本身是常规函数。然而，类模板的每个实例都有自己的成员版本。因而，每个类模板的成员函数具有与类模板一样的模板参数。因而，在类模板外定义的成员函数以关键字 template 跟随类模板参数列表开始。并且，在类外定义成员函数必须说明其属于哪个类。而且这个类名由于是从模板实例化而来的，需要包含模板实参。当定义成员函数时，模板的实参与模板的参数一样。因而整个形式如下：
````
template <typename T>
ret-type Blob<T>::member-name(parm-list)
````
具体例子：
````cpp
template <typename T>
void Blob<T>::check(size_type i, const std::string &msg) const
{
    if (i >= data->size())
        throw std::out_of_range(msg);
}
````
与其它定义在类模板外的成员函数一样，构造函数也是先声明类模板的模板参数。如：
````cpp
template <typename T>
Blob<T>::Blob(): data(std::make_shared<std::vector<T>()) { }
````
此处亦是使用类模板自己的类型参数作为 vecotr 的模板实参。

**实例化类模板成员函数**

默认情况下，类模板的成员函数只有在程序使用模板函数时才会实例化。如果一个成员函数没有用到，那么就不会被实例化。成员只有在使用到时才会实例化的事实，使得我们可以实例化一个类，其使用到的类型实参只符合模板操作的部分要求。通常，一个类模板实例化类的成员只有在使用时才会被实例化。

**简化在类代码中使用模板类名（Template Class Name）**

在类模板自身的作用域中，可以使用模板名而不需要实参。当在类模板的作用域内，编译器认为使用模板自己的地方就像是指定了模板实参为模板自己的参数。如：
````cpp
template <typename T> class BlobPtr {
public:
    BlobPtr& operator++();
};
````
与以下代码是一样的：`BlobPtr<T>& operator++();`

**在类模板体外使用类模板名字**

当在类模板体外定义成员时，必须记住直到看到类名时才处于类地作用域中。如：
````cpp
template <typename T>
BlobPtr<T> BlobPtr<T>::operator++(int)
{
    BlobPtr ret = *this;
    ++*this;
    return ret;
}
````
由于返回类型出现在类作用域的外面，所有必须告知返回类型是 BlobPtr 以其类型参数为实参的实例。在函数体内部，我们处于类的作用域中，所以在定义 ret 时不再需要重复模板实参，当不再提供模板实参，编译器认为我们使用与成员实例一样的类型实参。

**类型模板和友元**

当类定义中包含友元声明时，类和友元可以相互不影响的时模板或者不是模板。类模板可以有非模板的友元，授权友元访问其所有的模板实例。如果友元自身是模板，授权友元的类控制访问权限是授给模板的所有实例还是给特定的实例。

**一对一友元**

最常见的友元形式就是一个类模板与另一个模板（类或函数）的对应实例之间建立友元关系。如：Blob 类模板和 BlobPtr 类模板之间的友元关系，以及相等性判断（==）操作符之间的关系。代码如下：
````cpp
template <typename> class BlobPtr;
template <typename> class Blob;
template <typename T>
bool operator==(const Blob<T>&, const Blob<T>&);
template <typename T> class Blob {
friend class BlobPtr<T>;
friend bool operator==<T>(const Blob<T>&, const Blob<T>&);
};
````
为了指定模板（类或函数）的特定实例，我们必须首先声明模板本身。模板的声明包括模板的模板参数列表。

友元声明使用 Blob 的模板参数作为它们的模板实参。因而，这种友元被严格限定在具有相同类型的模板实参的 BlobPtr 和相等操作符的实例之间。如：
````cpp
Blob<char> ca; //BlobPtr<char> and operator==<char> are friends
Blob<int> ia; //BlobPtr<int> and operator==<int> are friends
````
`BlobPtr<char>` 的成员可以访问 ca 的非共有部分，但 ca 与 ia 之间没有任何特殊的访问权限。

**通用和特例（Specific）的模板友元**

一个类可以让另一个模板的所有实例都是其友元，或者将友元限定在某一个特定的实例。如：
````cpp
template <typename T> class Pal;
class C {
friend class Pal<C>; //Pal instantiated with class C is a friend to C
//all instances of Pal2 are friends to C;
//no forward declaration required when we befriend all instantiations
template <typename T> friend class Pal2;
};
template <typename T> class C2 {
//each instantiation of C2 has the same instance of Pal as a friend
friend class Pal<T>; //a template declaration for Pal must be in scope

//all instances of Pal2 are friends of each instance of C2,
//prior declaration is not needed
template <typename X> friend class Pal2;

//Pal3 is a nontemplate class that is a friend of every instance of C2
//prior declaration for Pal3 is not needed
friend class Pal3;
};
````
为了让所有的实例都是友元，友元的声明中所使用的模板参数必须与类模板所使用的不一样，就像这里的 C2 中的 T 和 Pal2 中的 X 一样。

**与模板本身的类型参数成为友元**

在新标准中，可以使得模板的类型参数成为友元：
````cpp
template <typename Type> class Bar {
friend Type;
};
````
这里指明所有用于实例化 Bar 的任何类型都是 Bar 的友元。这里需要指出的是尽管一个友元通常是一个类或函数，但是 Bar 也可以用内置类型进行实例化。这种友元关系是允许的，这样才可以将 Bar 用内置类型进行实例化。

**模板的类型别名**

类模板的一个实例就是一个类类型，与任何别的类类型一样，可以定义一个 typedef 来作为实例化类的别名。如：
````cpp
typedef Blob<string> StrBlob;
````
由于模板不是类型，所以不能定义 typedef 作为模板的别名。也就是说不能定义 typedef 来指向 `Blob<T>`。

然而，在新标准下可以用 using 声明来指定类模板的别名。如：
````cpp
template <typename T> using twin = pair<T, T>;
twin<string> authors; //authors is a pair<string, string>
````
模板类型别名是一族类的别名。当定义模板类型别名时，可以固定一个或多个模板参数，如：
````cpp
template <typename T> using partNo = pair<T, unsigned>;
partNo<string> books;
partNo<Vehicle> cars;
partNo<Student> kids;
````
这里将 partNo 定义为一族 pair 类，其中第二个成员是 unsigned，partNo 的使用者只能指定 pair 的第一个成员，却不能对第二个成员做出选择。

**类模板的静态成员**

与其它类一样，类模板可以声明静态成员（static members）。如：
````cpp
template <typename T> class Foo {
public:
    static std::size_t count() { return ctr; }
private:
    static std::size_t ctr;
};
````
Foo 的每个类实例都有自己的静态成员实例。也就是说对于每个给定类型 X，都有一个  `Foo<X>::ctr` 和一个 `Foo<X>::count` 成员，而 `Foo<X>` 的所有对象都共享相同的 ctr 对象和 count 函数。

与任何别的 static 数据成员一样，每个类实例必须只有一个 static 数据成员的定义。然而，每个类模板的实例有一个完全不一样的对象，因而，在定义静态数据成员时与在类外定义成员函数类似。如：
````cpp
template <typename T>
size_t Foo<T>::ctr = 0;
````
与类模板的任何成员类似，定义的开始部分是模板参数列表，后跟随成员的的类型，然后是成员名字。成员名字中包含成员的类名，而此时的类名是从一个模板中生成而来的，所以包含模板的实参。因而，当 Foo 为每个特定义的模板实参进行实例化时，一个独立的 ctr 将为此类类型进行实例化并初始化为 0。

与非模板类的静态成员类似，可以通过类的对象或者使用作用域操作符对静态成员进行直接访问。当然，为了通过类使用一个静态成员，必须是一个特定的类实例才行。如：
````cpp
Foo<int> fi;  //实例化类 Foo<int> 和静态数据成员 ctr
auto ct = Foo<int>::count(); //实例化 Foo<int>::count 函数
ct = fi.count(); //使用 Foo<int>::count
ct = Foo::count(); //error: which template instantiation?
````
与任何别的成员函数类似，静态成员函数仅在被使用时实例化。

### 16.1.3 模板参数

与函数参数名字类似，模板参数名字没有本质的含义。通常将类型参数记作 T，但可以使用任何名字：
````cpp
template <typename Foo>
Foo calc(const Foo &a, const Foo &b)
{
    Foo temp = a;
    return temp;
}
````

**模板参数和作用域**

模板参数遵循常规的作用域规则。模板参数的名字可以在其声明之后使用，直到模板的声明或定义的尾部。与任何别的名字类似，模板参数隐藏任何外部作用域的相同名字的声明。与绝大多数上下文不一致的是，作为模板参数的名字不能在模板中复用。如：
````cpp
typedef double A;
template <typename A, typename B>
void f(A a, B b)
{
    A tmp = a; //tmp has same type as the template parameter A, not double
    double B; //error: redeclares template parameter B
}
````
常规的名称隐藏规则导致 A 的类型别名被类型参数 A 所隐藏。由于不能复用模板参数的名字，将 B 声明为变量名是一个错误。

由于模板参数名字不能被复用，模板参数名字只能在给定模板参数列表中出现一次。如：
````cpp
//error: illegal reuse of template parameter name V
template <typename V, typename V> //...
````

**模板声明**

模板声明必须包含模板的参数列表，如：
````cpp
//declares but does not define compare and Blob
template <typename T> int compare(const T &, const T &);
template <typename T> class Blob;
````
与函数参数一样，模板参数的名字不需要在声明和定义之间完全一样，如：
````cpp
//all three uses of calc refer to the same function template
template <typename T> T calc(const T &, const T &);
template <typename U> U calc(const U &, const U &);
template <typename Type>
Type calc(const Type &a, const Type &b) { /* ... */ }
````
以上三个用法都是表示同一个函数模板。当然，给定模板的所有声明和定义都必须具有相同数目和种类（类型或非类型）的参数。

最佳实践：被某个文件所需要的所有模板声明都应该放在文件的头部，它们最好放在一起，并且是在所有使用这些名字之前就声明。

**使用类的类型成员**

前面的章节描述过使用作用域操作符（::）来访问静态成员和类型成员（type member），在常规代码中，编译器是知道一个名字是类型成员或者是静态成员。然而，当遇到模板时，编译器很可能就无法知道这个信息了，如，给定 T 模板类型参数，当编译器看到 `T::mem` 时，它将直到实例化时才能直到 mem 是类型成员还是静态成员。然而，有时必须得让编译器知道一个名字表示类型才能正确编译。例如以下表达式：
````cpp
T::size_type * p;
````
编译器必须知道 size_type 是类型，这是在定义一个名字 p 的变量，不然，就不会被处理为静态数据成员 size_type 与变量 p 相乘。

默认情形下，语言认为通过作用域操作符访问的名字不是类型。如果要使用一个模板类型参数的类型成员，必须显式告知编译器这个名字是类型。那就得用 typename 这个关键字了。如：
````cpp
template <typename T>
typename T::value_type top(const T &c)
{
    if (!c.empty())
        return c.back();
    else
        return typename T::value_type();
}
````
以上函数期待一个容器作为其实参，使用 typename 类指定其返回类型，并且在没有元素的情况下生成一个值初始化的元素用于返回。

当想要告知编译器一个名字表示类型时，必须使用关键字 typename 而不是 class。

**默认模板实参**

与可以给函数参数提供默认实参一样，可以提供默认模板实参（default template arguments），在新标准下可以给函数和类模板提供默认实参。早期的语言版本只允许给类模板提供默认实参。如：
````cpp
template <typename T, typename F = less<T>>
int compare(const T &v1, const T &v2, F f = F())
{
    if (f(v1, v2)) return -1;
    if (f(v2, v1)) return 1;
    return 0;
}
````
这里同时提供了模板模板实参和模板函数实参。默认模板实参使用 less 函数对象的 T 类型实例，而默认函数参数告知 f 是 F 类型的默认初始化对象。使用 compare 时可以提供自己的比较操作，但不是必须这么做。如：
````cpp
bool i = compare(0, 42);
Sales_data item1(cin), item2(cin);
bool j = compare(item1, item2, compareIsbn);
````
当 compare 以三个实参进行调用时，第三个参数的必须是可调用对象，并且返回值可以转为 bool ，其参数类型必须与前两个参数的类型可以转换。与函数默认参数一样，模板参数的默认实参只有在其右侧的所有参数都具有默认实参时才是合法的。

**模板默认实参和类模板**

无论何时使用类模板，都必须在模板名之后跟随尖括号。尖括号表示类是从一个模板实例化而来的。特别是，如果一个类模板给所有模板参数都提供了默认实参，并且我们也希望使用这些默认值，还是必须得在模板名字后提供一个空的尖括号对。如：
````cpp
template <class T = int>
class Numbers {
public:
    Numbers(T v = 0):val(v) { }
private:
    T val;
};
Numbers<long double> lots_of_precision;
Numbers<> average_precision; //empty <> says we want the default type
````

### 16.1.4 成员模板

无论是常规的类还是类模板都可以有一个本身就是模板的成员函数，这种成员被称为成员模板（member templates），成员模板一定不能是虚函数。

**常规类的成员模板**

常规类中的成员模板与模板函数的写法完全一样。如：
````cpp
class DebugDelete {
public:
    DebugDelete(std::ostream &s = std::cerr):os(s) { }
    template <typename T>
    void operator()(T *p) const
    {
        os << "delete unique_ptr" << std::endl;
        delete p;
    }
private:
    std::ostream &os;
};
````
成员模板与别的模板一样，都是从自己的模板参数列表开始的。每个 DebugDelete 对象有自己的 ostream 成员，并且有一个成员函数本身是模板。用法如下：
````cpp
double *p = new double;
DebugDelete d;
d(p);
int *ip = new int;
DebugDelete()(ip);
````
也可以被用于构建 `unique_ptr` 对象。如：
````cpp
unique_ptr<int, DebugDelete> p(new int, DebugDelete());
unique_ptr<string, DebugDelete> sp(new string, DebugDelete());
````
以上当 `unique_ptr` 的析构函数被调用时，DebugDelete 的调用操作符将会被调用。因而，无论何时 `unique_ptr` 的析构函数被实例化，DebugDelete 的调用操作符将被实例化。

**类模板的成员模板**

可以给类模板定义成员模板，在这种情况下，类和成员的模板参数是各自独立的。如：
````cpp
template <typename T> class Blob {
    template <typename It> Blob(It b, It e);
};
````
此构造函数有其自己的模板类型参数 It 。不同于类模板的常规成员函数，成员模板是函数模板。当在类模板外部定义成员模板时，必须提供同时为类模板和函数模板提供模板参数列表。先提供类模板的参数列表，紧跟着成员模板自己的模板参数列表。如：
````cpp
template <typename T>
template <typename It>
Blob<T>::Blob(It b, It e):data(std::make_shared<std::vector<T>>(b, e)) { }
````

**初始化和成员模板**

为了实例化类模板的成员模板，必须同时给类和函数模板同时提供模板参数。与往常一样，类模板的实参必须显式提供，而成员模板的实参则从函数调用中推断出来。如：
````cpp
int ia[] = {0,1,2,3,4,5,6,7,8,9};
vector<long> vi = {0,1,2,3,4,5,6,7,8,9};
list<const char*> w = {"now", "is", "the", "time"};
Blob<int> a1(begin(ia), end(ia));
Blob<int> a2(vi.begin(), vi,end());
Blob<string> a3(w.begin(), w.end());
````
当定义 a1 时，显式告知编译器去实例化模板参数绑定到 int 的 Blob 版本。而其构造函数自己的类型参数则从 begin(ia) 中推断出来，在此例是 `int*`。因此，a1 的实例定义是：
````cpp
Blob<int>::Blob(int*, int*);
````

### 16.1.5 控制实例化

模板只有在使用时才会生成实例意味着同一个实例可能会出现在多个 obj 文件中。当两个或多个分离编译的源文件使用同一个模板且模板实参是一样的，那么在每个文件中都由一个此模板的相同实例。

在大的系统中，在多个文件中过度实例化同一个模板造成的影响将是很大的。在新标准中，可以通过显式实例化（explicit instantiation）来避免这个消耗。显式实例化的形式如下：
````cpp
extern template declaration; //instantiation declaration
template declaration; //instantiation definition
````
显式实例化的声明或定义之前，必须要能够看到模板体的代码。其中 declaration 是将所有的模板参数替换为模板实参，如：
````cpp
extern template class Blob<string>; //declaration
template int compare(const int &, const int &); //definition
````
当编译器看到 extern 模板声明时，它将不会生成在当前文件中生成实例代码。extern 声明意味着在程序的某个地方存在着一个非 extern 的实例，程序中可以有多个 extern 声明，但是只能有一个定义。

由于当使用模板时会自动实例化，所以 extern 声明必须出现在所有使用此实例的代码之前。

函数模板必须被显式实例化，而类模板不一定需要，编译器会隐式实例化类模板。这个特性可能是编译器自己的特性，所以不应依赖于此。最好是对于每个实例声明都对应一个显式地实例定义。

**实例定义实例化所有成员**

类模板的实例定义实例化模板的所有成员，包括内联成员函数。当编译器看到一个实例定义时，它无法直到到底哪个成员函数将会程序使用，因此，于常规的类模板实例化不同的是，编译器将实例化类的所有成员。即便不使用某个成员，其也必须实例化。结果就是，我们只能显式实例化所有成员都可以使用的模板实例。

### 16.1.6 效率和灵活性

智能指针类型提供了描述模板设计者的设计选择很好的材料。`shared_ptr` 可以在创建或 reset 指针时传递一个删除器（deleter）来轻松覆盖之前的。而 `unique_ptr` 的删除器却是类型的一部分，我们必须在定义 `unique_ptr` 就显式提供一个类型作为模板实参，因而，给 `unique_ptr` 定制删除器会更加复杂。 

删除器是如何被处理的与类的功能似乎并无本质上的关系，但这种实现策略却对性能有重大的影响。

**在运行时绑定删除器**

`shared_ptr` 的删除器是间接存储的，意味着可能作为指针或者一个包含指针的类，这是由于其删除器直到运行时才能被知道是何种类型，而且在其生命周期中还可以不断改变。一般来说，一个类的成员类型不会在运行时改变，所以此删除器必须是间接存储的。调用方式如：
````cpp
//value of del known only at runtime; call through a pointer
//del(p) requires runtime jump to del's location
del ? del(p) : delete p;
````

**在编译期绑定删除器**

由于删除器的类型是作为 `unique_ptr` 的类型参数指定的，意味着删除器的类型可以在编译期就知道，因而，此删除器可以被直接存储。调用方式如：
````cpp
//del bound at compile time;
//direct call to the deleter is instantiated
del(p); // no runtime overhead
````
这个方式的好处在于不论代码执行哪个类型的删除器，其都是就地执行，意味着不需要做任何跳转，甚至对于简单的函数可以内联到调用处。这是编译期绑定的功劳。

通过在编译期绑定删除器，`unique_ptr` 避免了调用删除器的运行时消耗；通过在运行时绑定删除器，`shared_ptr` 带来了灵活性，使其更容易定制新的删除器；

## 16.2 模板实参推断

默认情况下编译器使用调用实参来决定函数模板的模板参数。这个过程称为模板实参推断（template argument deduction），在推断过程中编译器使用调用的实参类型来选择哪个生成的函数版本是最合适的。

### 16.2.1 转换和模板类型参数

与常规函数一样，传递给函数的模板的实参被用于初始化函数的参数。类型是模板类型参数的函数参数有特殊的初始化规则。只有非常有限的几个转换是自动运用于这种实参的。相比于转换实参，编译器会生成一个新的实例。

与之前的描述一样，不论是参数还是实参中的顶层 const 都会被忽略。在函数模板中会执行的有限转换分别是：

- const 转换：如果一个函数参数是 const 的引用或指针，可以传递一个非 const 对象的引用或指针；
- 数组或函数至指针的转换：如果函数参数不是引用类型，那么指针转换将被运用于数组或函数类型。数组实参将被转为指向其首元素的指针，同样，函数实参将被自动转为指向函数类型的指针；

其它任何类型的转换（算术转换、子类到基类的转换、用户定义转换）都不会执行。如：
````cpp
template <typename T> T fobj(T, T);
template <typename T> T fref(const T &, const T &);
int a[10], b[42];
fobj(a, b);
fref(a, b);
````
在 fobj 调用中，数组的类型不一样是无所谓的。两个数组都转为了指针，fobj 中的模板参数类型是 `int*`，而调用 fref 却是非法的，当参数是引用时，数组不能自动转为指针，此时 a 和 b 的类型是不匹配的，因而调用是错误。

const 转换和数组或函数至指针的转换是模板类型中的实参到形参的唯一自动转换。

**使用相同模板参数类型的函数参数**

一个模板类型参数可以被用于多于一个的函数参数类型。由于只存在十分有限的转换，传递给这种参数的实参必须具有完全一样的类型，如果推断出来的类型不匹配，那么调用将会出错。如：`compare(lng, 1024);` 的调用将会出错，原因在于第一个参数被推断出来是 long，而第二个是 int，这并不匹配，所以模板实参推断就失败了。为了允许这种实参的常规转换，必须定义两个不同类型的模板参数。如：
````cpp
template <typename A, typename B>
int flexibleCompare(const A &v1, const B &v2)
{
    if (v1 < v2) return -1;
    if (v2 < v1) return 1;
    return 0;
}
````
此时用户可以提供两个不同类型的实参了。

**Normal Conversion Apply for Ordinary Arguments**

如果函数模板的参数的类型不是模板类型参数，即其是具体的类型，那么实参可以执行之前描述过的各种转换。如：
````cpp
template <typename T> ostream &print(ostream &os, const T &obj)
{
    return os << obj;
}
````
其中第一个参数是具体的类型 ostream&，所以，可以执行正常的类型转换。

### 16.2.2 函数模板显式实参

在一些情形下根本不可能让编译器推断出模板的实参。在另外一些情形下，则是我们自己想控制模板的实例化。两者绝大多数时候发生在函数的返回类型与任何参数列表中的类型都不一样时。

**指定显示模板实参**

通过定义额外的模板参数来表示返回值的类型，如：
````cpp
template <typename T1, typename T2, typename T3>
T1 sum(T2, T3);
````
在这种情况下编译器没有任何办法来推断 T1 的类型，调用必须在每次调用 sum 时为此模板参数提供显示模板实参（explicit template argument）。给函数模板提供显式模板实参与定义类模板实例是一样的，显式模板实参将被放在函数名之后的尖括号中，并且在实参列表之前。如：
````cpp
// T1 is explicitly specified; T2 and T3 are inferred from the argument types
auto val3 = sum<long, long>(i, lng);
````
此调用中显式指定了 T1 的类型，编译器将从 i 和 lng 的类型中推断出 T2 和 T3 的类型。显式模板实参是从模板参数列表中的左边向右边依次匹配的。显式模板实参只能省略尾部的参数，并且是在可以从函数实参中推断出来的那些参数。意味着如下函数必须每次都提供所有的模板实参：
````cpp
template <typename T1, typename T2, typename T3>
T3 alternative_sum(T2, T1);
````

**正常的转换可以运用于显式模板实参上**

与正常的转换可以运用于具体类型函数参数一样，正常的转换也可以运用于被显式指定模板实参的函数参数上：
````cppp
long lng;
compare(lng, 1024); // error: template parameters don't match
compare<long>(lng, 1024); // ok: instantiates compare(long, long)
compare<int>(lng, 1024); // ok: instantiates compare<int, int>
````

### 16.2.3 Trailing Return Types and Type Transformation

使用显式模板参数来表示模板函数的返回值，当确实是想自己指定返回类型时是很好的。但有时强制显式模板实参却给自己增加了不必要的负担。比如想返回一个迭代器的解引用得到的元素的引用，在新的标准中可以使用尾后返回类型从而让编译器推断出来，而不需要显式指定模板类型参数。如：
````cpp
// a trailing return lets us declare the return type after the parameter list is seen
template <typename It>
auto fcn(It beg, It end) -> decltype(*beg)
{
    return *beg;
}
````
这里告知编译器 fcn 的返回类型与解引用 beg 参数是一样的，解引用操作符返回左值，所以 decltype 推断出来的类型是一个元素的引用。如此，如果 fcn 在 string 序列上调用返回类型将是 `string&`，如果序列是 int 的，则返回值是 `int&` 的。

**The Type Transformation Library Template Classes**

有时我们不能直接返回我们需要的类型，如：想让 fcn 返回元素的值类型而不是引用。从迭代器中根本不可能得到一个值类型。为了获取元素的类型，我们可以使用类型转移模板（type gransformation template）。这些模板被定义在 `type_traits` 头文件中，通常定义在其中的类型被称为模板元编程（template metaprogramming）。

| For Mod&lt;T&gt;, where Mod is | If T is                                | Then Mod&lt;T&gt;::type is |
| :--                            | :--                                    | :--                        |
| `remove_reference`             | X& or X&&<br/>otherwise                | X<br/>T                    |
| `add_const`                    | X&, const X, or function<br/>otherwise | T<br/>const T              |
| `add_lvalue_reference`         | X&<br/>X&&<br/>otherwise               | T<br/>X&<br/>T&            |
| `add_rvalue_reference`         | X& or X&&<br/>otherwise                | T<br/>T&&                  |
| `remove_pointer`               | `X*`<br/>otherwise                     | X<br/>T                    |
| `add_pointer`                  | X& or X&&<br/>otherwise                | `X*`<br/>`T*`              |
| `make_signed`                  | unsigned X<br/>otherwise               | X<br/>T                    |
| `make_unsigned`                | signed type<br/>otherwise              | unsigned T<br/>T           |
| `remove_extent`                | X[n]<br/>otherwise                     | X<br/>T                    |
| `remove_all_extents`           | X[n1][n2]...<br/>otherwise             | X<br/>T                    |

以上表格中的 `remove_reference` 用于获取元素类型，如：`remove_reference<int&>::type` 的结果是 int 类型。上面的难题的解决方案就是用 `remove_reference<decltype(*beg)>::type` 表示 beg 所指向的元素的值类型。如：
````cpp
template <typename It>
auto fcn2(It beg, It end) -> typename remove_reference<decltype(*beg)>::type
{
    return *beg;
}
````
type 是类模板的一个类型成员，这个类型由模板参数决定，因而，必须将 typename 关键字放在尾后返回类型的前面，用于告知编译器 type 表示一个类型。

上面表格中所有的类型转义模板都以相同的方式进行工作，每个模板都有一个公共成员 type 来表示类型。这个类型将是与模板参数相关的，且这种关系由模板的名字表达。如果不能进行对应的转换，type 成员将返回模板参数类型本身。如 T 本身就是不是指针，那么 `remove_pointer<T>::type` 就是 T 本身。

### 16.2.4 函数指针和实参推断

当使用函数模板来初始化或赋值函数指针时，编译器使用指针的类型来推断模板的实参。如：
````cpp
template <typename T>
int compare(const T &, const T &);
int (*pf1)(const int &, const int &) = compare;
````
pf1 的参数类型决定了 T 的模板实参类型，此处模板实参类型是 int，此时 pf1 指向 compare 的以 int 实例化的 compare 函数。如果模板实参不能从函数指针中推断出来，如：
````cpp
void func(int(*)(const string &, const string &));
void func(int(*)(const int &, const int &));
func(compare); // error: which instantiation of compare?
````
问题在于从 func 无法推断出 compare 的模板参数类型，可以通过显示指定模板实参，如：
````cpp
func(compare<int>); // passing compare(const int &, const int &)
````
当取函数模板实例的地址，上下文必须要让其能够让所有的模板参数（类型的或值的）可以被唯一推断。

### 16.2.5 模板实参推断和引用

如果函数的参数是模板类型的引用，需要记住的是：常见的引用绑定规则依然有效（左值只能绑定到左值，右值只能绑定到右值）；并且此时 const 是底层 const 而不是顶层 const 。

**左值引用函数参数的类型推断**

当一个函数参数是模板类型参数的左值引用如：`T&`，绑定规则告诉我们只能传递左值过去，实参可以有 const 修饰，如果实参是 const 的，那么 T 将被推断为 const 类型。如：
````cpp
template <typename T> void f1(T&);
f1(i); // i is an int; T is int
f1(ci); // ci is a const int; T is const int
f1(5); // error: argument to a & parameter must be an lvalue
````
如果一个函数参数是 `const T&` 的，那么绑定规则告诉我们可以传递任何类型（const 或非 const 对象、临时量或字面量）的实参过去。由于函数参数本身是 const 的，T 推断出来的类型将不在是 const 的了，因为，const 已经是函数参数类型的一部分；因而，它将不必在是模板参数的一部分。如：
````cpp
template <typename T> void f2(const T&); // can take an rvalue
// parameter in f2 is const &; const in the argument is irrelevant
// in each of these three calls, f2's function parameter is inferred as const int&
// and T is int
f2(i);
f2(ci);
f2(5);
````

**右值引用函数参数的类型推断**

当函数参数是一个右值引用时，形如：`T&&`，绑定规则告诉我们可以传递右值给这个参数。当这样做时，类型推断表现的类似于给左值引用函数参数一样，给类型参数 T 推断出来的类型就是这个右值类型。如：
````cpp
template <typename T> void f3(T&&);
f3(42); // argument is an rvalue of type int; template parameter T is int;
````

**Reference Collapsing（引用折叠） and Rvalue Reference Parameters**

虽然语言不允许将右值绑定到左值引用上，但是语言允许将左值绑定到右值引用上。语言通过两点实现了这个特性，其一将影响如何从右值引用参数中推断出 T 的类型，当传递左值（i）给函数参数是模板类型参数的右值引用（`T&&`）时，编译器会将 T 推断为左值引用，所以 `f3(i)` 中，编译器将 T 推断为 `int&` 而不是 int 。将 T 推断为 int& 似乎意味着 f3 的函数参数是对类型 int& 的右值引用。然而，语言不允许直接定义引用的引用，却允许间接通过类型别名或模板类型参数来达到此目标。

其二就是：如果间接创建了引用的引用，那么这些引用将被折叠（collapse），除了一个之外所有的折叠结果都是左值引用类型，新标准中将折叠规则扩展到了右值引用。只有右值引用的右值引用才会被折叠为右值引用。规则如下：

- `X& &`, `X& &&` 和 `X&& &` 被折叠为类型 `X&`；
- `X&& &&` 被折叠为 `X&&`；

引用折叠只会发生于间接创建的引用的引用，比如在类型别名或模板参数时。

结合引用折叠规则和右值引用参数的特殊类型推断规则，将使得 f3 可以对左值进行调用。如：
````cpp
f3(i); // template parameter T is int&, result in f3<int&>(int&)
f3(ci); // template parameter T is const int&, result in f3<const int&>(const int&)
````
这导致了两个重要的结果：

- 函数参数是模板类型参数的右值引用（`T&&`）可以被绑定到左值；
- 如果其实参是左值，那么推断的模板实参类型将是左值引用类型，且函数参数将被实例化为一个左值引用参数；

可以传递任何类型的实参给类型为 T&& 的函数参数，当传递左值时，函数参数将被实例化为左值引用 T& 。

**书写函数参数是右值引用的模板函数**

模板参数可以被推断为引用类型对模板内的代码具有惊人的影响。如：
````cpp
template <typename T> void f3(T&& val)
{
    T t = val;  // copy or binding a reference?
    t = fcn(t); // does the assignment change only t or val and t?
    if (val == t) { /* ... */ }  // always true if T is a reference type
}
````
根据传递个实参是左值还是右值，会产生行为非常不同的代码。要保证这种代码正确工作是非常困难的。在现实中，右值引用参数只会被运用于以下两种情况：函数模板将其参数进行转发（forwarding）或者函数模板是重载的，这个将在后面介绍。这里可以先提前声明的是这种函数模板与参数是右值引用的常规函数一样可以进行重载。如：
````cpp
template <typename T> void f(T&&); // binds to nonconst rvalues (1)
template <typename T> void f(const T&); // lvalues and const rvalues (2)
````
上面遵循十三章介绍过的右值引用函数参数的重载规则，所有的左值将优先选择函数 (2)，可修改的右值将优先选择函数 (1)。

### 16.2.6 理解 `std::move`

库函数 move 就是使用右值引用很好的例子。标准库定义 move 函数如下：
````cpp
template <typename T>
typename remove_reference<T>::type&& move(T&& t)
{
    return static_cast<remove_reference<T>::type&&>(t);
}
````
代码简短而微妙，由于 move 的参数是模板类型参数的右值引用 T&&，通过引用折叠这个函数参数可以匹配任何类型的实参，特别是既可以传左值或者右值给 move，如：
````cpp
string s1("hi!"), s2;
s2 = std::move(string("bye!")); // ok, moving from an rvalue
s2 = std::move(s1); // ok, but after the assignment s1 has indeterminate value
````
需要注意的是，当传递左值过去时，`static_cast<string&&>(t)` 代码会将一个左值引用（string&）强转为右值引用（string&&） 。

**`static_cast`将左值引用转为右值引用是允许的**

虽然不能隐式将左值转为右值引用，但是可以使用 `static_cast` 将左值强制转为右值引用。将右值引用绑定到左值使得操作右值引用的代码可以攫取左值的内容。通过让程序员只能使用强制转换，语言允许这样的用法。而通过强制（forcing）这样做，语言避免了程序员无意中这样做。最后，虽然可以直接写强转表达式，更为简单的方式是使用 move 函数。而且，使用 move 函数将更容易查找这些想要攫取左值内容的代码。

### 16.2.7 Forwarding

一些函数需要其一个或多个实参以类型保持完全不变的方式转发（forwarding）给另外一个函数。在这种情况下，我们需要保存转发实参的所有信息，包括实参是否为 const 或者实参是左值还是右值。如以下函数：
````cpp
// template that takes a callable and two parameters
// and calls the given callable with the parameters "flipped"
// flip1 is an incomplete implementation: top-level const and references are lost
template <typename F, typename T1, typename T2>
void flip1(F f, T1 t1, T2 t2)
{
    f(t2, t1);
}
````
这个函数将在传给 flip1 一个其参数是引用类型的函数 f 之前工作完全正常。如以下将无法正常工作：
````cpp
void f(int v1, int &v2) // note v2 is a reference
{
    cout << v1 << " " << ++v2 << endl;
}
````
如果以此函数去调用 flip1 函数，那么 f 将不能改变原始参数，改变的将是被复制的参数。为了达到转发的目的非得将 flip1 的参数改成右值引用形式，只有这样才能保持参数的类型信息。通过将其参数定义为模板类型参数的右值引用（T&&）来保持实参的整个类型信息（引用以及 const 性质）。通过将参数定义为引用可以保持参数的 const 性质，这是因为 const 在引用类型中是底层的。通过引用折叠，如果将函数参数定义为 T1&& 或 T2 &&，将可以保留 flip1 函数实参的左值/右值属性。修改代码如下：
````cpp
template <typename F, typename T1, typename T2>
void flip2(F f, T1 &&t1, T2 &&t2)
{
    f(t2, t1);
}
````
这个版本的 flip2 只解决了问题的一般，flip2 函数可以调用以左值引用为参数的函数，但是对以右值引用为参数的函数就无能为力了。如：
````cpp
void g(int &&i, int &j)
{
    cout << i << " " << j << endl;
}
flip2(g, i, 42); // error: can't initialize int&& from an lvalue
````
原因在于任何函数参数与变量都是左值，即便其初始值是右值。因而，在 flip2 中调用 g 将传递左值给 g 的右值引用参数。解决的办法自然是使用 `std::forward` 标准函数来保留原始实参的全部类型信息，与 move 一样，forward 定义在 utility 头文件中。forward 在调用时必须传递一个显式的模板实参，forward 将返回显式实参类型的右值引用。意味着 `forward<T>` 的返回类型是 T&& 。forward 的实现如下：
````cppp
// Sample implementation of std::forward
// For lvalues (T is T&), take/return lvalue refs.
// For rvalues (T is T), take/return rvalue refs.
template <typename T>
T&& forward(T &&param)
{
    return static_cast<T&&>(param);
}
````
通常 forward 被用于给传递一个定义为模板类型参数的右值引用的函数参数。通过对其返回类型进行引用折叠，forward 保留了其给定实参的左值/右值属性，如：
````cpp
template <typename Type> void intermediary(Type &&arg)
{
    finalFcn(std::forward<Type>(arg));
}
````
此处使用它 Type 作为 forward 的显式模板类型参数，由于 arg 是模板类型参数的右值引用，Type 将表示传递给 arg 的实参的所有类型信息。如果 arg 的实参是一个右值，那么 Type 将是非引用类型，则 `forward<Type>` 将返回 Type&& ；如果 arg 的实参是左值，那么通过引用折叠规则，Type 本身就是左值引用类型，则 `forward<Type>` 的返回类型将是左值引用的右值引用，通过引用折叠最终的返回类型将是左值引用。

通过在类型为模板类型参数的右值引用（T&&）的函数参数调用 forward 标准函数，将保持函数实参的所有类型细节。

使用 forward 来重写 flip 函数：
````cpp
template <typename F, typename T1, typename T2>
void flip(F f, T1 &&t1, T2 &&t2)
{
    f(std::forward<T2>(t2), std::forward<T1>(t1));
}
````

## 16.3 重载和模板

函数模板可以被别的模板或非模板函数重载。与往常一样，同名的函数必须在参数的数目或类型上有所差异。由于函数模板的出现导致的函数匹配的差异将表现在以下几个方面：

- 一个调用的候选函数（candidate function）包括所有模板实参推断成功的函数模板实例；
- 候选函数模板实例讲总是可行函数（viable function），这是由于模板实参推断会排除所有不可行的模板；
- 与往常一样，可行函数事按照需要进行的转型进行排序的，对于函数模板来说这种转型是非常有限的；
- 与往常一样，如果一个函数比其它函数提供了更优的匹配，此函数将被选中。然而，如果有好几个函数提供了一样好的匹配，那么：
    * 如果其中有一个非模板函数，那么这个非模板函数将被调用；
    * 如果没有非模板函数，但是多个函数模板中其中一个更加特化（specialized），那么这个更特化的函数模板将被调用；
    * 否则，调用是模糊的（ambiguous）；

为了正确定义重载的函数模板需要对类型之间的关系以及模板函数的有限转型有一个良好的理解；

**书写重载模板**

如下两个函数是重载的模板，如：
````cpp
template <typename T> string debug_rep(const T &t)
{
    ostringstream ret;
    ret << t;
    return ret.str();
}

template <typename T> string debug_rep(T *p)
{
    ostringstream ret;
    ret << "pointer: " << p;
    if (p)
        ret << " " << debug_rep(*p);
    else
        ret << " null pointer";
}
````
如果调用：`string s("hi"); cout << debug_rep(s) << endl;` 将只调用第一个函数 `debug_rep` 函数。如果以 `cout << debug_rep(&s) << endl;` 进行调用则两个模板函数都是可行的实例。第一个是 `debug_rep(const string* &)` 其中 T 被推断为 `string*`；第二个是 `debug_rep(string*);` 其 T 被推断为 string ；

然而第二个是精确匹配这个调用，第一个却需要将指针转为 const 的，函数匹配将更加青睐于第二个模板。

**多个可行模板**

考虑以下调用：
````cpp
const string *sp = &s;
cout << debug_rep(sp) << endl;
````
这里两个模板都是可行的而且是精确匹配的。第一个将被实例化为 `debug_rep(const string* &)`，其 T 将绑定到 `string*`；第二个将被实例化为 `debug_rep(const string*)` 其 T 将绑定到 `const string` 上。此时常规的函数匹配将无法区别哪一个调用是更优的。然而，由于新加的关于模板的规则，将调用 `debug_rep(T*)` 函数，这个函数是更加特化的模板。原因在于，如果没有这个规则，将没有任何方法在这种情况下以 const 指针调用指针版本的 `debug_rep`。问题在于，`debug_rep(const T&)` 可以在任何类型上调用，包括指针类型。而 `debug_rep(T*)` 则只能在指针左值上调用，显然是更为特化的版本。

当多个重载版本的模板提供一样优秀的函数调用匹配，最特化的版本将胜出。

**非模板和模板之间的重载**

考虑非模板版本的 `debug_rep` 函数：
````cpp
string debug_rep(const string &s)
{
    return '"' + s + '"';
}
````
以及调用：
````cpp
string s("hi");
cout << debug_rep(s) << endl;
````
此时将有两个一样优秀的函数匹配：`debug_rep<string>(const string&)` 其 T 绑定到 string，以及这里定义的非模板函数。此时编译器将选择非模板版本。这是由于同样的原因，即最特化的将会被选择，非模板函数是比模板版本更加特化的。

需要注意的是在使用任何重载的函数时记得将这些函数（包括模板函数与非模板函数）在同一个头文件中进行声明。这样在进行函数匹配时编译器可以进行完全的考虑，而不至于忽略任何一个。

## 16.4 可变参数模板

可变参数模板（variadic template）是新加的一种函数模板或类模板，这种模板可以接收可变数量的模板参数。这种可变参数被称为参数包（parameter pack）。语言允许两种参数包：模板参数包（template parameter pack）表示 0 或多个模板参数，以及函数参数包（function parameter pack）表示 0 个或多个函数参数。

语言使用省略号（ellipsis）来表示模板或函数参数包。对于模板参数列表，`class...` 或 `typename...` 表示接下的参数表示一系列 0 个或多个类型；省略号后的名字表示其参数包的任何类型的名字。在函数参数列表中，如果一个参数的类型是一个模板参数包，那么它就是一个函数参数包。如：
````cpp
// Args is a template parameter pack; rest is a function parameter pack
template <typename T, typename... Args>
void foo(const T &t, const Args& ... rest);
````
将 foo 声明为一个可变函数，其中一个类型参数是 T，以及一个模板参数包是 Args，这个参数包表示 0 个或多个额外的类型参数。函数参数列表则有一个参数，其类型是 `const T&`，以及一个函数参数包 rest，这个参数包表示 0 个或多个函数参数。

与之前一样，编译器从函数实参中推断模板参数类型。对于可变模板，编译器还会推断参数包中的数量，如：
````cpp
int i = 0; double d = 3.14; string s= "how now brown cow";
foo(i, s, 42, d); // three parameters in the pack
foo(s, 42, "hi"); // two parameters in the pack
foo(d, s); // one parameter in the pack
foo("hi"); // empty pack
````
编译器将会实例化 4 个不同的 foo 实例：
````cpp
void foo(const int&, const string&, const int&, const double&);
void foo(const string&, const int&, const char[3]&);
void foo(const double&, const string&);
void foo(const char[3]&);
````

**sizeof... 操作符**

当希望知道参数包中有多少个元素时，可以使用 `sizeof...` 操作符，与 sizeof 一样，其返回一个常量表达式并且不会对其实参进行求值：
````cpp
template <typename... Args> void g(Args... args) {
    cout << sizeof...(Args) << endl;
    cout << sizeof...(args) << endl;
}
````

### 16.4.1 书写可变参数函数模板

在前面用过 `initializer_list` 来定义函数可以接收不定数目的实参，但是这种实参必须是相同的类型，或者类型可以转为相同的类型。不定参数函数被用于及不知道实参的数目也不知道实参的类型。

不定参数函数将总是递归的，第一个调用处理包中的第一个实参，并以后面的实参调用其自身。如：
````cpp
template <typename T>
ostream &print(ostream &os, const T &t)
{
    return os << t;
}

// this version of print will be called for all but the last element
// in the pack
template <typename T, typename... Args>
ostream &print(ostream &os, const T &t, const Args&... rest)
{
    os << t << ", ";
    return print(os, rest...);
}
````
参数包可以是一个空包。

### 16.4.2 包扩展（Pack Expansion）

除了获取包的大小，另外一件可以做的事是对参数进行展开。当展开包时，可以提供一个模式（pattern）给它使用在每个展开的元素上。展开一个包将使得所有元素称为连续的逗号分隔的列表，并且将模式运用于每个元素上。使用省略号来进行包扩展。如 print 函数就有两个扩展。

第一个扩展将模板参数包进行扩展并产生了 print 的函数参数列表，第二个扩展发生在函数体内调用 print 处，这次将产生一个调用 print 的函数实参列表。

Args 的扩展运用 const Args& 模式给每个 Args 模板参数包中的元素，扩展的结果是一个逗号分隔的 0 个或多个参数类型，每个的类型都是 const T & 。

**理解包扩展**

函数参数包扩展可以运用更为复杂的模式，如：
````cpp
template <typename... Args>
ostream &errorMsg(ostream &os, const Args&...rest)
{
    return print(os, debug_rep(rest)...);
}
````
此调用 print 将运用模式 `debug_rep(rest)` ，将对函数参数包 rest 的每个元素调用 `debug_rep` 扩展的结果将是逗号分隔的一列 `debug_rep` 函数调用。

### 16.4.3 转发参数包（Forwarding Parameter Packs）

在新标准下，可以将 forward 与可变参数模板一起使用从而让函数在不改变其实参的任何类型信息的情况下转发给其它函数。如：
````cpp
template <class... Args>
inline
void StrVec::emplace_back(Args&&... args)
{
    chk_n_alloc();
    alloc.construct(first_free++, std::forward<Args>(args)...);
}
````
其中 `std::forwad<Args>(args)...` 将同时扩展模板参数包 Args 和函数参数包 args 。如：`svec.emplace_back(10, 'c')` 将被扩展为 `std::forward<int>(10), std::forward<char>(c)`。

实践中绝大多数的可变参数参数都将其参数转发给其它函数，就如 `emplace_back` 函数一样。

## 16.5 模板特例（Template Sepcializations）

一个模板并不总是能满足可以所有使其实例化的模板实参的要求。在某些情况下，通用的模板定义还可能是错误的：通用的定义可能无法编译或者做错误的事情。在某些情况下，可以利用特定类型的知识来写出更加有针对性且高效的代码（至少比从模板中实例化更加高效）。当不想或者不能使用模板版本时，可以定义一个类或函数模板的特例化版本。如：
````cpp
template <typename T> int compare(const T &, const T &);  // (1)

template <size_t N, size_t M>
int compare(const char(&)[N], const char(&)[M]);  // (2)
````
以上两个函数，(2) 函数只能对数组或字符串字面量进行调用，如果传入了字符指针，那么总是 (1) 函数被调用。这在有时并不是想要的结果。如：
````cpp
const char *p1 = "hi", *p2 = "mom";
compare(p1, p2); // calls (1) template
compare("hi" "mom"); // calls (2) template
````
为了处理字符指针的清醒，需要给 (1) 模板定义特例（template specialization）。特例是模板的另外一个定义，其中一个或多个目标那参数具有特定的类型。

**定义函数模板特例**

当定义函数模板特例时，需要给原模板中所有的模板参数提供实参。为了表示我们的确是在特例化一个模板，需要使用关键字 tempalte 后跟随一个空的尖括号 `<>` ，空的尖括号表示给原模板中的所有模板参数都提供了实参。如：
````cpp
template <>
int comapre(const char* const &p1, const char* const &p2)  // (3)
{
    return strcmp(p1, p2);
}
````
这里最难理解的部分就是特例中的函数参数类型必须与模板中的对应类型一样，这里对应的是 (1) 函数，与类型别名一样，当模板参数类型与指针、const 混杂在一起时也会变得复杂。此处 T 的类型时 `const char*` 即指向 const 对象的指针。

**函数重载 vs 模板特例**

当定义函数模板特例时，我们是在抢编译器的工作。就是说我们给原始模板的特定实例提供我们自己的定义。这里特别需要留意的是特例是一个实例；它不是重载；由于特例实例化一个模板；它不重载这个模板，因而，特例不会影响函数匹配过程。

如果将模板特例定义为独立的非模板函数将会影响函数匹配。如 `compare("hi", "mom")` 如果在将 (3) 定义为模板特例时，选择的依然是 (2) 函数，原因是它更加的特化（specialized）而 (3) 是不参与函数匹配的，而如果将 (3) 定义为接受指针的非模板函数，此时 (3) 将参与函数匹配并且将选择 (3) ，原因是非模板函数是最特化的版本。

为了定义个模板的特例，原始模板必须被声明在作用域内。并且，在任何使用模板实例的代码之前必须特例的声明必须在作用域内，如果不这么做的话就很可能会从模板中实例化出一个与要求不符合的函数，这种错误通常是很难定位的。

模板和它的特例应该定义在同一个头文件中，而且同一个名字的所有模板都应该出现在前面，后面跟随这些模板的特例。

**类模板特例**

参考代码：[template_specialization.cc](https://github.com/chuenlungwang/cppprimer-note/blob/master/code/template_specialization.cc)

`hash<Sales_data>` 以 `template<>` 开始用于表示我们定义一个完全的模板特例。在特例中只需要定义需要另行定义的成员函数，可以将特例的成员函数定义在类内或者类外。

**类模板部分特例**

与函数模板不同的是，类模板特例不用必须提供所有的模板参数实参。可以指定其中一些但不是全部的模板参数（或者参数的某个方面，如：左值引用或右值引用性质）。一个类模板的部分特例（partial specialization）本身是一个模板。用户必须给特例没有固定的模板参数提供实参。

只能部分特化一个类模板，但不能部分特化函数模板。

部分特化的一个很好的例子是 `remove_reference` 类，实现如下：
````cpp
// original, most general template
template <class T> struct remove_reference {
    typedef T type;
};
// partial specializations that will be used for lvalue and rvalue references
template <class T> struct remove_reference<T&>
{ typedef T type; } // lvalue references

template <class T> struct remove_reference<T&&>
{ typedef T type; } // rvalue references
````

该模板的第一个定义是最通用的一个版本，可以实例化任何一个类型。它使用模板实参作为成员类型 type 的类型。后面的两个类是原始模板的部分特例。由于部分特例也是一个模板，所以需要先定义模板参数，与别的特例一样，部分特例具有与其进行特化的模板一样的名字，特例的模板参数列表包含所有类型没有完全固定的模板参数，在类名之后的尖括号中是特例所指定的模板实参，这些实参与原始模板中的参数在位置上一一对应。

部分特例的模板参数列表是原始模板参数列表的一个子集，在此例中，特例具有与原始模板相同数目的模板参数，然而，特例中的参数类型却与原始模板不一样，特例分别使用了左值引用或右值引用类型。以下是一些使用场景：
````cpp
int i;
remove_reference<decltype(42)>::type a;
remove_reference<decltype(i)>::type b;
remove_reference<decltype(std::move(i))>::type c;
````

**仅仅特化成员而不是整个类**

相比于特化整个模板，可以只特化特定的成员函数。如：
````cpp
template <typename T> struct Foo {
    Foo(const T &t = T()) : mem(t) { }
    void Bar() { /* ... */}
    T mem;
};
template <>
void Foo<int>::Bar()
{
    // do whatever specialized processing that applies to ints
}
````
此例中仅特化一个 `Foo<int>` 的一个成员 Bar，`Foo<int>` 的其他成员将由 Foo 模板提供。如：
````cpp
Foo<string> fs; // instantiates Foo<string>::Foo()
fs.Bar(); // instantiates Foo<string>::Bar()
Foo<int> fi; // instantiates Foo<int>::Foo()
fi.Bar(); // uses our specialization of Foo<int>::Bar()
````
当将 Foo 运用于任何类型而不是 int 时，成员照旧进行实例化，而当将 Foo 运用 int 时，除了 Bar 之外的成员照旧进行实例化，只有 Bar 将会使用特化的版本。

## 关键术语

- 类模板（class template）：可以实例化类的模板定义，其定义形式是使用 template 关键字后跟随逗号分割的一个或多个模板参数，这些参数都放在了尖括号中，后面跟随一个类定义；
- 默认模板实参（default template arguments）：当用户不提供对应的模板实参时，模板将使用的类型或值；
- 显式实例（explicit instantiation）：为模板参数提供了所有显式实参的声明，用于指导实例化过程。如果一个声明是 extern 的，模板将不会被实例化；否则，模板将使用这个指定的实参进行实例化。对于任何一个 extern 模板定义，必须有另外一个非 extern 的显式模板实例；
- 显式模板实参（explicit template argument）：当定义一个模板类类型或调用函数时由用户提供的模板实参。显式模板实参将放在模板名字之后的尖括号中；
- 函数参数包（function parameter pack）：参数包表示 0 个或多个函数参数；
- 函数模板（function template）：可以从中实例化特定函数的模板定义，函数模板的定义将使用 template 关键字后跟随放置于尖括号中的用逗号分割的一个或多个模板参数，后再跟随函数定义；
- 实例化（instantiate）：用模板实参产生模板的一个特定实例的编译器过程，其中模板实参将替换模板参数。函数通过调用中的实参自动进行实例化，类模板则通过显式提供模板实参来实例化；
- 成员模板（member template）：本身是模板的成员函数，成员模板不是虚函数；
- 非类型参数（nontype parameter）：表示一个值的模板参数，非类型模板参数的模板实参必须是一个常量表达式；
- 包扩展（pack expansion）：一个参数包被替换为对应的元素列表的过程；
- 参数包（parameter pack）：模板或函数参数，其表示 0 个或多个参数；
- 部分特例（partial specialization）：类模板的一个版本，其中一些但不是全部的模板参数被指定（specified）或完全指定（completely specified）；
- 模式（pattern）：定义了扩展参数包时运用于每个元素的动作；
- 类型转发（type transformation）：由库定义的类模板，将其模板类型参数转发给相关的类型；
- 类型参数（type parameter）：在模板参数列表中用于表示一个类型的名字；

<!--vim: :formatoptions=croql :wrap :-->
