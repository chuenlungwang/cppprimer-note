本章内容是对第三章的一个扩展，并且是对标准库顺序容器的一个完整讨论。顺序容器（sequential containers）中的元素顺序就是其被添加到容器中的位置。标准库还定义了几个关联容器（associative containers），关联容器中元素顺序由每个元素的 key 决定。

容器类共享一些共同的接口，然后在对其进行自己的扩展。共同接口使得容器更容易学习；在一个容器上所学的东西可以运用到另外一个容器上，除了每种容器提供不同的性能和功能性上的抉择。

容器（container）可以容纳一系列特定类型的对象，顺序容器使得程序员可以控制元素被存储和访问的顺序。这些顺序不由元素的值决定，而是根据其对应的置入容器中的位置决定的。相反，有序和无序关联容器基于键值来存储器元素。

标准库还提供了三个容器适配器，适配器通过为容器类型的操作定义不同的接口来达到不同的语义。

以下内容是基于 §3.2 §3.3 §3.4 的内容的，在阅读下面的材料之前请先阅读这三节。

## 9.1 顺序容器概述

所有下面的容器都能够快速顺序访问其元素。它们之前的区别在于以下功能的性能取舍：

- 给容器添加和删除带来的消耗；
- 非顺序访问容器中的元素带来的消耗；

顺序容器列表如下：

- vector：灵活尺寸的数组，支持快速随机访问，除了在尾部插入或删除元素外会很慢；
- deque：双端队列，支持快速随机访问，支持首部和尾部的快速插入和删除；
- list：双链表，只支持双向顺序访问，在任何位置上都支持快速插入和删除；
- `forward_list`：单链表，只支持单一方向的顺序访问，在任何位置上都支持快速插入和删除；
- array：固定尺寸的数组，支持快速随机访问，但是不能添加和删除元素；
- sring：包含字符的特殊容器，类似于 vector，支持快速随机访问，以及在尾部的快速插入和删除；

除了 array 是固定尺寸（fixed-size）的容器，所有容器都提供了高效而灵活的内存管理。我们可以添加和删除元素，可以增加（growing）或缩减（shrinking）容器的大小。容器使用的存储元素的策略对于这些操作的性能有固有和重要的影响。在一些情况下甚至会影响一个特定的容器是否会提供特定的操作。

如：string 和 vector 将其元素放在连续的内存中，由于元素是连续的，能够快速按照其索引计算元素的地址。然而，在容器的中间添加或移除元素就是很耗时的操作了：所有位置在插入或删除之后的元素都必须移动以保持连续性。更甚者，添加元素有时会导致需要分配额外的内存，此时，所有元素都需要移动到新的内存中，并销毁原来使用的内存。

list 和 `forward_list` 被设计使得可以在容器的任何位置快速添加和删除，作为交换，这些类型不支持元素的随机访问：只能通过迭代的方式访问元素。并且相较于 vector, deque 和 array 消耗更多的内存。

deque 是一个更为复杂的数据结构。与 string 和 vector 一样，deque 支持快速随机访问（fast random access），并且在 deque 的中间添加和移除元素是很耗时的。然而，在 deque 的两端进行插入和删除则是很快速的操作，可以比得上在 `list` 或 `foward_list` 中添加元素。

`forwad_list` 和 array 是在新标准中添加进来的。array 是内置数组的一个更安全且易用的替代品。与内置数组一样，array 是固定尺寸的。因而，array 不支持添加和移除元素，或者改变容器的大小。`forward_list` 则是为了提供了性能上比得上最好的手写单链表。`forward_list` 没有提供 size 操作，这是由于存储和计算大小会导致其性能比不上手写的单链表。其它容器的 size 操作则保证是快速且是固定时间（constant-time）的操作。

由于移动操作的出现，现在的库容器的性能已经比之前的版本有了戏剧性的提升。库容器几乎肯定比绝大多数手写版本性能要好，现代 C++ 程序应该更多使用库容器而不是更基础的结构如内置数组。

**决定使用哪个顺序容器**

有一些选择使用哪个容器的首要原则：

- 除非了更好的理由，优先使用 vector；
- 如果程序有许多小的元素并且对空间消耗很敏感，不要使用 list 和 `forward_list`；
- 需要随机访问，使用 vector 或 deque；
- 需要在容器中间部分进行快速插入和删除，使用 list 或 `forward_list`；
- 如果程序需要是首部和尾部插入或删除元素，而不是中间部分，使用 deque；
- 如果程序在读取输入时需要在中间部分插入元素，而接下来则需要随机访问元素：
    - 先看是否真的需要在中间部分插入元素，通常更容易的做法是添加到 vector 中，然后调用库函数 sort 进行排序；
    - 如果确实需要在中间部分插入，考虑在输入阶段使用 list ，一旦完成了输入，将其复制到 vector 中去；

如果同时需要随机访问和在中间部分插入或删除元素？此时需要看占主要的操作是什么（更多的是插入或删除，还是更多的是访问），来决定使用哪种类型的容器。在这种情况下，使用多种容器进行性能测试是必不可少的。

最佳实践：如果你不知道该选择哪种容器，那就进使用 vector 和 list 共同的接口：迭代器而不是下标，避免随机访问元素。这样就可以在需要时很容易的替换。

## 9.2 容器库概述

容器类型上的操作组成了一种层级结构。一些操作是所有容器类型都提供的，其它一些则是顺序容器特有的，或者关联容器或无序容器特有的。而最后还有一些只有少部分的容器类型拥有。

在本节，我们将描述所有类型的容器都拥有的共同部分。§9.3 描述顺序容器特有的操作。¶11 描述关联容器特有的操作。

通常每个容器定义在与其名字相同的头文件中。deque 定义在 `<deque>` 头文件中，list 定义在 `<list>` 头文件中。容器都是类模板。与 vector 一样，使用时必须提供额外的信息来生成特定的容器类型。对于绝大部分容器来说，提供的是元素的类型，如：`list<Sales_data>` `deque<double>` 。

以下是所有容器共同的操作：

**类型别名**
- iterator ：每个容器类型自己的迭代器类型；
- `const_iterator` ：可以读取但不能修改元素的迭代器类型；
- `size_type` ：容器类型足够容纳最大的容器的尺寸的无符号整形值类型；
- `difference_type` ：两个迭代器之间的距离的有符号整形类型；
- `value_type` ：元素类型；
- reference ：元素的左值类型，与 `value_type&` 是同义词；
- `const_reference` 元素的 const 左值类型；

**赋值和 swap**
- `c1 = 2` 将 c1 中的元素替换为 c2 中的元素；
- `c1 = {a,b,c...}` 将 c1 中的元素替换为列表中的值（不适用于 array）；
- `a.swap(b)` 将 a、b 中的值进行交换；
- `swap(a, b)` 等于交换 `a.swap(b)`；

**Size**
- `c.size()` c 中元素个数（不适用于 `forward_list`）；
- `c.max_size()` c 所处的容器类型可以容纳的元素的最大数目；
- `c.empty()` 如果 c 有元素返回 false，否则返回 true；

**添加、移除元素（不适用于 array）**
Note：每个容器的接口参数不一样
- `c.insert(args)` 将 args 中的元素拷贝到 c 中；
- `c.emplace(inits)` 使用 inits 来构建 c 中的一个元素；
- `c.erase(args)` 从 c 中移除由 args 所表示的元素；
- `c.clear()` 移除 c 中的所有元素；返回 void；

**相等性和关系操作符**
- `==` `!=` 相等性操作符使用所有容器类型；
- `<` `<=` `>` `>=` 关系操作符（对于无序关联容器不适用）；

**获取迭代器**
- `c.begin()` `c.end()` 返回首元素和尾后元素（one past the last element）的迭代器；
- `c.cbegin()` `c.cend()` 返回 `const_iterator`；

**可反转的容器的额外成员（不适用于 `forward_list`）**
- `reverse_iterator` 以相反方向访问元素的迭代器；
- `const_reverse_iterator` 不能写元素的反向迭代器；
- `c.rbegin()` `c.rend()` 返回尾元素和首前元素（one past the first element）；
- `c.crbegin()` `c.crend()` 返回 `const_reverse_iterator`；

以下是定义和初始化容器的方式：

- `C c;` 默认构造函数，如果 C 是 array，那么 c 中的元素默认初始化的；否则 c 是空的；
- `C c1(c2);` c1 是 c2 的副本。c1 和 c2 必须是相同的类型（容器类型和元素类型都必须一样，对于 array 还必须有相同的 size）；
- `C c{a,b,c...}` `C c={a,b,c...}` c 是初始化列表元素的一个副本；列表中元素的类型必须与 C 的元素类型相互兼容，如果是 array，列表中的数目必须小于等于 array 中的大小，所有缺失的元素是值初始化的；
- `C c(b,e)` c 是迭代器 b 和 e 所表示的范围的元素的副本，其元素的类型必须与 c 中的元素类型互相兼容（不适用于 array）；

