int main()
{
    double d = 0;
    void *p = &d;
    auto dp = static_cast<double*>(p);
    const double *cdp = static_cast<const double*>(dp);
    double *dp2 = const_cast<double*>(cdp);

    return 0;
}
