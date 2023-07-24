#include <string.h>
#include "stdio.h"
#include <conio.h>
#include <synchapi.h>
#include <stdbool.h>
#include <time.h>

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

typedef struct {
    char x;
    char y;
} vec2c;

vec2c init_vec2c(char x, char y) {
    vec2c tmp;
    tmp.x = x;
    tmp.y = y;
    return tmp;
}

int check_line(char board[SIZE][SIZE], int color, int enemyColor, vec2c coordinate, vec2c vec) {
    int count = 0;
    while (1) {
        coordinate.x += vec.x;
        coordinate.y += vec.y;
        if (board[coordinate.y][coordinate.x] == enemyColor) {
            count++;
        } else if (board[coordinate.y][coordinate.x] == color) {
            return count;
        } else {
            return 0;
        }
    }
}

char get_placeable(char board[SIZE][SIZE], int color, char cells[SIZE][SIZE]) {
    int enemyColor = (color == BLACK) ? WHITE : BLACK;
    char count = 0;
    for (int i = 1; i < SIZE - 1; i++) {
        for (int j = 1; j < SIZE - 1; j++) {
            cells[i][j] = 0;
            if (board[i][j] == EMPTY) {
                int t, tr, r, br, b, bl, l, tl = 0;
                vec2c coordinate = init_vec2c(j, i);
                //上
                t = check_line(board, color, enemyColor, coordinate, init_vec2c(0, -1));
                //右上
                tr = check_line(board, color, enemyColor, coordinate, init_vec2c(1, -1));
                //右
                r = check_line(board, color, enemyColor, coordinate, init_vec2c(1, 0));
                //右下
                br = check_line(board, color, enemyColor, coordinate, init_vec2c(1, 1));
                //下
                b = check_line(board, color, enemyColor, coordinate, init_vec2c(0, 1));
                //左下
                bl = check_line(board, color, enemyColor, coordinate, init_vec2c(-1, 1));
                //左
                l = check_line(board, color, enemyColor, coordinate, init_vec2c(-1, 0));
                //左上
                tl = check_line(board, color, enemyColor, coordinate, init_vec2c(-1, -1));
                if (t || tr || r || br || b || bl || l || tl) {
                    cells[i][j] = 1;
                    count++;
                }
            }
        }
    }
    return count;
}

void count_cells(char board[SIZE][SIZE], int *black, int *white) {
    *black = 0;
    *white = 0;
    for (int i = 1; i < SIZE - 1; i++) {
        for (int j = 1; j < SIZE - 1; j++) {
            if (board[i][j] == BLACK) {
                *black += 1;
            } else if (board[i][j] == WHITE) {
                *white += 1;
            }
        }
    }
}

