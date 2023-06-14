#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <algorithm>
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

void fileSort(string fileName, vector<vector<int32_t>> & numbers){
    vector<int32_t> buffer;
    int32_t n1, n2;
    ifstream file(fileName, ios::binary);
    if (file.is_open()) {
        file.read((char*)&n1, sizeof(int32_t));
        cout << n1 << endl;
        for(int i=0; i<n1; i++){
            file.read((char*)&n2, sizeof(int32_t));
            buffer.push_back(n2);
        }
    } else {
        throw ("error");
    }

    sort(buffer.begin(), buffer.end());
    cout << buffer.size() << endl;
    numbers.push_back(buffer);
    return;
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

    string input;
    vector<string> fileNames;
    vector<thread> threads;
    vector<vector<int32_t>> numbers;

    int i=0;
    getline(cin, input);
    fileNames= split(input, " ");

    for(int i=0; i<fileNames.size()-1; i++){
        threads.emplace_back(thread(fileSort, fileNames[i], ref(numbers)));
    }

    for(int i=0; i<fileNames.size()-1; i++) {
        threads[i].join();
    }
    merge(numbers, fileNames.back());
    return 0;
}

//from int to bin(32 bit)
/*
int main() {
    std::vector<int32_t> numbers = {3, 9, 5, 257};

    std::ofstream file("file3.bin", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(numbers.data()), numbers.size() * sizeof(int32_t));
        file.close();
        std::cout << "File binario creato con successo." << std::endl;
    } else {
        std::cout << "Impossibile aprire il file." << std::endl;
    }

    return 0;
}
*/

//from bin(32 bit) to int
/*
int main() {
    std::ifstream file("output.bin", std::ios::binary);
    if (file.is_open()) {
        // Determina la dimensione del file
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Calcola il numero di interi nel file
        std::size_t numIntegers = fileSize / sizeof(int32_t);

        // Crea un vettore per contenere gli interi
        std::vector<int32_t> numbers(numIntegers);

        // Leggi gli interi dal file
        file.read(reinterpret_cast<char*>(numbers.data()), fileSize);

        // Stampa gli interi
        for (const auto& number : numbers) {
            std::cout << number << " ";
        }
        std::cout << std::endl;

        file.close();
    } else {
        std::cout << "Impossibile aprire il file." << std::endl;
    }

    return 0;
}
*/