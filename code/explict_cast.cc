int main()
{
    double d = 0;
    void *p = &d;
    auto dp = static_cast<double*>(p);
    const double *cdp = static_cast<const double*>(dp);
    double *dp2 = const_cast<double*>(cdp);
    static_cast<void>(dp2);

    double * const xdp3 = &d;
    double *dp3 = static_cast<double *>(xdp3);
    static_cast<void>(dp3);
    double *dp4 = const_cast<double *>(xdp3);
    static_cast<void>(dp4);

    return 0;
}
