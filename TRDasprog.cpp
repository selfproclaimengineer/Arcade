#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 50

#define PLAYER 'X'
#define BOT 'O'
#define PLAYER1 'A'
#define PLAYER2 'B'

void loading();
void pacman_animation();
void show_loading_screen();
void set_input_mode();
void menu();
void set_nonblocking(int state);
void login();
void sign_in();
void forgot_password();
void game_choice();
int tic_tac_toe();
void showTutorial();
void initBoard(char board[3][3]);
void printBoard(char board[3][3]);
int isWinner(char board[3][3], char symbol);
int isDraw(char board[3][3]);
void playerMove(char board[3][3]);
void easyBotMove(char board[3][3]);
void mediumBotMove(char board[3][3]);
void hardBotMove(char board[3][3]);
int isValidMove(char board[3][3], int row, int col);
void playerMovePvP(char board[3][3], char symbol);

int main() {
    bool proving = 0; // Variable to control the menu loop
    loading();
    while(1){
        menu();
    }
}

void loading() {
    printf("=========================================================");
    printf("\n||               Welcome to The Arcade!                ||\n");
    printf("=========================================================\n");
    printf("Press Enter to start the game...\n");
    char enter;
    pacman_animation();

    // while ((enter = getchar()) != '\n') {
        
    //     if (enter == '\n') {
    //         fflush(stdin);
    //         break;
    //     }
    // }
    
    printf("\n=======================================");
    printf("\n||          Welcome!        ||");
    printf("\n=======================================\n");
    usleep(1000000); // Delay for 1 second to show the welcome message

}

void show_loading_screen() {
    char loading_messages [3][60] = {
        "Loading Game Assets",
        "Initializing Game Engine",
        "Setting Up Game Environment"
    };

    char dot[4][4] = {
        " ",
        ".",
        "..",
        "..."};
    system("clear");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            system("clear");
            printf("Loading%s", dot[j]);
            printf("\n%s", loading_messages[i]);
            fflush(stdout);
            usleep(10000);
            
        }
    }
}

void loading_pacman(){
    char frames[2][10] = {":O", ":C"};
    for(int i = 0; i<20;i++){
        system("clear");
        printf("Loading, please wait!\n");
        printf("......................\r");
        for (int j = 0; j < i; j++) {            
            printf(" ");     
        }
        
            printf("%s", frames[i % 2]);
            fflush(stdout);  
        
        usleep(10000);
    }
    
}

void set_nonblocking(int state) {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);

    if (state == 1) {
        ttystate.c_lflag &= ~ICANON;
        ttystate.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    } else {
        ttystate.c_lflag |= ICANON;
        ttystate.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
        fcntl(STDIN_FILENO, F_SETFL, 0);
    }
}

void pacman_animation() {
    const char *frames[] = {":O...  ", ":C..   "};
    set_nonblocking(1);

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
            int ch = getchar();
            if (ch == '\n') {
                stop = 1;
                break;
            }
            // Uncomment the following lines if you want to handle arrow keys or other special keys
            // if (ch == 27) { // ESC
            //     int ch1 = getchar();
            //     int ch2 = getchar();
            //     if (ch1 == 91 && ch2 == 65) { // '[' and 'A'
            //         stop = 1;
            //         break;

            //     }
            // }
        }
    }

    set_nonblocking(0);
    show_loading_screen();
}

void set_input_mode(int enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

void menu() {
    const char *options[] = {
        "1. Log in",
        "2. Sign up",
        "3. Forgot Password",
        ">> Exit"
    };
    int num_options = 4;
    int selected = 0;
    int running = 1;

    set_input_mode(1);

    while (running) {
        // Clear screen
        system("clear");
        printf("=========================================================\n");
        printf("||                   Main Menu                         ||\n");
        printf("=========================================================\n");
        for (int i = 0; i < num_options; i++) {
            printf("|| ");
            if (i == selected) {
                printf("\033[1;32m%s\033[0m", options[i]); // Green highlight
                
            } else {
                printf("%s", options[i]);
            }
            int spaces = 48 - (int)strlen(options[i]);
            for (int s = 0; s < spaces; s++) printf(" ");
            printf("||\n");
        }
        printf("=========================================================\n");
        printf("(Select using arrow keys and press Enter)\n");

        int ch = getchar();
        if (ch == 27) { // ESC
            int ch1 = getchar();
            int ch2 = getchar();
            if (ch1 == 91) {
                if (ch2 == 65) { // Up arrow
                    selected = (selected - 1 + num_options) % num_options;
                } else if (ch2 == 66) { // Down arrow
                    selected = (selected + 1) % num_options;
                }
            }
        } else if (ch == '\n' || ch == 13) { // Enter
            running = 0;
        }
    }

    set_input_mode(0);

    // Do something with the selected option

    switch (selected) {
        case 0:
            printf("Logging in...\n");
            // Add login logic here
            login();
            break;
        case 1:
            printf("Signing up...\n");
            // Add sign-up logic here
            sign_in();
            break;
        case 2:
            printf("Forgot Password...\n");
            // Add forgot password logic here
            forgot_password();
            break;
        case 3:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid option selected.\n");
    }
}

int check_credentials(const char *username, const char *password) {
    FILE *file = fopen("user_data.txt", "r");
    if (!file) return 0;
    char line[2*MAX_LEN+2];
    char stored_user[MAX_LEN], stored_pass[MAX_LEN];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%[^:]:%s", stored_user, stored_pass) == 2) {
            if (strcmp(stored_user, username) == 0 && strcmp(stored_pass, password) == 0) {
                found = 1;
                break;
            }
        }
    }
    fclose(file);
    return found;
}

