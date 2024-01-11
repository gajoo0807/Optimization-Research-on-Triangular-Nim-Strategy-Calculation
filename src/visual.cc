#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>

#define LAYER 7
#define WALKNUM 194
void printTriangle(int chessboard) {
    // 7列三角殺棋視覺化
    int bitIndex = 0;
    if(chessboard >= (1 << 28)) {
        std::cout << "the board situation is not valid " << std::endl;
        return;
    }
    std::string symbol = "O ";
    for (int i = 1; i <= LAYER; ++i) {
        // Print spaces for formatting
        for (int j = 0; j < LAYER - i; ++j) {
            std::cout << "  ";
        }

        // Print bits with 'O' or 'X'
        for (int j = 0; j < i; ++j) {
            symbol = ((chessboard & 1) == 0) ? "O " : "X ";
            
            std::cout << symbol << "  ";
            chessboard >>= 1;
        }
        std::cout << std::endl;
    }
}

void printSampleTriangle() {
    int seq = 0;
    for (int i = 1; i <= LAYER; ++i) {
        // Print spaces for formatting
        for (int j = 0; j < LAYER - i; ++j) {
            std::cout << "  ";
        }
        
        // Print bits with 'O' or 'X'
        for (int j = 0; j < i; ++j) {
            seq++;
            if(seq < 10) {
                std::cout << " ";
                
            } 
            std::cout << seq << "  "; 
        }

        std::cout << std::endl;
    }
}
int main(int argc, char **argv) {
    
    // 呼叫函數印出三角形
    int test = argc == 2 ? std::stoi(argv[1]) : 0;
    printTriangle(test);
    printSampleTriangle();
    return 0;
}
