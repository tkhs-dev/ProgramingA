#include <stdbool.h>
#include <stdio.h>

int getDayOfWeek(int y, int m, int d) {
    int result;
    if (m == 1 || m == 2) {
        m += 12;
        y--;
    }
    result = (y + y / 4 - y / 100 + y / 400 + (13 * m + 8) / 5 + d) % 7;
    return result;
}

int getDaysOfMonth(int y, int m) {
    switch (m) {
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            if (y % 4 == 0) {
                if (y % 100 == 0) {
                    if (y % 400 == 0) {
                        return 29;
                    } else {
                        return 28;
                    }
                } else {
                    return 29;
                }
            } else {
                return 28;
            }
        default:
            return 31;
    }
}

bool validateDate(int y, int m) {
    return y >= 0 && 1 <= m && m <= 12;
}

void renderCalendar(int y, int m) {
    if(!validateDate(y,m)){
        printf("invalid year or month were given");
        return;
    }

    int days = getDaysOfMonth(y, m);
    int firstDay = getDayOfWeek(y, m, 1);
    printf("%d/%02d\n", y, m);
    printf("Sun Mon Tue Wed Thu Fri Sat\n");
    for (int i = 0; i < firstDay; i++) {
        printf("    ");
    }
    for (int i = 1; i <= days; i++) {
        printf("%3d ", i);
        if ((i + firstDay) % 7 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

void main() {
    renderCalendar(2023, 1);
}