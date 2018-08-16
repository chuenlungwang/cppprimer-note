class Base {
public:
    virtual int fcn();
};

class D1 : public Base {
public:
    int fcn(int);
    virtual void f2();
};

class D2 : public D1 {
public:
    int fcn(int);
    int fcn() override;
    void f2() override;
};

int
main()
{
    Base bobj;
    D1 d1obj;
    D2 d2obj;
    Base *bp1 = &bobj, *bp2 = &d1obj, *bp3 = &d2obj;

    bp1->fcn(); //Base::fcn
    bp2->fcn(); //Base::fcn
    bp3->fcn(); //D2::fcn

    D1 *d1p = &d1obj;
    D2 *d2p = &d2obj;
    //bp2->f2(); //错误: Base 没有 f2() 函数
    d1p->f2();
    d2p->f2();

    Base *p1 = &d2obj;
    D1 *p2 = &d2obj;
    D2 *p3 = &d2obj;
    //p1->fcn(42); //错误: Base 没有 fcn(int) 函数
    p2->fcn(42); //D1::fcn(int)
    p3->fcn(42); //D2::fcn(int)
    return 0;
}
