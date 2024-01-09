#include<iostream> 
#include <fstream> 
#include <stdio.h> 
#include <stdlib.h> 
#include <bitset>
#include<vector>
#include <cmath>
#include <omp.h>
#include <unistd.h>
#include <chrono>
#define LAYER 7
#define WALKNUM 196
#define TOTALPIECE 28
using namespace std;

int walk[196];	//儲存可行著手數
int blockid[3];
vector<int> ans(std::pow(2, 28), 0);

void readwalk(){
    int i; 
    std::fstream fin;
    fin.open("sort.txt", std::ios::in);
    for (i=0; i<WALKNUM; i++){
        int temp; 
        fin>>temp; 
        walk[i]=temp;
    }
    fin.close();
} 

void gowalk(int start, int end) {
    int last_situation = 0;
    for(int i = start; i >= end; i--){
        if(ans[i] == 0) { 
            for(int j = 0; j < WALKNUM; j++){
                if((walk[j] & i) == walk[j]){ 
                    last_situation = (walk[j] ^ i );	
                    ans[last_situation] = i;
                }
            }
        }
    }
}
void soul(){
    int last_situation; 
    gowalk((1 << 28)-1, (7 << 25));
    #pragma omp parallel num_threads(3) shared(walk, ans)
    {
        int tid = omp_get_thread_num();
        if(tid == 0) {
            gowalk((1 << 28) - (1 << 25) - 1, blockid[tid] << 25);
        } else if(tid == 1) {
            gowalk((1 << 28) - (1 << 26) - 1, blockid[tid] << 25);
        } else if(tid == 2) {
            gowalk((1 << 27) - 1, blockid[tid] << 25);
        }
        #pragma omp barrier
        if(tid == 0) {
            gowalk((1 << 28) - (1 << 26) - (1 << 25) - 1, 1 << 27);
        } else if(tid == 1) {
            gowalk((1 << 27) - (1 << 25) - 1, 1 << 26);
        } else if(tid == 2) {
            gowalk((1 << 26) - 1, 1 << 25);
        }
    }
    gowalk((1 << 25) - 1, 0);
}
void write_binary(){
    std::ofstream outfile("para3_ans.bin", std::ios::binary);
    outfile.write(reinterpret_cast<const char*>(&ans[0]), ans.size() * sizeof(int));
    outfile.close();
}

int main(){
    readwalk();
    blockid[0] = 6; // 110
    blockid[1] = 5; // 101
    blockid[2] = 3; // 011
    
    soul();
    write_binary();


}