#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <algorithm>
#include <future>
#include <bits/stdc++.h>
#include <chrono>
#define TIMING

#ifdef TIMING
#define INIT_TIMER auto start = std::chrono::high_resolution_clock::now();
#define START_TIMER  start = std::chrono::high_resolution_clock::now();
#define STOP_TIMER(name)  std::cout << "RUNTIME of " << name << ": " << \
    std::chrono::duration_cast<std::chrono::milliseconds>( \
            std::chrono::high_resolution_clock::now()-start \
    ).count() << " ms " << std::endl;
#else
#define INIT_TIMER
#define START_TIMER
#define STOP_TIMER(name)
#endif
using namespace std;

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


vector<int32_t> merge(vector<vector<int32_t>> numbers){

    int n=0;
    vector<int32_t> fullVector;

    for(int i=0; i<numbers.size(); i++){
        fullVector.insert(fullVector.end(),numbers[i].begin(), numbers[i].end());
        n+=numbers[i].size();
    }
    sort(fullVector.begin(), fullVector.end());
    return fullVector;
}
auto fileSort = [](string fileName){
    vector<int32_t> buffer;
    int32_t n1, n2;
    ifstream file(fileName, ios::binary);
    if (file.is_open()) {
        file.read((char*)&n1, sizeof(int32_t));

        for(int i=0; i<n1; i++){
            file.read((char*)&n2, sizeof(int32_t));
            buffer.push_back(n2);
        }
    } else {
        throw ("error");
    }

    sort(buffer.begin(), buffer.end());
    cout << fileName << endl;
    return buffer;
};
int main() {
    INIT_TIMER
    string input;
    vector<string> fileNames;
    vector<thread> threads;
    vector<vector<int32_t>> numbers;
    vector<future<vector<int32_t>>>fut ;

    getline(cin, input);
    fileNames= split(input, " ");
    START_TIMER

    for(int i=0; i<fileNames.size()-1; i++){
        fut.push_back(std::async(launch::async, fileSort, fileNames[i]));
    }
    int i=0;
    int cont = 0;
    while(!fut.empty()){
        cont = 0;
        for(auto &f: fut){
            if(f.wait_for(0s)==std::future_status::ready){
                numbers.push_back(f.get());
                fut.erase(fut.begin()+cont);
                i++;
                cout << cont << endl;
                break;
            }
            cont++;
        }
        if(i>=2){
            numbers[0] = merge(numbers);
            numbers.erase(numbers.begin()+1, numbers.end());
            i--;
        }
    }
    ofstream of;
    of.open(fileNames.back(), ios::binary);
    if(of.is_open()){
        int n = numbers[0].size();
        of.write(reinterpret_cast<char*>(&n), sizeof(int32_t));
        of.write(reinterpret_cast<char*>(numbers[0].data()), numbers[0].size() * sizeof(int32_t));
        of.close();
    }else{
        throw ("error");
    }

    STOP_TIMER("finished merged: ")

    return 0;
}

