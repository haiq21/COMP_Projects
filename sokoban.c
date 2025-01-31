// cs_sokoban
// Written by Haiqa Qazi (z5590411),
// on 03/06/24
// Implements the game, Sokoban. The program exits when a player wins, or
// decides to quit. 

#include <stdio.h>
#include <string.h>

#define ROWS 10
#define COLS 10
#define UNDO_SIZE 1000


// Every tile on the map has to be one of the following values.
enum base {
    NONE,
    WALL,
    STORAGE
};

// A single tile of our board.
// box should only contain the value:
// - true (1): there exists a box here
// - false (0): there doesn't exist a box here
struct tile {
    enum base base;
    int box;
};

// At any point of time, struct stores player position and count of valid 
// moves. 
struct player_position {
    int player_row;
    int player_col;
    int counter;
};

struct undo_state{
    struct tile board[ROWS][COLS];
    struct player_position position;
};

// Function Prototypes
void init_board(struct tile board[ROWS][COLS]);
void init_undo_array (struct undo_state undo_array[UNDO_SIZE]);
void print_line(void);
void print_title(void);
void print_board(struct tile board[ROWS][COLS], int player_row, int player_col);
int validate(int x, int y);
struct player_position move_w(
    struct tile board[ROWS][COLS], 
    struct player_position position
);
struct player_position move_a(
    struct tile board[ROWS][COLS], 
    struct player_position position
);
struct player_position move_s(
    struct tile board[ROWS][COLS], 
    struct player_position position
);
struct player_position move_d(
    struct tile board[ROWS][COLS],
    struct player_position position
);
int win_check(struct tile board[ROWS][COLS]);
void board_clone(
    struct tile board[ROWS][COLS],
    struct tile destination[ROWS][COLS]
);
void draw_walls(struct tile board[ROWS][COLS], int row, int col);
int setup_board(struct tile board[ROWS][COLS]);
int do_movement(struct tile board[ROWS][COLS], struct player_position position);
void setup_box(struct tile board[ROWS][COLS], int row, int col);
void setup_wall(struct tile board[ROWS][COLS], int row, int col);
struct player_position do_reset(
    struct tile board[ROWS][COLS],
    struct player_position position,
    struct tile cloned[ROWS][COLS],
    int reset_row,
    int reset_col
);
struct player_position do_wasd(
    struct tile board[ROWS][COLS], 
    struct player_position position, 
    char movement,
    struct undo_state undo_array[UNDO_SIZE]
);
int invalid_digit(int digit);
struct player_position setup_player (
    struct tile board[ROWS][COLS], 
    struct player_position position
);


