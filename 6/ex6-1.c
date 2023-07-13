//インポート
#include <stdio.h>

#define N 2

void main(){
    int id[N];
    int score[N];
    for(int i = 0;i<N;i++){
        printf("%d人目の学籍番号を入力:",i+1);
        scanf("%d",&id[i]);
        if(id[i]<0){
            printf("学籍番号は正の整数で入力してください");
            return;
        }
        printf("%d人目の点数を入力:",i+1);
        scanf("%d",&score[i]);
        if(score[i]<0){
            printf("点数は正の整数で入力してください");
            return;
        }
    }
    printf("+ 学籍番号 + 成績 + \n");
    for(int i = 0;i<N;i++){
        printf("| %8d | %4d |\n",id[i],score[i]);
    }
    printf("+----------+------+\n");

    int av = 0;
    for(int i = 0;i<N;i++){
        av += score[i];
    }
    av /= N;

    int sd = 0;
    for(int i = 0;i<N;i++){
        sd += (score[i]-av)*(score[i]-av);
    }
    sd /= N;

    printf("平均点:%d\n",av);
    printf("標準偏差:%d\n",sd);
}