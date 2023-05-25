#include<stdio.h>
 int main(){
    printf("Please enter a strings\n");

    char* s;
    s = (char *)malloc(sizeof(char) * 128);
    scanf("%s",s);
    printf("The string is:%s\n",s);

    return 0;
 }