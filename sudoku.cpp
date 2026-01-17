#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cstdlib> // Needed for atoi (text to number)

using namespace std;

// A standard 9x9 grid
int grid[9][9] = {0};
int solution[9][9] = {0}; // New: To store the answer key

// Check if placing num at (row, col) is valid
bool isValid(int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (grid[row][i] == num || grid[i][col] == num) return false;
    }
    // Check 3x3 box
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i + startRow][j + startCol] == num) return false;
        }
    }
    return true;
}

// Simple Backtracking Solver to fill the board
bool fillBoard() {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (grid[row][col] == 0) {
                vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                // Shuffle to make it random every time!
                random_device rd;
                mt19937 g(rd());
                shuffle(nums.begin(), nums.end(), g);

                for (int num : nums) {
                    if (isValid(row, col, num)) {
                        grid[row][col] = num;
                        if (fillBoard()) return true;
                        grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    srand(time(0));

    // 1. Determine Difficulty
    // Default to Easy (30 holes) if no argument is given
    int holes = 30;

    if (argc > 1) {
        int level = atoi(argv[1]); // Convert text "2" to number 2

        switch(level) {
            case 1: holes = 30; break; // Easy
            case 2: holes = 40; break; // Medium
            case 3: holes = 50; break; // Hard
            case 4: holes = 58; break; // Expert
            default: holes = 30;
        }
    }

    // 2. Generate Board
    fillBoard();

    // 3. SAVE THE SOLUTION BEFORE WE POKE HOLES!
    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            solution[i][j] = grid[i][j];
        }
    }

    // 4. Remove Numbers (poke holes)
    for(int i = 0; i < holes; i++) {
        int r = rand() % 9;
        int c = rand() % 9;
        // If it's already 0, find another spot
        while(grid[r][c] == 0) {
             r = rand() % 9;
             c = rand() % 9;
        }
        grid[r][c] = 0;
    }

    // 5. Output
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << grid[i][j];
        }
    }

    cout << " "; // The separator
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << solution[i][j];
        }
    }

    return 0;
}