void game_menu(){
    loading_pacman();
    bool backmenu = true; //kontrol loop
    while(backmenu){
        system("clear");
        const char *options[] = {
            "1. Tic-Tac-Toe",
            "2. Dino Run",
            "3. Pac Man",
            ">> Exit"
        };
        int num_options = 4;
        int selected = 0;
        int running = 1;

        set_input_mode(1);

        while (running) {
            // Clear screen
            system("clear");
            printf("=========================================================\n");
            printf("||                   Game Choice                         ||\n");
            printf("=========================================================\n");
            for (int i = 0; i < num_options; i++) {
                printf("|| ");
                if (i == selected) {
                    printf("\033[1;32m%s\033[0m", options[i]); // Green highlight
                    
                } else {
                    printf("%s", options[i]);
                }
                int spaces = 48 - (int)strlen(options[i]);
                for (int s = 0; s < spaces; s++) printf(" ");
                printf("||\n");
            }
            printf("=========================================================\n");
            printf("(Select using arrow keys and press Enter)\n");

            int ch = getchar();
            if (ch == 27) { // ESC
                int ch1 = getchar();
                int ch2 = getchar();
                if (ch1 == 91) {
                    if (ch2 == 65) { // Up arrow
                        selected = (selected - 1 + num_options) % num_options;
                    } else if (ch2 == 66) { // Down arrow
                        selected = (selected + 1) % num_options;
                    }
                }
            } else if (ch == '\n' || ch == 13) { // Enter
                running = 0;
            }
        }
        set_input_mode(0);

        // Do something with the selected option
        int c;
        switch (selected) {
            case 0:
                printf("\n");
                printf("Starting Tic-Tac-Toe...\n");
                usleep(1000000);
                system("clear");
                tic_tac_toe();                
                while ((c = getchar()) != '\n' && c != EOF);
                break;
            case 1:
                printf("Starting Dino Run...\n");
                usleep(1000000);
                break;
            case 2:
                printf("Starting Pac Man\n");
                usleep(1000000);
                break;
            case 3:
                printf("Exiting...\n");
                backmenu=0;
                break;
            default:
                printf("Invalid option selected.\n");
        }
    }
}

