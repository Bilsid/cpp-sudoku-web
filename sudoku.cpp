#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstdlib>

using namespace std;

const int N = 9;
const int EMPTY = 0;

void printBoard(const vector<vector<int>>& board) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << board[i][j];
        }
    }
}

bool isValid(const vector<vector<int>>& board, int row, int col, int num) {
    for (int i = 0; i < N; ++i) {
        if (board[row][i] == num || board[i][col] == num) return false;
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i + startRow][j + startCol] == num) return false;
        }
    }
    return true;
}

bool solveSudoku(vector<vector<int>>& board) {
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (board[row][col] == EMPTY) {
                for (int num = 1; num <= 9; ++num) {
                    if (isValid(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board)) return true;
                        board[row][col] = EMPTY;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

void fillDiagonal(vector<vector<int>>& board) {
    for (int i = 0; i < N; i += 3) {
        vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(nums.begin(), nums.end(), default_random_engine(seed));
        int idx = 0;
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                board[i + r][i + c] = nums[idx++];
            }
        }
    }
}

void removeDigits(vector<vector<int>>& board, int holes) {
    int count = holes;
    while (count > 0) {
        int cellId = rand() % (N * N);
        int i = cellId / N;
        int j = cellId % N;
        if (board[i][j] != EMPTY) {
            board[i][j] = EMPTY;
            count--;
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(0));
    vector<vector<int>> board(N, vector<int>(N, EMPTY));
    fillDiagonal(board);
    solveSudoku(board);

    // Save the full solution
    vector<vector<int>> solution = board;

    // Difficulty Logic (The New Update)
    int level = 3; // Default to Hard
    if (argc > 1) {
        level = atoi(argv[1]);
    }

    int holes;
    if (level > 10) {
        // Custom Mode: If input is > 10, use it as the exact hole count
        holes = level;
        // Cap it safely between 10 (Too Easy) and 64 (Insane)
        if (holes > 64) holes = 64;
        if (holes < 10) holes = 10;
    } else {
        // Standard Modes
        switch (level) {
            case 1: holes = 30; break; // Easy
            case 2: holes = 40; break; // Medium
            case 3: holes = 50; break; // Hard
            case 4: holes = 58; break; // Expert
            default: holes = 50;
        }
    }

    removeDigits(board, holes);

    printBoard(board);
    cout << " "; // Separator
    printBoard(solution);

    return 0;
}
