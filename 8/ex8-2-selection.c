//インポート
#include <stdio.h>

#define N 100

void main(){
    int n = N;
    int id[N];
    int score[N];
    for(int i = 0;i<n;i++){
        printf("%d人目の学籍番号を入力:",i+1);
        scanf("%d",&id[i]);
        if(id[i]==-1) {
            n = i;
            break;//-1が入力されたら終了
        }
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

    //セレクトソート
    for(int i = 0;i<n;i++){
        int max_index = i;
        int max = 0;
        for(int j = i;j<n;j++){
            if(score[j]>max){
                max = score[j];
                max_index = j;
            }
        }
        int tmp = score[i];
        score[i] = score[max_index];
        score[max_index] = tmp;
        tmp = id[i];
        id[i] = id[max_index];
        id[max_index] = tmp;
    }

    printf("+ 学籍番号 + 成績 + \n");
    for(int i = 0;i<n;i++){
        if(id[i]==-1)break;
        printf("| %8d | %4d |\n",id[i],score[i]);
    }
    printf("+----------+------+\n");

    int av = 0;
    for(int i=0;i<n;i++){
        av += score[i];
    }
    av /= n;

    int sd = 0;
    for(int i = 0;i<n;i++){
        if(id[i]==-1)break;
        sd += (score[i]-av)*(score[i]-av);
    }
    sd /= n;

    printf("平均点:%d\n",av);
    printf("標準偏差:%d\n",sd);
}