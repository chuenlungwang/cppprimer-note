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
这里将 partNo 定义为一族 pair 类，其中第二个成员是 unsigned，partNo 的第一个成员只要指定其第一个成员即可。

### 16.1.3 模板参数
### 16.1.4 成员模板
### 16.1.5 控制实例化
### 16.1.6 效率和灵活性

## 16.2 模板实参推断
### 16.2.1 转换和模板类型参数
### 16.2.2 函数模板显示实参
### 16.2.3 尾部返回类型和类型转换
### 16.2.4 函数指针和实参推断
### 16.2.5 模板实参推断和引用
### 16.2.6 理解 `std::move`
### 16.2.7 Forwarding

## 16.3 重载和模板

## 16.4 可变参数模板
### 16.4.1 书写可变参数函数模板
### 16.4.2 包扩展（Pack Expansion）
### 16.4.3 转发参数包（Forwarding Parameter Packs）

## 16.5 模板特例化

## 关键术语