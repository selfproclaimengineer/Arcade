#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

void loading();
void pacman_animation();
void show_loading_screen();
int kbhit();
int getch();

int main() {
    loading();
}

void loading() {
    printf("=========================================================");
    printf("\n||               Welcome to The Arcade!                ||\n");
    printf("=========================================================\n");
    printf("Press Enter to start the game...\n");
    char enter;
    pacman_animation();

    while ((enter = getchar()) != '\n') {
        
        if (enter == '\n') {
            fflush(stdin);
            break;
        }
    }
    

}

void show_loading_screen() {
    system("clear");
    for (int i = 0; i < 60; i++) printf(" ");
    for (int i = 0; i < 4; i++) {
        printf("\rLoading");
        fflush(stdout);
        for (int j = 0; j < 4; j++) {
            usleep(500000);
            printf(".");
            fflush(stdout);
        }
    printf("\r");
    printf("                 ");
    }
}

void pacman_animation() {
    const char *frames[] = {":O...  ", ":C..   "};

    int stop = 0;
    for (int i = 0; i < 100 && !stop; ++i) {
        for (int j = 0; j < 53 && !stop; j++) {
            for (int k = 0; k < j; k++) {
                printf(" ");
            }
            printf("%s", frames[j % 2]);
            fflush(stdout);
            usleep(100000);
            printf("\r");
            for (int l = 0; l < 60; l++) {
                printf(" ");
            }
            printf("\r");
            if (kbhit()) {
              char key = getch(); // ambil input tanpa enter
               // proses input
}

            // int ch = getchar();
            // if (ch == '\n') {
            //     stop = 1;
            //     break;
            //}
        }
    }

    show_loading_screen();
}

int kbhit(void) { //kbhit dari linux
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int getch(void) { //getch dari linux
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}