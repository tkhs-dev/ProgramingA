//インポート
#include <stdio.h>

#define N 10
unsigned long calc(unsigned long seq[],int n){
    if(seq[n]!=0) return seq[n];
    if(n != 0 && n != 1){
        seq[n] = calc(seq,n-1)+calc(seq,n-2);
    }else{
        seq[n] =  n;
    }
    return seq[n];
}
void main(){
    int seq[N+1] = {0};
    calc(seq,N);
    for(int i = 0;i<=N;i++){
        printf("%d ",seq[i]);
    }
}