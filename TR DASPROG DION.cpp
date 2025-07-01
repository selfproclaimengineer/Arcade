#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 50
#define PLAYER1 'A'
#define PLAYER2 'B'

#define MAP_W 79
#define MAP_H 21
#define MAX_LIFE 3

void loading();
void pacman_animation();
void show_loading_screen();
void menu();
void login();
void sign_in();
void forgot_password();
void game_menu();

int tic_tac_toe();
void showTutorial();
void initBoard(char board[3][3]);
void printBoard(char board[3][3]);
int isWinner(char board[3][3], char symbol);
int isDraw(char board[3][3]);
void playerMove(char board[3][3], char playerSymbol);
void easyBotMove(char board[3][3], char botSymbol);
void mediumBotMove(char board[3][3], char botSymbol, char playerSymbol);
void hardBotMove(char board[3][3], char botSymbol, char playerSymbol);
int tryWinOrBlock(char board[3][3], char testSymbol, char botSymbol);
int minimax(char board[3][3], int depth, int isMaximizing, char botSymbol, char playerSymbol);
int isValidMove(char board[3][3], int row, int col);
void playerMovePvP(char board[3][3], char symbol);

void dino_run();
void set_text_color(int color);
void save_dino_highscore(int score);
int load_dino_highscore();

void pacman_menu();
char maze[MAP_H][MAP_W+1];
int pac_x, pac_y, ghost_x, ghost_y;
int score, life = 0;
int people_saved;
void load_maze();
void reset_maze();
void draw_maze();
int is_wall(int x, int y);
void move_ghost();
void pacman();
void pacman_instruction();
void save_pacman_highscore(int score);
int load_pacman_highscore(char *name);

time_t start;

int main() {
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
    pacman_animation();
    printf("\n=======================================");
    printf("\n||          Welcome!        ||");
    printf("\n=======================================\n");
    Sleep(1000); // Delay for 1 second to show the welcome message
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
    system("cls");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            system("cls");
            printf("Loading%s", dot[j]);
            printf("\n%s", loading_messages[i]);
            fflush(stdout);
            Sleep(500);
        }
    }
}

