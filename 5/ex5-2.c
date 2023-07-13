#include "stdio.h"

void main() {
    int n;
    printf("Please enter number:");
    scanf("%d", &n);
    printf("%d=", n);
    if(n <= 1){
        printf("%d",n);
        return;
    }
    for (int i = 2; i <= n; i++) {
        while (n % i == 0) {
            printf("%d", i);
            n /= i;
            if (n != 1) {
                printf("*");
            }
        }
    }
}
