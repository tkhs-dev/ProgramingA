#include <stdio.h>

int main(int argc, char const *argv[])
{
    /* code */
    int a,b;

    a = 100;
    b = 23;

    printf("%d+%d=%d\n",a,b,a+b);
    printf("%d-%d=%d\n",a,b,a-b);
    printf("%d*%d=%d\n",a,b,a*b);
    printf("%d/%d=%d\n",a,b,a/b);
    printf("%d\%%d=%d",a,b,a%b);

    return 0;
}