**以 size 初始化顺序容器（不适用于 array）**
- `C c(n);` c 具有 n 个值初始化元素，此构造函数是 explicit 的；
- `C c(n,t);` c 中具有 n 个值 t；

**容器可以容纳的元素类型**

几乎任何类型都可以作为顺序容器的元素类型。特别是可以定义容器的元素类型是另外一个容器。如：`vector<vector<string>> lines;` 在一些旧式的编译器中可能会要求相连的两个尖括号间用空格隔开。如：`vector<vector<string> >`。

尽管可以将几乎任何类型存储在容器中，一些容器操作对元素类型是有要求的。我们可以定义不支持某些操作要求的容器类型，但是想要使用这些操作就必须让元素类型满足这些操作的需求。

比如，接收一个 size 参数的顺序容器构造函数将使用元素类型的默认构造函数。一些类型是没有默认构造函数的，可以定义容器包含这种类型的对象，但是不能只使用元素数目去构建这样的容器。如：
````cpp
vector<noDefault> v1(10, init); // 正确：提供了元素初始值
vector<noDefault> v2(10); // 错误：必须提供元素的初始值
````

在接下来的介绍中会逐步描述其它操作对于容器元素类型的要求。

### 9.2.1 迭代器

与容器一样，迭代器（iterator）本身也是有共同的接口的。如果一个迭代器提供了某种操作，那么所有迭代器类型都提供的这种操作的行为都是一致的。如所有的标准容器都提供解引用操作符以访问容器元素，所有的标准库容器迭代器都提供自增运算符从一个元素移动到下一个元素。

`forward_list` 容器的迭代器不支持自减操作符，只有 string，vector，deque 和 array 支持迭代器的算术运算。

**迭代器范围**

迭代器范围的概念是标准库的一个基石。迭代器范围（iterator range）是由同一个容器中的一对迭代器来表示的，其中一个第一个元素，第二个指向最后的一个元素的下一个位置（one past the last element）。这两个迭代器通常用 begin 和 end 或者 first 和 last 来表示一个容器中的元素范围。

名字 last 带有一点误导的作用，第二个迭代器并不是指向最后一个元素，而是指向最后一个元素的下一个位置。元素范围包括 first 和一直到 last 之前的所有元素。这种元素范围成为左包含区间（left-inclusive interval），用数学表示就是 `[begin, end)` ，表示范围从 begin 开始，以 end 结束但是不包括 end。迭代器 begin 和 end 必须指向同一个容器。

end 一定不能指向 begin 之前的元素，只有这样才能通过自增 begin 迭代器而到达 end ，否则，所表示的范围就会无效。编译器并不能强制要求这种需求，只能有程序自己去保证这种约定。

**左包含范围的编程隐喻**

标准库使用左包含范围是由于这种范围有三个遍历的属性：

- 如果 begin 和 end 相等，那么范围是空的；
- 如果 begin 与 end 不相等，那么范围中至少有一个元素，并且 begin 表示范围内的第一个元素；
- 通过自增 begin 几次之后会达到 `begin == end`；

这几个属性使得我们可以这样书写循环，如：
````cpp
while (begin != end) {
    *begin = val;
    ++begin;
}
````

### 9.2.2 容器类型的成员

每个容器都定义了几个类型。如：`size_type` ，iterator 和 `const_iterator` 。绝大多数的容器还提供反向迭代器，反向迭代器的 `++` 将使得迭代器指向前一个元素，从而达到反向遍历容器元素的目的。

剩下的类型别名使得我们使用容器的元素类型时不需要真正知道其类型是什么。如果我们需要元素类型，我们只要使用 `value_type` 就可以，如果需要值的引用类型，就使用 reference 或者 `const_reference` 就可以。这些元素相关的类型别名在泛型编程中十分有用处。

使用这些类型需要用容器的类型加以限定，如：
````cpp
list<string>::iterator iter;
vector<int>::difference_type count;
````

### 9.2.3 begin 和 end 成员

begin 和 end 操作能够产生指向首元素和尾后元素的迭代器。它们通常用于表示容器中全部元素的迭代器范围。

标准库中有多个版本的 begin 和 end：r 版本返回反向迭代器，c 返回 const 的迭代器。如：
````cpp
list<string> a = {"Milton", "Shakespeare","Austen"};
auto it1 = a.begin();
auto it2 = a.rbegin();
auto it3 = a.cbegin();
auto it4 = a.crbegin();
````
没有以 c 开头的函数是重载的，在 const 成员返回的是 `const_iterator`，非 const 版本返回的 iterator 。这适用于 begin 、rbegin 、 end 和 rend。当在非 const 对象上调用时返回的是 iterator，只有当在 const 对象上调用时调用才是 const 版本。与指针和引用一样，可以将 iterator 转为 `const_iterator` ，但不能反着操作。

c 版本是新标准引入的，这样做是为了支持 auto 声明变量。在早期只能使用指明需要哪个类型如：
````cpp
list<string>::iterator it5 = a.begin();
list<string>::const_iterator it6 = a.begin();
auto it7 = a.begin();
auto it8 = a.cbegin();
````
如果使用 auto 那么 begin 和 end 返回的迭代器类型就由容器类型决定。使用 c 则可以不管是什么类型的容器都可以得到 `const_iterator`，当不需要写入时，使用 cbegin 和 cend。

### 9.2.4 定义和初始化容器

每个容器类型都定义了默认构造函数。除了 array ，其它容器的默认构造函数都创建一个空的容器。除了 array 之外，其它的容器可以接收一个 size 的参数，将初始化为有 size 个元素被值初始化的容器。

**初始化容器作为另外一个容器的副本**

有两种方式可以将一个容器初始化为别的容器的副本：调用拷贝构造函数或者（除 array 之外）拷贝由一对迭代器表示元素范围。

如果通过拷贝构造函数那么容器和元素的类型都必须一样。如果通过迭代器则没有这样的要求，只要元素类型之间是可以转换的。如：
````cpp
list<string> authors = {"Milton", "Shakespeare","Austen"};
vector<const char*> articles = {"a","an","the"};
list<string> list2(authors);
deque<string> authList(authors); // 错误：容器类型不匹配
vector<string> words(articles); // 错误：元素类型不匹配
// 将 const char* 转为 string
forward_list<string> words(articles.begin(), articles.end());
````
接收两个迭代器的构造函数使用它们来表示想要拷贝的元素范围。其中一个表示首元素另一个表示尾后元素。新的容器的大小与范围中的大小一致。新容器中的每个元素都是用范围内元素进行初始化的。

由于迭代器表示一个范围，可以使用构造函数拷贝一个容器的子序列。如 it 表示 authors 中的一个元素：
````cpp
deque<string> authList(authors.begin(), it);
````
以上将拷贝从 authors 的首元素直到 it 之前的所有元素。

**列表初始化**

在新标准下，可以使用列表初始化一个容器，如：
````cpp
list<string> authors = {"Milton","Shakespeare","Austen"};
vector<const char*> articles = {"a","an","the"};
````
列表初始化会指定容器中的每个元素的值。除了 array 之外，初始化列表同时暗含了容器的大小：容器大小与元素的初始化列表中的个数一样多。

**顺序容器指定大小的构造函数**

上面所介绍的所有构造函数同时适用于关联容器。除此之外顺序容器（除了 array 之外）还可以指定大小和可选的元素初始值。如果没有提供元素初始值，那么将创建一个值初始化的容器。如：
````cpp
vector<int> ivec(10, -1);
list<string> svec(10, "hi!");
forward_list<int> ivec(10);
deque<string> svec(10);
````
如果元素类型是内置类型或者具有默认构造函数的类类型，那么就可以使用接收一个大小的参数的构造函数来构建容器。如果元素类型没有默认构造函数，在初始化时必须同时显式提供元素初始值。

注意：指定大小的构造函数只适用于顺序容器；并不适用于关联容器。

**array 类是固定尺寸**

标准库 array 类型与内置数组一样，其尺寸是类型的一部分。当定义数组时除了指定元素类型时，还要指定大小。如：
````cpp
array<int, 42> arr1;
array<string, 10> arr2;
````
使用 array 类型同时需要指定元素类型和大小：
````cpp
array<int, 10>::size_type i;
array<int>::size_type j; // 错误：array<int> 不是一个类型
````
由于大小是 array 的一部分，array 不支持通常的容器构造函数。这些构造函数或隐式或显式决定了容器的大小。给 array 的构造函数传递表示大小的参数本身就是不必要且易错的。

