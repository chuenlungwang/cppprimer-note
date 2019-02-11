关联容器与顺序容器的本质区别就在于关联容器是按照键的来存取元素的，而顺序容器是按照元素在容器中的位置进行存取的。尽管关联容器和顺序容器共享了很多行为，然而在如何处理键（key）时有很大的区别。

关联容器（Associative containers）支持通过键进行高效的查找。其中最关键的两个关联容器是 map 和 set。map 中的元素是键值对（key-value）：键被当做 map 中的索引，值表示与这个索引相关联的数据。set 只包含键，set 支持高效查询一个键是否在在集合中。

标准库提供了八种关联容器，这八种容器在三个维度上有所不一样：（1）要么是集合（set）要么是映射（map）；（2）是要求键唯一和不唯一；（3）将元素按照有序方式进行存储还是没有此要求；其中可以包含多个键的容器包含 multi ，以无序方式存储的容器以 unordered 开头。

无序容器使用散列函数（hash function）来组织元素。

**Elements Ordered by Key**

- `map` 关联数组；保存键值对；
- `set` 集合，只包含键；
- `multimap` 键可以出现多次的 map；
- `multiset` 键可以出现多次的 set；

**Unordered Collections**

- `unordered_map` 由散列函数组织的关联数组；
- `unordered_set` 由散列函数组织的集合；
- `unordered_multimap` 无序且键可以出现多次的关联数组；
- `unordered_multiset` 无序且键可以出现多次的集合；

## 11.1 使用关联容器

map 类型经常被称为是关联数组（associative array），关联数组类似于常规数组，除了其可以用非整数（integer）作为下标。map 中的值使用键进行查找而不是位置。

set 就是键的集合。set 最常用于查询一个值是否在集合中。

**使用 map**

如以下计数代码：

````cpp
map<string, size_t> word_count; // empty map from string to size_t
string word;
while (cin >> word)
    ++word_count[word]; // fetch and increment the counter for word
for (const auto &w : word_count) // for each element in the map
    // print the results
    cout << w.first << " occurs " << w.second
        << ((w.second > 1) ? " times" : " time") << endl;
````
此处需要注意的是以单词作为下标对 `word_count` 取值时，如果值不存在就会创建一个新的将值设置为 0，并返回值的引用。

我们可以用范围 for（range for) 来遍历 map，其每个结果是一个 pair 对象，pair 类本身也是一个模板类并且有两个公有的数据成员 first 和 second。此处的 first 就是键，second 就是关联的值。

需要了解的是迭代遍历的结果是按照映射的键的顺序进行输出的。

**使用 set**

````cpp
set<string> exclude = {"The", "But", "And", "Or", "An", "A"};
````

与别的容器一样，set 是模板。想要定义一个 set ，需要指定元素的类型，在上面的例子就是 string。与顺序容器一样，可以用列表初始化关联容器的元素。

为了检查键是否存在，可以调用 set 的 find 函数：

````cpp
if (exclude.find(word) == exclude.end())
````

find 函数返回一个迭代器，如果键在 set 中，那么将返回指向那个键的迭代器。如果元素没有找到，find 将返回尾后迭代器。

## 11.2 关联容器简介

有序和无序的关联容器都支持通用的容器操作，这些在 §9.2 中介绍过。关联容器不支持顺序容器特有的与位置相关的操作，如 `push_front` 或 `back` 。由于关联容器中的元素是按照键进行存储的，这些操作对于关联容器来说就没有意义了。同样，关联容器不支持以一个元素值和个数的构造函数和插入操作。

除了支持与顺序容器一样的通用操作之外，关联容器还提供另外一些操作和类型别名，并且无序关联容器还提供了对 hash 性能进行调优的操作。

关联容器的迭代器是双向迭代器的（bidirectional）。

### 11.2.1 定义关联容器

当定义 map 时，需要指定键和值的类型；当定义 set 时需要指定键的类型。每个关联容器都定义了一个默认构造函数，其将创建一个指定类型的空容器。可以将关联容器初始化为另外一个相同类型容器的副本，或者以一系列值进行初始化，只要这些值可以转为容器的类型。在新标准下可以用列表初始化元素：

