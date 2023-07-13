//インポート
#include <stdio.h>

#define N 10

void main(){
    int id[N];
    int score[N];
    for(int i = 0;i<N;i++){
        printf("%d人目の学籍番号を入力:",i+1);
        scanf("%d",&id[i]);
        if(id[i]==-1)break;//-1が入力されたら終了
        else if(id[i]<0){
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
    printf("成績入力完了\n");
    printf("成績検索\n");
    while(1){
        printf("学籍番号を入力:");
        int input;
        scanf("%d",&input);
        if(input==-1)break;
        int i = 0;
        for(i;i<N;i++){
            if(id[i]==input)break;
        }
        if(i==N){
            printf("該当する学籍番号はありません\n");
        }else{
            printf("学籍番号:%d\n",id[i]);
            printf("点数:%d\n",score[i]);
        }
    }

    int av = 0;
    int n = 0;
    for(n;n<N;n++){
        if(id[n]==-1)break;
        av += score[n];
    }
    av /= n;

    int sd = 0;
    for(int i = 0;i<N;i++){
        if(id[i]==-1)break;
        sd += (score[i]-av)*(score[i]-av);
    }
    sd /= n;

    printf("平均点:%d\n",av);
    printf("標準偏差:%d\n",sd);
}