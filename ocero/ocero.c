#include <string.h>
#include "stdio.h"
#include <conio.h>
#include <synchapi.h>

#define BOARD_SIZE 8
#define SIZE BOARD_SIZE + 2
#define EDGE -1
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define SELECTED 3
#define PLACEABLE 4
#define TOP 'w'
#define BOTTOM 's'
#define LEFT 'a'
#define RIGHT 'd'

int checkLine(char board[SIZE][SIZE],int color,int enemyColor,int x,int y,int vecX,int vecY){
    int count = 0;
    while(1){
        x += vecX;
        y += vecY;
        if(board[y][x] == enemyColor) {
            count++;
        }else if(board[y][x] == color) {
            return count;
        }else{
            return 0;
        }
    }
}
void getPlaceable(char board[SIZE][SIZE],int color,char cells[SIZE][SIZE]){
    int enemyColor = (color == BLACK)?WHITE:BLACK;
    for(int i = 1;i<SIZE-1;i++){
        for(int j = 1;j<SIZE-1;j++){
            cells[i][j] = 0;
            if(board[i][j] == EMPTY){
                int t,tr,r,br,b,bl,l,tl = 0;
                //上
                t = checkLine(board, color, enemyColor, j, i, 0, -1);
                //右上
                tr = checkLine(board, color, enemyColor, j, i, 1, -1);
                //右
                r = checkLine(board, color, enemyColor, j, i, 1, 0);
                //右下
                br = checkLine(board, color, enemyColor, j, i, 1, 1);
                //下
                b = checkLine(board, color, enemyColor, j, i, 0, 1);
                //左下
                bl = checkLine(board, color, enemyColor, j, i, -1, 1);
                //左
                l = checkLine(board, color, enemyColor, j, i, -1, 0);
                //左上
                tl = checkLine(board, color, enemyColor, j, i, -1, -1);
                if(t||tr||r||br||b||bl||l||tl){
                    cells[i][j] = 1;
                }
            }
        }
    }
}

void countCells(char board[SIZE][SIZE],int* black,int* white){
    *black = 0;
    *white = 0;
    for(int i = 1;i<SIZE-1;i++){
        for(int j = 1;j<SIZE-1;j++){
            if(board[i][j] == BLACK){
                *black += 1;
            }else if(board[i][j] == WHITE){
                *white += 1;
            }
        }
    }
}

void renderBoard(char board[SIZE][SIZE],char* message){
    printf("\033[20F");
    printf("\x1b[48;5;22m");
    printf("+");
    for(int i = 1;i<SIZE-1;i++){
        printf(" - +");
    }
    printf("\x1b[0m");
    printf("\n");
    for(int i = 1;i<SIZE-1;i++){
        printf("\x1b[48;5;22m");
        printf("|");
        for(int j = 1;j<SIZE-1;j++){
            switch(board[i][j]){
                case EDGE:
                    printf("###");
                    break;
                case EMPTY:
                    printf("   ");
                    break;
                case BLACK:
                    printf(" ◯ ");
                    break;
                case WHITE:
                    printf(" ● ");
                    break;
                case SELECTED:
                    printf(" ◎ ");
                    break;
                case PLACEABLE:
                    printf(" □ ");
                    break;
            }
            printf("|");
        }
        printf("\x1b[0m");
        printf("          ");
        switch(i){
            case 3:
                printf("操作：WASDを２回押しでカーソル移動.Enterで決定.");
                break;
            case 8:
                printf(message);
        }
        printf("\n");
        printf("\x1b[48;5;22m");
        printf("+ - + - + - + - + - + - + - + - +");
        printf("\x1b[0m");
        printf("\n");
    }
    printf("\x1b[0m");
    printf("\n\n");
}

