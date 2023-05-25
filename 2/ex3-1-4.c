#include <stdio.h>

int main(int argc, char const *argv[])
{
    /* code */
    unsigned int a,b;

    printf("Please enter number:");
    scanf("%u",&a);
    printf("Please entあr second number:");
    scanf("%u",&b);

    printf("%u+%u=%u\n",a,b,a+b);
    printf("%u-%u=%u\n",a,b,a-b);
    printf("%u*%u=%u\n",a,b,a*b);
    printf("%u/%u=%u\n",a,b,a/b);
    printf("%u%%%u=%u",a,b,a%b);

    return 0;
}
