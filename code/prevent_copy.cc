struct NoCopy {
    NoCopy() = default;
    NoCopy(const NoCopy&) = delete;
    NoCopy &operator=(const NoCopy&) = delete;
    ~NoCopy() = default;
};

struct NoDtor {
    NoDtor() = default;
    ~NoDtor() = delete;
};

class PrivateCopy {
    PrivateCopy(const PrivateCopy &);
    PrivateCopy& operator=(const PrivateCopy &);
public:
    PrivateCopy() = default;
    ~PrivateCopy();
};

int main()
{
    NoCopy nc1;
    //NoCopy nc2 = nc1;
    //nc2 = nc1;

    //static NoDtor nd; //错误!! 不能创建删除了析构函数的普通
    NoDtor *p = new NoDtor(); //依然可以动态分配这种对象
    //delete p; //错误!! 不能删除对象，其析构函数已经被删除了

    PrivateCopy pc1;
    //PrivateCopy pc2 = pc1;
    return 0;
}