int main(void) {
    struct tile board[ROWS][COLS];
    init_board(board);

    if (setup_board(board) == 0) {
        return 0;
    }

    struct player_position position;

    position.counter = 0;
    position.player_col = 0;
    position.player_row = 0;

    position = setup_player(board, position);

    printf("\n=== Starting Sokoban! ===\n");
    print_board(board, position.player_row, position.player_col);

    if (do_movement(board, position) == 0) {
        return 0;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// This function will print out the beginning functions, scanning the input and 
// running it through the respective function to store in the beginning 
// conditions. It also sets up until the player clicks q, moving to set up the
// player. 
int setup_board(struct tile board[ROWS][COLS]) {
    char input = 'x';

    printf("=== Level Setup ===\n");

    while (input != '\0') {
        int row = 0, col = 0;

        input = '\0';
        scanf("%c", &input);

        if (input == '\0') {
            return 0;
        }

        if (input == 'q') {
            break;
        }

        scanf(" %d %d", &row, &col);
        int bounds_ok = row <= 9 && row >= 0 && col <= 9 && col >= 0;

        if (bounds_ok == 0 && input != 'W') {
            printf("Location out of bounds\n");
            print_board(board, -1, -1);
        }

        if (input == 'w' && bounds_ok) {
            setup_wall(board, row, col);
        }

        // This places a storage location at the coordinates inputted by the 
        // user.
        if (input == 's' && bounds_ok) {
            board[row][col].base = STORAGE;
            print_board(board, -1, -1);
        }

        if (input == 'b' && bounds_ok) {
            setup_box(board, row, col);
        }

        if (input == 'W') {
            draw_walls(board, row, col);
        }
    }
    return 1;
}

// This function is called when the player inputs 'w'. This will place a wall 
// in the position given and returns in to the setup_board function. 
void setup_wall(struct tile board[ROWS][COLS], int row, int col) {
    board[row][col].base = WALL;
    if (board[row][col].box == 1) {
        board[row][col].box = 0;
    }

    print_board(board, -1, -1);
}

// This function is called when the player inputs 'b'. This will place a box in 
// the position that is inputted by the user on the board, and returns the board 
// to the setup_board function.

void setup_box(struct tile board[ROWS][COLS], int row, int col) {
    board[row][col].box = 1;

    if (board[row][col].base == WALL) {
        board[row][col].base = NONE;
    }

    print_board(board, -1, -1);
}

// The function is called when the player inputs 'W'. This will place walls
// in multiple positions of the board, from the beginning of the instructed
// coordinates placed by the user till the end. 

void draw_walls(struct tile board[ROWS][COLS], int row, int col) {
    int row_end = 0, col_end = 0;
    scanf(" %d %d", &row_end, &col_end);

    int bounds_fail_pos = (row > 9 && row_end > 9) || (col > 9 && col_end > 9);
    int bounds_fail_neg = (row < 0 && row_end < 0) || (col < 0 && col_end < 0);
    int bounds_fail = bounds_fail_pos || bounds_fail_neg;
    int col_index = 0, row_index = 0, i = 0, j = 0;

    if (row < 0 && (row_end > 0 && row_end < 9)) {
        row = 0;
    }

    if (row_end > 9 && (row > 0 && row < 9)) {
        row_end = 9;
    }

    if (col < 0 && (col_end > 0 && col_end < 9)) {
        col = 0;
    }

    if (col_end > 9 && (col > 0 && col < 9)) {
        col_end = 9;
    }

    if (bounds_fail == 1) {
        printf("Location out of bounds\n");
    }

    else {
        row_index = row;
        while (i <= (row_end - row)) {
            row_index = row + i;
            board[row_index][col].base = WALL;
            i++;
        }

        col_index = col;

        while (j <= (col_end - col)) {
            col_index = col + j;
            board[row][col_index].base = WALL;
            j++;
        }
    }

    print_board(board, -1, -1);
}

// The function defines what digits are within bounds for the player to be set.
int invalid_digit(int digit){

    return digit > 9 || digit < 0;

}

// Once the board is set, the function will scan in the player position and 
// checks if it is within the bounds. If this is valid, it will store the 
// player position and return it's new positions to the main function. 

struct player_position setup_player (
    struct tile board[ROWS][COLS], 
    struct player_position position
){
    int check = 0;

    while (check == 0) {
        printf("Enter player starting position: ");
        scanf(" %d %d", &position.player_row, &position.player_col);

        if (invalid_digit(position.player_row) || 
            invalid_digit(position.player_col)) {
            printf("Position (%d, %d) is invalid\n", 
            position.player_row, position.player_col);
            check = 0;
            continue;
        }
        if (board[position.player_row][position.player_col].base == WALL || 
            board[position.player_row][position.player_col].box == 1) {
            printf("Position (%d, %d) is invalid\n", 
            position.player_row, position.player_col);
            check = 0;
        }
        else {
            check = 1;
        }
    }

    return position;


}

// Once the player is set, this function is called and checks for the player
// movement, then calls the function to execute it. It takes the input until it 
// is given the CRTL-D key, which ends the game. It also allows the player to 
// undo, reset the game, anad check the count of (valid) moves. 
//  

int do_movement(
    struct tile board[ROWS][COLS], 
    struct player_position position) {

    struct tile cloned[ROWS][COLS];
    struct undo_state undo_array[UNDO_SIZE];
    init_board(cloned);
    board_clone(board, cloned);
    int reset_row = position.player_row;
    int reset_col = position.player_col;
    char movement = 'x';
    struct undo_state this_state;
    this_state.position = position;
    board_clone(board, this_state.board);
    undo_array[0] = this_state;

    while (movement != '\0') {
        movement = '\0';
        scanf("%c", &movement);

        if (movement == '\0') {
            return 0;
        }
        position = do_wasd(board, position, movement, undo_array);

        if (movement == 'c') {
            printf("Number of moves so far: %d\n", position.counter);
            print_board(board, position.player_row, position.player_col);
        }
        if (movement == 'r') {
            position = do_reset(board, position, cloned, reset_row, reset_col);
        }
        if (movement == 'u') {
            if(position.counter - 1 <0 ){
                position.counter = 1;
            }
            position = undo_array[position.counter -1].position;
            board_clone(undo_array[position.counter].board, board);
            print_board(board, position.player_row, position.player_col);
        }
        if (win_check(board) == 1) {
            if (position.counter == 1) {
                printf("=== Level Solved in 1 Move! ===\n");
            } else {
                printf("=== Level Solved in %d Moves! ===\n", position.counter);
            }
            return 0;
        }
    }
    return 1;
}

// This function resets the game to the board before any moves are executed.
// It also ensures that the counter is reset to zero. 

struct player_position do_reset(
    struct tile board[ROWS][COLS],
    struct player_position position,
    struct tile cloned[ROWS][COLS],
    int reset_row,
    int reset_col
) {

    board_clone(cloned, board);
    position.player_row = reset_row;
    position.player_col = reset_col;
    printf("=== Resetting Game ===\n");
    print_board(cloned, position.player_row, position.player_col);
    position.counter = 0;

    return position;
}


// The function calls the functions that allows the character to move up, down,
// left, and right according to the players wish. 

struct player_position do_wasd(
    struct tile board[ROWS][COLS],
    struct player_position position, 
    char movement,
    struct undo_state undo_array[UNDO_SIZE]
) {
    struct undo_state this_state;

    if (movement == 'w') {
        position = move_w(board, position);
        board_clone(board, this_state.board);
        this_state.position = position;
        undo_array[position.counter] = this_state;
        
    }

    if (movement == 'a') {
        position = move_a(board, position);
        board_clone(board, this_state.board);
        this_state.position = position;
        undo_array[position.counter] = this_state;
    }

    if (movement == 's') {
        position = move_s(board, position);
        board_clone(board, this_state.board);
        this_state.position = position;
        undo_array[position.counter] = this_state;
    }

    if (movement == 'd') {
        position = move_d(board, position);
        board_clone(board, this_state.board);
        this_state.position = position;
        undo_array[position.counter] = this_state;
    }

    return position;
}

// Moves the character up one position in the board, while also wrapping the 
// board and ensuring the player doesnt move into a wall. 

struct player_position move_w(struct tile board[ROWS][COLS], 
struct player_position position) {
    position.player_row = position.player_row - 1;

    if (position.player_row < 0) {
        position.player_row = 9;

        if (board[position.player_row][position.player_col].base == WALL) {
            position.player_row = 0;
            print_board(board, position.player_row, position.player_col);
            return position;
        }

        if (board[position.player_row][position.player_col].box == 1) {
            board[position.player_row][position.player_col].box = 0;
            board[position.player_row - 1][position.player_col].box = 1;
            position.counter++;
            print_board(board, position.player_row, position.player_col);
            return position;
        }
    }

    int box_location_w = position.player_row - 1;

    if (position.player_row - 1 < 0) {
        box_location_w = 9;
    }

    if (board[position.player_row][position.player_col].box == 1) {
        if (board[box_location_w][position.player_col].base == WALL) {
            position.player_row = position.player_row + 1;
            print_board(board, position.player_row, position.player_col);
            return position;
        } else {
            board[position.player_row][position.player_col].box = 0;
            board[box_location_w][position.player_col].box = 1;
        }
    }

    if (board[position.player_row][position.player_col].base == WALL) {
        position.player_row = position.player_row + 1;
        print_board(board, position.player_row, position.player_col);
        return position;
    }
    else {
        position.counter++;
        print_board(board, position.player_row, position.player_col);
    }

    return position;
}

// Moves the character down one position in the board, while also wrapping the 
// board and ensuring the player doesnt move into a wall. 
struct player_position move_s(struct tile board[ROWS][COLS], 
struct player_position position) {
    position.player_row = position.player_row + 1;

    if (position.player_row > 9) {
        position.player_row = 0;

        if (board[position.player_row][position.player_col].base == WALL) {
            position.player_row = 9;
            print_board(board, position.player_row, position.player_col);
            return position;
        }

        if (board[position.player_row][position.player_col].box == 1) {
            board[position.player_row][position.player_col].box = 0;
            board[position.player_row + 1][position.player_col].box = 1;
            position.counter++;
            print_board(board, position.player_row, position.player_col);
            return position;
        }
    }

    int box_location_s = position.player_row + 1;

    if (box_location_s > 9) {
        box_location_s = 0;
    }

    if (board[position.player_row][position.player_col].box == 1) {
        if (board[box_location_s][position.player_col].base == WALL) {
            position.player_row = position.player_row - 1;
            print_board(board, position.player_row, position.player_col);
            return position;
        } else {
            board[position.player_row][position.player_col].box = 0;
            board[box_location_s][position.player_col].box = 1;
        }
    }
    if (board[position.player_row][position.player_col].base == WALL) {
        position.player_row = position.player_row - 1;
        print_board(board, position.player_row, position.player_col);
        return position;
    }
    else {
        position.counter++;
        print_board(board, position.player_row, position.player_col);
    }

    return position;
}

// Moves the character left one position in the board, while also wrapping the 
// board and ensuring the player doesnt move into a wall. 

struct player_position move_a(struct tile board[ROWS][COLS], 
struct player_position position) {
    position.player_col = position.player_col - 1;

    if (position.player_col < 0) {
        position.player_col = 9;

        if (board[position.player_row][position.player_col].base == WALL) {
            position.player_col = 0;
            print_board(board, position.player_row, position.player_col);
            return position;
        }
        if (board[position.player_row][position.player_col].box == 1) {
            board[position.player_row][position.player_col].box = 0;
            board[position.player_row][position.player_col - 1].box = 1;
            position.counter++;
            print_board(board, position.player_row, position.player_col);
            return position;
        }
    }

    int box_location_a = position.player_col - 1;

    if (position.player_col - 1 < 0) {
        box_location_a = 9;
    }

    if (board[position.player_row][position.player_col].box == 1) {
        if (board[position.player_row][box_location_a].base == WALL) {
            position.player_col = position.player_col + 1;
            print_board(board, position.player_row, position.player_col);
            return position;
        } else {
            board[position.player_row][position.player_col].box = 0;
            board[position.player_row][box_location_a].box = 1;
        }
    }

    if (board[position.player_row][position.player_col].base == WALL) {
        position.player_col = position.player_col + 1;
        print_board(board, position.player_row, position.player_col);
        return position;
    } else {
        position.counter++;
        print_board(board, position.player_row, position.player_col);
    }

    return position;
}

// Moves the character right one position in the board, while also wrapping the 
// board and ensuring the player doesnt move into a wall. 
struct player_position move_d(struct tile board[ROWS][COLS], 
struct player_position position) {
    position.player_col = position.player_col + 1;

    if (position.player_col > 9) {
        position.player_col = 0;

        if (board[position.player_row][position.player_col].base == WALL) {
            position.player_col = 9;
            print_board(board, position.player_row, position.player_col);
            return position;
        }
        if (board[position.player_row][position.player_col].box == 1) {
            board[position.player_row][position.player_col].box = 0;
            board[position.player_row][position.player_col + 1].box = 1;
            position.counter++;
            print_board(board, position.player_row, position.player_col);
            return position;
        }
    }

    int box_location_d = position.player_col + 1;

    if (box_location_d > 9) {
        box_location_d = 0;
    }

    if (board[position.player_row][position.player_col].box == 1) {
        if (board[position.player_row][box_location_d].base == WALL) {
            position.player_col = position.player_col - 1;
            print_board(board, position.player_row, position.player_col);
            return position;
        } else {
            board[position.player_row][position.player_col].box = 0;
            board[position.player_row][box_location_d].box = 1;
        }
    }

    if (board[position.player_row][position.player_col].base == WALL) {
        position.player_col = position.player_col - 1;
        print_board(board, position.player_row, position.player_col);
        return position;
    } else {
        position.counter++;
        print_board(board, position.player_row, position.player_col);
    }

    return position;
}

// Checks if all boxes are in storage locations. Ends game with winning  
// statement if true.  
int win_check(struct tile board[ROWS][COLS]) {
    int i = 0;
    int j = 0;

    while (i < ROWS) {
        j = 0;

        while (j < COLS) {
            if (board[i][j].box == 1 && board[i][j].base != STORAGE) {
                return 0;
            }

            j++;
        }
        i++;
    }

    return 1;
}

// This function creates a copy of the current state of board, and is used for 
// the purpose of reset and undo. 

void board_clone(
    struct tile board[ROWS][COLS], 
    struct tile destination[ROWS][COLS]
) {

    int i = 0;
    int j = 0;

    while (i < ROWS) {
        j = 0;

        while (j < COLS) {
            destination[i][j] = board[i][j];
            j++;
        }
        i++;
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PROVIDED FUNCTIONS //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// initialises the board to default values.
void init_board(struct tile board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j].base = NONE;
            board[i][j].box = 0;
        }
    }
}

void init_undo_array (struct undo_state undo_array[UNDO_SIZE]){
    for (int i = 0; i < UNDO_SIZE; i++) {
        struct tile temp_board[ROWS][COLS];
        init_board(temp_board);
        board_clone(temp_board, undo_array[i].board);
        struct player_position temp_position;
        temp_position.player_col = 0;
        temp_position.player_row = 0;
        temp_position.counter = 0;
        undo_array[i].position = temp_position;

    }
}

// Helper function for print_board().
// Prints a line the width of the sokoban board.
void print_line(void) {
    for (int i = 0; i < COLS * 4 + 1; i++) {
        printf("-");
    }
    printf("\n");
}

// Helper function for print_board().
// Prints out the title for above the sokoban board.
void print_title(void) {
    print_line();
    char *title = "S O K O B A N";
    int len = COLS * 4 + 1;
    int n_white = len - strlen(title) - 2;
    printf("|");
    for (int i = 0; i < n_white / 2; i++) {
        printf(" ");
    }
    printf("%s", title);
    for (int i = 0; i < (n_white + 1) / 2; i++) {
        printf(" ");
    }
    printf("|\n");
}

// Prints out the current state of the sokoban board.
// It will place the player on the board at position player_row, player_col.
// If player position is out of bounds, it won't place a player anywhere.
void print_board(
    struct tile board[ROWS][COLS], int player_row, int player_col) {
    print_title();
    for (int i = 0; i < ROWS; i++) {
        print_line();
        for (int j = 0; j < COLS; j++) {
            printf("|");

            struct tile curr = board[i][j];
            if (i == player_row && j == player_col) {
                printf("^_^");
            } else if (curr.base == WALL) {
                printf("===");
            } else if (curr.box && curr.base == STORAGE) {
                printf("[o]");
            } else if (curr.box) {
                printf("[ ]");
            } else if (curr.base == STORAGE) {
                printf(" o ");
            } else {
                printf("   ");
            }
        }
        printf("|\n");
    }
    print_line();
    printf("\n");
}