````cpp
map<string, size_t> word_count;
set<string> exclude = {"the", "but", "and", "or", "an"};
map<string, string> authors = { {"Joyce", "James"}, {"Austen", "Jane"} };
````

与往常一样，初始值必须可以转为容器中的元素类型。对于 set ，其元素类型就是键的类型。当初始化 map 时，必须同时提供 key 和 value。我们将键-值对放在大括弧中如：`{key, value}` 用于表示这两个值共同构成 map 中的一个元素。

**初始化 multimap 和 multiset**

map 和 key 中的键必须是唯一的；对于一个给定的 key 只能有唯一元素与之对应。multimap 和 multiset 容器没有类似的限制；相同的键可以有多个元素与之对应。

### 11.2.2 对于 key 类型的要求

关联容器的键是有限制的。我们将在 11.4 中介绍无序容器的键的要求。对于有序容器 —— map，multimap，set 和 multiset —— 键类型必须定义操作对元素进行比较。默认情况下，标准库使用 `<` 操作符用于建之间的比较。在 set 类型中，键就是元素类型；在 map 类中，键是元素的 first 的类型。

注意：传递给排序算法的可调用对象必须满足与关联容器中的键相同的要求。

**有序容器的 key 类型**

类似于给算法提供我们自己的比较操作，我们也可以给键提供我们的比较操作用于替换 `<` 操作符。这个操作必须符合严格弱顺序（strict weak ordering）。可以认为严格弱顺序与“小于”等同，尽管我们函数可能实现的比较复杂。定义的比较函数必须有以下性质：

- 两个 key 不能互相“小于”对方；如果 k1 小于 k2，那么 k2 必须不小于 k1；
- 如果 k1 小于 k2，并且 k2 小于 k3，那么 k1 必须小于 k3；
- 如果两个键，两个 key 都不小于对方，那么可以认为这两个键是相等的，如果 k1 == k2，并且 k2 == k3 那么，k1 必须与 k3 相等；

如果两个键是相等的，容器认为这两者是一样。当用作 map 的键时，那么将只要一个元素与这两个键匹配，而其中任何一个键都可以作为 map 中的键。

注意：在实践中，作为键的类型需要其 `<` 操作的行为与我们通常遇到的行为是一致的；

**使用比较函数来比较 key 类型**

容器用于组织元素的操作也是容器类型的一部分。为了定义我们自己的操作，必须在定义关联容器类型时提供操作的类型。操作类型被放置在元素类型的后面，这两个类型都放在尖括号中。提供给关联容器的比较函数必须与关联容器的类型相符合。如：

````cpp
bool compareIsbn(const Sales_data &lhs, const Sales_data &rhs) {
    return lhs.isbn() < rhs.isbn();
}

multiset<Sales_data, decltype(compareIsbn)*> bookstore(compareIsbn);
````
当用 decltype 来表示一个函数指针时，必须提供 `*` 来表示我们确实需要的函数的指针，这是由于 decltype 推断出来的是函数的真实类型。当将 compareIsbn 指针作为构造函数实参时，可以直接使用其名字，而不必使用 `&compareIsbn` 的形式，这是由于用于参数的函数名字会隐式转为函数指针。

### 11.2.3 pair 类型

pair 类型定义在 utility 头文件中。pair 中有两个数据成员。与容器一样，pair 是模板。在创建 pair 时必须提供两个类型名字用于表示数据成员的类型。对于两个类型是否相同没有要求，如：
````cpp
pair<string, string> anon;
pair<string, size_t> word_count;
pair<string, vector<int>> line;
````
pair 的默认构造函数对数据成员进行值初始化。我们依然可以给每个成员提供初始值：
````cpp
pair<string, string> author{"James", "Joyce"};
````
pair 的数据成员是 public 的。这两个成员的名字是 first 和 second，通过成员访问符就可以访问这两个成员。

pair 只有很受限的一小部分操作：