array 的固定尺寸的属性同样影响其构造函数的行为。与其它容器不一样，其默认构造的 array 不是空的，而是所有的元素都是默认初始化的，这与内置数组的行为一致。如果想要使用列表初始化 array，提供的初始值必须小于等于 array 的大小。如果初始值更少的话，前面的部分将会被初始值初始化，后面的元素将被值初始化。在这两种情况下，如果元素类型是类类型，都必须要有默认构造函数。如：
````cpp
array<int, 10> ia1;
array<int, 10> ia2 = {0,1,2,3,4,5,6,7,8,9};
array<int, 10> ia3 = {42};
````
值得注意的是尽管我们不能拷贝或赋值内置数组，但是标准库 array 是可以拷贝和赋值的。如：
````cpp
int digs[10] = {0,1,2,3,4,5,6,7,8,9};
int cpy[10] = digs; // 错误：内置数组不能拷贝和赋值
array<int, 10> digits = {0,1,2,3,4,5,6,7,8,9};
array<int, 10> copy = digits;
````
与任何容器一样，拷贝或赋值时类型必须是完全匹配的，对于 array 来说元素类型和尺寸都必须完全一样。

### 9.2.5 赋值和 swap

赋值相关的操作符作用域整个容器。其将左边容器的全部元素替换为右边容器中的元素的副本。如：
````cpp
c1 = c2; // 将 c1 的内容替换为 c2 中的元素的副本
c1 = {a,b,c}; // 将 c1 的内容替换为初始化列表中的元素的副本
````
当执行完赋值后，左右边的容器的内容就完全一样了。

与内置数组不一样，标准库 array 类型支持赋值。左右边的操作数必须是相同的类型：
````cpp
array<int, 10> a1 = {0,1,2,3,4,5,6,7,8,9};
array<int, 10> a2 = {0};
a1 = a2; // 元素类型和大小都一样
a1 = {0}; // !!!错误：array 不支持初始值列表的赋值操作
````
由于初始值列表中的元素个数可能与 array 中的元素个数不一样，array 不支持用初始值列表进行赋值。基于同样的原因 array 同样不支持 assign 操作。

**容器的赋值操作**

- `c1 = c2` 将 c1 中的元素替换为 c2 中元素的副本。c1 和 c2 必须是相同类型；
- `c = {a,b,c...}` 将 c1 中元素替换为初始值列表中的元素的副本（不适用于数组）；
- `swap(c1, c2)` `c1.swap(c2)` 交换 c1 和 c2 中的元素。c1 和 c2 必须是相同的类型。swap 通常比 c2 到 c1 的赋值要*快很多*。

**assign 操作不适用于关联容器和 array**

- `seq.assign(b,e)` 将 seq 中的元素替换为由迭代器表示的范围中的值的副本，b 和 e 不能指向 seq 中的元素；
- `seq.assign(il)` 将 seq 中的元素替换为初始值列表 il 中的值的副本；
- `seq.assign(n,t)` 将 seq 中的元素替换为 n 个值 t 的副本；

赋值操作将使得左边容器中的迭代器、引用和指针失效，除了 string 之外，swap 之后却保持有效并且其所指向的容器被改变了（注：就是之前指向左边容器的迭代器，在 swap 之后指向右边容器中的元素了）。

**使用 assign（只有顺序容器支持）**

赋值操作符需要左边和右边操作具有相同的类型，它将其右边操作数的所有元素拷贝到左边操作数。同时顺序容器还定义了一个名为 assign 的成员（array 没有定义），assign 的存在使得可以从一个不同但是兼容的容器类型进行赋值，或者从一个容器的子序列进行赋值。assign 将左边容器中的元素全部替换为其参数中的元素。如可以将 `vector<const char*>` 赋值给 `list<string>` ：
````cpp
list<string> names;
vector<const char*> oldstyle;
names = oldstyle; // 错误：容器类型不匹配
names.assign(oldstyle.cbegin(), oldstyle.cend());
````
注意：由于现存的元素被替换掉，传递给 assign 的迭代器一定不能指向调用 assign 的容器。

assign 的第二个版本接收一个整数以及一个元素值。它将容器中的元素全部替换为特定数目的元素，每个元素值都与参数中给出的元素值一样。如：
````cpp
list<string> slist1(1);
slist1.assign(10, "Hiya!");
````

**使用 swap**

swap 操作交换两个类型相同的容器的内容，在 swap 之后两个容器的元素是互换的。如：
````cpp
vector<string> svec1(10);
vector<string> svec2(24);
swap(svec1, svec2);
````
除了 array 之外，语言标准保证 swap 两个容器是很快的，元素本身不会被交换；内部数据结构被交换。

注意：除了 array，swap 不做任何拷贝、删除或者插入元素，因而保证在固定时间内完成。

元素不被移动意味着迭代器、引用和指针都不会失效（对于 string 不成立）。它们在 swap 之后指向与之前同一个元素，但是需要明白的一点是这些元素被交换到了不同的容器中。如 iter 本身指向 svec1[3] 的，swap 之后指向 svec2[3] 了。与容器不同，在 string 调用 swap 会导致迭代器、引用和指针失效。

array 的 swap 有特别之处，它会交换所有元素值。所以交换两个 array 需要与 array 元素数目成比例的时间。想要 swap 一个 array 它们的元素类型和大小都必须一致。

array 在 swap 之后，其迭代器、指针和引用都指向原来指向的元素不变。当然，其元素值已经被替换为了另外一个 array 中对应位置的元素值。

在新标准中，同时提供了一个成员版本和非成员版本的 swap。早期版本只提供了成员版本的 swap，非成员版本对于泛型编程来说很重要。所以，最好是使用费成员版本的 swap。

### 9.2.6 容器 size 操作

除了 `forward_list` ，所有容器类型都提供 size 成员返回容器中元素的数目；除此之外，所有容器类型还都提供 empty 返回 bool 表示容器是否为空；`max_size` 返回此容器类型能够容纳的元素数目的限制；

`forwad_list` 只提供了 `max_size` 和 empty 操作。

### 9.2.7 关系操作符

所有容器类型都提供相等操作符（== 和 !=）；除了无序关联容器所有容器都提供关系操作符（`>` `>=` `<` `<=`）。左边和右边操作数必须是相同类型的容器，并且其元素类型也必须是一样的。

比较两个容器执行的是两两之间的比较，这与 string 的关系操作是一样的：

- 如果两个容器具有相同的大小，并且所有元素都是一样的，那么这两个容器就是相等的；否则他们就是不相等的‘
- 如果两个容器的大小不一样，但是短一点的容器中的每个元素都与对应位置的长的容器中的元素值相等，那么短的就比长的小；
- 如果没有一个容器是另一个的头部子序列，那么比较取决于第一个不相等的元素；

如以下代码：
````cpp
vector<int> v1 = {1,3,5,7,9,12};
vector<int> v2 = {1,3,9};
vector<int> v3 = {1,3,5,7};
vector<int> v4 = {1,3,5,7,9,12};
v1 < v2; // true
v1 < v3; // false
v1 == v4; // true
v1 == v2; // false
````

注意：只有元素类型同样定义了关系操作符，我们才能使用容器的关系操作符进行比较。

**关系操作符使用元素的关系操作符**

容器的相等操作符使用元素的 `==` 操作符，关系操作符使用了元素的 `<` 操作符。如果元素类型不支持需要的操作符，那么我们就无法使用容器的对应的操作。如：`Sales_data` 不支持 `==` 和 `<` 操作。那么就不能比较两个 `Sales_data` 的容器元素：
````cpp
vector<Sales_data> storeA, storeB;
if (storeA < storeB) // 错误：Sales_data 没有小于操作符
````

## 9.3 顺序容器操作

顺序容器和关联容器的区别在于如何组织元素。这些区别会影响元素是如何被存储、访问、添加和移除的。前面的部分主要介绍的是所有容器共有的操作。

以下将介绍顺序容器特有的操作。

### 9.3.1 给顺序容器添加元素

除了 array 之外的容器都提供了灵活的内存管理机制。可以动态的添加或移除容器中的元素从而在运行时改变容器的大小。以下是给顺序容器添加元素的操作：

以下的操作都会改变容器的大小；array 不支持这些操作。`forward_list` 有自己的 insert 和 emplace 的版本；`forward_list` 并不支持 `push_back` 和 `emplace_back`。string 和 vector 不支持 `push_front` 和 `emplace_front`；

