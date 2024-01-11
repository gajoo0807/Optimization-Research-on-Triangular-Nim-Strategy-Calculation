#include<iostream> 
#include <fstream> 
#include <bitset>
#include<vector>
#include <cmath>
#include <chrono>
#include <pthread.h>
#include <algorithm>
#include <deque>
using namespace std;

#define WALKNUM 196
#define LAYER 7
#define numNode 28

int walk[WALKNUM];	//儲存可行著手數
vector<int> ans(std::pow(2, 28), 0);
std::deque<int> cur_task;
pthread_mutex_t mutex1;

pthread_cond_t cond_consumer;

int task_id;
int Round = 28, int_Round = std::pow(2, 28) - 1;
int total_task = std::pow(2, 28) - 1;

struct ThreadArgs {
    int thread_id;
};

void readwalk(){
    int i; 
    std::fstream fin;
    fin.open("sort.txt", std::ios::in);
    for (i=0; i< WALKNUM; i++){
        int temp; 
        fin>>temp; 
        walk[i]=temp;
    }
    fin.close();
} 


// Visualize the current chessboard, where 1 represents X (piece taken), and 0 represents O (empty space).
void visual(int board){
    std::bitset<32> binary(board);
    for(int i = 0; i < LAYER; i++){
        for(int j = 0; j < LAYER - i; j++){
            std::cout << " ";
        }
        for(int j = 0; j < i + 1; j++){
            if(binary[(i * (i + 1) / 2 + j)] == 1){
                std::cout << "X ";
            }else{
                std::cout << "O ";
            }
        }
        std::cout << std::endl;
    }
}

// Initial sequential code.
void soul_seq(){
    int j; 
    int temp; 
    for(int i = std::pow(2, 28) - 1; i >= 0; i--){
        bool found = false;
        for(int j = 0; j < WALKNUM; j++){
            if((walk[j] & i) == 0){ 
                temp =	(walk[j] | i );	
                if(ans[temp] == 0){ 
                    ans[i] = temp; 
                    break;
                }
            }

        }
    }
}


// Consumer: receive one task from Producer and process it.
void* processTask(void* arg) {
    int i, id;
    int temp;
    int threadId = *(int*)arg;

    // 每個線程處理一個 cur_task 任務
    while(1){
        pthread_mutex_lock(&mutex1);
        while(cur_task.empty()){
            pthread_cond_wait(&cond_consumer, &mutex1); 
        }
        i = cur_task[0];
        if(i == -1){
            pthread_mutex_unlock(&mutex1);
            break;
        }
        cur_task.pop_front();
        pthread_mutex_unlock(&mutex1);
        for(int j = 0; j < WALKNUM; j ++){
            if((walk[j] & i) == 0){ 
                temp =	(walk[j] | i );	
                if(ans[temp] == 0){ 
                    ans[i] = temp; 
                    break;
                }
            }
        }
    }
    pthread_exit(NULL);
}

// Producer: generate all combinations of 28 bits. 2 thread
void* generateCombination(void* arg){
    int threadId = *(int*)arg;
    for (int i = numNode - 1; i >= 0; i--) {
        std::vector<bool> bitmask(numNode - 1, false);
        std::fill(bitmask.end() - i, bitmask.end(), true);  
        do{
            int temp = 0; 
            for (int j = 0; j < numNode; ++j) {
                if (bitmask[j]) {
                    temp += (1 << j);
                }
            }
            temp += (threadId << 27);
            pthread_mutex_lock(&mutex1); // 這邊會卡住，不知道為什麼
            cur_task.push_back(temp);
            pthread_cond_signal(&cond_consumer);
            pthread_mutex_unlock(&mutex1);
        }while(std::next_permutation(bitmask.begin(), bitmask.end()));
    }

    pthread_mutex_lock(&mutex1);
    cur_task.push_back(-1);
    pthread_cond_broadcast(&cond_consumer);
    pthread_mutex_unlock(&mutex1);
    pthread_exit(NULL);
}

void soul_parallels() {
    cur_task.push_back(std::pow(2, 28) - 1);
    const int num_threads = 4;
    pthread_t threads[num_threads];
    int thread_id[num_threads];

    for(int t = 0; t < 2; t ++){
        thread_id[t] = t;
        pthread_create(&threads[t], NULL, generateCombination, (void*)&thread_id[t]);
    }
    for (int t = 2; t < num_threads; t++) {
        thread_id[t] = t;
        pthread_create(&threads[t], NULL, processTask, (void*)&thread_id[t]);
    }

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }
}



void write_binary(){
    std::ofstream outfile("ans.bin", std::ios::binary);
    outfile.write(reinterpret_cast<const char*>(&ans[0]), ans.size() * sizeof(int));
    outfile.close();
}


int main(){
    pthread_cond_init(&cond_consumer, NULL);
    pthread_mutex_init(&mutex1, NULL);
    readwalk();
    soul_parallels(); 
    write_binary();
    pthread_mutex_destroy(&mutex1);
}