int getch(void) { //getch buat c++ linux di github
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

void login() {
        char username[MAX_LEN], password[MAX_LEN];
    int ch;
    int try_again = 1;
    while (try_again) {
        printf("\033[2J\033[H"); // clear screen
        printf("=== Login ===\n");
        printf("Username: ");
        fgets(username, MAX_LEN, stdin);
        username[strcspn(username, "\n")] = 0; // remove newline

        printf("Password: ");
        int idx = 0, ch;
        memset(password, 0, sizeof(password));
        while ((ch = getch()) != '\n' && idx < MAX_LEN-1) {
            if (ch == 127 || ch == 8) { // backspace
                if (idx > 0) {
                    idx--;
                    printf("\b \b");
                }
            } else if (ch >= 32 && ch <= 126) {
                password[idx++] = ch;
                putchar('*');
            }
        }
        password[idx] = '\0';

        if (check_credentials(username, password)) {
            printf("\nLogin successful! Press any key to continue...");
            getch();
            game_menu();
            return;
        } else {
            printf("\nLogin failed! Press Y to try again, any other key to return to main menu...");
            ch = getch();
            if (ch == 'Y' || ch == 'y') {
                try_again = 1;
            } else {
                try_again = 0;
            }
        }
    }
}



int username_exists(const char *username) { //cek username dah ada apa belum
    FILE *file = fopen("user_data.txt", "r");
    if (!file) return 0;
    char line[2*MAX_LEN+2];
    while (fgets(line, sizeof(line), file)) {
        char stored_user[MAX_LEN];
        sscanf(line, "%[^:]:%*s", stored_user);
        if (strcmp(stored_user, username) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void sign_in() {
        char username[MAX_LEN], password[MAX_LEN];
    int esc = 0;
    while (1) {
        printf("\033[2J\033[H"); // clear screen
        printf("=== Sign Up ===\n");
        printf("Enter username (ESC to cancel): ");
        int idx = 0, ch;
        memset(username, 0, sizeof(username));
        while ((ch = getch()) != '\n' && idx < MAX_LEN-1) {
            if (ch == 27) { esc = 1; break; }
            if (ch == 127 || ch == 8) { // biar isa backspace
                if (idx > 0) {
                    idx--;
                    printf("\b \b");
                }
            } else if (ch >= 32 && ch <= 126) {
                username[idx++] = ch;
                putchar(ch);
            }
        }
        if (esc) break;
        username[idx] = '\0';
        if (strlen(username) == 0) continue;
        if (username_exists(username)) {
            printf("\nUsername already taken! Press any key to try again...");
            getch();
            continue;
        }
        break;
    }
    if (esc) return;

    printf("\nEnter password: ");

    int idx = 0;
    int ch;

    memset(password, 0, sizeof(password));
    while ((ch = getch()) != '\n' && idx < MAX_LEN-1) {
        if (ch == 127 || ch == 8) { // backspace
            if (idx > 0) {
                idx--;
                printf("\b \b");
            }
        } else if (ch >= 32 && ch <= 126) {
            password[idx++] = ch;
            putchar('*');
        }
    }
    password[idx] = '\0';

    FILE *file = fopen("user_data.txt", "a");
    if (file) {
        fprintf(file, "%s:%s\n", username, password);
        fclose(file);
        printf("\nSign up successful! Press any key to return to menu...");
    } else {
        printf("\nError saving user data!");
    }
    getch();
}


void forgot_password() {
    char username[MAX_LEN];
    char line[2 * MAX_LEN + 2];
    char stored_user[MAX_LEN], stored_pass[MAX_LEN];
    int found = 0;

    printf("\033[2J\033[H"); // clear screen
    printf("=== Forgot Password ===\n");
    printf("Enter your username: ");
    fgets(username, MAX_LEN, stdin);
    username[strcspn(username, "\n")] = 0; // remove newline

    FILE *file = fopen("user_data.txt", "r");
    if (!file) {
        printf("Could not open user data file.\n");
        printf("Press any key to return to menu...");
        getch();
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%[^:]:%s", stored_user, stored_pass) == 2) {
            if (strcmp(stored_user, username) == 0) {
                found = 1;
                break;
            }
        }
    }
    fclose(file);

    if (found) {
        printf("Your password is: %s\n", stored_pass);
    } else {
        printf("Username not found.\n");
    }
    printf("Press any key to return to menu...");
    getch();
}

int tic_tac_toe() {
    int backmenu = 1;
    while (backmenu == 1) {
        char board[3][3];
        char difficulty;

        srand(time(NULL)); // Seed untuk random

        // Ask for difficulty every time
        printf("Tic Tac Toe - Melawan Bot Atau Pemain\n");
        printf("Pilih Mode Permainan:\n");
        printf("1. Easy (Bot)\n2. Medium (Bot)\n3. Hard (Bot)\n4. Tutorial\n5. Player Vs Player\n[M] untuk keluar\n");
        printf("Masukkan pilihanmu (1-5): ");
        scanf(" %c", &difficulty);
        // while (getchar() != '\n' && getchar() != EOF);
        fflush(stdin); // clear input buffer

        system("clear"); // clear screen (Linux/Mac)

        switch (difficulty) {
            case '1':
                printf("Mode Easy dipilih.\n");
                getch();
                break;
            case '2':
                printf("Mode Medium dipilih.\n");
                getch();
                break;
            case '3':
                printf("Mode Hard dipilih.\n");
                getch();
                break;
            case '4':
                showTutorial();
                printf("Tekan Enter untuk kembali ke menu...");
                while (getchar() != '\n');
                getchar();
                system("clear");
                continue; // Kembali ke menu utama;
            case 'M':
            case 'm':
                return 0;
                
            case '5': {
                initBoard(board);
                int i = 0;
                while (1) {
                    printBoard(board);
                    char currentPlayer = (i % 2 == 0) ? PLAYER1 : PLAYER2;
                    printf("Giliran Pemain %c\n", currentPlayer);
                    playerMovePvP(board, currentPlayer);

                    if (isWinner(board, currentPlayer)) {
                        printBoard(board);
                        printf("Pemain %c menang!\n", currentPlayer);
                        break;
                    }
                    if (isDraw(board)) {
                        printBoard(board);
                        printf("Seri!\n");
                        break;
                    }
                    i++;
                }
                // printf("Tekan Enter untuk kembali ke menu...");
                // while (getchar() != '\n' && getchar() != EOF); // clear buffer ONCE
                // system("clear");
                break;
            }
           
            default:
                printf("Pilihan tidak valid. Silakan pilih 1-5.\n");
                getchar();
                continue;
        }

        // Only run the bot game if difficulty is 1, 2, or 3
        int playing = 1; // kontrol loop
        while(playing == 1){
            if (difficulty == '1' || difficulty == '2' || difficulty == '3') {
                initBoard(board);    
                while (1) {
                    printBoard(board);
                    playerMove(board);
                    if (isWinner(board, PLAYER)) {
                        printBoard(board);
                        printf("Kamu menang!\n");
                        break;
                    }
                    if (isDraw(board)) {
                        printBoard(board);
                        printf("Seri!\n");
                        break;
                    }

                    // BOT MOVE
                    if (difficulty == '1') {
                        easyBotMove(board);
                    } else if (difficulty == '2') {
                        mediumBotMove(board);
                    } else if (difficulty == '3') {
                        hardBotMove(board);
                    }

                    if (isWinner(board, BOT)) {
                        printBoard(board);
                        printf("Bot menang!\n");
                        break;
                    }
                    if (isDraw(board)) {
                        printBoard(board);
                        printf("Seri!\n");
                        break;
                    }
                }
                printf("Tekan Y untuk main lagi atau N untuk kembali ke main menu...\n");
                char choice = 0;
                while (1) {
                    choice = getchar();
                    if (choice == '\n') continue; // skip newlines
                    if (choice == 'Y' || choice == 'y') {
                        system("clear");
                        break; // will restart the outer while(1) loop (play again)
                    } else if (choice == 'N' || choice == 'n') {
                        system("clear");
                        playing = 0;
                        backmenu = 0;
                        break;
                    } else {
                        printf("Masukkan Y untuk main lagi, N untuk kembali ke menu: ");
                    }
                }
            }
        }
    }
    return 0;
}

void showTutorial() {
    printf("=====================================\n");
    printf("      TUTORIAL: TIC TAC TOE vs BOT\n");
    printf("=====================================\n");
    printf("Tujuan:\n");
    printf("  Susun 3 simbol X (kamu) secara horizontal, vertikal, atau diagonal.\n\n");

    printf("Cara Bermain:\n");
    printf("  - Kamu bermain sebagai X.\n");
    printf("  - Bot bermain sebagai O.\n");
    printf("  - Kamu selalu jalan duluan.\n");
    printf("  - Input posisi dengan format: baris kolom (angka 0-2)\n");
    printf("    Contoh: Masukkan baris dan kolom: 1 2\n\n");

    printf("Mode Bot:\n");
    printf("  1. Easy   - Langkah acak\n");
    printf("  2. Medium - Bisa menang/blok\n");
    printf("  3. Hard   - Tidak bisa dikalahkan\n\n");

    printf("Peta Papan:\n");
    printf("     0   1   2\n");
    printf("   -------------\n");
    printf(" 0 |   |   |   |\n");
    printf("   -------------\n");
    printf(" 1 |   |   |   |\n");
    printf("   -------------\n");
    printf(" 2 |   |   |   |\n");
    printf("   -------------\n\n");

    printf("Tekan E untuk keluar kapanpun\n\n");
    printf("Tips:\n");
    printf("  Ambil tengah lebih dulu. Jangan biarkan bot punya 2 peluang menang!\n");
    printf("  Tapi kalau main lawan Hard Bot, ya… good luck ??\n\n");
}

void initBoard(char board[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';
}

void printBoard(char board[3][3]) {
    printf("\nCurrent Board:\n");
    for (int i = 0; i < 3; i++) {
        printf(" ");
        for (int j = 0; j < 3; j++) {
            printf("%c", board[i][j]);
            if (j < 2) printf(" | ");
        }
        if (i < 2) printf("\n---+---+---\n");
    }
    printf("\n\nCell numbers:\n");
    for (int i = 0; i < 3; i++) {
        printf(" ");
        for (int j = 0; j < 3; j++) {
            printf("%d", i * 3 + j + 1);
            if (j < 2) printf(" | ");
        }
        if (i < 2) printf("\n---+---+---\n");
    }
    printf("\n");
}

int isValidMove(char board[3][3], int row, int col) {
    return row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ';
}


void playerMove(char board[3][3]) {
    int move = -1;
    int row, col;
    while (1) {
        printf("Enter your move (1-9): ");
        if (scanf("%d", &move) != 1) {
            while (getchar() != '\n'); // clear invalid input
            printf("Invalid input. Try again.\n");
            continue;
        }
        if (move < 1 || move > 9) {
            printf("Move must be between 1 and 9.\n");
            continue;
        }
        row = (move - 1) / 3;
        col = (move - 1) % 3;
        if (!isValidMove(board, row, col)) {
            printf("Cell already taken. Try again.\n");
            continue;
        }
        break;
    }
    board[row][col] = PLAYER;
}


int isWinner(char board[3][3], char symbol) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) return 1;
        if (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol) return 1;
    }
    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) return 1;
    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) return 1;
    return 0;
}