void render_board(char board[SIZE][SIZE], char *message) {
    printf("\033[20F");
    printf("\x1b[48;5;22m");
    printf("+");
    for (int i = 1; i < SIZE - 1; i++) {
        printf(" - +");
    }
    printf("\x1b[0m");
    printf("\n");
    for (int i = 1; i < SIZE - 1; i++) {
        printf("\x1b[48;5;22m");
        printf("|");
        for (int j = 1; j < SIZE - 1; j++) {
            switch (board[i][j]) {
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
        switch (i) {
            case 1:
                printf("操作：WASDでカーソル移動.");
                break;
            case 2:
                printf("操作：Enterで置く.");
                break;
            case 5:
                printf("黒:%2d", (board[0][0] - EDGE) * (-1));
                break;
            case 6:
                printf("白:%2d", (board[0][SIZE - 1] - EDGE) * (-1));
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

char place(char board[SIZE][SIZE], vec2c dst, int color) {
    int enemyColor = (color == BLACK) ? WHITE : BLACK;
    board[dst.y][dst.x] = color;

    int t, tr, r, br, b, bl, l, tl = 0;
    //上
    t = check_line(board, color, enemyColor, dst, init_vec2c(0, -1));
    //右上
    tr = check_line(board, color, enemyColor, dst, init_vec2c(1, -1));
    //右
    r = check_line(board, color, enemyColor, dst, init_vec2c(1, 0));
    //右下
    br = check_line(board, color, enemyColor, dst, init_vec2c(1, 1));
    //下
    b = check_line(board, color, enemyColor, dst, init_vec2c(0, 1));
    //左下
    bl = check_line(board, color, enemyColor, dst, init_vec2c(-1, 1));
    //左
    l = check_line(board, color, enemyColor, dst, init_vec2c(-1, 0));
    //左上
    tl = check_line(board, color, enemyColor, dst, init_vec2c(-1, -1));

    int count = 0;
    while (1) {
        count++;
        if (t >= count) board[dst.y - count][dst.x] = color;
        if (tr >= count) board[dst.y - count][dst.x + count] = color;
        if (r >= count) board[dst.y][dst.x + count] = color;
        if (br >= count) board[dst.y + count][dst.x + count] = color;
        if (b >= count) board[dst.y + count][dst.x] = color;
        if (bl >= count) board[dst.y + count][dst.x - count] = color;
        if (l >= count) board[dst.y][dst.x - count] = color;
        if (tl >= count) board[dst.y - count][dst.x - count] = color;
        if (count >= BOARD_SIZE)break;
    }
    count = t + tr + r + br + b + bl + l + tl;
    if (color == BLACK) {
        board[0][0] += -count - 1;
        board[0][SIZE - 1] += count;
    } else {
        board[0][0] += count;
        board[0][SIZE - 1] += -count - 1;
    }
    return count;
}

char *get_color_display_name(int color) {
    switch (color) {
        case BLACK:
            return "黒";
        case WHITE:
            return "白";
    }
}

char *get_player_display_name(char plr) {
    switch (plr) {
        case 0:
            return "PLAYER";
        case 1:
            return "COMPUTER Lv.0";
        case 2:
            return "COMPUTER Lv.1";
        case 3:
            return "COMPUTER Lv.2";
    }
}

vec2c intelligent_computer_lv1(char board[SIZE][SIZE], char placeable[SIZE][SIZE], char num, int color) {
    char idx;
    vec2c dst[num];
    char dstCount = 0;
    char maxChanged = 0;
    for (char i = 1; i < SIZE - 1; i++) {
        for (char j = 1; j < SIZE - 1; j++) {
            if (placeable[i][j]) {
                char tmp[SIZE][SIZE] = {0};
                memcpy(tmp, board, 100);
                char changed_tmp = place(tmp, init_vec2c(j, i), color);
                if (i == 1 || i == SIZE - 2) {
                    changed_tmp *= 2;
                    if (j == 1 || j == SIZE - 2) {
                        changed_tmp *= 2;
                    }
                }
                if (j == 1 || j == SIZE - 2) {
                    changed_tmp *= 2;
                    if (i == 1 || i == SIZE - 2) {
                        changed_tmp *= 2;
                    }
                }

                if (changed_tmp > maxChanged) {
                    maxChanged = changed_tmp;
                    dstCount = 1;
                    dst[0] = init_vec2c(j, i);
                } else if (changed_tmp == maxChanged) {
                    dst[dstCount] = init_vec2c(j, i);
                    dstCount++;
                }
            }
        }
    }
    idx = rand() % dstCount;
    return dst[idx];
}

vec2c intelligent_computer_lv2(char board[SIZE][SIZE], char placeable[SIZE][SIZE], char num, int color) {
    int enemyColor = (color == BLACK) ? WHITE : BLACK;
    char idx;
    vec2c dst[num];
    char dstCount = 0;
    char maxChanged = 0;
    for (char i = 1; i < SIZE - 1; i++) {
        for (char j = 1; j < SIZE - 1; j++) {
            if (placeable[i][j]) {
                char tmp[SIZE][SIZE] = {0};
                memcpy(tmp, board, 100);
                char changed_tmp = place(tmp, init_vec2c(j, i), color);
                if (i == 1 || i == SIZE - 2) {
                    changed_tmp *= 2;
                    if (j == 1 || j == SIZE - 2) {
                        changed_tmp *= 2;
                    }
                }
                if (j == 1 || j == SIZE - 2) {
                    changed_tmp *= 2;
                    if (i == 1 || i == SIZE - 2) {
                        changed_tmp *= 2;
                    }
                }

                char tmp_placeable[SIZE][SIZE] = {0};
                memcpy(tmp_placeable, placeable, 100);

                bool enemy_can_place_corner = false;

                if (get_placeable(tmp, enemyColor, tmp_placeable) > 0) {
                    enemy_can_place_corner = tmp_placeable[1][1] == 1 || tmp_placeable[1][SIZE - 2] == 1 ||
                                             tmp_placeable[SIZE - 2][1] == 1 || tmp_placeable[SIZE - 2][SIZE - 2] == 1;

                    vec2c dst = intelligent_computer_lv1(tmp, tmp_placeable, num, enemyColor);
                    place(tmp, dst, enemyColor);
                }
                if (get_placeable(tmp, color, tmp_placeable) > 0) {
                    char maxChanged2 = 0;
                    char tmp2[SIZE][SIZE] = {0};
                    for (char i2 = 1; i2 < SIZE - 1; i2++) {
                        for (char j2 = 1; j2 < SIZE - 1; j2++) {
                            if (placeable[i2][j2]) {
                                memcpy(tmp2, tmp, 100);
                                char changed_tmp2 = place(tmp2, init_vec2c(j2, i2), color);
                                if (i2 == 1 || i2 == SIZE - 2) {
                                    changed_tmp2 *= 2;
                                    if (j2 == 1 || j2 == SIZE - 2) {
                                        changed_tmp2 *= 6;
                                    }
                                }
                                if (j2 == 1 || j2 == SIZE - 2) {
                                    changed_tmp2 *= 2;
                                    if (i2 == 1 || i2 == SIZE - 2) {
                                        changed_tmp2 *= 6;
                                    }
                                }
                                if (changed_tmp2 > maxChanged2) {
                                    maxChanged2 = changed_tmp2;
                                }
                            }
                        }
                    }
                    changed_tmp += maxChanged2 * 0.5;
                    if (enemy_can_place_corner)changed_tmp *= 0;
                }

                if (changed_tmp > maxChanged) {
                    maxChanged = changed_tmp;
                    dstCount = 1;
                    dst[0] = init_vec2c(j, i);
                } else if (changed_tmp == maxChanged) {
                    dst[dstCount] = init_vec2c(j, i);
                    dstCount++;
                }
            }
        }
    }
    idx = rand() % dstCount;
    return dst[idx];
}

vec2c random_computer(char board[SIZE][SIZE], char placeable[SIZE][SIZE], char num, int color) {
    char idx = rand() % num;
    char count = 0;
    for (char i = 1; i < SIZE - 1; i++) {
        for (char j = 1; j < SIZE - 1; j++) {
            if (placeable[i][j]) {
                if (count == idx) {
                    return init_vec2c(j, i);
                }
                count++;
            }
        }
    }
}

int turn_computer(char board[SIZE][SIZE], int color, int level) {
    char placeable[SIZE][SIZE] = {0};
    char num = get_placeable(board, color, placeable);
    if (num == 0)return 2;

    char message[30];
    sprintf(message, "ターン: %s(%s)", get_color_display_name(color), get_player_display_name(level + 1));

    char tmp[SIZE][SIZE] = {0};
    memcpy(tmp, board, 100);

    vec2c dst;
    switch (level) {
        case 0:
            dst = random_computer(board, placeable, num, color);
            break;
        case 1:
            dst = intelligent_computer_lv1(board, placeable, num, color);
            break;
        case 2:
            dst = intelligent_computer_lv2(board, placeable, num, color);
            break;
    }
    //_getch()==0x0d;
    //_getch();
    //Sleep(50);

    place(board, dst, color);
    render_board(board, message);
    return 1;
}

int turn_player(char board[SIZE][SIZE], int color) {
    char placeable[SIZE][SIZE] = {0};
    int count = get_placeable(board, color, placeable);
    if (count == 0)return 2;

    printf("プレイヤー(%s)のターン\n", get_color_display_name(color));

    vec2c pos = init_vec2c(5, 5);
    char *message = "";
    while (1) {
        message = "";
        Sleep(10);
        wait:
        switch (_getch()) {
            case TOP:
                if (board[pos.y - 1][pos.x] > EDGE)pos.y--;
                break;
            case BOTTOM:
                if (board[pos.y + 1][pos.x] > EDGE)pos.y++;
                break;
            case LEFT:
                if (board[pos.y][pos.x - 1] > EDGE)pos.x--;
                break;
            case RIGHT:
                if (board[pos.y][pos.x + 1] > EDGE)pos.x++;
                break;
            case 0x0d:
                if (placeable[pos.y][pos.x]) {
                    place(board, pos, color);
                    render_board(board, "");
                    return 1;
                } else {
                    message = "そこには置けません。";
                }
                break;
            case 'q':
                return 0;
            default:
                goto wait;
        }
        char tmp[SIZE][SIZE] = {0};
        memcpy(tmp, board, 100);
        tmp[pos.y][pos.x] = SELECTED;
        if (color == BLACK)message = "ターン: 黒(YOU)        ";
        else message = "ターン: 白(YOU)        ";
        render_board(tmp, message);
    }
}

int turn(char board[SIZE][SIZE], int color, int type) {
    switch (type) {
        case 0:
            return turn_player(board, color);
        case 1:
        case 2:
        case 3:
            return turn_computer(board, color, type - 1);
    }
}

char set_player(char idx) {
    printf("プレイヤー%dを設定してください。\n", idx);

    char selected = 0;
    while (1) {
        if (selected == 0) printf("●");
        printf("　人間\n");
        if (selected == 1) printf("●");
        printf("　コンピューター　Lv.0(ランダム) \n");
        if (selected == 2) printf("●");
        printf("　コンピューター　Lv.1(弱い) \n");
        if (selected == 3) printf("●");
        printf("　コンピューター　Lv.2(強い) \n");

        switch (_getch()) {
            case TOP:
                if (selected > 0)selected--;
                break;
            case BOTTOM:
                if (selected < 3)selected++;
                break;
            case 0x0d:
                return selected;
        }
        printf("\033[4F");
    }
}

void setting(int *plr1, int *plr2) {
    printf("---ゲーム設定---\n");
    *plr1 = set_player(1);
    *plr2 = set_player(2);
}

void main() {
    int plr1, plr2 = 0;
    setting(&plr1, &plr2);
    printf("\033[11F");
    char board[SIZE][SIZE] = {
            {EDGE, EDGE,  EDGE,  EDGE,  EDGE,  EDGE,  EDGE,  EDGE,  EDGE,  EDGE},
            {EDGE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EDGE},
            {EDGE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EDGE},
            {EDGE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EDGE},
            {EDGE, EMPTY, EMPTY, EMPTY, BLACK, WHITE, EMPTY, EMPTY, EMPTY, EDGE},
            {EDGE, EMPTY, EMPTY, EMPTY, WHITE, BLACK, EMPTY, EMPTY, EMPTY, EDGE},
            {EDGE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EDGE},
            {EDGE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EDGE},
            {EDGE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EDGE},
            {EDGE, EDGE,  EDGE,  EDGE,  EDGE,  EDGE,  EDGE,  EDGE,  EDGE,  EDGE}
    };
    render_board(board, "");
    int flag = 1;
    int count = 0;
    int black, white = 0;
    bool skipFlag = false;
    count_cells(board, &black, &white);
    board[0][0] = EDGE - black;
    board[0][SIZE - 1] = EDGE - white;
    srand((int) time(NULL));
    while (flag) {
        if (count % 2) {
            flag = turn(board, count % 2 ? WHITE : BLACK, plr1);
        } else {
            flag = turn(board, count % 2 ? WHITE : BLACK, plr2);
        }

        if (((board[0][0] - EDGE) * -1) + ((board[0][SIZE - 1] - EDGE) * -1) == (BOARD_SIZE * BOARD_SIZE) ||
            board[0][0] == EDGE || board[0][SIZE - 1] == EDGE || (skipFlag && flag == 2)) {
            if ((board[0][0] - EDGE) * -1 + (board[0][SIZE - 1] - EDGE) * -1 == 0) {
                printf("引き分け!\n");
                break;
            }
            char plr = (board[0][0] - EDGE) * -1 < (board[0][SIZE - 1] - EDGE) * -1 ? plr1 : plr2;
            printf("%s(%s)の勝利!              \n", get_player_display_name(plr),
                   get_color_display_name(
                           (board[0][0] - EDGE) * -1 < (board[0][SIZE - 1] - EDGE) * -1 ? WHITE : BLACK));
            break;
        }
        skipFlag = flag == 2;
        count++;
    }
    printf("Press any key to close.");
    _getch();
}