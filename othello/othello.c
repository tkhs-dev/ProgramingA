#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <synchapi.h>
#include <stdbool.h>
#include <time.h>

//ボード用
#define EDGE -1
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define SELECTED 3
#define PLACEABLE 4
//キーバインド
#define TOP 'w'
#define BOTTOM 's'
#define LEFT 'a'
#define RIGHT 'd'

//盤面の大きさ 可変だが,main関数内の配列の初期値を変更する必要がある
int BOARD_SIZE  = 8;
int SIZE = 10;

//座標を扱う構造体
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

/**
 * @brief 指定方向にあるひっくり返すことができるコマの数を返す
 * @param board
 * @param color
 * @param enemy_color
 * @param coordinate 始点となる座標
 * @param vec 方向を指定するベクトル
 * @return ひっくり返すことができるコマの数
 */
int check_line(int board[SIZE][SIZE], int color, vec2c coordinate, vec2c vec) {
    int enemy_color = (color == BLACK) ? WHITE : BLACK;
    int count = 0;
    while (1) {
        coordinate.x += vec.x;
        coordinate.y += vec.y;
        if (board[coordinate.y][coordinate.x] == enemy_color) {
            count++;
        } else if (board[coordinate.y][coordinate.x] == color) {
            return count;
        } else {
            return 0;
        }
    }
}

/**
 * @brief 設置可能な場所を探索し、cells配列に結果を代入する.設置可能な場所の数を返す
 * @param board
 * @param color
 * @param cells 設置可能な位置にフラグが立った結果を返す用の配列
 * @return 設置可能な位置の数
 */
int get_placeable(int board[SIZE][SIZE], int color, int cells[SIZE][SIZE]) {
    int enemyColor = (color == BLACK) ? WHITE : BLACK;
    int count = 0;
    for (int i = 1; i < SIZE - 1; i++) {
        for (int j = 1; j < SIZE - 1; j++) {
            cells[i][j] = 0;
            if (board[i][j] == EMPTY) {
                int t, tr, r, br, b, bl, l, tl = 0;
                vec2c coordinate = init_vec2c(j, i);
                //上
                t = check_line(board, color, coordinate, init_vec2c(0, -1));
                //右上
                tr = check_line(board, color, coordinate, init_vec2c(1, -1));
                //右
                r = check_line(board, color, coordinate, init_vec2c(1, 0));
                //右下
                br = check_line(board, color, coordinate, init_vec2c(1, 1));
                //下
                b = check_line(board, color, coordinate, init_vec2c(0, 1));
                //左下
                bl = check_line(board, color, coordinate, init_vec2c(-1, 1));
                //左
                l = check_line(board, color, coordinate, init_vec2c(-1, 0));
                //左上
                tl = check_line(board, color, coordinate, init_vec2c(-1, -1));
                if (t || tr || r || br || b || bl || l || tl) { //どこか１つでもひっくり返せるならば,このマスは設置可能
                    cells[i][j] = 1;
                    count++;
                }
            }
        }
    }
    return count;
}

/**
 * @brief コマの数を数える
 * @param board
 * @param black 黒の結果を返すポインタ
 * @param white 白の結果を返すポインタ
 */
void count_cells(int board[SIZE][SIZE], int *black, int *white) {
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

/**
 * @brief ボードを描画する
 * @param board
 */
void render_board(int board[SIZE][SIZE], char *message) {
    printf("\033[%dF",BOARD_SIZE*4+2);
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
        printf("+");
        for (int i = 1; i < SIZE - 1; i++) {
            printf(" - +");
        }
        printf("\x1b[0m");
        printf("\n");
    }
    printf("\x1b[0m");
    printf("\n\n");
}

/**
 * @brief コマを設置する
 * @param board
 * @param dst 設置対象の座標
 * @param color
 * @return ひっくり返ったコマの数
 */