- `c.push_back(t)` `c.emplace_back(args)` 在容器 c 的尾部创建一个元素，其值为 t 或者以 args 为参数进行构建，无返回值；
- `c.push_front(t)` `c.emplace_front(args)` 在容器 c 的头部创建一个元素，其值为 t 或者以 args 为参数进行构建，无返回值；
- `c.insert(p,t)` `c.emplace(p,args)` 在迭代器 p 所指向的元素之前插入元素，其值为 t 或者以 args 为参数构建，返回被添加元素的迭代器；
- `c.insert(p,n,t)` 在迭代器 p 所指向的元素之前插入 n 个值为 t 的元素。返回指向第一个被添加的元素的迭代器；如果 n 是 0，返回 p；
- `c.insert(p,b,e)` 在迭代器 p 所指向的元素之前插入由迭代器 b 和 e 所表示的范围中的元素。b 和 e 不能 c 中的元素。返回指向第一个被插入的元素的迭代器；如果范围是空，返回 p；
- `c.insert(p,il)` il 是一个括号中的初始值列表。将给定值插入到由 p 所指向的元素之前；返回第一个被添加的元素的迭代；如果列表是空的，返回 p；

注意：添加元素到 vector、string 或者 deque 中有可能会使得所有容器中现存的迭代器、引用和指针失效；

当我们使用这些操作时，需要记住这些容器使用不同的策略来分配内存给元素，这将会影响到性能。除了在尾部给 vector 和 string 添加元素，或者除了在首尾给 deque 添加元素都会导致元素移动。而且，添加元素给 vector 和 string 可能会导致整个容器的重新分配。重新分配整个对象需要重新分配内存，然后将其元素从旧的位置移动到新的位置。

**使用 `push_back`**

`push_back` 可以在容器的尾部添加元素，除了 array 和 `forward_list` 所有的顺序容器都支持 `push_back`。如：
````cpp
string word;
while (cin >> word)
    container.push_back(word);
````
由于 string 就是字符的容器，可以用 `push_back` 在 string 的尾部添加字符：
````cpp
word.push_back('s');
````
**关键概念：容器元素是拷贝的**

当我们使用对象来初始化容器，或者插入一个对象到容器中时，放入容器中的是哪个对象值的拷贝，不是对象本身。这就像我们传递一个对象给非引用参数是一样的，容器中的元素与这个对象没有任何关系。接下来如果改变了元素的值并不会影响到原始值，反之亦然。

**使用 `push_front`**

list 、`forward_list` 和 deque 支持类似的操作 `push_front` ，这个操作在容器的首部插入一个新的元素：

````cpp
list<int> ilist;
for (size_t ix = 0; ix != 4; ++ix)
    ilist.push_front(ix);
````
在执行完这个循环之后，ilist 中包含值 3,2,1,0

deque 和 vector 一样支持对其元素的快速随机访问，并且提供 `push_front` 成员。deque 保证在其首尾添加或删除元素是固定时间的。与 vector 一样，在中间部分插入元素将是很耗时的操作。

**在容器的特定位置添加元素**

`push_back` 和 `push_front` 操作提供了方便的方式在顺序容器的头尾插入单个元素。更为通用的是 insert 成员允许我们在容器的任何位置插入零个或更多元素。vector 、 deque 、 list 和 string 支持 insert 成员。 `forward_list` 提供了这些成员的特别版本。

每个 insert 函数都以一个迭代器作为其第一个参数。这个迭代器表示元素将被插入的位置，它可以是容器中的任何位置，包括容器的尾后位置。由于迭代器可能指向尾后的不存在的元素，并且需要支持在容器的头部插入元素，元素将被插入到迭代器表示的位置之前。如：
````cpp
slist.insert(iter, "Hello!");
````
将插入一个字符串到 iter 所表示的元素之前。

尽管有些容器没有 `push_front` 操作，insert 并没有这种限制。可以在容器的前面 insert 元素而不用担心容器是否支持 `push_front`：
````cpp
vector<string> svec;
list<string> slist;
slist.insert(slist.begin(), "Hello!");
svec.insert(svec.begin(), "Hello!");
````
注意：在 vector 、deque 、string 的任何位置插入元素都是合法的，但是这样做是很耗时的操作。

**插入元素范围**

insert 除了第一个参数后的参数的模式与构造函数具有相同的参数模式。

其中一个版本 insert 以元素数目和值作为参数，将添加指定数目的同一元素到给定的位置：
````cpp
svec.insert(svec.end(), 10, "Anna");
````
这段代码添加 10 个元素到 svec 的尾部，每个元素的值都是 "Anna"。

还有一个版本的 insert 以一对迭代器或者一个初始值列表作为参数，将给定范围内的值插入到指定位置：
````cpp
vector<string> v = {"quasi","simba","frollo","scar"};
slist.insert(slist.begin(), v.end()-2, v.end());
slist.insert(slist.end(), {"these","words","will","go","at","the","end"});
// 运行时错误：表示拷贝源的迭代器范围不能指向插入的容器，这是因为迭代器会失效
slist.insert(slist.begin(), slist.begin(), slist.end());
````
当以一对迭代器作为拷贝源时，这些迭代器一定不能指向将要插入元素的容器。

在新标准下 insert 成员会返回指向被插入的第一个元素迭代器，之前的版本没有返回值。如果范围是空的，没有元素被插入，并且返回 insert 函数的第一个参数。

**使用 insert 的返回值**

使用 insert 的返回值可以重复的插入元素到容器中指定的位置：
````cpp
list<string> lst;
auto iter = lst.begin();
while (cin >> word)
    iter = lst.insert(iter, word);
````

**使用 emplace 操作**

新标准加入三个新的成员 `emplace_front` 、 emplace 和 `emplace_back` ，这三个成员对应于 `push_front` 、 insert 和 `push_back`，只是它们不是拷贝元素而是直接构建。

当我们调用 emplace 成员时，传递给元素类型的构造函数的参数，并以这些参数直接直接构建一个对象放在容器中。如在 c 中置入 `Sales_data` 元素：
````cpp
c.emplace_back("987-0590353403", 25, 15.99); //(1)
c.push_back(Sales_data("987-0590353403", 25, 15.99)); //(2)
````
(1) 是直接在容器中构建对象，(2) 则是先创建一个本地临时量，然后将其置入容器中。

传递给 emplace 函数的参数与元素类型有关。参数必须与元素类型的一个构造函数参数列表匹配：
````cpp
c.emplace_back(); // 调用 Sales_data 的默认构造函数
c.emplace_back(iter, "999-999999999");
c.emplace_back("987-0590353403", 25, 15.99);
````

### 9.3.2 访问元素

下面的列表中列举了可以用于访问顺序容器中元素的操作，这些操作在容器中没有元素时是*未定义*的：

at 和 [] 是适用于 string 、 vector 、 deque 和 array，back 不适用于 `forward_list`

- `c.back()` 返回指向容器 c 的最后一个元素的引用，如果 c 是空的结果将是未定义的；
- `c.front()` 返回指向容器 c 的首元素的引用，如果 c 是空的结果将是未定义的；
- `c[n]` 返回由无符号整数 n 索引的元素，如果 `n >= c.size()` 结果是未定义的；
- `c.at(n)` 返回由无符号整数 n 索引的元素，如果索引超出了范围，将抛出 `out_of_range` 的异常；

注意：在空的容器中调用 front 或 back 或者在使用下标时超出范围，是非常严重的编程错误。

每个顺序容器都有 front 成员（包括 array），除了 `forward_list` 之外的顺序容器都有 back 成员，这些操作分别返回首元素和尾元素的引用，如：
````cpp
if (!c.empty()) {
    auto val = *c.begin(), val2 = c.front();
    auto last = c.end();
    auto val3 = *(--last);
    auto val4 = c.back();
}
````
以上程序用两种不同的方式获取容器 c 的首元素和尾元素。需要注意的 end 返回的迭代器指向的是一个不存在的元素，在解引用之前需要先递减一。而且在获取元素之前，需要先检查 c 是不是空的，如果容器是空的，那么 if 内的操作就是未定义的。

**访问成员返回的是引用**

访问成员返回的是容器中元素的引用，如果容器是 const 对象，那么返回的引用也是 const 的。如果容器是非 const 的，那么引用就是常规引用，我们可以使用其来改变元素的值：
````cpp
if (!c.empty()) {
    c.front() = 42;
    auto &v = c.back();
    v = 1024;
    auto v2 = c.back();
    v2 = 0;
}
````
用 auto 保存返回的引用，必须将 auto 定义引用类型。

