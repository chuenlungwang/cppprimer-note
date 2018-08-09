class Foo {
public:
    Foo sorted() &&; //provide a reference qualifier on all or none of functions
    Foo sorted() const &;

    using Comp = bool(const int &, const int &);
    Foo sorted(Comp *);
    Foo sorted(Comp *) const;
};