int isDraw(char board[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}

void easyBotMove(char board[3][3]) {
    int row, col;
    do {
        row = rand() % 3;
        col = rand() % 3;
    } while (!isValidMove(board, row, col));
    printf("Bot (Easy) memilih: %d %d\n", row, col);
    board[row][col] = BOT;
}

// Mencoba menang atau blok, tergantung simbol ('O' untuk menang, 'X' untuk blok)
int tryWinOrBlock(char board[3][3], char symbol) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = symbol;
                if (isWinner(board, symbol)) {
                    if (symbol == BOT)
                        printf("Bot (Medium) memilih: %d %d\n", i, j);
                    board[i][j] = BOT; // Pasang langkah
                    return 1; // Langkah ditemukan
                }
                board[i][j] = ' '; // Undo langkah
            }
        }
    }
    return 0;
}

void mediumBotMove(char board[3][3]) {
    // 1. Coba menang
    if (tryWinOrBlock(board, BOT)) return;

    // 2. Coba blok pemain
    if (tryWinOrBlock(board, PLAYER)) return;

    // 3. Kalau tidak bisa, random
    easyBotMove(board);
}

int minimax(char board[3][3], int depth, int isMaximizing) {
    if (isWinner(board, BOT)) return 1;
    if (isWinner(board, PLAYER)) return -1;
    if (isDraw(board)) return 0;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = BOT;
                    int score = minimax(board, depth + 1, 0);
                    board[i][j] = ' ';
                    if (score > bestScore) bestScore = score;
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = PLAYER;
                    int score = minimax(board, depth + 1, 1);
                    board[i][j] = ' ';
                    if (score < bestScore) bestScore = score;
                }
            }
        }
        return bestScore;
    }
}

