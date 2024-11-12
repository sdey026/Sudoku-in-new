
# Grid Game

## Overview
Welcome to the Grid Game project! This game uses a two-dimensional grid to create an engaging and interactive Sudoku experience. The game is implemented in C and uses the ncurses library for terminal-based graphics. The primary goal is to fill t
## Feat
- A playable Sudoku game with a 9x9 grid.
- Pre-filled numbers and hints to assist the player.
- Input validation to ensure correct and intuitive gameplay.
- Ncurses-based graphical interface for an enhanced user experience.

## Getting Started

### Prerequisites
Make sure you have the ncurses library installed on your system. You can install it using the following command:
```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

### Clone the Repository
Clone the repository to your local machine:
```bash
git clone https://github.com/raghavrajsah/Sudoku-in-C.git

```

### Compile the Game
Use the included Makefile to compile the game:
```bash
make
```

## How to Play

### Running the Game
Start the game by running the compiled executable:
```bash
./game
```

### Game Instructions
- **Objective**: Fill the 9x9 grid with numbers 1-9 such that each number appears exactly once in each row, column, and 3x3 subgrid.
- **Input**: Enter the column and row coordinates followed by the number you wish to place. If the placement is correct, the number will appear; otherwise, it will not.
- **Hints**: You have 3 hints available. Enter any column and row coordinates followed by 'h' to use a hint, which will fill the cell with the correct number.
- **End Game**: The game ends when all cells are correctly filled.

### Controls
- **Column Input**: Enter the column number (0-8).
- **Row Input**: Enter the row number (0-8).
- **Number Input**: Enter the number (1-9) to place in the selected cell or 'h' for a hint.

## Example Gameplay
1. Start the game.
2. Follow the on-screen instructions to fill the grid.
3. Use hints wisely to help solve difficult cells.
4. The game announces a win once all cells are filled correctly.

## Implementation Details

### Main Functions
- **create_answer_board()**: Initializes the answer board with predefined numbers.
- **create_player_board()**: Creates the player's board with some cells pre-filled for gameplay.
- **instructions()**: Displays the game instructions using ncurses.
- **check_valid()**: Validates player inputs against the solution board.
- **give_hint()**: Provides a hint by filling a cell with the correct number.
- **play_game()**: Main game loop handling user input and game logic.
- **refresh_screen()**: Refreshes the screen with updated instructions and the current state of the board.
- **print_board()**: Prints the current state of the board to the screen.

### Memory Management
- **free_board()**: Frees the dynamically allocated memory for the game boards to prevent memory leaks.

### Ncurses
- The game uses ncurses for a terminal-based graphical interface, providing an interactive and visually appealing experience.