- `pair<T1, T2> p;` p 是一个 pair，其中类型 T1 和 T2 所表示的成是值初始化的；
- `pair<T1, T2> p(v1, v2);` p 是一个 pair，其中类型 T1 和 T2 所表示的成员分别从 v1 和 v2 进行初始化；
- `pair<T1, T2> p = {v1, v2};` 与 `p(v1, v2)` 是相同的；
- `make_pair(v1, v2)` 从 v1 和 v2 中初始化一个 pair 对象，pair 的类型是通过 v1 和 v2 的类型进行推断的；
- `p.first` 返回 p 的 public 数据成员 first；
- `p.second` 返回 p 的 public 数据成员 second；
- `p1 relop p2` 关系操作符 relop（`<` `>` `<=` `>=`），关系操作符被定义为字典序：`p1 < p2` 的含义与 `p1.first < p2.first` 或者 `!(p2.first < p1.first) && p1.second < p2.second` 是一样的；
- `p1 == p2` `p1 != p2` 如果两个 pair 的 first 和 second 数据成员分别相等，那么可以认为这两个 pair 是相等的；

**创建 pair 的函数**

如果一个函数需要返回 pair 对象，在新标准下可以列表初始化这个返回值，如：
````cpp
pair<string, int>
process(vector<string> &v)
{
    if (!v.empty()) 
        return {v.back(), v.back().size()};
    else
        return pair<string, int>();
}
````
同样还可以使用 `make_pair` 来创建新的 pair 对象。

## 11.3 关联容器的操作

关联容器还定义了额外的一些类型，这些类型表示容器的 key 和 value 类型：

- `key_type` 容器的 key 的类型；
- `mapped_type` 每个 key 对应的值的类型；只有 map 定义了此类型；
- `value_type` 对于 set 来说与 `key_type` 一样，对于 map 来说就是 `pair<const key_type, mapped_type>`；

对于 set 类型来说，`key_type` 和 `value_type` 是一样的；set 中保存的值就是键。对于 map 来说，其元素是键值对，意味着每个元素是一个 pair 对象，pair 对象中包含了键和与其对应的值。由于我们不能改变元素的键，所以 pair 中的键是 const 的：
````cpp
set<string>::value_type v1; //string
set<string>::key_type v2; //string
map<string, int>::value_type v3; //pair<const string, int>
map<string, int>::key_type v4; //string
map<string, int>::mapped_type v5; //int
````
与顺序容器一样，我们使用作用域操作符（::）来获取类型成员如：`map<string, int>::key_type` ，只有 map 类型 （`unordered_map`, `unordered_multimap`, multimap 和 map）才有 `mapped_type` 。

### 11.3.1 关联容器迭代器

当解引用一个迭代器时，我们得到容器的 `value_type` 类型的值的引用。对于 map 来说其 `value_type` 是一个 pair 其中 first 保存着一个 const 的键，而 second 保存着其对应的值，如：
````cpp
auto map_it = word_count.begin();
cout << map_it->first;
cout << " " << map_it->second;
map_it->first = "new key"; //error: key is const
++map_it->second;
````
笔记：需要记住的是 map 的 `value_type` 是一个 pair 对象，可以改变其 second 数据成员，而不能改变其 first 成员。

**set 的迭代器是 const 的**

尽管 set 类型同时定义了 iterator 和 `const_iterator` 类型，两种迭代器类型都是只读的。正如我们不能改变 map 元素的键，set 中的键也是 const 的。我们可以使用 set 的迭代器进行读而不能写其元素的值：
````cpp
set<int> iset = {0,1,2,3,4,5,6,7,8,9};
set<int>::iterator set_it = iset.begin();
if (set_it != iset.end()) {
    *set_it = 42; //error: set 中的键是只读的
    cout << *set_it << endl;
}
````

**遍历关联容器**

map 和 set 类型都提供了 begin 和 end 操作。使用这两个函数来获取迭代器来遍历容器，如：
````cpp
auto map_it = word_count.cbegin();
while (map_it != word_count.cend()) {
    cout << map_it->first << " occurs "
         << map_it->second << " times" << endl;
    ++map_it;
}
````
当我们用迭代器去遍历 map，multimap，set 以及 multiset 时，其迭代器产生的元素是按照键的正序排列的。

