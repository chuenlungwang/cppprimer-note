void reset(int &i)
{
    i = 0;
}

int main()
{
    int i = 100;
    reset(i);
    //reset(100); //非法的，100是常量引用
    return 0;
}
