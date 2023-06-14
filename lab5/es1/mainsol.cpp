/* Exercise 01
C++ or C synch
--------------

Write a small C++ program for simulating the control of a bar to let cars enter
and exit a parking lot.
For the sake of simulation, consider that a car:
- arrives every t1 random seconds (1<= t1 <= 3)
- leaves the parking lot every t2 random seconds (4<= t2 <=7).
If a car cannot enter because the parking lot is complete, it will just search
for parking somewhere else (hence, no queues will form in front of the bar
at the entrance).

The following parameters can be initialized within the program:
-  Number of places in the parking lot;
-  Duration of the simulation (in seconds), computed starting from the first car
   that enters.

Use the following function to put a thread in the sleep status (e.g., for 1 second):
std::this_thread::sleep_for (std::chrono::seconds(1))

Write the code of the program and manage threads synchronization.
Make sure all threads finish running before the main program terminates.
 */

#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <mutex>

#define DURATION 60
#define SIZE 4

using namespace std;

void incr(int &num, mutex &mtx, int &t, mt19937 &rng) {
    std::uniform_int_distribution<unsigned int> dist(1, 3);
    bool stop = false;
    do {
        if (num < SIZE) {
            lock_guard<mutex> lock(mtx);
            ++num;
            cout << "  1 in" << endl;
        }else{

            cout << "  1 ciaone" << endl;
        }
        int delay = dist(rng);
        if (t + delay < DURATION)
            this_thread::sleep_for(std::chrono::seconds(delay));
        else
            stop = true;
    }
    while (!stop);
}

void decr(int &num, mutex &mtx, int &t, mt19937 &rng) {
    std::uniform_int_distribution<unsigned int> dist(4, 7);
    bool stop = false;
    do {
        int delay = dist(rng);
        if (t + delay < DURATION)
            this_thread::sleep_for(std::chrono::seconds(delay));
        else
            stop = true;

        if (num>0)
        {
            lock_guard<mutex> lock(mtx);
            --num;
            cout << "  1 out " << endl;
        }
    }
    while (!stop);
}

int main(void) {
    int my_time = 0;
    int occupied = 0;
    mutex mtx;

    std::random_device rd;           // Inizializza un generatore di numeri casuali
    std::mt19937 mt(rd());          // Utilizza il generatore Mersenne Twister

    thread t1(incr, ref(occupied), ref(mtx), ref(my_time), ref(mt));
    thread t2(decr, ref(occupied), ref(mtx), ref(my_time), ref(mt));

    do {
        this_thread::sleep_for(std::chrono::seconds(1));
        ++my_time;
        cout << "Time " << my_time << ": " << occupied << "/" << SIZE << endl;
    }
    while (my_time < DURATION);

    t1.join();
    t2.join();
    return 0;
}