int place(char board[SIZE][SIZE], int x,int y, int color){
    int enemyColor = (color == BLACK)?WHITE:BLACK;
    board[y][x] = color;

    int t,tr,r,br,b,bl,l,tl = 0;
    //上
    t = checkLine(board, color, enemyColor, x, y, 0, -1);
    //右上
    tr = checkLine(board, color, enemyColor, x, y, 1, -1);
    //右
    r = checkLine(board, color, enemyColor, x, y, 1, 0);
    //右下
    br = checkLine(board, color, enemyColor, x, y, 1, 1);
    //下
    b = checkLine(board, color, enemyColor, x, y, 0, 1);
    //左下
    bl = checkLine(board, color, enemyColor, x, y, -1, 1);
    //左
    l = checkLine(board, color, enemyColor, x, y, -1, 0);
    //左上
    tl = checkLine(board, color, enemyColor, x, y, -1, -1);

    int count = 0;
    while(1){
        count++;
        if(t>=count) board[y-count][x] = color;
        if(tr>=count) board[y-count][x+count] = color;
        if(r>=count) board[y][x+count] = color;
        if(br>=count) board[y+count][x+count] = color;
        if(b>=count) board[y+count][x] = color;
        if(bl>=count) board[y+count][x-count] = color;
        if(l>=count) board[y][x-count] = color;
        if(tl>=count) board[y-count][x-count] = color;
        if(count>=8)break;
    }
    return t+tr+r+br+b+bl+l+tl;
}

int playerTurn(char board[SIZE][SIZE],int color){
    printf("Player's turn\n");
    char tmp[SIZE][SIZE];
    char placeable[SIZE][SIZE] = {0};
    getPlaceable(board, color, placeable);

    int x,y = 3;
    char* message = "";
    while(1){
        message = "";
        Sleep(10);
        wait:
        switch(_getch()){
            case TOP:
                if(board[y-1][x] != EDGE)y--;
                break;
            case BOTTOM:
                if(board[y+1][x] != EDGE)y++;
                break;
            case LEFT:
                if(board[y][x-1] != EDGE)x--;
                break;
            case RIGHT:
                if(board[y][x+1] != EDGE)x++;
                break;
            case 0x0d:
                if(placeable[y][x]){
                    place(board,x,y,color);
                    getPlaceable(board,color,placeable);
                    renderBoard(board,"");
                    return 1;
                }else{
                    message = "そこには置けません。";
                }
                break;
            case 'q':
                return 0;
            default:
                goto wait;
        }
        memcpy(tmp, board, 100);
        tmp[y][x] = SELECTED;
        renderBoard(tmp,message);
    }
}

void main(){
    char board[SIZE][SIZE]={
        {EDGE,EDGE,EDGE,EDGE,EDGE,EDGE,EDGE,EDGE,EDGE,EDGE},
        {EDGE,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EDGE},
        {EDGE,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EDGE},
        {EDGE,EMPTY,WHITE,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EDGE},
        {EDGE,EMPTY,EMPTY,WHITE,WHITE,BLACK,EMPTY,EMPTY,EMPTY,EDGE},
        {EDGE,EMPTY,EMPTY,EMPTY,BLACK,WHITE,EMPTY,EMPTY,EMPTY,EDGE},
        {EDGE,EMPTY,EMPTY,EMPTY,WHITE,WHITE,EMPTY,EMPTY,EMPTY,EDGE},
        {EDGE,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EDGE},
        {EDGE,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EDGE},
        {EDGE,EDGE,EDGE,EDGE,EDGE,EDGE,EDGE,EDGE,EDGE,EDGE}
    };
    renderBoard(board,"");
    char cells[SIZE][SIZE] = {0};
    getPlaceable(board, BLACK, cells);
//    for (int i = 1; i < SIZE-1; ++i) {
//        for (int j = 1; j < SIZE-1; ++j) {
//            if(cells[i][j])board[i][j] = PLACEABLE;
//        }
//    }

    renderBoard(cells,"");
    int flag = 1;
    int count = 0;
    while(flag){
        flag = playerTurn(board,count%2?WHITE:BLACK);
        count++;
    }
}