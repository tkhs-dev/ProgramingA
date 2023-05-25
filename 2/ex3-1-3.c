#include <stdio.h>

int main(int argc, char const *argv[])
{
    /* code */
    int a,b;

    printf("Please enter number:");
    scanf("%d",&a);
    printf("Please enter second number:");
    scanf("%d",&b);

    printf("%d+%d=%d\n",a,b,a+b);
    printf("%d-%d=%d\n",a,b,a-b);
    printf("%d*%d=%d\n",a,b,a*b);
    printf("%d/%d=%d\n",a,b,a/b);
    printf("%d\%%d=%d",a,b,a%b);

    return 0;
}