**下标操作和安全的随机访问**

提供快速随机访问的容器（string、vector、deque 和 array）同时提供了下标操作。下标操作符用一个索引返回那个位置的元素的引用。索引必须是在安全范围内的（大于等于 0 并且小于容器的大小）。保证索引是合法的是程序的责任；下标操作不会检查索引是否在范围内。使用超出范围的索引是严重的编程错误，但是这个错误是编译器发现不了的。

如果想要保证索引是合法的，可以使用 at 成员。at 成员的行为与下表操作符类型，但是当索引是不合法的时候，at 将抛出 `out_of_range` 异常：
````cpp
vector<string> svec;
cout << svec[0]; // 运行时错误，但是编译器发现不了
cout << svec.at(0); // 将抛出 out_of_range 异常
````

### 9.3.3 移除元素

正如有多种方式添加元素到容器中去，同样有多种方式移除元素。（array 不支持这些函数），以下是这些成员：

这些操作会改变容器的大小，所以 array 不支持这些操作。`forward_list` 有自己特殊的 erase 版本；`pop_back` 不适用于 `forward_list`；`pop_front` 不适用于 vector 和 string；

- `c.pop_back()` 移除容器 c 中的尾元素，如果 c 是空的，结果将是未定义的，返回值是 void；
- `c.pop_front()` 移除容器 c 的首元素，如果 c 是空的，结果将是未定义的，返回值是 void；
- `c.erase(p)` 移除迭代器 p 指向的元素，并返回被删除元素之后的迭代器，如果 p 指向最后一个元素则返回尾后迭代器。如果 p 本身就是尾后迭代器，结果将是未定义的；
- `c.erase(b,e)` 移除由迭代器 b 和 e 表示的范围中的元素，返回最后一个被删除的元素之后的迭代器，如果 e 本身就是尾后迭代器，返回尾后迭代器；
- `c.clear()` 移除 c 中所有的元素，返回 void；

注意：除了在 deque 的首尾删除元素之外，其它位置的删除操作将会导致所有的迭代器、引用和指针失效。对于 vector 和 string，处于删除点之后的所有元素的迭代器、引用和指针将失效；

移除元素的成员不会检查其参数，保证元素在移除前存在是程序员的责任。

**`pop_front` 和 `pop_back` 成员**

`pop_front` 和 `pop_back` 分别移除首尾元素。就像 vector 和 string 没有 `push_front` 一样，它们也没有 `pop_front` 。类似的 `forward_list` 没有 `pop_back` 成员。就像元素访问成员一样，不要将 pop 操作用于空的容器。

这些操作返回 void，如果你需要被 pop 的值，你需要在 pop 之前先将其存储起来：
````cpp
while (!ilist.empty()) {
    process(ilist.front());
    ilist.pop_front();
}
````

**在容器的中间部分移除元素**

erase 成员移除容器中特定位置的元素，可以删除由迭代器表示的单个元素或者由一对迭代器表示的元素范围。两种形式的 erase 都返回被移除的最后一个元素的之后的位置的迭代器。也就是说如果 j 是元素 i 之后的元素，那么 erase(i) 将会返回指向 j 的迭代器。

**移除多个元素**

以一对迭代器作为参数的 erase 版本允许我们删除范围内的元素，如：
````cpp
elem1 = slist.erase(elem1, elem2); // elem2 将会失效
````
elem1 指向要删除的首元素，elem2 指向最后一个元素的下一个位置。

为了删除容器中的所有元素，可以调用 clear 或者将 begin 和 end 传给 erase ：
````cpp
slist.clear();
slist.erase(slist.begin(), slist.end()); // 两者的作用是相等的
````

### 9.3.4 特定于 `forward_list` 的操作

为了理解为何 `forward_list` 有特别的添加和移除元素操作的版本，需要知道从单链表（singly linked list）中移除元素将会发生什么。如：

````
elem1 -> elem2 -> elem3 -> elem4
````

为了移除 elem3 必须改变 elem2 使得其 next 重新指向 elem4

当我们给单链表添加或删除元素时，被增加或删除的元素的之前那个元素的 next 改变了。所以，为了添加或移除元素，我们需要访问其前置元素，从而才能更新那个元素的 next 连接。然而，对于 `forward_list` 这种单链表来说很难获得一个元素的前置元素。基于这个原因，在 `forward_list` 中的添加或移除元素操作改变的是给定元素后面的那个元素。这样总能访问到由于改变而影响到的元素。

由于这些操作在 `forward_list` 上的不同表现，`forward_list` 没有定义 insert 、 emplace 和 erase ，相反它定义 `insert_after` `emplace_after` 和 `erase_after`，如为了移除 elem3 ，我们可以在 elem2 的迭代器上调用 `erase_after`，为了支持这些操作，`forward_list` 定义了 `before_begin` 返回首前迭代器（off-the-beginning iterator）。这个迭代器允许我们在一个首元素之前的不存在的元素之后添加或移除一个元素。

`forward_list` 中插入和移除元素的操作：

- `lst.before_begin()` `lst.cbefore_begin()` 返回一个迭代器表示链表首元素之前的不存在的元素，这个迭代器不能被解引用。 `cbefore_begin()` 返回一个 `const_iterator`；
- `lst.insert_after(p,t)` `lst.insert_after(p,n,t)` `lst.insert_after(p,b,e)` `lst.insert_afer(p,il)` 在由迭代器 p 表示的元素之后插入新的元素。t 是一个对象，n 是数目，b 和 e 是表示元素范围的迭代器（b 和 e 不能指向 lst)，il 是一个括号中的值列表。返回插入的最后一个元素的迭代器。如果范围是空的，则返回 p，如果 p 是尾后迭代器，行为是未定义；
- `emplace_after(p, args)` 使用 args 在 p 迭代器所表示的元素之后构建一个新的元素。返回新元素的迭代器，如果 p 是尾后迭代器，行为将是未定义的；
- `lst.erase_after(p)` `lst.erase_after(b,e)` 移除 p 迭代器所表示的元素的后一个元素，或者 b 之后直到 e 但是不包括 e 范围内的元素。返回删除的元素之后的下一个元素的迭代器，如果不存在下一个元素则返回尾后迭代器。如果 p 表示链表中最后一个元素或者是尾后迭代器，行为将是未定义的；

当我们想要在 `forward_list` 中添加或移除元素时，我们需要记录两个迭代器，一个用于检查元素值 curr，一个是这个元素的前置迭代器 prev

### 9.3.5 resize 容器大小

除了 array 的容器可以使用 resize 操作来使得容器更大或更小。如果当前大小比请求的尺寸大的话，元素将从后面开始删除；如果当前大小比新的尺寸小的话，元素将被添加到容器的尾部，以下是顺序容器支持的改变大小的操作：

- `c.resize(n)` 改变容器 c 的大小，使得其有 n 个元素，如果 `n < c.size()` 额外的元素将被删除。如果需要添加新的元素，它们将是值初始化的。
- `c.resize(n,t)` 改变容器 c 的大小，使得其有 n 个元素，如果需要添加元素，其值将是 t ；

如果 resize 缩小了容器的大小，那么指向被删除的元素的迭代器、引用和指针将会失效；在 vector 、string 或者 deque 上调用 resize 将会使得所有的迭代器、指针和引用失效。

以下是一些实例：
````cpp
list<int> ilist(10, 42);
ilist.resize(15); // 添加 5 个元素到 ilist 的尾部，其值为 0
ilist.resize(25, -1); // 添加 10 个元素到 ilist 的尾部，其值为 -1
ilist.resize(5); // 移除 ilist 尾部的 20 个元素
````

resize 操作以一个可选的元素值作为参数，用于初始化添加到容器中的元素。如果没有这个参数，添加的元素将是值初始化的。如果容器中的元素是类类型，那么当 resize 添加元素时，要么提供初始值要么元素类型有一个默认构造函数。

### 9.3.6 容器操作会使得迭代器失效

改变容器大小的操作将会使得迭代器、引用和指针失效。失效的迭代器、引用和指针将不再指向一个元素。使用失效的迭代器、引用和指针是一个严重的编程错误，这就像使用未初始化的指针引发的问题是一样的。

在添加元素到容器之后：

- 如果 vector 或 string 重新分配了内存，迭代器、指针或引用将会失效。如果没有重新分配，插入位置之前的间接引用将保持有效；在插入点之后的元素的引用将会失效；
- 在 deque 首尾位置之外的任何位置添加元素都将导致迭代器、指针和引用失效。如果在首尾添加元素，迭代器将失效，但是引用和指针将保持有效；
- list 和 `forward_list` 的迭代器、指针和引用（包括尾后和首前迭代器）都将保持有效；

