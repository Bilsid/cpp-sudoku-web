#include <iostream>
#include <algorithm>
#include <vector>
#include <random>   // NEW: Required for modern shuffling
#include <cstring>  // For memcpy

using namespace std;

// --- GLOBAL RANDOM ENGINE ---
// We create one shared random generator for the whole program
// This prevents "re-seeding" issues and makes it faster.
std::random_device rd;
std::mt19937 rng(rd());

// --- CORE FUNCTIONS ---

// Check if placing num at board[index] is valid
bool isSafe(int board[], int index, int num) {
    int row = index / 9;
    int col = index % 9;

    // Check Row
    for (int i = 0; i < 9; i++) {
        if (board[row * 9 + i] == num) return false;
    }

    // Check Column
    for (int i = 0; i < 9; i++) {
        if (board[i * 9 + col] == num) return false;
    }

    // Check 3x3 Box
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[(startRow + i) * 9 + (startCol + j)] == num) return false;
        }
    }

    return true;
}

// Generate a full valid board
bool solveBoard(int board[]) {
    int index = -1;
    for (int i = 0; i < 81; i++) {
        if (board[i] == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) return true; // No empty spots left = Solved

    // Randomize numbers 1-9 for variety
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // NEW: Modern Shuffle
    shuffle(nums.begin(), nums.end(), rng);

    for (int num : nums) {
        if (isSafe(board, index, num)) {
            board[index] = num;
            if (solveBoard(board)) return true;
            board[index] = 0; // Backtrack
        }
    }
    return false;
}

// --- UNIQUENESS CHECKER ---

// Modified solver that counts how many solutions exist
void countSolutions(int board[], int &count) {
    if (count > 1) return; // Optimization: Stop if we already found 2 solutions

    int index = -1;
    for (int i = 0; i < 81; i++) {
        if (board[i] == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        count++; // Found a solution
        return;
    }

    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, index, num)) {
            board[index] = num;
            countSolutions(board, count);
            board[index] = 0; // Backtrack
        }
    }
}

// Wrapper to check if board is unique
bool isUnique(int board[]) {
    int copyBoard[81];
    // Make a copy because the solver modifies the board
    memcpy(copyBoard, board, 81 * sizeof(int));

    int count = 0;
    countSolutions(copyBoard, count);
    return (count == 1);
}

// --- MAIN GAME LOGIC ---

void generatePuzzle(int board[], int holes) {
    // 1. Create a full solution first
    fill(board, board + 81, 0);
    solveBoard(board);

    // 2. Poke Holes
    vector<int> positions;
    for (int i = 0; i < 81; i++) positions.push_back(i);

    // NEW: Modern Shuffle
    shuffle(positions.begin(), positions.end(), rng);

    for (int i = 0; i < holes; i++) {
        board[positions[i]] = 0;
    }
}

int main(int argc, char* argv[]) {
    // No need for srand(time(0)) anymore!

    int holes = 30; // Default
    if (argc > 1) {
        string arg = argv[1];
        if (arg == "1") holes = 30;       // Easy
        else if (arg == "2") holes = 40;  // Medium
        else if (arg == "3") holes = 50;  // Hard
        else if (arg == "4") holes = 58;  // Expert
        else holes = atoi(argv[1]);       // Custom
    }

    // Safety clamp (Don't remove ALL numbers, or loop runs forever)
    if (holes > 64) holes = 64;

    int board[81];
    int solution[81];
    int attempts = 0;
    bool uniqueFound = false;

    // --- THE MAGIC LOOP ---
    // Keep generating until we find one with EXACTLY 1 solution
    while (!uniqueFound && attempts < 1000) {
        attempts++;

        generatePuzzle(board, holes);

        if (isUnique(board)) {
            uniqueFound = true;
        }
    }

    // Solve the unique board one last time to get the "Answer Key" string
    memcpy(solution, board, 81 * sizeof(int));
    solveBoard(solution);

    // Output: PUZZLE_STRING SOLUTION_STRING
    for (int i = 0; i < 81; i++) cout << board[i];
    cout << " "; // Space separator
    for (int i = 0; i < 81; i++) cout << solution[i];

    return 0;
}
