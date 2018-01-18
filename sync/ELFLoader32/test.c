volatile unsigned int* const UART0_PTR = (unsigned int *)0x101f1000;

void print_char(char c) { *UART0_PTR = (unsigned int)c; }

void print_str(const char* str)
{
    while (*str != '\0') {
		print_char(*str);
		str++;
    }
}

int _pow(int x, int n)
{
    return n < 2 ? x : x * _pow(x, n - 1);
}

int _start(void)
{
    print_str("Hello, world!\n");
    return _pow(2, 3);
}
