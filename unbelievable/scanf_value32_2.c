#include <stdio.h>
#include <stdlib.h>
int main()
{
    int a;
    scanf("%d", &a);
    a+=a*=a-=a*=3;
    printf("a = %d\n", a);
    return 0;
}