在移除一个元素之后，除了被移除的元素的迭代器、引用和指针失效之外：

- list 和 `forward_list` 的所有其它元素的迭代器、引用和指针（包括尾后和首前迭代器）都将保持有效；
- 除了在 deque 的首尾位置移除元素，所有其它迭代器、引用和指针都将失效。如果在 deque 的尾部移除元素，尾后迭代器将失效，但是其它迭代器、引用和指针将保持有效；如果是从首部移除元素，所有迭代器、引用和指针都将保持有效；
- 在 vector 和 string 中，所有在移除点之前的迭代器、引用和指针都将保持有效。而尾后迭代器将总是失效；

注意：使用失效的迭代器、引用和指针是一个严重的运行时错误。

**建议：管理迭代器**

当使用容器元素的迭代器、引用或者指针，尽可能缩短需要迭代器、引用、指针保持有效的代码的范围。由于给容器添加或删除元素的代码会使得迭代器、引用和指针失效，需要在每次改变容器之后重新获取迭代器。这个建议对于 vector 、string 和 deque 特别重要。

**书写改变容器大小的循环**

给 vector 、string 和 deque 添加或移除元素的循环必须了解的一个事实是迭代器、引用和指针可能会失效。程序必须保证每次循环之后迭代器、引用和指针都会更新。如果循环调用的是 insert 或 erase 的话，更新迭代器将会很贱。这些操作都会返回迭代器，从而可以用于重置迭代器。

**避免存储由 end 返回的迭代器**

当在 vector 和 string 中添加和移除元素，或者在 deque 中添加元素或者移除非首元素时，其 end 返回的迭代器将总是失效。所以添加和移除元素的循环需要总是调用 end 而不是将获取到的迭代器存起来。部分由于这个原因，C++ 标准库将 end() 调用实现为一个很快的操作。

所以不要在会往 deque 、string 或 vector 中插入或删除元素的循环中缓存 end() 返回的迭代器。

## 9.4 vector 如何增长

为了支持快速随机访问，vector 的元素是连续存储的，每个元素都毗邻前一个元素。通常，我们不关心库类型是如何实现的；我们需要关心的仅仅是如何使用它。然而，对于 vector 和 string 其部分实现泄露到了接口中。

如果元素是连续的，而容器的尺寸是可变的，想象一下当添加元素到 vector 和 string 中会发生什么：如果没有用于存储新元素的空位置，容器必须要分配新的内存并将所有元素移动到新位置，并且添加新元素，然后释放掉旧的内存。如果 vector 每次添加元素分配和释放内存，那么性能将会很差。

为了避免这种消耗，库实现使用一种分配策略来减少容器需要重新分配内存的次数。当需要获取新的内存时，vector 和 string 实现通常会分配超出其所直接需要的内存。容器将保留这部分内存用于存储新添加的元素。这样容器就不需要每次添加一个新元素就重新分配内存。

这种分配策略比每次添加一个元素都重新分配更加高效。事实上，vector 的性能甚至比 list 或 deque 更加好，即便 vector 每次重新分配内存时都需要移动其所有元素。

**管理容量的成员**

vector 和 string 类型提供了一些成员用于与内存分配部分的实现进行交互。capacity 操作告知我们一个容器在需要分配更多内存之前可以容纳多少元素。reserve 操作则允许我们告知容器准备让它持有多少个元素。

`shrink_to_fit` 适用于 vector, string 和 deque。capacity 和 reserve 仅适用于 vector 和 string。

- `c.shrink_to_fit()` 将 capacity() 减少到与 size() 一样；
- `c.capacity()` 在重新分配内存之前 c 可以存储的元素数目；
- `c.reserve(n)` 分配至少保存 n 个元素的内存；

reserve 不会改变容器中元素的数目；它仅仅影响 vector 预分配的内存的大小。

调用 reserve 只有在请求的空间大于当前容量时才会改变 vector 的容量。如果请求的大小待遇当前容量，reserve 将会分配至少相当于（甚至更多）请求的内存大小。

如果请求的大小小于或等于当前的容量，reserve 将不做任何事。特别是，如果 reserve 的参数所表示的大小比 capacity 小，不会导致容器收缩内存。所以，在调用 reserve 之后，capacity 将会大于等于传递给 reserve 的参数。

因而，调用 reserve 将永远不会减少容器使用的空间大小。而前面介绍的 resize 成员将仅仅改变容器中元素的数目，而不是其容量。我们不能使用 resize 来减少容器持有的内存。

在新标准下，可以使用 `shrink_to_fit` 来要求 deque, vector 或 string 归还未使用的内存。这个函数表明我们不需要超出范围的容量。然而，到底归不归还则由实现自己决定，并不保证 `shrink_to_fit` 会归还内存。

**capacity 和 size**

理解 capacity 和 size 之间的区别是很重要的。size 是容器中已经拥有的元素的数目，capacity 是这个容器在重新分配内存之前可以容纳多少元素。

当刚创建一个空的容器时，size 和 capacity 都是 0，随着不断添加元素，capacity 总是大于等于 size。容器只有在必须要分配内存时才会重新分配，而且似乎目前实现的策略是将容量扩大为原来的两倍，不过这是由实现决定的。

vector 只在 size 等于 capacity 时执行插入操作，或者调用 resize 或 reserve 时它们的参数值大于当前的 capacity 的情况下才会重新分配内存。至于分配多少内存则是由实现决定的。

每个实现都需要实现一种分配策略从而保证 `push_back` 元素到 vector 中是高效的。通过在一个原本是空的 vector 上调用 `push_back` n 次创建一个具有 n 个元素的 vector 的执行时间不应该超过一个常量乘以 n 。 

## 9.5 额外的 string 操作

string 类型在共同的顺序容器操作之外还提供了很多额外的操作。其中的大部分操作要么是提供 sting 与 C 风格字符数组之间的交互，要么是提供使用索引而不是迭代器来操作 string 。

string 库提供了大量的函数，幸运的是，这些函数有着重复的模式。

### 9.5.1 构建 string 的其它方式

除了第三章中介绍构造函数以及前面介绍的与其它顺序容器共用的构造函数，string 类型还支持三个额外的构造函数。

n，len2 和 pos2 都是无符号值。

- `string s(cp, n);` s 是 cp 所指向的字符串数组中的前 n 个字符的拷贝，这个数组至少要有 n 个字符；
- `string s(s2, pos2);` s 是 string s2 从索引 pos2 开始的字符串的拷贝，如果 pos2 > s2.size() 结果将是未定义的；
- `string s(s2, pos2, len2);` s 是 string s2 中从索引 pos2 开始最多 len2 个字符的拷贝，如果 pos2 > s2.size() 结果将是未定义的。不管 len2 的值是多少，最多拷贝 s2.size() - pos2 个字符；

以 string 或 `const char*` 为参数的构造函数，还会接收一个额外的参数来指定需要拷贝的字符。当我们传入 string 时，我们还需要指定开始拷贝的字符的索引，如：
````cpp
const char *cp = "Hello World!!!";
char noNull[] = {'H', 'i'};
string s1(cp);
string s2(noNUll, 2);
string s3(noNull); // undefined: noNull not null terminated
string s4(cp + 6, 5);
string s5(s1, 6, 5);
string s6(s1, 6);
string s7(s1, 6, 20);
string s8(s1, 16); // out_of_range
````
通常如果我们从 `const char*` 创建一个 string，指针指向的字符数组必须是 NULL 结尾的；字符将被拷贝直到 NULL。如果同时传入 count ，数组不必必须以 NULL 结尾。如果没有传入 count 而数组没有以 NULL 结尾，或者给了 count 但是其值大于数组的长度，那么操作将是未定义的。

当我们从 string 拷贝时，可以提供额外的参数来指定开始拷贝的位置，以及一个 count。开始位置必须小于等于给定 string 的长度。如果起点大于长度，那么构造函数将抛出 `out_of_range` 异常。当传入 count，将从起点拷贝 count 个字符，不管 count 是多少，只会最多拷贝到 string 的结尾处，不可能更多字符。

**substr操作**

substr 返回原始 string 的一部分或全部的拷贝 string。可以给 substr 传入可选的起点和计数器：

- `s.substr(pos, n);` 返回 s 字符串中从 pos 位置开始的 n 个字符的 string 拷贝。pos 默认是 0，n 默认是从 pos 直到 string 结尾的总长度；

如：
````cpp
string s("hello world");
string s2 = s.substr(0, 5);
string s3 = s.substr(6);
string s4 = s.substr(6, 11);
string s5 = s.substr(12); // 抛出 out_of_range
````