**关联容器和算法**

通常我们是不将通用算法用于关联容器的。关联容器的 key 是 const 的意味着我们不能将关联容器的迭代器传递给那些需要修改元素或者对容器元素进行重排序的算法。这些算法需要对元素进行写操作。而 set 类型中的元素是 const 的，map 的元素是 pair 类型，其 first 成员是 const 的。

关联容器可以用于读元素的算法。然而，这些算法中的大部分都适用于搜索顺序容器。由于关联容器中的元素可以通过其键快速被查找到，使用通用搜索算法几乎总是错误的想法。每个关联容器都定义了名为 find 的成员函数，可以用于直接从关联容器中查找与给定 key 相关联的元素。我们也可以使用通用 find 算法来查找元素，但是算法执行的是顺序搜索。使用 find 成员函数明显是更加快的方式。

在实践中，如果我们真的将关联容器用于通用算法，要么将其用于 source sequence 要么用于 destination。如，调用通用 copy 算法从一个关联容器拷贝元素到另一个序列中。类似的，我们可以调用 inserter 来绑定一个插入迭代器到关联容器上。使用 inserter，我们可以将关联容器作为另外一个算法的目的地（destination）。

### 11.3.2 添加元素

insert 成员添加一个元素或一系列元素。由于 map 和 set（以及与之对应的无序类型）包含的 key 都是唯一的，如果插入的元素是已经存在的将没有任何效果，如：
````cpp
vector<int> ivec = {2,4,6,8,2,4,6,8};
set<int> set2;
set2.insert(ivec.cbegin(), ivec.cend()); //set2有4个元素，没有重复的元素
set2.insert({1,3,5,7,1,3,5,7}); //set2有8个元素
````

关联容器的 insert 操作：
- `c.insert(v)` `c.emplace(args)` v `value_type` 对象；args 用于构建一个元素。对于 map 和 set，只有与给定 key 关联的元素不在 c 中才会被插入。返回值是 pair 类型，其中 first 是给定 key 关联的元素的迭代器，其 second 成员是一个 bool 值表示元素是否被插入成功；对于 multimap 和 multiset，插入给定元素，并返回新元素的迭代器；
- `c.insert(b, e)` `c.insert(il)` b 和 e 是表示类型为 `c::value_type` 的元素范围；il 则是括弧中的一系列值（初始值列表），返回 void；对于 map 和 set，将插入不存在于容器中的 key 关联的元素；对于 multimap 和 multiset 则将插入范围内的每个元素；
- `c.insert(p, v)` `c.emplace(p, args)` 类似于 `insert(v)` 和 `emplace(args)` ，但是使用迭代器 p 作为开始搜索插入元素的位置的索引，返回与给定 key 关联的元素的迭代器；

以一对迭代器或初始值列表作为参数的 insert 版本的运作模式与对应的构造函数是类似的，只有与给定 key 关联的第一个元素会被插入。

**添加元素到 map 中**

当给 map 插入元素时，需要记住元素的类型是 pair。通常当我们插入时，我们并没有现成的 pair 对象。相反，我们会在 insert 的参数中创建一个新的 pair 对象：
````cpp
word_count.insert({word, 1});
word_count.insert(make_pair(word, 1));
word_count.insert(pair<string, size_t>(word, 1));
word_count.insert(map<string, size_t>::value_type(word, 1));
````
正如我们所见，在新标准下创建 pair 最简单的方式是使用括弧初始化（brace initialization）于参数列表。作为另外一种选择，我们可以调用 `make_pair` 或者显式构建 pair。请注意最后一个 insert 中的参数：`map<string, size_t>::value_type(s, 1)` 构建一个合适的 pair 类型的新对象用于插入到 map 中去。

**检查 insert 的返回值**