int place(int board[SIZE][SIZE], vec2c dst, int color) {
    int enemyColor = (color == BLACK) ? WHITE : BLACK;
    board[dst.y][dst.x] = color;

    int t, tr, r, br, b, bl, l, tl = 0;
    //上
    t = check_line(board, color, dst, init_vec2c(0, -1));
    //右上
    tr = check_line(board, color, dst, init_vec2c(1, -1));
    //右
    r = check_line(board, color, dst, init_vec2c(1, 0));
    //右下
    br = check_line(board, color, dst, init_vec2c(1, 1));
    //下
    b = check_line(board, color, dst, init_vec2c(0, 1));
    //左下
    bl = check_line(board, color, dst, init_vec2c(-1, 1));
    //左
    l = check_line(board, color, dst, init_vec2c(-1, 0));
    //左上
    tl = check_line(board, color, dst, init_vec2c(-1, -1));

    int count = 0;
    while (1) { //一回のループで各方向一コマずつひっくり返す
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
    if (color == BLACK) {//ひっくり返した結果をコマ数に反映する
        board[0][0] += -count - 1;
        board[0][SIZE - 1] += count;
    } else {
        board[0][0] += count;
        board[0][SIZE - 1] += -count - 1;
    }
    return count;
}

/**
 * @brief 色の表示名を返す
 * @param color
 * @return 色の表示名
 */
char *get_color_display_name(int color) {
    switch (color) {
        case BLACK:
            return "黒";
        case WHITE:
            return "白";
    }
}

/**
 * @brief プレイヤーの表示名を返す
 * @param plr プレイヤータイプ
 * @return プレイヤーの表示名
 * @sa set_player
 */
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

/**
 * @brief レベル１のコンピューター
 * @param board
 * @param placeable 設置可能位置を現す配列
 * @param num 設置可能位置の数
 * @param color
 * @return 決定した座標
 * @details 一番多くひっくり返せる場所に置く.ただし四辺と隅は優先して置く.
 */
vec2c intelligent_computer_lv1(int board[SIZE][SIZE], int placeable[SIZE][SIZE], int num, int color) {
    int idx;
    vec2c dst[num];
    int dstCount = 0;
    int maxChanged = 0;
    for (char i = 1; i < SIZE - 1; i++) {
        for (char j = 1; j < SIZE - 1; j++) { //すべてのマスについて処理を行う
            if (placeable[i][j]) {//設置可能ならば
                int tmp[SIZE][SIZE];
                memcpy(tmp, board, SIZE*SIZE*4); //ボード配列を退避させる
                int changed_tmp = place(tmp, init_vec2c(j, i), color); //設置した結果をシミュレートする
                if (i == 1 || i == SIZE - 2) {//四辺の場合,ひっくり返せる駒を二倍として計算
                    changed_tmp *= 2;
                    if (j == 1 || j == SIZE - 2) {//隅の場合,さらに二倍として計算
                        changed_tmp *= 2;
                    }
                }
                if (j == 1 || j == SIZE - 2) {
                    changed_tmp *= 2;
                    if (i == 1 || i == SIZE - 2) {
                        changed_tmp *= 2;
                    }
                }

                //ひっくり返せる駒数が最大となる場所を決定する
                //複数ある場合は配列
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
    idx = rand() % dstCount;//複数ある場合はランダムに選択する
    return dst[idx];
}

/**
 * @brief レベル２のコンピューター
 * @param board
 * @param placeable 設置可能位置を現す配列
 * @param num 設置可能位置の数
 * @param color
 * @return 決定した座標
 * @details 一手先まで読んで,係数をかけつつ一番多くひっくり返せる場所に置く.ただし四辺と隅は優先して置く.相手に角をとられてしまう手は避ける.
 */
vec2c intelligent_computer_lv2(int board[SIZE][SIZE], int placeable[SIZE][SIZE], int num, int color) {
    int enemyColor = (color == BLACK) ? WHITE : BLACK;
    int idx;
    vec2c dst[num];
    int dstCount = 0;
    int maxChanged = 0;
    for (char i = 1; i < SIZE - 1; i++) {
        for (char j = 1; j < SIZE - 1; j++) {
            if (placeable[i][j]) {
                int tmp[SIZE][SIZE];
                memcpy(tmp, board, SIZE*SIZE*4);
                int changed_tmp = place(tmp, init_vec2c(j, i), color);
                if (i == 1 || i == SIZE - 2) {
                    changed_tmp *= 2;
                    if (j == 1 || j == SIZE - 2) {
                        changed_tmp *= 5;
                    }
                }
                if (j == 1 || j == SIZE - 2) {
                    changed_tmp *= 2;
                    if (i == 1 || i == SIZE - 2) {
                        changed_tmp *= 5;
                    }
                }
                //ここまではLv1と同様

                //設置可能位置配列を退避
                int tmp_placeable[SIZE][SIZE];
                memcpy(tmp_placeable, placeable, SIZE*SIZE*4);

                bool enemy_can_place_corner = false;//敵が四隅をとれるか保存するフラグ

                if (get_placeable(tmp, enemyColor, tmp_placeable) > 0) {
                    //敵が角に設置可能かを調べる
                    enemy_can_place_corner = tmp_placeable[1][1] == 1 || tmp_placeable[1][SIZE - 2] == 1 ||
                                             tmp_placeable[SIZE - 2][1] == 1 || tmp_placeable[SIZE - 2][SIZE - 2] == 1;

                    //Lv1のコンピューターを敵であるとして敵の行動をシミュレートする
                    vec2c dst = intelligent_computer_lv1(tmp, tmp_placeable, num, enemyColor);
                    place(tmp, dst, enemyColor);
                }
                //一手先を読む
                if (get_placeable(tmp, color, tmp_placeable) > 0) {
                    int maxChanged2 = 0;
                    int tmp2[SIZE][SIZE];
                    for (char i2 = 1; i2 < SIZE - 1; i2++) {
                        for (char j2 = 1; j2 < SIZE - 1; j2++) {
                            if (placeable[i2][j2]) {
                                memcpy(tmp2, tmp, SIZE*SIZE*4);
                                int changed_tmp2 = place(tmp2, init_vec2c(j2, i2), color);
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
                    changed_tmp += maxChanged2 * 0.5; //一手先でひっくり返せる駒数は0.5倍して加算する
                    if (enemy_can_place_corner)changed_tmp *= 0; //敵が角をとれる場合はその手を避ける
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

/**
 * @brief ランダムに設置位置を決定するするコンピューター
 * @param board
 * @param placeable 設置可能位置を現す配列
 * @param num 設置可能位置の数
 * @param color
 * @return 決定した座標
 */
vec2c random_computer(int board[SIZE][SIZE], int placeable[SIZE][SIZE], int num, int color) {
    int idx = rand() % num;
    int count = 0;
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

/**
 * @brief コンピューターのターン
 * @param board
 * @param color
 * @param level
 * @return 1:成功,2:パス
 */
int turn_computer(int board[SIZE][SIZE], int color, int level) {
    int placeable[SIZE][SIZE];
    memset(placeable, 0, SIZE*SIZE*4);
    int num = get_placeable(board, color, placeable);
    if (num == 0)return 2; //設置可能な場所がなければスキップ

    char message[30];
    sprintf(message, "ターン: %s(%s)", get_color_display_name(color), get_player_display_name(level + 1));

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
    Sleep(500);

    place(board, dst, color); //決定された位置にコマを設置する
    render_board(board, message); //結果を描画
    return 1;
}

/**
 * @brief プレイヤーのターン
 * @param board
 * @param color
 * @return 0:中断,1:成功,2:パス
 */
int turn_player(int board[SIZE][SIZE], int color) {
    int placeable[SIZE][SIZE];
    memset(placeable, 0, SIZE*SIZE*4);
    int count = get_placeable(board, color, placeable);
    if (count == 0)return 2; //設置可能な場所がなければスキップ

    printf("プレイヤー(%s)のターン\n", get_color_display_name(color));

    vec2c pos = init_vec2c(5, 5); //カーソルの初期位置
    char *message = "";
    while (1) {
        message = "";
        Sleep(10);
        wait:
        switch (_getch()) { //キー入力をハンドル
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
            case 0x0d: //ENTERで設置
                if (placeable[pos.y][pos.x]) {
                    place(board, pos, color);
                    render_board(board, "");
                    return 1;
                } else {
                    message = "そこには置けません。";
                }
                break;
            case 'q': //Qで中断してゲームを終了
                return 0;
            default: //それ以外のキーなら次の入力を待つ
                goto wait;
        }
        int tmp[SIZE][SIZE];
        memcpy(tmp, board, SIZE*SIZE*4);
        tmp[pos.y][pos.x] = SELECTED; //退避させたボードにカーソルの情報を加える
        if (color == BLACK)message = "ターン: 黒(YOU)        ";
        else message = "ターン: 白(YOU)        ";
        render_board(tmp, message); //描画
    }
}

/**
 * @brief プレイヤータイプによって関数を切り替える
 * @param board
 * @param color
 * @param type プレイヤータイプ
 * @return ターンの結果
 */
int turn(int board[SIZE][SIZE], int color, int type) {
    switch (type) {
        case 0:
            return turn_player(board, color);
        case 1:
        case 2:
        case 3:
            return turn_computer(board, color, type - 1);
    }
}

int set_board_size(){
    printf("盤面サイズを設定 W:増加 S:減少 ENTERで決定\n");
    int value = 8;
    while (1) {
        printf("現在のサイズ: %d \n", value);

        wait:
        switch (_getch()) {
            case BOTTOM:
                if (value > 4)value-=2;
                break;
            case TOP:
                value+=2;
                break;
            case 0x0d:
                return value;
            default:
                goto wait; //それ以外のキーなら次の入力を待つ
        }
        printf("\033[1F");
    }
}

/**
 * @brief プレイヤーを設定する
 * @param idx
 * @return 設定されたプレイヤータイプ
 */
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

        wait:
        switch (_getch()) {
            case TOP:
                if (selected > 0)selected--;
                break;
            case BOTTOM:
                if (selected < 3)selected++;
                break;
            case 0x0d:
                return selected;
            default:
                goto wait; //それ以外のキーなら次の入力を待つ
        }
        printf("\033[4F");
    }
}

/**
 * @brief ゲーム設定
 * @param plr1 設定されたプレイヤー１のプレイヤータイプ
 * @param plr2 設定されたプレイヤー２のプレイヤータイプ
 */
void setting(int *board_size, int *plr1, int *plr2) {
    printf("---ゲーム設定(WASD操作)---\n");
    *board_size = set_board_size();
    *plr1 = set_player(1);
    *plr2 = set_player(2);
}

void main() {
    int board_size, plr1, plr2 = 0;
    setting(&board_size, &plr1, &plr2);
    printf("\033[13F"); //描画を消す

    BOARD_SIZE = board_size;
    SIZE = board_size + 2;

    int board[SIZE][SIZE];

    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            //もし四辺ならEDGEを代入
            if(i == 0 || i == SIZE - 1 || j == 0 || j == SIZE - 1) {
                board[i][j] = EDGE;
            } else if(i == SIZE / 2 - 1 && j == SIZE / 2 - 1) {
                board[i][j] = WHITE;
            } else if(i == SIZE / 2 && j == SIZE / 2) {
                board[i][j] = WHITE;
            } else if(i == SIZE / 2 - 1 && j == SIZE / 2) {
                board[i][j] = BLACK;
            } else if(i == SIZE / 2 && j == SIZE / 2 - 1) {
                board[i][j] = BLACK;
            } else {
                board[i][j] = EMPTY;
            }
        }
    }

    render_board(board, ""); //盤面を描画
    int flag = 1; //ターンの結果を判定するフラグ
    int count = 0; //ターン数を数える変数
    int black, white = 0; //初期の駒の数を数え上げる変数
    bool skipFlag = false; //パスをしたかどうかを判定するフラグ
    count_cells(board, &black, &white); //初期の駒を数え上げる
    //ボードの上部の２つの隅に,コマ数の情報を保存する
    board[0][0] = EDGE - black;
    board[0][SIZE - 1] = EDGE - white;
    srand((int) time(NULL)); //乱数の初期化
    while (flag) { //ゲームのメインループ
        if (count % 2) { //ターンを実行
            flag = turn(board, count % 2 ? WHITE : BLACK, plr1);
        } else {
            flag = turn(board, count % 2 ? WHITE : BLACK, plr2);
        }
        if (flag == 2) printf("passed!");

        /**
         * ゲームの結果を判定する
         * 1. すべての駒が設置された場合
         * 2. どちらかのプレイヤーのコマ数が０になったとき
         * 3.両者の設置できる場所がなくなったとき
         */
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