#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define LENGTH 9
#define MAX_HINTS 3
#define EMPTY_CELL 99
#define BOARD_SIZE (LENGTH * LENGTH)

// Function prototypes for code to compile and make program readable by bottom top approach

int **create_answer_board();
int **create_player_board(int **a_board, int *total_solved);
void instructions();
int check_valid(int **solved, int row, int col, int num);
void give_hint(int **p_board, int **a_board, int row, int col);
void filled();
int valid_input(int cord);
void free_board(int **board);
void print_board(int **board);
void refresh_screen(int **p_board);
void play_game(int **a_board, int **p_board, int *t_solved, int *hint);

int main();

int main() {
    initscr();
    start_color(); // Enable color functionality for our game design
    init_pair(1, COLOR_CYAN, COLOR_BLACK);   // Color pair 1 for instructions
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Color pair 2 for numbers 
    init_pair(3, COLOR_RED, COLOR_YELLOW);   // Color pair 3 for Headlines
    init_pair(4, COLOR_RED, COLOR_YELLOW);    // Color pair 1 for Headlines
    init_pair(5, COLOR_GREEN, COLOR_BLACK);     // Color pair 5 for correct answer
    init_pair(6, COLOR_RED, COLOR_BLACK);       // Color pair 6 for incorrect answer
    

    cbreak();
    noecho();

    instructions();

    int ts = 0;
    int *t_solved = &ts; // total number of revealed numbers to keep track to end game

    int h = 0;
    int *hint = &h; // keeps track of total hints; game provides 3 hints

    int **a_board = create_answer_board();   //creates solution board
    int **p_board = create_player_board(a_board, t_solved); // creates player board to play the game

    print_board(p_board);
    
    play_game(a_board, p_board, t_solved, hint); // MAIN FUNCTION WITH ALL LOGIC TO PLAY GAME
    
    attron(COLOR_PAIR(3)| A_BOLD);
    printw("\n\nYou win!");
    attroff(COLOR_PAIR(3)| A_BOLD);
    
    refresh();

    sleep(5);
    clear();
    free_board(a_board); //frees the heap memory 
    free_board(p_board);    
    endwin();
    return 0;
}

/**
 * Function: create_answer_board
 * -----------------------------
 * Creates the answer board with predefined numbers.
 * Returns a pointer to the created answer board.
 */
int **create_answer_board() {
    static const int board_numbers[] = {
        5, 6, 1, 9, 2, 8, 3, 4, 7,
        3, 7, 8, 4, 1, 5, 9, 2, 6,
        4, 2, 9, 7, 6, 3, 1, 5, 8,
        8, 3, 2, 1, 5, 7, 4, 6, 9,
        7, 4, 5, 3, 9, 6, 2, 8, 1,
        1, 9, 6, 2, 8, 4, 7, 3, 5,
        9, 8, 4, 6, 7, 2, 5, 1, 3,
        2, 5, 7, 8, 3, 1, 6, 9, 4,
        6, 1, 3, 5, 4, 9, 8, 7, 2
    };

    int **board = malloc(sizeof(int *) * LENGTH);
    if (board == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < LENGTH; i++) {
        int *row = malloc(sizeof(int) * LENGTH);
        if (row == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < LENGTH; j++) {
            row[j] = board_numbers[i * LENGTH + j];
        }

        board[i] = row;
    }

    return board;
}
/**
 * Function: create_player_board
 * -----------------------------
 * Creates the player's board based on the answer board and randomly reveals some numbers.
 * Returns a pointer to the created player's board.
 */