insert 或者 emplace 的返回值将根据容器类型和参数的不同而不同。对于 key 是唯一的容器，insert 和 emplace 的添加一个元素的版本将返回一个 pair 对象用于判断插入是否发生。pair 的 first 成员是一个与给定 key 关联的元素迭代器；second 成员是一个 bool 值用于指示元素是否被插入，或者是否已经存在。如果键已经存在于容器中，那么 insert 将不做任何事，并且返回值的 bool 部分是 false，如果键不存在，那么元素将被插入并且 bool 是 true。

**添加元素到 multiset 和 multimap 中**

有时我们希望一个 key 可以关联多个值，比如我们希望映射作者到其所写的书的名字上。在这种情况下，一个作者可能有个条目，所以我们使用 multimap 而不是 map。用于 multi 容器中的 key 不需要是唯一的，在这些键上 insert 总是插入元素：
````cpp
multimap<string, string> authors;
authors.insert({"Brath, John", "Sot-Weed Factor"});
authors.insert({"Brath, John", "Lost in the Funhouse"});
````
对于允许多个 key 的容器，只插入一个元素的 insert 操作将返回新元素的迭代器。没有必要返回 bool，因为 insert 总是添加新的元素。

### 11.3.3 移除元素

关联容器定义了三个版本的 erase，与顺序容器一样，可以用 erase 擦除一个元素（通过传递一个迭代器）或者传递迭代器范围来擦除一个范围内的元素。这些版本的 erase 类似于对应的顺序容器的操作：指定的元素被移除并返回 void。

- `c.erase(k)` 从 c 中移除任何与键 k 相对应的元素，返回 `size_type` 的值用于表示移除的元素的个数；
- `c.erase(p)` 从 c 中移除由迭代器 p 表示的元素，p 必须确实表示 c 中的一个元素；它不能等于 `c.end()` ，返回指向 p 的下一个元素的迭代器或者当 p 为最后一个元素时返回 `c.end()` ；
- `c.erase(b, e)` 从 c 中移除由 b 和 e 所表示的范围中的元素，返回 e；

关联容器还支持一个额外的 erase 操作，这个版本的 erase 有一个 `key_type` 参数，这个版本的 erase 将移除所有与给定 key 相关的元素，并返回一个计数器告知移除了多少个元素。对于键是唯一的容器来说，erase 的返回值要么是 0 要么是 1，如果返回 0 则想要移除的元素不在容器中。对于 multi 容器来说，被移除的元素的数目可能会大于一。

### 11.3.4 map 的下标操作

map 和 `unordered_map` 容器提供了下标操作符和一个对应的 at 函数。set 类型不支持下标，原因是 set 中没有“值”与键相对应。元素自己就是键，所以“获取与 key 相关的值”是没有意义的。我们不能对 `multimap` 和 `unordered_multimap` 进行下标操作，原因是可能有超过一个值与给定的键对应。

- `c[k]` 返回与键 k 关联的元素；如果 k 不在 c 则添加一个新的值初始化的元素与键 k 进行关联；
- `c.at(k)` 检查并访问与键 k 相关的元素；如果 k 不在 c 中则抛出 `out_of_range` 异常；

与别的下标操作符一样，map 的下标取一个索引（键 key）然后提取出与这个键关联的值。然而，与别的下标操作符不一样的是，如果键不存在，一个新的元素将被创建出来与这个 key 关联并且插入到 map 中去。关联的值是值初始化的（value initialized）。

由于下标操作符可能会插入一个元素，我们只能将下标操作用于非 const 的 map 之上。

注意：map 的下标运算与数组和 vector 的下标操作有非常大的不同：使用不存在于容器中的 key，将导致与之关联的元素添加到 map 中。

**使用下标操作返回的值**

map 的下标操作与别的下标操作符的不同之处在于其返回值。通常，对迭代器进行解引用得到的类型与下标操作符得到的类型是一样的。但对于 map 来说是不成立的：当我们对 map 进行下标运算时，我们得到一个 `mapped_type` 类型的对象；当我们解引用 map 的迭代器时，我们得到一个 `value_type` 类型的对象。

