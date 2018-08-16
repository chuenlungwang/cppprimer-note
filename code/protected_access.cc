class Base {
friend class Pal;
public:
    void pub_mem();
protected:
    int prot_mem;
private:
    char priv_mem;
};

class Sneaky : public Base {
friend void clobber(Sneaky &);
friend void clobber(Base &);
private:
    int j;
};

void
clobber(Sneaky &s)
{
    s.j = s.prot_mem;
}

void
clobber(Base &b)
{
    //非派生类不能访问其受保护字段
    //b.prot_mem = 0;
}

//////////////////////////////////////////////////////////////////
struct Pub_Derv : public Base {
    int f() { return prot_mem; }
    //派生类不能访问私有成员
    //char g() { return priv_mem; }
};

struct Priv_Derv : private Base {
    int f1() const { return prot_mem; }
};

struct Prot_Derv : protected Base {
    int f() { return prot_mem; }
};

struct Derived_from_Public : public Pub_Derv {
    int use_base() {
        pub_mem();
        return prot_mem;
    }
};

struct Derived_from_Private : public Priv_Derv {
    int use_base() {
        //pub_mem();
        //错误：Base::prot_mem 是 Priv_Derv 的私有成员
        //return prot_mem;
        return 0;
    }
};

struct Derived_from_Protected : public Prot_Derv {
    int use_base() {
        pub_mem();
        return prot_mem;
    }
};

//////////////////////////////////////////////////////////////////////////

class Pal {
public:
    int f(Base b) { return b.prot_mem; }
    //int f2(Sneaky s) { return s.j; }
    int f3(Sneaky s) { return s.prot_mem; }
};

class D2 : public Pal {
public:
    int mem(Base b) {
        //return b.prot_mem;
        return 0;
    }
};

int
main()
{
    Pub_Derv d1;
    Priv_Derv d2;
    Prot_Derv d3;
    d1.pub_mem();
    //私有派生的用户代码不能访问子类的公有成员
    //d2.pub_mem();
    //d3.pub_mem();
    return 0;
}
