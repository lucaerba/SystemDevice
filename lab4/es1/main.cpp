#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
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

void fileSort(promise<vector<::int32_t>>&& promise, string fileName){
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
    promise.set_value(buffer);
}
void merge(vector<vector<int32_t>> numbers, string out){
    ofstream of;
    int n=0;
    vector<int32_t> fullVector;
    of.open(out, ios::binary);
    if(of.is_open()){
        for(int i=0; i<numbers.size(); i++){
            fullVector.insert(fullVector.end(),numbers[i].begin(), numbers[i].end());
            n+=numbers[i].size();
        }
        sort(fullVector.begin(), fullVector.end());
        of.write(reinterpret_cast<char*>(&n), sizeof(int32_t));
        of.write(reinterpret_cast<char*>(fullVector.data()), fullVector.size() * sizeof(int32_t));
        of.close();
    }else{
        throw ("error");
    }
}

int main() {
    INIT_TIMER
    string input;
    vector<string> fileNames;
    vector<thread> threads;
    vector<vector<int32_t>> numbers;
    vector<future<vector<::int32_t>>> futures;

    getline(cin, input);
    fileNames= split(input, " ");

    START_TIMER
    for(int i=0; i<fileNames.size()-1; i++){
        promise<vector<::int32_t>> promise;

        futures.push_back(promise.get_future());
        threads.emplace_back(thread(fileSort, move(promise), fileNames[i]));
        threads[i].detach();
    }

    for(int i=0; i<fileNames.size()-1; i++) {
        numbers.push_back(futures[i].get());
    }
    merge(numbers, fileNames.back());

    STOP_TIMER("finished merged: ")
    return 0;
}
// f1.bin f2.bin f3.bin f4.bin f5.bin f6.bin f7.bin out.bin