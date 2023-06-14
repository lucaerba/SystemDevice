
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include <mutex>
#include <algorithm>
#include <condition_variable>

#define MIN1 1
#define MIN2 4
#define MAX1 3
#define MAX2 7

using namespace std;
void cars(vector<int> *parking, int id);

int n_places;
int n_seconds;
bool hastoterminate = false;
mutex mtx1;
condition_variable cv;

void cars_in(){
    int n=0;
    vector<thread> threads;
    vector<int> parking;

    while(!hastoterminate){
        int wait_time;

        wait_time = (rand() % (MAX1-MIN1+1)) + MIN1;
        this_thread::sleep_for(chrono::seconds(wait_time));
        threads.emplace_back(cars, &parking, n++);
    }

    for(int i = 0; i<threads.size(); i++){
        threads[i].join();
    }
}

void cars(vector<int> *parking, int id){
    int wait_time;
    //lock
    {
        std::lock_guard<std::mutex> lock(mtx1);

        if(parking->size() >= n_places) {
            cout << id << " Porco dio manco qua ce posto" << endl;
            return;
        }
        parking->push_back(id);
        for(auto c: *parking)
            cout << c << " ";
        cout << endl;
    }
    //unlock

    wait_time = (rand() % (MAX2-MIN2+1)) + MIN2;
    this_thread::sleep_for(chrono::seconds(wait_time));

    //lock
    {
        std::lock_guard<std::mutex> lock(mtx1);

        parking->erase(remove(parking->begin(), parking->end(), id), parking->end());
        cout << id << " se ne va" << endl;
    }
    //unlock
}
int main(int argc, char* argv[]){
    thread generator;
    srand(time(NULL));

    if(argc != 3){
        return -1;
    }
    n_places = stoi(argv[2]);
    n_seconds = stoi(argv[1]);
    generator = thread(cars_in);
    this_thread::sleep_for(chrono::seconds(n_seconds));

    hastoterminate = true;

    generator.join();

    return 0;
}