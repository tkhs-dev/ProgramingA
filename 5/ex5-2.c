#include <malloc.h>
#include <intrin.h>
#include "stdio.h"
//入力として与えられた数を因数分解して文字列として返す関数
char* factorize(int n){
    int i;
    char* result;
    result=(char*)malloc(sizeof(char)*100);
    for(i=2;i<=n;i++){
        while(n%i==0){
            sprintf(result,"%s%d*",result,i);
            n/=i;
        }
    }
    result[strlen(result)-1]='\0';
    return result;
}
void main(){
    printf(factorize(87936484));
}