与别的下标运算一样的是，map 的下标运算返回一个左值。由于返回的值是左值，我们对元素进行读或写：
````cpp
cout << word_count["Anna"];
++word_count["Anna"];
cout << word_count["Anna"];
````

注意：与 vector 和 string 不一样的是，map 的下标操作符的返回值类型与通过解引用 map 的迭代器得到的类型是不一样的。

有时我们仅仅想知道一个元素是否在容器中，而不想在元素不存在时添加一个。在这种情况下，我们一定不能使用下标操作符。

### 11.3.5 访问元素

关联容器提供了许多中方式来查找一个给定的元素。使用哪个操作取决于我们尝试解决什么样的问题。如果我们只关心一个特定的元素是否在容器中，最好是使用 find。对于键是唯一的容器来说，使用 find 或者 count 都是一样的。然而，对于具有 multi 键的容器来说，count 所做的工作更多：如果一个元素是存在的，它依然必须对有多少个元素具有相同的键进行计数。如果我们不需要计数，那么最好是使用 find：

**`lower_bound` 和 `upper_bound`对于无序容器来说是非法的，下标和 at 操作只对非 const 的 map 和 `unordered_map`有效**

- `c.find(k)` 返回与键 k 关联的第一个元素的迭代器，当 k 不在容器中时返回尾后迭代器；
- `c.count(k)` 返回与键 k 关联的元素的个数。对于键是唯一的容器来说，结果总是零或一；
- `c.lower_bound(k)` 返回键不小于 k 的第一个元素的迭代器；
- `c.upper_bound(k)` 返回键大于 k 的第一个元素的迭代器；
- `c.equal_range(k)` 返回与键 k 相关的所有元素的一个迭代器范围的 pair，如果 k 不在容器中，两个成员都是 `c.end()`；

**使用 find 替换 map 的下标**

对于 map 和 `unordered_map` 类型，下标操作符是最简单的获取一个值的方式。然而，正如我们所看到的，使用下标有一个重要的副作用：如果 key 当时不在 map 中，那么下标操作会插入一个与这个 key 相关联的元素。这个行为是否正确取决于我们的期望。

有时我们仅仅只是想知道一个给定的 key 是否在 map 中而不想改变这个关联容器。我们就不能用下标操作符来确认元素是否包含在容器中，这是由于下标操作符会在键不在容器中时插入一个新的与之相关的元素。在这种情况下，我们应该使用 find 函数：
````cpp
if (word_count.find("foobar") == word_count.end())
    cout << "foobar is not in the map" << endl;
````

**在 multimap 和 multiset 中查找元素**

在键是唯一的关联容器中查找一个元素是十分简单的——元素要么在要么不在容器中。对于允许多个相同的键存在的容器，这个过程就更为复杂了：与给定 key 关联的元素可能多个。当 multimap 或者 multiset 有多个与给定 key 关联的元素时，这些元素在容器中是相邻存储的。如上面提到的作者和标题之间的 multimap，我们想要打印跟特定作者关联的所有书名，至少有三种方式来解决这个问题，最简单而明显的方式使用 find 和 count：
````cpp
string search_item("Alain de Botton");
auto entries = authors.count(search_item);
auto iter = authors.find(search_item);
while (entries) {
    cout << iter->second << endl;
    ++iter; //推进到下一个标题
    --entries; //记录已经打印的个数
}
````
find 返回与给定 key 关联的第一个元素的迭代器。标准库保证迭代 multimap 或 multiset 将顺序返回与给定键关联的所有元素。

**迭代器方式的解决方案**

另一种选择是使用 `lower_bound` 和 `upper_bound` 来解决问题。这两个操作都以 key 为参数并返回一个迭代器。如果 key 在容器中，由 `lower_bound` 返回的迭代器将会指向与 key 关联的第一个元素，而由 `upper_bound` 返回的迭代器则指向与 key 关联的最后一个元素的后一个位置。如果元素不在 multimap 钟，那么 `lower_bound` 和 `upper_bound` 将返回相等的迭代器；两个都将返回一个迭代器指向 key 可以合适插入的位置（插入后依然保持有序）。因此，调用 `lower_bound` 和 `upper_bound` 于相同的 key 上时，将产生与此 key 关联的所有元素的迭代器范围。

