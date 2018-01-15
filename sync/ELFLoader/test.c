int _pow(int x, int n)
{
    return n < 2 ? x : x * _pow(x, n - 1);
}

int _start(void)
{
    return _pow(2, 3);
}