void hardBotMove(char board[3][3]) {
    int bestScore = -1000;
    int moveRow = -1, moveCol = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = BOT;
                int score = minimax(board, 0, 0);
                board[i][j] = ' ';
                if (score > bestScore) {
                    bestScore = score;
                    moveRow = i;
                    moveCol = j;
                }
            }
        }
    }

    printf("Bot (Hard) memilih: %d %d\n", moveRow, moveCol);
    board[moveRow][moveCol] = BOT;
}

void playerMovePvP(char board[3][3], char symbol) {
    char input[10];
    int row, col;
    while (1) {
        printf("Masukkan baris dan kolom (0-2), atau E untuk keluar: ");
        if (!fgets(input, sizeof(input), stdin)) continue;
        system("clear");
        if (input[0] == 'E' || input[0] == 'e') {
            printf("Keluar dari permainan. Sampai jumpa!\n");
            exit(0);
        }
        if (sscanf(input, "%d %d", &row, &col) == 2) {
            if (row < 0 || row > 2 || col < 0 || col > 2) {
                printf("Input tidak valid! Masukkan angka antara 0-2.\n");
                continue;
            }
            if (!isValidMove(board, row, col)) {
                printf("Posisi sudah terisi! Coba lagi.\n");
                continue;
            }
            board[row][col] = symbol;
            break;
        } else {
            printf("Input tidak valid! Masukkan dua angka atau E untuk keluar.\n");
        }
    }
}
