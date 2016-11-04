#include <stdio.h>
#include <stdlib.h>
int main()
{
    int a;
    //scanf("%d", &a);
    a = 2;
    unsigned char *p  = (unsigned char *)&a;
    printf("o:%02hx", *p);
    printf("%02hx", *(p + 1));
    printf("%02hx", *(p + 2));
    printf("%02hx\n", *(p + 3));
    a+=a*=a-=a*=3;
    printf("a = %d\n", a);
    return 0;
}