如果 pos 超出了 string 的长度，substr 函数将抛出 `out_of_range` 异常。如果 pos+count > s.size()，count 将被调整只拷贝到 string 的结尾。

### 9.5.2 改变 string 的其它方式

string 类型除了支持顺序容器的赋值操作符和 assign，insert 以及 erase 操作外。它自己还定义了额外的 insert 和 erase 版本。

除了定义接收迭代器的 insert 和 erase，string 提供了接收索引的版本。索引表示开始移除的元素，或者表示插入值到指定位置前。如：
````cpp
s.insert(s.size(), 5, '!');
s.erase(s.size() - 5, 5);
````
string 还提供 insert 和 assign C风格字符数组的版本。如：
````cpp
const char *cp = "Stately, plump Buck";
s.assign(cp, 7);
s.insert(s.size(), cp+7);
````
首先将 s 的内容通过调用 assign 来替换，赋值给 s 的字符时从由 cp 开始的 7 个字符。请求的字符数必须小于等于从 cp 开始直接数组尾部的长度（不包括 NULL 字符）。

当我们调用 insert 时，我们是想在 s 的尾部（一个不存在的字符 `s[s.size()]` 之前）插入字符，在这种情况下我们从 cp 之后的第 7 个字符开始拷贝知道数组的 NULL 字符为止（不包括 NULL 字符）。

我们还以指定让 insert 或 assign 的字符来自于另外一个 string 或者它的子串：
````cpp
string  s = "some string", s2 = "some other string";
s.insert(0, s2);
// 在 s[0] 之前插入从 s2[0] 开始的 s2.size() 个字符
s.insert(0, s2, 0, s2.size());
````

**append 和 repalce 函数**

string 类还定义了两个额外的成员 append 和 replace 来改变 string 的内容。append 操作是一种在字符串尾部插入字符的快捷方式：
````cpp
string s("C++ Primer"), s2 = s;
s.insert(s.size(), " 4th Ed."); // (2)
s2.append(" 4th Ed."); // (3) 两种方式是相同的
````
replace 操作是调用 erase 和 insert 的快捷方式：
````cpp
s.erase(11, 3);
s.insert(11, "5th");
s2.replace(11, 3, "5th");
````
replace 替换的 string 可以与移除的字符数目不一致，`s.repalce(11, 3, "Fifth");` 这里移除了三个字符串但是插入了五个字符。

**改变 string 的多种重载方式**

append, assign, insert 和 replace 函数有多个重载版本。这个参数随着指定添加字符和改变 string 的哪个部分的不同方式而改变。

replace 函数提供两种方式来指定要移除的字符范围。可以通过指定位置和长度来表示范围，或者通过迭代器范围。insert 函数提供了两种方式来指定插入点：用索引或者迭代器。在两种方式中都是新元素都被插入到给定索引或者迭代器之前。

有多种方式来指定添加到 string 中的字符。新的字符可以来自于另外一个 string，来自于字符指针或者来自于花括号中的字符列表，或者一个字符加计数器。当字符来自于 string 或字符指针时，可以传递额外的参数来告知是拷贝参数中的部分还是全部字符。

### 9.5.3 string 搜索操作

string 类提供了六个不同的搜索函数，每个有四个重载版本。以下是这些成员函数以及它们的参数，每个搜索都返回一个 `string::size_type` 来表明匹配发生的索引。如果没有匹配，函数将返回一个 static 成员 `string::npos` ，库定义 npos 的值为 -1 ，由于 `string::size_type` 是无符号值，意味着 npos 将是最大的整数值。

搜索操作返回期待的字符的索引或者在没有找到的情况下返回 npos 。

- `s.find(args)` 查找 args 在 s 中第一次出现的位置；
- `s.rfind(args)` 查找 args 在 s 中最后一次出现的位置；
- `s.find_first_of(args)` 查找 args 中任一字符在 s 中第一次出现的位置；
- `s.find_last_of(args)` 查找 args 中任一字符在 s 中最后一次出现的位置；
- `s.find_first_not_of(args)` 查找 s 中第一个不是在 args 中的字符；
- `s.find_last_not_of(args)` 查找 s 中最后一个不是在 args 中的字符；

args 必须是其中之一：

- `c, pos` 在 s 中从位置 pos 开始查找字符 c，pos 默认是 0；
- `s2, pos` 在 s 中从位置 pos 开始查找 string s2，pos 默认是 0；
- `cp, pos` 在 s 中从位置 pos 开始查找由指针 cp 指向的 C 风格字符串（以 NULL 结尾的字符数组），pos 默认是 0；
- `cp, pos, n` 在 s 中从位置 pos 开始查找由指针 cp 指向的字符数组的前 n 个字符。pos 和 n 都没有默认值；

string 搜索函数返回 `string::size_type` ，这是一个无符号类型。因而，用 int 或者其它带符号的类型来接收返回值不是一个好的想法。

find 函数是最简单的，它在 string 中搜索参数，然后返回第一个匹配的索引，如果没有找到就返回 npos：
````cpp
string name("AnnaBelle");
auto pos1 = name.find("Anna"); // pos1 == 0
````
string 的搜索操作是大小敏感的。

一个稍微复杂的问题是在 string 中查找参数字符串中的任何一个字符，如以下查找 name 中的第一个数字：
````cpp
string numbers("0123456789"), name("r2d2");
auto pos = name.find_first_of(numbers);
````
与之相反，我们可以调用 `find_first_not_of` 来查找第一个不存在于搜索参数中的字符，如以下查找第一个非数字字符：
````cpp
string dept("03714p3");
auto pos = dept.find_first_not_of(numbers);
````

**指定从哪里开始搜索**

可以传递额外的起始位置给 find 操作，这个额外的参数告知从哪里开始搜索。默认的情况下开始搜索的位置是 0。这个额外的参数常用于循环查找所有的匹配位置：
````cpp
string::size_type pos = 0;
while ((pos = name.find_first_of(numbers, pos)) != string::npos) {
    cout << "found number at index: " << pos
         << " element is " << name[pos] << endl;
    ++pos; // 如果忽略了自增，将导致进入无限循环
}
````

**发现搜索**

find 操作是自左向右进行搜索的，库中还提供了类似的操作进行自右向左的搜索。rfind 成员查找最后一个也就是最右边的匹配，如：
````cpp
string river("Mississippi");
auto first_pos = river.find("is");
auto last_post = river.rfind("is");
````

`find_last_of` 搜索参数中任一一个字符出现在 string 中的最后的位置；`find_last_not_of` 搜索不匹配搜索参数中任何字符的最后一个位置；

以上这些操作都有第二个参数用于表示在 string 中开始搜索的位置。

### 9.5.4 compare 函数

除了关系操作符，string 库还提供一系列 compare 函数，这些函数类似于 C 库中的 strcmp 函数。与 strcmp 一样，s.compare 返回 0 或者正数或者负数分别表示 s 等于,大于,小于其参数。

这六个版本的 compare 函数不同之处在于其参数是 string 或者字符数组，是比较全部还是部分：

- `s.compare(s2);` 比较string s 和 string s2；
- `s.compare(pos1, n1, s2);` 比较string s 中从 pos1 开始的 n1 个字符和 string s2；
- `s.compare(pos1, n1, s2, pos2, n2)` 比较 string s 中从 pos1 开始的 n1 个字符和 string s2 中从 pos2 开始的 n2 个字符；
- `s.compare(cp)` 比较 string s 和 cp 所表示的 C 风格字符串；
- `s.compare(pos1, n1, cp);` 比较 string s 中从 pos1 开始的 n1 个字符和 cp 所表示的 C 风格字符串；
- `s.compare(pos1, n1, cp, n2);` 比较 string s 中从 pos1 开始的 n1 个字符和 cp 所表示的 C 风格字符串中的前 n2 个字符；

### 9.5.5 数字转换

string 中经常包含表示数字的字符。在新标准中引入了多个函数可以在数字与 string 之间进行转换：
````cpp
int i = 42;
string s = to_string(i);
double d = stod(s);
````

- `to_string(val)` 返回 val 的字符串表现形式，val 可以是任何算术类型；
- `stoi(s,p,b)` `stol(s,p,b)` `stoul(s,p,b)` `stoll(s,p,b)` `stoull(s,p,b)` 如果 s 的最开始的部分是数字，将它转换为 int, long, unsigned long, long long, unsigned long long。b 表示进制；b 默认是 10 进制；p 是一个 `size_t` 类型值的指针，用于存储第一个非数字字符的索引；p 默认是 0 ，表示不存储这个索引；
- `stof(s,p)` `stod(s,p)` `stold(s,p)` 将 s 最开始部分的数字内容转为 float, double 或 long double。p 用于存储非数字字符的索引；