当然，这两个操作返回的迭代器可能是容器的尾后迭代器（off-the-end iterator）。如果查找的 key 在容器中是最大的，那么 `upper_bound` 将返回尾后迭代器。如果 key 不在容器中且比容器中任何键都大，那么 `lower_bound` 将返回尾后迭代器。

注意：`lower_bound` 返回的迭代器可能不会指向指定 key 所关联的元素。如果 key 不在容器中，那么 `lower_bound` 将返回一个表示这个 key 可以插入的位置的迭代器，当插入之后元素的顺序将保持不变。

使用这两个操作，我们可以重写程序为：
````cpp
for (auto beg = authors.lower_bound(search_item),
          end = authors.upper_bound(search_item);
     beg != end; ++beg) {
         cout << beg->second << endl;
}
````
这两个操作不会告知 key 是否在容器中，其重要之处在于返回值的行为与迭代器范围（iterator range）一样。如果这个 key 没有关联的元素，那么 `lower_bound` 和 `upper_bound` 的返回值相同。两个结果都指向 key 插入的位置。

假如有元素与这个 key 关联，beg 将指向第一个元素。通过递增 beg 可以遍历与这个 key 关联的元素。迭代器 end 表示我们已经遍历了所有的元素，当 beg 和 end 相等时，就表示我们已经遍历了全部的元素。

注意：当 `lower_bound` 和 `upper_bound` 返回相同的迭代器，表示给定的 key 不在容器中。

**`equal_range` 函数**

剩下的解决方案是最直接的：相较于调用 `upper_bound` 和 `lower_bound`，我们可以调用 `equal_range`。这个函数以一个 key 为参数并返回一个 pair 类型以包含 iterator，如果 key 存在于容器中，那么 first 表示与键关联的第一个元素，second 表示与 key 关联的最后一个元素的下一个位置。如果没有匹配的元素，两个迭代器都指向 key 插入的位置。我们可以用 `equal_range` 再次修改程序：
````cpp
for (auto pos = authors.equal_range(search_item);
     pos.first != pos.second; ++pos.first) {
         cout << pos.first->second << endl;
     }
````
`equal_range` 的返回值 pair 中的 first 成员包含的迭代器与 `lower_bound` 的返回值一样，second 成员与 `upper_bound` 函数的返回值一样。因此，在这个程序中，`pos.first` 与 `beg` 是一样的，`pos.second` 与 `end` 是一样的。

## 11.4 无序容器

在新标准中定义四个无序关联容器（unordered associative containers）。相较于前面的关联容器使用比较操作（comparison operation）来组织元素，这些容器使用的哈希函数（hash function）以及 key 类型的相等比较操作符。无序容器最常用于 key 类型没有明显的顺序关系的情形。这些容器也用于那些维护元素的顺序是很昂贵的应用。

尽管哈希在原则上可以达到更好的平均性能，想要在实践中达到好的结果通常需要一些性能上的测试和调整。因而，通常使用有序容器更加容器且通常拥有更好的性能。

提示：仅仅在 key 类型天生是不可排序的，或者性能测试显示哈希可以解决问题时才使用无序容器。

**使用无序容器**

除了管理哈希的操作之外，无序容器提供与有序容器一样的操作（find，insert 和别的操作）。意味着用于 map 和 set 的操作同样可以用于 `unordered_map` 和 `unordered_set` 容器。对于 multi 的无序容器也是一样的，有序容器的所有操作都可以用于无序容器。

因而，我们通常使用无序容器来替换相应的有序容器，或者执行相反的替换。然而，由于元素不是有序存储的，使用无序容器的输出与使用有序容器的相同程序的输出通常是不一样的。

**管理 bucket**

