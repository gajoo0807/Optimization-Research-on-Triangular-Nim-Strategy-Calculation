#include<iostream> 
#include <fstream> 
#include <stdio.h> 
#include <stdlib.h> 
#include <cmath>
#include<vector>
using namespace std;

long long walk[288];	//儲存可行著手數


int Linking(int nodei, int way, int num_nodes){
    // way = 0: right, way = 1: left down, way = 2: right down
    int total_layer = 7;
    int total_nodes = total_layer * (total_layer + 1) / 2;
    int new_node = 0;
    int layer = static_cast<int>(std::sqrt(2 * nodei + 0.25) - 0.5);
    int first_node = layer * (layer + 1) / 2, last_node = first_node + layer;
    if(way == 0){
        new_node = nodei + num_nodes;
        if(new_node > last_node) return -1;
        else return new_node;
    }else if(way == 1){
        new_node = nodei + layer * num_nodes + (1 + num_nodes) * num_nodes / 2;
        if(new_node >= total_nodes) return -1;
        else return new_node;
    }else if(way == 2){
        new_node = nodei + (layer + 1) * num_nodes + (1 + num_nodes) * num_nodes / 2;
        if(new_node >= total_nodes) return -1;
        else return new_node;
    }
    return 0;
}

void generating_walk(){
    int n = 0;
    int new_node = 0;
    vector<int> node_set;
    std::ofstream outFile("sort.txt");
    int count = 0;
    for(int i = 0; i < 28; i++){
        n = std::pow(2, i);
        // 將自己加入sort.txt
        std::cout << n << std::endl;
        outFile << n << std::endl; // 寫入 sort.txt
        count ++;
        for(int way = 0; way <= 2; way ++){ // way = 0: right, way = 1: left down, way = 2: right down
            n = std::pow(2, i);
            node_set.push_back(i);
            for(int k = 1; k <= 6; k++){
                new_node = Linking(i, way, k);
                if(new_node == -1) {
                    break;
                }
                n += std::pow(2, new_node);
                std::cout << n << std::endl;
                outFile << n << std::endl; // 寫入 sort.txt
                count ++;
                node_set.push_back(new_node);
            }
            std::cout << "node set: ";
            for(int j = 0; j < node_set.size(); j++){
                std::cout << node_set[j] << " ";
            }
            std::cout << std::endl;
            node_set.clear();
        }

    }
    outFile.close();
    std::cout << "count: " << count << std::endl;
}

int main (int argc, char *argv[]) {
    generating_walk();
    return 0;
}