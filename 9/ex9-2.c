#include "stdio.h"
int max_val(int ary[]){
    int tmp = 0;
    for(int i = 0;i<sizeof(ary)/2;i++){
        if(ary[i]>tmp)tmp = ary[i];
    }
    return tmp;
}
void main(){
    int ary[10] = {1,2,3,4,5,6,7,8,9,10};
    printf("%d",max_val(ary));
}