为了将 string 转为数字，其第一个非空白符字符必须可以存在于数字中：
````cpp
string s2 = "pi = 3.14";
d = stod(s2.substr(s2.find_first_of("+-.0123456789")));
````
`find_first_of` 先获取第一个可以出现在数字中的字符位置，然后传递那个位置开始的子串给 stod，std 函数读取 string 直到找到一个不是数字一部分的字符，然后将前面部分的数字字符转为一个双精度浮点数。

如果是十六进制的数，string 可以以 0x 或 0X 开始，转为浮点数的 string 可以以小数点开始，并且可能包含 e 或 E 表示指数。根据基数的不同，string 中可以包含对应的字母用于表示超出 9 的数字。

注意：如果 string 不能转为数字，这些函数将抛出 `invalid_argument` 异常，如果转换出来的值不能被表示，将抛出 `out_of_range` 异常。

## 9.6 容器适配器

除了顺序容器，标准库还定义了三个顺序容器适配器（adaptor）：stack, queue 和 `priority_queue`。适配器是标准库中的通用概念。其中包含容器适配器、迭代器适配器和函数适配器。本质上，适配器是一种使得让一个物件表现得像另外一个物件的机制。容器适配器（container adaptor）使得一个现存的容器类型在行为上看起来像是一个不同的类型。如 stack 适配器让一个顺序容器（非 array 或 `forward_list`）的行为看起来好像是一个 stack。

以下是所有的容器适配器共有的操作和类型：

- `size_type` 足够容纳此类型中最大对象的尺寸类型；
- `value_type` 元素类型；
- `container_type` 实现适配器的底层容器类型；
- `A a;` 创建一个空的适配器 a；
- `A a(c);` 创建适配器 a，其内容是容器 c 的拷贝；
- 关系操作符，每个适配器都支持所有的关系操作符：`==` `!=` `<` `<=` `>` `>=` 这些操作符返回底层容器的比较结果；
- `a.empty()` 如果 a 有任何元素返回 false，否则返回 true；
- `a.size()` a 中元素的数目；
- `swap(a,b)` `a.swap(b)` 交换 a 和 b 中的内容；a 和 b 必须是相同的类型，包括其底层实现的容器类型；

**定义一个适配器**

每个适配都定义了两个构造函数：默认构造函数以创建一个空的对象，以及接收一个容器的构造函数并将适配器初始化为给定容器的拷贝。如假设 deq 是 `deque<int>` 可以将 deq 用于初始化 stack：
````cpp
stack<int> stk(deq);  // 将 deq 中的元素拷贝到 stk 中
````

默认情况下 stack 和 queue 都是将 deque 作为底层实现的，而 `priority_queue` 将 vector 作为底层实现。可以在创建适配器时将一个顺序容器作为第二个类型参数从而创建不同类型的适配器：
````cpp
stack<string, vector<string>> str_stk;  // 以 vector 创建空的 stack
// 以 vector 创建 stack，此 stack 将 svec 中的元素值拷贝作为初始值；
stack<string, vector<string>> str_stk2(svec);
````

对于哪些容器可以被用于一个给定的适配器是有限制的。所有的适配器都需要有添加和移除元素的能力。因而不能将 array 用于适配器，同样不能使用 `forward_list` ，原因是所有适配器都需要能够添加、移除和访问容器中最后一个元素。stack 只需要 `push_back`, `pop_back` 和 back 操作，所以可以将所有的剩下的容器类型用于 stack。queue 适配器需要 back, `push_back`, front 和 `push_front` 操作，所以它可以建立在 list 或 deque 上而不能是 vector。`priority_queue` 除了 front, `push_back` 和 `pop_back` 操作以及快速随机访问，所以它可以建立在 vector 或 deque 上，而不能在 list 上。

**stack 适配器**

stack 类型定义在 stack 头文件中。以下是 stack 提供的操作：

stack 将 deque 作为默认的底层实现；同样可以实现在 list 或 vector 之上。

- `s.pop()` 移除但是不返回 stack 的顶部元素；
- `s.push(item)` `s.emplace(args)` 在 stack 的顶部创建一个新的元素，这个元素要么是拷贝或移动 item 或者直接从 args 中构建此元素；
- `s.top()` 返回但是不移除 stack 的顶部元素；

实例如：
````cpp
stack<int> intStack;
for (size_t ix = 0; ix != 10; ++ix)
    intStack.push(ix);
while (!intStack.empty()) {
    int value = intStack.top();
    intStack.pop();
}
````
虽然每个容器适配器的所有的操作都是用底层容器类型的操作进行定义的。但是我们只能使用适配器的操作，而不能使用底层容器类型的操作，如：`intStack.push(ix);` 调用 deque 对象的 `push_back` 。尽管 stack 是使用 deque 实现的，却不能直接使用 deque 的操作，即不能在 stack 上调用 `push_back`；相反，必须使用 stack 的 push 操作。

**queue 适配器**

queue 和 `priority_queue` 适配器都定义在 queue 头文件中。以下是所有操作：

默认情况下 queue 使用 deque，`priority_queue` 使用 vector；queue 可以使用 list 或 vector 作为底层容器类型，`priority_queue` 可以使用 deque。

- `q.pop()` 移除但是不返回，queue 首元素或者 `priority_queue` 最高优先级的元素；
- `q.front()` `q.back()` 返回但是不移除 q 的首元素或尾元素，只有 queue 支持这两个操作；
- `q.top()` 返回但是不移除最高优先级的元素，只有 `priority_queue` 支持此操作；
- `q.push(item)` `q.emplace(args)` 在 queue 的尾部创建一个元素，其值是 item 或者直接以 args 为参数调用构造函数构造一个元素，对于 `priority_queue` 则是在队列的合适的位置；

queue 类使用先进先出（FIFO）的存取策略。进入队列的对象被放在尾部，对象从队列的首部开始移除。`priority_queue` 让我们在元素之间建立一个优先级，新添加的元素被放在所有更低的优先级的元素之前。默认情况下，库使用小于操作符来比较元素类型。

**总结**

标准库中的容器都是模板类型，用来保存给定类型的对象。顺序容器的元素按照他们位置进行排序和访问。顺序容器共享一个共同的标准的接口：如果两个顺序容器提供了一个特定操作，那么在两个顺序容器中这两个操作的接口和函数都一样。

对于大多数人来说，容器定义了特别少的操作。容器定义了构造函数，用于添加和移除元素的操作，已经返回容器的大小的操作，返回特定元素的迭代器的操作。其它的操作，如搜索和排序都没有定义在容器类型中，而定定义在标准算法中。

## 关键术语

- 适配器（adaptor）：一种标准库中的类型、函数或迭代器，将另外一种类型、函数或迭代器包装的像是另外一种类型、函数或迭代器。下一章会介绍迭代器的适配器；
- array ：一种长度固定的顺序容器。想要定义 array 必须同时给定元素类型以及长度。array 中的元素可以通过他们的位置索引进行访问，支持元素的快速随机访问；
- begin ：返回指向容器中首元素的迭代器的容器操作，如果容器是空的，返回尾后迭代器。返回的迭代器是否为 const 取决于容器本身的类型；
- cbegin ： 行为如 begin 一样，不过返回的是 `const_iterator` 类型；
- end ： 返回指向容器中尾元素的下一个位置的迭代器，返回的迭代器是否为 const 取决于容器的类型；
- cend ： 行为与 end 一样，不过返回的是 `const_iterator` 类型；
- 迭代器范围（iterator range）：由一对迭代器表示的元素范围。第一个迭代器表示序列中的首元素，第二个迭代器表示尾元素的下一个位置。如果范围是空的，那么两个迭代器相等（相反的，如果迭代器不相等意味着范围不为空）。如果范围不为空，那么可以通过重复的递增第一个迭代器，从而与第二个迭代器相等。通过递增迭代器，序列中的每个元素都可以被处理；
- 左包含区间（left-inclusive interval）：一个值范围其中包含其首元素，但不是尾元素。形如 `[i,j)` 表示一个序列从 i 并且包含 i 直到 j 但是不包含 j；
- 首前迭代器（off-the-beginning iterator)：在 `forward_list` 中表示一个在首元素之前的一个不存在元素的迭代器。由 `forward_list` 的 `before_begin` 返回，与 end() 迭代器一样，它不可以被解引用；
- 尾后迭代器（off-the-end iterator）：表示序列中最后一个元素的下一个位置的迭代器，通常也被称为尾部迭代器；