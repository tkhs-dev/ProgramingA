#include "stdio.h"
void main(){
    int n;
    printf("please input natural number\n");
    scanf("%d",&n);
    if(!(2<n && n<10)){
        printf("your input is not natural number\n");
        return;
    }
    int data[n][n];
    data[0][0] = 1;
    for(int i = 1;i<n;i++){
        data[i][0] = 1;
        data[i][i] = 1;
        for(int j = 1;j<i;j++){
            data[i][j] = data[i-1][j-1]+data[i-1][j];
        }
    }
    for(int i = 0;i<n;i++){
        for(int j = 0;j<n-i-1;j++){
            printf(" ");
        }
        for(int j = 0;j<=i;j++){
            printf("%d ",data[i][j]);
        }
        printf("\n");
    }
}