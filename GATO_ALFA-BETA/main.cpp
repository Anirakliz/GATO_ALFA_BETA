#include <iostream>
#include <vector>

using namespace std;

const int SIZE = 3;

enum class Player { None, Human, AI };

struct Move {
    int row;
    int col;
};

struct GameState {
    vector<vector<char>> board;
    Player currentPlayer;
};

bool isGameOver(const GameState& state) {
    const vector<vector<char>>& board = state.board;

    // Verificar filas
    for (int i = 0; i < SIZE; ++i) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][0] == board[i][2]) {
            return true;
        }
    }

    // Verificar columnas
    for (int i = 0; i < SIZE; ++i) {
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[0][i] == board[2][i]) {
            return true;
        }
    }

    // Verificar diagonales
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[0][0] == board[2][2]) {
        return true;
    }
    if (board[2][0] != ' ' && board[2][0] == board[1][1] && board[2][0] == board[0][2]) {
        return true;
    }

    // Verificar empate
    bool isBoardFull = true;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == ' ') {
                isBoardFull = false;
                break;
            }
        }
        if (!isBoardFull) {
            break;
        }
    }

    return isBoardFull;
}

int evaluateGameState(const GameState& state) {
    const vector<vector<char>>& board = state.board;

    // Verificar filas
    for (int i = 0; i < SIZE; ++i) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][0] == board[i][2]) {
            if (board[i][0] == 'X') {
                return 1;
            } else {
                return -1;
            }
        }
    }

    // Verificar columnas
    for (int i = 0; i < SIZE; ++i) {
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[0][i] == board[2][i]) {
            if (board[0][i] == 'X') {
                return 1;
            } else {
                return -1;
            }
        }
    }

    // Verificar diagonales
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[0][0] == board[2][2]) {
        if (board[0][0] == 'X') {
            return 1;
        } else {
            return -1;
        }
    }
    if (board[2][0] != ' ' && board[2][0] == board[1][1] && board[2][0] == board[0][2]) {
        if (board[2][0] == 'X') {
            return 1;
        } else {
            return -1;
        }
    }

    // Empate
    return 0;
}

int alphaBeta(const GameState& state, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (isGameOver(state) || depth == 0) {
        return evaluateGameState(state);
    }

    if (maximizingPlayer) {
        int maxEval = INT32_MIN;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (state.board[i][j] == ' ') {
                    GameState nextState = state;
                    nextState.board[i][j] = 'X';
                    nextState.currentPlayer = Player::AI;
                    int eval = alphaBeta(nextState, depth - 1, alpha, beta, false);
                    maxEval = max(maxEval, eval);
                    alpha = max(alpha, eval);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return maxEval;
    } else {
        int minEval = INT32_MAX;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (state.board[i][j] == ' ') {
                    GameState nextState = state;
                    nextState.board[i][j] = 'O';
                    nextState.currentPlayer = Player::Human;
                    int eval = alphaBeta(nextState, depth - 1, alpha, beta, true);
                    minEval = min(minEval, eval);
                    beta = min(beta, eval);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return minEval;
    }
}

Move findBestMove(const GameState& state) {
    int bestEval = INT32_MIN;
    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (state.board[i][j] == ' ') {
                GameState nextState = state;
                nextState.board[i][j] = 'X';
                nextState.currentPlayer = Player::AI;
                int eval = alphaBeta(nextState, SIZE * SIZE, INT32_MIN, INT32_MAX, false);
                if (eval > bestEval) {
                    bestEval = eval;
                    bestMove.row = i;
                    bestMove.col = j;
                }
            }
        }
    }

    return bestMove;
}

void printBoard(const vector<vector<char>>& board) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            cout << board[i][j];
            if (j < SIZE - 1) {
                cout << " | ";
            }
        }
        cout << endl;
        if (i < SIZE - 1) {
            cout << "---------" << endl;
        }
    }
}

int main() {
    vector<vector<char>> board(SIZE, vector<char>(SIZE, ' '));
    GameState state;
    state.board = board;
    state.currentPlayer = Player::Human;

    while (!isGameOver(state)) {
        printBoard(state.board);

        if (state.currentPlayer == Player::Human) {
            int row, col;
            cout << "Ingresa la fila (0-2): ";
            cin >> row;
            cout << "Ingresa la columna (0-2): ";
            cin >> col;

            if (state.board[row][col] != ' ') {
                cout << "Movimiento inválido. Intenta de nuevo." << endl;
                continue;
            }

            state.board[row][col] = 'O';
            state.currentPlayer = Player::AI;
        } else {
            cout << "Turno de la IA..." << endl;
            Move aiMove = findBestMove(state);
            state.board[aiMove.row][aiMove.col] = 'X';
            state.currentPlayer = Player::Human;
        }
    }

    printBoard(state.board);
    int gameResult = evaluateGameState(state);

    if (gameResult == 1) {
        cout << "¡Ganaste!" << endl;
    } else if (gameResult == -1) {
        cout << "¡Perdiste!" << endl;
    } else {
        cout << "¡Empate!" << endl;
    }

    return 0;
}
