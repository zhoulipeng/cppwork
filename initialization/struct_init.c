#include <stdio.h>

struct abc{
    int a;
    int b;
    int c;
};

int main(int argc, char *argv[])
{
    struct abc st = { 0 };
    char *p = &st;
    printf("%hhx", *p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    return 0;
}
