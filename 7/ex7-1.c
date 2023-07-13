#include "stdio.h"

#define START 'a'
#define END 'z'
void main(){
    char c;
    while(1){
        printf("input character:");
        scanf(" %c",&c);
        if(c == '.') break;
        for(int i = 0;i<=END-START;i++){
            printf("%c",START+i);
        }
        printf("\n");
        if(START > c || c > END) continue;
        for(int i = 0;i<c-START;i++){
            printf(" ");
        }
        printf("^\n");
    }
}