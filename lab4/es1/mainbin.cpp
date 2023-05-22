//
// Created by s319024 on 22/05/2023.
//
//from int to bin(32 bit)
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;
int main() {
    std::vector<int32_t> numbers = {100, 8287, 64344, 85289, 87166, 10812, 51426, 42098, 64202, 62351, 74984, 16601, 20902, 33788, 64382, 89314, 21620, 34044, 2596, 18887, 53640, 37710, 27793, 81085, 59339, 14901, 23008, 86498, 75205, 16275, 44962, 96636, 52779, 67692, 4169, 4735, 98252, 27236, 31448, 41979, 37774, 68053, 8649, 99339, 78824, 60299, 30235, 86479, 83770, 57456, 16234, 38529, 85604, 43448, 38996, 20264, 68824, 22675, 2070, 58913, 87748, 73640, 91003, 41541, 9013, 15457, 28115, 33908, 45610, 53067, 36027, 95162, 68511, 98300, 19278, 76259, 35599, 49899, 58335, 87875, 94044, 57024, 67028, 84142, 52578, 15030, 39809, 2093, 62544, 28897, 92351, 77560, 2181, 54909, 28901, 59280, 57234, 17177, 4930, 44427, 7015};

    std::ofstream file("f7.bin", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(numbers.data()), numbers.size() * sizeof(int32_t));
        file.close();
        std::cout << "File binario creato con successo." << std::endl;
    } else {
        std::cout << "Impossibile aprire il file." << std::endl;
    }

    return 0;
}


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