无序容器被组织为一系列桶（bucket），每个桶中装有零个或多个元素。这些容器使用哈希函数（hash function）来将元素映射到桶上。为了访问元素，容器首先计算元素的哈希值（hash code），用以得到需要搜索的桶。容器将所有哈希值相同的元素都放在一个相同的桶中。如果容器允许一个给定的 key 有多个元素与之相关联，那么这些关联的元素都在同一个桶中。因而，无序容器的性能取决于哈希函数的质量以及桶的数量（number）和大小（size）。

其中哈希函数必须对于相同的参数总是产生相同的结果。理想情况下，哈希函数将每个特定的值映射到独一无二的桶上。然而，一个哈希函数允许将多个不同的 key 的元素映射到相同的桶上。当一个桶中有多个元素时，这些元素将被顺序查找以发现我们想要的那一个。通常情况下，计算元素的哈希值和查找其 bucket 是很快的操作。然而，如果桶中有很多元素，将需要非常多的比较操作来找到那个特定的元素。

无序容器提供了一系列函数用于管理 bucket。这些成员函数让我们可以查询容器的状态以及强制容器在需要时重新组织。

**Table 11.8 无序容器管理操作**

**Bucket Interface**

- `c.bucket_count()` 查询正在使用的 bucket 的个数；
- `c.max_bucket_count()` 这个容器可以容纳的 bucket 的最大数目；
- `c.bucket_size(n)` n 号桶容纳的元素的个数；
- `c.bucket(k)` 返回键 k 可能被找到的桶，类型是`size_type`

**Bucket 迭代器**

- `local_iterator` 可以访问 bucket 中的元素的迭代器类型；
- `const_local_iterator` bucket iterator 的 const 版本；
- `c.begin(n)` `c.end(n)` bucket n 中的首元素迭代器以及尾后元素迭代器；
- `c.cbegin(n)` `c.cend(n)` 上一条目中的 const 版本迭代器；

**hash 策略**

- `c.load_factor()` 每个 bucket 中的平均元素个数，返回值是 float 类型；
- `c.max_load_factor()` c 试图维护的桶的平均大小。c 会增加桶的数量以维护 `load_factor <= max_load_factor`，返回值是 float 类型；
- `c.rehash(n)` 重新调整存储从而 `bucket_count >= n` 并且 `bucket_count > size/max_load_factor`，如果 n 大于容器的当前桶的数目（`bucket_count`），rehash 将强制执行，新的桶数目将大于或等于 n，如果 n 小于容器的当前桶的数目那么这个函数可能没有任何作用；
- `c.reserve(n)` 重新调整存储从而 c 可以包含 n 个元素而不需要 rehash；

**无序容器的 key 类型要求**

默认情况下，无序容器使用相等操作符于键类型上来比较元素。它们使用 `hash<key_type>` 类型的对象来产生每个元素的哈希值。标准库提供了内置类型（包括指针）的 hash 模板来产生哈希值。标准库同时还定义了一些库类型的 hash 模板，包括 string 类型和智能指针类型。因而，我们可以直接定义内置类型或者 string 或智能指针的无序容器。

然而，我们不能直接定义我们自己的类类型作为键的无序容器。与容器不一样，我们不能直接使用 hash 模板。相反，我们必须提供自己的 hash 模板版本，在 16.5 中将介绍使用类模板特例化（class template specializations）来提供自己的 hash 模板版本。

除了使用默认的 hash 模板之外，我们还可以提供自己的相等比较函数和计算哈希值的函数。这个技术在 11.2.2 中使用过。如：
````cpp
size_t hasher(const Sales_data &sd)
{
    return hash<string>()(sd.isbn());
}
bool eqOp(const Sales_data &lhs, const Sales_data &rhs)
{
    return lhs.isbn() == rhs.isbn();
}

using SD_multiset = unordered_multiset<Sales_data, decltype(hasher)*, decltype(eqOp)*>;

SD_multiset bookstore(42, hasher, eqOp);
````
上面的代码同时指定了 hash 函数和相等性比较函数，如果我们的类有自己的 `==` 操作符，可以仅仅只覆盖哈希函数：
````cpp
unordered_set<Foo, decltype(FooHash)*> fooSet(10, FooHash);
````