void loading_pacman(){
    char frames[2][10] = {":O", ":C"};
    for(int i = 0; i<20;i++){
        system("cls");
        printf("Loading, please wait!\n");
        printf("......................\r");
        for (int j = 0; j < i; j++) {            
            printf(" ");     
        }
        printf("%s", frames[i % 2]);
        fflush(stdout);  
        Sleep(10);
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
            Sleep(500);
            printf("\r");
            for (int l = 0; l < 60; l++) {
                printf(" ");
            }
            printf("\r");
            if (_kbhit()) {
                int ch = _getch();
                if (ch == 13) { // Enter
                    stop = 1;
                    break;
                }
            }
        }
    }
    show_loading_screen();
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

    while (running) {
        system("cls");
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

        int ch = _getch();
        if (ch == 224) { // Arrow keys
            int arrow = _getch();
            if (arrow == 72) { // Up arrow
                selected = (selected - 1 + num_options) % num_options;
            } else if (arrow == 80) { // Down arrow
                selected = (selected + 1) % num_options;
            }
        } else if (ch == 13) { // Enter
            running = 0;
        }
    }

    switch (selected) {
        case 0:
//            printf("Logging in...\n");
            login();
            break;
        case 1:
//            printf("Signing up...\n");
            sign_in();
            break;
        case 2:
//            printf("Forgot Password...\n");
            forgot_password();
            break;
        case 3:
            system("cls");
            printf("Thank You For Playing!");
            Sleep(500);
			printf("\nExiting...\n");
			Sleep(1000);
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
    int backmenu = 1;
    while(backmenu){
        system("cls");
        const char *options[] = {
            "1. Tic-Tac-Toe",
            "2. Dino Run",
            "3. Pac Man",
            ">> Exit"
        };
        int num_options = 4;
        int selected = 0;
        int running = 1;

        while (running) {
            system("cls");
            printf("=========================================================\n");
            printf("||                   Game Choice                         ||\n");
            printf("=========================================================\n");
            for (int i = 0; i < num_options; i++) {
                printf("|| ");
                if (i == selected) {
                    printf("\033[1;32m%s\033[0m", options[i]);
                } else {
                    printf("%s", options[i]);
                }
                int spaces = 48 - (int)strlen(options[i]);
                for (int s = 0; s < spaces; s++) printf(" ");
                printf("||\n");
            }
            printf("=========================================================\n");
            printf("(Select using arrow keys and press Enter)\n");

            int ch = _getch();
            if (ch == 224) { // Arrow keys
                int arrow = _getch();
                if (arrow == 72) { // Up arrow
                    selected = (selected - 1 + num_options) % num_options;
                } else if (arrow == 80) { // Down arrow
                    selected = (selected + 1) % num_options;
                }
            } else if (ch == 13) { // Enter
                running = 0;
            }
        }

        int c;
        switch (selected) {
            case 0:
                printf("\n");
                printf("Starting Tic-Tac-Toe...\n");
                Sleep(1000);
                system("cls");
                tic_tac_toe();                
//                while ((c = getchar()) != '\n' && c != EOF);
                break;
            case 1:
                printf("Starting Dino Run...\n");
                Sleep(1000);
                system("cls");
                dino_run();
//                while ((c = getchar()) != '\n' && c != EOF);
                break;
            case 2:
                printf("Starting Pac Man\n");
                Sleep(1000);
                pacman_menu();
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

void login() {
    char username[MAX_LEN], password[MAX_LEN];
    int try_again = 1;
    while (try_again) {
        system("cls");
        printf("=== Login ===\n");
        printf("Username: ");
        fgets(username, MAX_LEN, stdin);
        username[strcspn(username, "\n")] = 0;

        printf("Password: ");
        int idx = 0, ch;
        memset(password, 0, sizeof(password));
        while ((ch = _getch()) != 13 && idx < MAX_LEN-1) { // 13 = Enter
            if (ch == 8) { // backspace
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
            printf("\nLogin successful! Hello %s!",username); 
			printf("\nPress any key to continue...");
            _getch();
            game_menu();
            return;
        } else {
            printf("\nLogin failed! Press Y to try again, any other key to return to main menu...");
            ch = _getch();
            if (ch == 'Y' || ch == 'y') {
                try_again = 1;
            } else {
                try_again = 0;
            }
        }
    }
}

int username_exists(const char *username) {
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
        system("cls");
        printf("=== Sign Up ===\n");
        printf("Enter username (ESC to cancel): ");
        int idx = 0, ch;
        memset(username, 0, sizeof(username));
        while ((ch = _getch()) != 13 && idx < MAX_LEN-1) {
            if (ch == 27) { esc = 1; break; }
            if (ch == 8) { // backspace
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
            _getch();
            continue;
        }
        break;
    }
    if (esc) return;

    printf("\nEnter password: ");
    int idx = 0;
    int ch;
    memset(password, 0, sizeof(password));
    while ((ch = _getch()) != 13 && idx < MAX_LEN-1) {
        if (ch == 8) { // backspace
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
    _getch();
}

void forgot_password() {
    char username[MAX_LEN];
    char line[2 * MAX_LEN + 2];
    char stored_user[MAX_LEN], stored_pass[MAX_LEN];
    int found = 0;

    system("cls");
    printf("=== Forgot Password ===\n");
    printf("Enter your username: ");
    fgets(username, MAX_LEN, stdin);
    username[strcspn(username, "\n")] = 0;

    FILE *file = fopen("user_data.txt", "r");
    if (!file) {
        printf("Could not open user data file.\n");
        printf("Press any key to return to menu...");
        _getch();
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
    _getch();
}

int tic_tac_toe() {
    int backmenu = 1;
    while (backmenu == 1) {
        char board[3][3];
        char difficulty;
        char pilihposisi, BOT = 'O', PLAYER = 'X';
        int playing = 1;

        srand((unsigned int)time(NULL));

        printf("Tic Tac Toe\n");
        printf("Pilih Mode Permainan:\n");
        printf("1. Easy (Bot)\n2. Medium (Bot)\n3. Hard (Bot)\n4. Tutorial\n\n[M] untuk keluar\n");
        printf("Masukkan pilihanmu (1-4): ");
        difficulty = _getch();
        printf("%c\n", difficulty);

        system("cls");

        switch (difficulty) {
            case '1':
                printf("Mode Easy dipilih.\n");
                _getch();
                break;
            case '2':
                printf("Mode Medium dipilih.\n");
                _getch();
                break;
            case '3':
                printf("Mode Hard dipilih.\n");
                _getch();
                break;
            case '4':
                showTutorial();
                printf("Tekan Enter untuk kembali ke menu...");
                while (_getch() != 13);
                system("cls");
                continue;
            case 'M':
            case 'm':
                return 0;
            default:
                printf("Pilihan tidak valid. Silakan pilih 1-5.\n");
                _getch();
                continue;
        }
        bool matching = 1;

while (playing == 1) {
    if (matching == 1 && (difficulty == '1' || difficulty == '2' || difficulty == '3')) {
    	system("cls");
        initBoard(board); 
        printf("Pilih posisimu!\n1. X\n2. O\n");
        pilihposisi = _getch();
        fflush(stdin);
        switch (pilihposisi){
            case '1':
                PLAYER = 'X';
                BOT = 'O';
                break;
            case '2':
                PLAYER = 'O';
                BOT = 'X';
                break;
            default:
                printf("Pilihan tidak valid. Silakan pilih 1 atau 2.\n");
                _getch();
                system("cls");
                continue;
	        }
	        // Main satu ronde
	        while (1) {
	        	system("cls");
	            printBoard(board);
	            playerMove(board, PLAYER);
	            if (isWinner(board, PLAYER)) {
	            	system("cls");
	                printBoard(board);
	                printf("Kamu menang!\n");
	                break;
	            }
	            if (isDraw(board)) {
	            	system("cls");
	                printBoard(board);
	                printf("Seri!\n");
	                break;
	            }
	
	            if (difficulty == '1') {
	                easyBotMove(board, BOT);
	            } else if (difficulty == '2') {
	                mediumBotMove(board, BOT, PLAYER);
	            } else if (difficulty == '3') {
	                hardBotMove(board, BOT, PLAYER);
	            }
	
	            if (isWinner(board, BOT)) {
	            	system("cls");
	                printBoard(board);
	                printf("Bot menang!\n");
	                break;
	            }
	            if (isDraw(board)) {
	            	system("cls");
	                printBoard(board);
	                printf("Seri!\n");
	                break;
	            }
	        }
	        // Setelah satu ronde selesai, tanya main lagi atau tidak
	        printf("Tekan Y untuk main lagi atau N untuk kembali ke menu...\n");
	        char choice = 0;
	        while (1) {
	            choice = _getch();
	            if (choice == 'Y' || choice == 'y') {
	                system("cls");
	                break; // akan mengulang while(playing==1)
	            } else if (choice == 'N' || choice == 'n') {
	                system("cls");
	                playing = 0; // keluar dari while(playing==1)
	                break;
	            } else {
	                printf("Masukkan Y untuk main lagi, N untuk kembali ke menu: ");
		            }
		        }
		    }
		}
	}
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

void playerMove(char board[3][3], char playerSymbol) {
    int move = -1;
    int row, col;
    while (1) {
        printf("Enter your move (1-9): ");
        if (scanf("%d", &move) != 1) {
            while (getchar() != '\n');
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
    board[row][col] = playerSymbol;
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

void easyBotMove(char board[3][3], char botSymbol) {
    int row, col;
    do {
        row = rand() % 3;
        col = rand() % 3;
    } while (!isValidMove(board, row, col));
    printf("Bot (Easy) memilih: %d %d\n", row, col);
    board[row][col] = botSymbol;
}

int tryWinOrBlock(char board[3][3], char testSymbol, char botSymbol) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = testSymbol;
                if (isWinner(board, testSymbol)) {
                    board[i][j] = botSymbol;
                    printf("Bot (Medium) memilih: %d %d\n", i, j);
                    return 1;
                }
                board[i][j] = ' ';
            }
        }
    }
    return 0;
}
  
void mediumBotMove(char board[3][3], char botSymbol, char playerSymbol) {
    if (tryWinOrBlock(board, botSymbol, botSymbol)) return;     
    if (tryWinOrBlock(board, playerSymbol, botSymbol)) return;  
    easyBotMove(board, botSymbol);
}

int minimax(char board[3][3], int depth, int isMaximizing, char botSymbol, char playerSymbol) {
    if (isWinner(board, botSymbol)) return 1;
    if (isWinner(board, playerSymbol)) return -1;
    if (isDraw(board)) return 0;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = botSymbol;
                    int score = minimax(board, depth + 1, 0, botSymbol, playerSymbol);
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
                    board[i][j] = playerSymbol;
                    int score = minimax(board, depth + 1, 1, botSymbol, playerSymbol);
                    board[i][j] = ' ';
                    if (score < bestScore) bestScore = score;
                }
            }
        }
        return bestScore;
    }
}

void hardBotMove(char board[3][3], char botSymbol, char playerSymbol) {
    int bestScore = -1000;
    int moveRow = -1, moveCol = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = botSymbol;
                int score = minimax(board, 0, 0, botSymbol, playerSymbol);
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
    board[moveRow][moveCol] = botSymbol;
}

int dino_highscore = 0;
int dino_color = 7; // Default: white

void set_text_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void save_dino_highscore(int score) {
    FILE *f = fopen("dino_highscore.txt", "w");
    if (f) {
        fprintf(f, "%d\n", score);
        fclose(f);
    }
}

int load_dino_highscore() {
    FILE *f = fopen("dino_highscore.txt", "r");
    int hs = 0;
    if (f) {
        fscanf(f, "%d", &hs);
        fclose(f);
    }
    return hs;
}

void dino_run() {
    dino_highscore = load_dino_highscore();
    int menu = 1;
    while (menu) {
        system("cls");
        set_text_color(dino_color);
        printf("============================\n");
		printf("||Selamat datang di DIN 0 RUN!|\n");
		printf("============================\n\n");
        printf("1. Ganti warna karakter\n");
        printf("2. Lihat highscore\n");
        printf("3. Mulai game\n");
        printf("M. Kembali ke menu game\n");
        printf("Pilih menu: ");
        char pilih = _getch();
        if (pilih == '1') {
            system("cls");
            printf("Pilih warna karakter:\n");
            printf("1. Putih\n2. Merah\n3. Hijau\n4. Kuning\n5. Biru\n");
            char w = _getch();
            switch(w) {
                case '1':
					dino_color = 7;
					break;
                case '2':
					dino_color = 12;
					break;
                case '3':
					dino_color = 10;
					break;
                case '4':
					dino_color = 14;
					break;
                case '5':
					dino_color = 9;
					break;
                default:
					dino_color = 7;
            }
        } else if (pilih == '2') {
            system("cls");
            printf("Highscore: %d\n", dino_highscore);
            printf("Tekan sembarang tombol untuk kembali...");
            _getch();
        } else if (pilih == '3') {
            // Mulai game
            int score = 0, y = 10, vy = 0, ground = 10, obstacle_x = 40, speed = 80, playing = 1;
            int gravity = 1, jump = -4;
            char dino = '0';
            while (playing) {
                // Input
                if (_kbhit()) {
                    char c = _getch();
                    if (c == ' ') {
                        if (y == ground) vy = jump;
                    }
                }
                // Physics
                y += vy;
                vy += gravity;
                if (y > ground) y = ground, vy = 0;
                // Move obstacle
                obstacle_x--;
                if (obstacle_x < 0) {
                    obstacle_x = 40;
                    score++;
                    if (speed > 30) speed -= 2; // tambah cepat
                }
                // Collision
                if (obstacle_x == 5 && y == ground) {
                    playing = 0;
                }
                // Draw
                system("cls");
                set_text_color(dino_color);
                for (int i = 0; i < y; i++) printf("\n");
                if (y+1<ground) printf("\r     %c\n", dino);
                for (int i = y+1; i < ground; i++) printf("\n");
                for (int i = 0; i < obstacle_x; i++) printf(" ");
                printf("|");
                if (ground<y+1) printf("\r     %c\n", dino);
                if (y+1<ground) printf("\n");
                printf("\nScore: %d\n", score);
                Sleep(speed);
            }
            
            // Game over
            system("cls");
            printf("GAME OVER!\nScore: %d\n", score);
            if (score > dino_highscore) {
                printf("Selamat! Highscore baru!\n");
                dino_highscore = score;
                save_dino_highscore(score);
            }
            printf("Tekan tombol apapun untuk kembali ke menu Dino Run...");
            _getch();
        } else if (pilih == 'M' || pilih == 'm') {
            menu = 0;
        }
    }
    set_text_color(7); // reset ke putih
}


void pacman_menu(){
	bool backmenu=true;
	while(backmenu){
	        system("cls");
	        const char *options[] = {
	            "1. Mulai Game",
	            "2. Lihat Highscore",
	            "3. Lihat Instruksi",
	            ">> Exit"
	        };
	        int num_options = 4;
	        int selected = 0;
	        int running = 1;
	
	        while (running) {
	            system("cls");
	            printf("=========================================================\n");
	            printf("||                   PACMAAAAN                         ||\n");
	            printf("=========================================================\n");
	            for (int i = 0; i < num_options; i++) {
	                printf("|| ");
	                if (i == selected) {
	                    printf("\033[1;32m%s\033[0m", options[i]);
	                } else {
	                    printf("%s", options[i]);
	                }
	                int spaces = 48 - (int)strlen(options[i]);
	                for (int s = 0; s < spaces; s++) printf(" ");
	                printf("||\n");
	            }
	            printf("=========================================================\n");
	            printf("(Select using arrow keys and press Enter)\n");
	
	            int ch = _getch();
	            if (ch == 224) { // Arrow keys
	                int arrow = _getch();
	                if (arrow == 72) { // Up arrow
	                    selected = (selected - 1 + num_options) % num_options;
	                } else if (arrow == 80) { // Down arrow
	                    selected = (selected + 1) % num_options;
	                }
	            } else if (ch == 13) { // Enter
	                running = 0;
	            }
	        }
	
	        int c;
	        char name[100];
			int hs = load_pacman_highscore(name);
	        switch (selected) {
	            case 0:
	                printf("\n");
	                system("cls");
	                pacman();                
	                break;
	            case 1:	                
				    system("cls");
				    if (hs > 0)
				        printf("Highscore adalah: %s dengan skor %d\n", name, hs);
				    else
				        printf("Belum ada highscore.\n");
				    getch();
	                break;
	            case 2:
	                pacman_instruction();
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

void load_maze() {
    FILE *f = fopen("map.txt", "r");
    if (!f) {
        printf("map.txt not found!\n");
        exit(1);
    }
    char buf[128];
    fgets(buf, sizeof(buf), f); // skip first line (size info)
    for (int i = 0; i < MAP_H; i++) {
        fgets(maze[i], sizeof(maze[i]), f);
        for (int j = 0; j < MAP_W-1; j++) {
            if (maze[i][j] == 'S') {
                pac_x = j;
                pac_y = i;
                maze[i][j] = ' '; // ubah jadi jalan kosong
            }
            if (maze[i][j] == 'H') {
                ghost_x = j;
                ghost_y = i;
                maze[i][j] = ' '; // ubah jadi jalan kosong
            }
        }
    }
    fclose(f);
}

void reset_maze() {
    load_maze();
    score = 0;
    people_saved = 0;
}

int all_people_saved() {
    for (int i = 0; i < MAP_H; i++)
        for (int j = 0; j < MAP_W; j++)
            if (maze[i][j] == 'P')
                return 0;
    return 1;
}

void draw_maze() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    for (int i = 0; i < MAP_H; i++) {
        for (int j = 0; j < MAP_W; j++) {
            if (i == pac_y && j == pac_x)
                printf("C");
            else if (i == ghost_y && j == ghost_x)
                printf("G");
            else if (maze[i][j] == '*') {
                SetConsoleTextAttribute(hConsole, 10); // green
                printf("%c", 219);
                SetConsoleTextAttribute(hConsole, 7);
            } else if (maze[i][j] == '|') {
                SetConsoleTextAttribute(hConsole, 11); // light blue
                printf("%c", 178);
                SetConsoleTextAttribute(hConsole, 7);
            } else
                putchar(maze[i][j]);
        }
//        printf("\n");
    }
    time_t now = time(NULL);
    int elapsed = (int)difftime(now, start);
    printf("\nScore: %d\nLife: %d\nPeople Saved: %d\nTime: %02d:%02d\n",
	score, life, people_saved, elapsed / 60, elapsed % 60);
}

int is_wall(int x, int y) {
    return maze[y][x] == '*' || maze[y][x] == '|';
}

void move_ghost() {
    int dx = 0, dy = 0;
    if (ghost_x < pac_x) dx = 1;
    else if (ghost_x > pac_x) dx = -1;
    if (ghost_y < pac_y) dy = 1;
    else if (ghost_y > pac_y) dy = -1;

    // lebih prioritasin horizontal drpd vertikal biar lebih gacor
    if (dx && !is_wall(ghost_x + dx, ghost_y))
        ghost_x += dx;
    else if (dy && !is_wall(ghost_x, ghost_y + dy))
        ghost_y += dy;
}

void pacman_instruction(){
	system("cls");
    printf("=========== PACMAN INSTRUCTIONS ===========\n");
    printf("Goal: Collect all points (+), save all people (P), and reach the finish (F).\n");
    printf("Controls:\n");
    printf("  Arrow keys : Move Pacman (C)\n");
    printf("  ESC        : Quit game\n");
    printf("Avoid the ghost (G)! You have 3 lives.\n");
    printf("Push blue walls (|) if needed to reach new areas.\n");
    printf("Game ends if time runs out or you lose all lives.\n");
    printf("===========================================\n");
    printf("Press any key to continue...\n");
    _getch();
}

void pacman() {
    reset_maze();
    start = time(NULL);
    life = MAX_LIFE;
    int playing = 1;

    while (playing) {
        draw_maze();
        if (difftime(time(NULL), start) >= 120) {
            printf("\nWaktu habis! Permainan selesai.\nSkor akhir: %d\n", score);
            printf("Tekan tombol apapun untuk kembali ke menu...");
            _getch();
            break;
        }

        // Handle input
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 27) { // ESC
                printf("\nPermainan dihentikan.\nSkor akhir: %d\n", score);
                save_pacman_highscore(score);

                printf("Tekan tombol apapun untuk kembali ke menu...");
                _getch();
                break;
            }

            if (ch == 224) { // Arrow keys
                int key = _getch();
                int dx = 0, dy = 0;
                switch (key) {
                    case 72: dy = -1; break; // Up
                    case 80: dy = 1; break;  // Down
                    case 75: dx = -1; break; // Left
                    case 77: dx = 1; break;  // Right
                }

                int tx = pac_x + dx, ty = pac_y + dy;
                if (tx >= 0 && tx < MAP_W && ty >= 0 && ty < MAP_H) {
                    char dest = maze[ty][tx];

                    if (dest == '|') {
                        int nnx = tx + dx, nny = ty + dy;
                        if (nnx >= 0 && nnx < MAP_W && nny >= 0 && nny < MAP_H && maze[nny][nnx] == ' ') {
                            maze[nny][nnx] = '|';
                            maze[ty][tx] = ' ';
                            pac_x = tx;
                            pac_y = ty;
                        }
                    } else if (!is_wall(tx, ty)) {
                        if (dest == '+') {
                            score += 1;
                            maze[ty][tx] = ' ';
                        } else if (dest == 'P') {
                            score += 5;
                            people_saved++;
                            maze[ty][tx] = ' ';
                        }
			            if (maze[ty][tx] == 'F') {
			                printf ("Kowe Menang!");
			                save_pacman_highscore(score);
			                getch();
			                fflush(stdin);
			                return;
						}
                        pac_x = tx;
                        pac_y = ty;
                    }
                }
            }
        }

        // Update state
        if (all_people_saved()) {
            for (int i = 0; i < MAP_H; i++)
                for (int j = 0; j < MAP_W; j++)
                    if (maze[i][j] == '|')
                        maze[i][j] = ' ';
        }

        move_ghost();

        // Collision check
        if (pac_x == ghost_x && pac_y == ghost_y) {
            life--;
            if (life == 0) {
                draw_maze();
                printf("\nPacman tertangkap! Nyawa habis.\nSkor akhir: %d\n", score);
                printf("Tekan tombol apapun untuk kembali ke menu...");
                save_pacman_highscore(score);
                _getch();
                break;
            } else {
                printf("\nPacman tertangkap! Nyawa tersisa: %d\nMasa game gampang gini aja gabisa?\n", life);
                Sleep(1200);
                reset_maze();
                start = time(NULL);
                continue;
            }
        }

        Sleep(30);
    }
}

void save_pacman_highscore(int score) {
    printf("\nMasukkan nama depan: ");
    char nama[100];
    scanf("%s", nama);

    FILE *f = fopen("pacman_highscore.txt", "a"); // append, not overwrite
    if (f) {
        fprintf(f, "%s ; %d\n", nama, score); // name first, then score
        fclose(f);
    }
}

int load_pacman_highscore(char *name) {
    FILE *f = fopen("pacman_highscore.txt", "r");
    if (!f) {
        printf("pacman_highscore.txt not found!\n");
        exit(1);
    }
    int hs = 0, temp_score;
    char line[256], temp_name[100];
    name[0] = '\0';
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, " %99[^;] ; %d", temp_name, &temp_score) == 2) {
            // Hilangkan spasi di akhir nama
            int len = strlen(temp_name);
            while (len > 0 && (temp_name[len-1] == ' ' || temp_name[len-1] == '\t')) {
                temp_name[--len] = '\0';
            }
            if (temp_score > hs) {
                hs = temp_score;
                strcpy(name, temp_name);
            }
        }
    }
    fclose(f);
    return hs;
}
