#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <sstream>

#define LAYER 7
#define WALKNUM 196
#define TOTALPIECE 28
using namespace std;

/* this game is for Normal Play!! (the last player able to move is the winner) */
/* retrogade is ok too, no using misere_ans.bin as solution */
int win_situation[(1<<TOTALPIECE) - 1] = {0};
void printTriangle(int);
void playGame();
void printIntro();
int computerTurn(int chessboard);
void readall_walk(vector<int>& allwalk);
void printSpecialTriangle(int chessboard, int next_move);

// compare current chessboard and walk to see if we can make this move
bool check_legal_move(int chessboard, int walk) {
    if((chessboard & walk) == 0) {
        return true;
    }
    return false;
}
// make crossing integer pieces into a real triangular walk
int makeInputToWalk(std::vector<int>& board) { 
    int res = 0;
    for(int i = 0, tmp; i < board.size() && board[i] <= TOTALPIECE; i++) {
        if(board[i] <= TOTALPIECE && board[i] >= 1) {
            tmp = (1 << (board[i]-1)); // if board[i] == 4, then we get 0..010000 binary
            res = res | tmp;
        } else {
            cout << "Mistake: " << board[i] << " is not a valid piece, ignored." << endl;
        }
    }
    return res;
}

// check if it's a possible 7-layer move by searching all walks
bool check_possible_walk(int move, vector<int>& allwalk) {
    for(int i = 0; i < allwalk.size(); i++) {
        if(move == allwalk[i]) {
            return true;
        }
    }
    return false;
}

// return when user gives a legal move
int userTurn(int chessboard, vector<int>& userInputInt, vector<int>& allwalk) {
    std::string chess_out = "";

    while(1) { 
        for(int i = 0; i < LAYER; i++) {
            userInputInt[i] = TOTALPIECE + 1; // initialize to a number > TOTALPIECE
        }
        cout << "Please enter pieces you want to get rid of (separate with 1 space): ";
        std::getline(std::cin, chess_out);
        std::istringstream input_stream(chess_out);
        int a_piece_int, count = 0;
        while (input_stream >> a_piece_int && count < LAYER) {
            userInputInt[count++] = a_piece_int; // store user's input in vector
        }
        int user_next_walk = makeInputToWalk(userInputInt); // a move that will OR with chessboard
        
        if(check_legal_move(chessboard, user_next_walk) && check_possible_walk(user_next_walk, allwalk)) { // check if it's legal for the current situation
            chessboard = user_next_walk | chessboard;
            printSpecialTriangle(chessboard, user_next_walk);
            return chessboard;
        } else {
            cout << "This move is Illegal." << endl;
            printTriangle(user_next_walk);
            cout << "Please enter a new move. Error move: " << user_next_walk << endl;
        }
    }
    return -1;
}
void playGame() {
    system("clear"); // clear terminal appearance

    int chessboard = 0; // can be visualized as a chessboard

    vector<int> userInputInt(LAYER, 29); // user enters integers of pieces that will be crossed out
    vector<int> allwalk(WALKNUM, 0); // all possible walks
    readall_walk(allwalk);

    while(1) {
        cout << "Computer's Turn:" << endl;
        chessboard = computerTurn(chessboard); // calculate the result after computer moves pieces

        if(chessboard == ((1 << TOTALPIECE) - 1)) {
            cout << "Computer Wins!\nPress Enter to Leave..." << endl;
            string str_waiting;
            std::getline(std::cin, str_waiting);
            break;
        }
        
        cout << "\nYour Turn:" << endl;
        chessboard = userTurn(chessboard, userInputInt, allwalk);
        
        if(chessboard == ((1 << TOTALPIECE) - 1)) {
            cout << "You Win!\n Press Enter to Leave..." << endl;
            string str_waiting;
            std::getline(std::cin, str_waiting);
            break;
        }
        cout << "Computer is Calculating ... " << endl;
        sleep(2);
        system("clear");
    }
}

// win_situation stores the next chessboard integer that eventually wins
void read_win_situation() {
    std::fstream fin;
    fin.open("misere_ans.bin", std::ios_base::binary|std::ios_base::in);
    if (!fin.is_open()) {
        perror ("Error opening file");
    } 
    
    fin.read((char*)&win_situation[0], sizeof(win_situation));
    fin.close();
}

int main(int argc, char **argv) {
    read_win_situation();
    playGame();
    return 0;
}

// computer responds with our must-win move
int computerTurn(int chessboard) {
    if(check_legal_move(chessboard, (win_situation[chessboard]^chessboard))) {
        printSpecialTriangle(chessboard, (win_situation[chessboard]^chessboard));
        return win_situation[chessboard];
    } else {
        cout << "Computer doesn't find a move. You win." << " Error: " << (win_situation[chessboard] ^ chessboard) << endl;
        printTriangle((1 << TOTALPIECE) - 2);
        return ((1 << TOTALPIECE) - 2);
    }
}

// 7列三角殺棋視覺化, 顯示新增的動作
void printSpecialTriangle(int chessboard, int next_move) {
    if(chessboard >= (1 << TOTALPIECE)) {
        std::cout << "the board situation is not valid " << std::endl;
        return;
    }
    char symbol = 'O';
    for (int i = 1; i <= LAYER; ++i) {
        // Print spaces for formatting
        for (int j = 0; j < LAYER - i; ++j) {
            std::cout << " ";
        }

        // Print bits with 'O' or 'X'
        for (int j = 0; j < i; ++j) {
            if(next_move & 1) {
                symbol = '#';
            } else if((chessboard & 1) == 1) {
                symbol = 'X';
            } else {
                symbol = 'O';
            }
            std::cout << symbol << " ";
            chessboard >>= 1;
            next_move >>= 1;
        }

        std::cout << std::endl;
    }
}
// 7列三角殺棋視覺化
void printTriangle(int chessboard) {
    if(chessboard >= (1 << TOTALPIECE)) {
        std::cout << "the board situation is not valid " << std::endl;
        return;
    }
    char symbol = 'O';
    for (int i = 1; i <= LAYER; ++i) {
        // Print spaces for formatting
        for (int j = 0; j < LAYER - i; ++j) {
            std::cout << " ";
        }

        // Print bits with 'O' or 'X'
        for (int j = 0; j < i; ++j) {
            symbol = ((chessboard & 1) == 0) ? 'O' : 'X';
            
            std::cout << symbol << " ";
            chessboard >>= 1;
        }

        std::cout << std::endl;
    }
}
// read our 7-layer all possible walks
void readall_walk(vector<int>& allwalk) {
    
    std::fstream fin;
    fin.open("sort.txt", std::ios::in);
    if (!fin.is_open()) {
        perror ("Error opening file");
    } 
    int count = 0;
    while(fin>>allwalk[count++]) {}
    fin.close();
}
void printIntro() {

}


// turn cooridinate to the encoding of chess
// int translate(int row, int col) { 
//     int res = 0;
//     for(int i = 1; i < row; i++) {
//         res += i;
//     }
//     res += col;
//     return res;
// }