int **create_player_board(int **a_board, int *total_solved) {
    int **board = malloc(sizeof(int *) * LENGTH);
    if (board == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    for (int i = 0; i < LENGTH; i++) {
        int *row = malloc(sizeof(int) * LENGTH);
        if (row == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        int revealed = 0;
        for (int j = 0; j < LENGTH; j++) {
            int num = rand() % 3;
            if (num == 2 && revealed < 4) {
                row[j] = a_board[i][j];
                revealed++;
                (*total_solved)++;
            } else if (j == LENGTH - 1 && revealed == 0) {
                row[j] = a_board[i][j];
                (*total_solved)++;
            } else {
                row[j] = EMPTY_CELL;
            }
        }

        board[i] = row;
    }

    return board;
}

/**
 * Function: instructions
 * -----------------------------
 * Displays the game instructions on the screen using colors and formatting.
 */
void instructions() {
    attron(COLOR_PAIR(3)| A_BOLD);
    printw("PLEASE RUN THE TERMINAL IN BIG WINDOW FOR BETTER EXPERIENCE\n\n");
    printw("Game Instructions:\n\n");
    attroff(COLOR_PAIR(3) | A_BOLD);
    attron(COLOR_PAIR(1)); // Use color pair 1 for instructions and make them bold
    printw("--Fill the board with numbers 1-9. Each number may only appear once in every row, column, and square.\n");
    printw("--The board has several numbers already filled in, and 'x's to represent empty spaces.\n\n");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(3)| A_BOLD);
    printw("How to Enter Numbers:\n\n");
    attroff(COLOR_PAIR(3)| A_BOLD);
    attron(COLOR_PAIR(1));
    printw("--First, enter the column, then row desired. Then, input the number you wish to put in the box.\n");
    printw("--Numbers will be automatically checked, and will only appear in the desired location if correct. If not correct, they will not appear.\n");
    printw("--There are three hints available. If you enter any column, any row, then h, then that box will be filled for you.\n");
    attroff(COLOR_PAIR(1) );
    attron(COLOR_PAIR(3)| A_BOLD);
    printw("\n\nImportant: \n\n");
    attroff(COLOR_PAIR(3)| A_BOLD);
    attron(COLOR_PAIR(1));
    printw("--During the game, please allow time for the screen to REFRESH between inputting answers\n\n");
    attroff(COLOR_PAIR(1));
    refresh();
}

/**
 * Function: check_valid
 * -----------------------------
 * Checks if a number input by the player is valid based on the solution board.
 * Returns 1 if the input is valid, 0 otherwise.
 */
int check_valid(int **solved, int row, int col, int num) {
    if (solved[row - '0'][col - '0'] == (num - '0')) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Function: give_hint
 * -----------------------------
 * Fills a box in the player's board with the correct number from the answer board as a hint.
 */
void give_hint(int **p_board, int **a_board, int row, int col) {
    p_board[row - '0'][col - '0'] = a_board[row - '0'][col - '0'];
}

/**
 * Function: filled
 * -----------------------------
 * Displays a message indicating that a box is already filled.
 */
void filled() {
    printw("This box was already filled");
    refresh();
    sleep(2);
}

/**
 * Function: valid_input
 * -----------------------------
 * Checks if the input is a valid number (0-8).
 * Returns 1 if valid, 0 otherwise.
 */
int valid_input(int cord) {
    if (cord < '0' || cord > '8') {
        printw("\nPlease enter a number 0-8");
        refresh();
        sleep(2);
        return 0;
    } else {
        return 1;
    }
}

/**
 * Function: free_board
 * -----------------------------
 * Frees the memory allocated for a board.
 */
void free_board(int **board) {
    for (int i = 0; i < LENGTH; i++) {
        free(board[i]);
    }
    free(board);
}

/**
 * Function: print_board
 * -----------------------------
 * Prints the current state of the board on the screen.
 */
void print_board(int **board) {
    attron(COLOR_PAIR(2) | A_BOLD);
    printw("     0 1 2   3 4 5   6 7 8\n");
    for (int i = 0; i < LENGTH; i++) {
        if (i % 3 == 0) {
            printw("----------------------------\n");
        }

        printw("%d  ", i);
        for (int j = 0; j < LENGTH; j++) {
            if (j % 3 == 0) {
                printw("| ");
            }
            if (board[i][j] == EMPTY_CELL) {
                printw("x ");
            } else {
                char num[50];
                sprintf(num, "%d", board[i][j]);
                printw("%c ", *num);
            }
        }
        printw("|\n");
    }
    printw("----------------------------\n\n");
    attroff(COLOR_PAIR(2) | A_BOLD);
    refresh();
}


/**
 * Function: refresh_screen
 * -----------------------------
 * Refreshes the screen with updated instructions and board.
 */
void refresh_screen(int **p_board) {
    clear();
    instructions();
    print_board(p_board);
}


/**
 * Function: play_game
 * -----------------------------
 * Manages the gameplay loop, taking player inputs and updating the board accordingly.
 */
void play_game(int **a_board, int **p_board, int *t_solved, int *hint) {
    while (*t_solved < BOARD_SIZE) {
        refresh_screen(p_board);
        attron(COLOR_PAIR(1) | A_BOLD);
        printw("Enter column: ");
        attroff(COLOR_PAIR(1) | A_BOLD);
        refresh();
        int col = getch();
        int val_c = valid_input(col);

        if (val_c == 0) {
            continue; // Go to the next iteration of the loop
        }
        attron(COLOR_PAIR(1) | A_BOLD);
        printw("\nEnter row: ");
        attroff(COLOR_PAIR(1) | A_BOLD);
        refresh();
        int row = getch();
        int val_r = valid_input(row);

        if (val_r == 0) {
            continue; 
        }

        if (p_board[row - '0'][col - '0'] != EMPTY_CELL) {
            filled();
            continue; 
        }
        attron(COLOR_PAIR(1) | A_BOLD);
        printw("\nEnter number: ");
        attroff(COLOR_PAIR(1) | A_BOLD);
        refresh();
        int num = getch();

        if ((num < '0' || num > '9') && num != 'h') {
            printw("\nPlease enter a valid input\n");
            refresh();
            sleep(2);
            continue; 
        } else if (num != 'h') {
            int v = check_valid(a_board, row, col, num);
            if (v == 1) {
                p_board[row - '0'][col - '0'] = (num - '0');
                (*t_solved)++;
                attron(COLOR_PAIR(5));
                printw(" Correct");
                attroff(COLOR_PAIR(5));
                refresh();
                sleep(2);
            } else {
                attron(COLOR_PAIR(6));
                printw(" Incorrect\n");
                attroff(COLOR_PAIR(6));
                refresh();
                sleep(2);
            }
        } else {
            if (*hint < MAX_HINTS) {
                give_hint(p_board, a_board, row, col);
                (*hint)++;
                (*t_solved)++;
            } else {
                printw("\nYou have used all your hints. Please wait for the board to refresh.");
                refresh();
                sleep(7);
                continue; 
            }
        }
    }
    return;
}
