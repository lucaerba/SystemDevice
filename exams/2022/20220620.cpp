/*
    @blocking
    typical IO call, calling a read() will block the execution until data available

    @non-blocking
    while a blocking call stop the execution, a non-blocking call try to do the operation, if not possible continues with normal execution 
    setting the O_NONBLOCK open() flag to say to not block on read 

    @multiplexed
    you start to wait the first call between a set of calls, you catch the first that get the lock or complete the operation depending on task
    FD_ZERO(&set)
    FD_SET(fd1, &set)
    FD_SET(fd2, &set)

    if(select(set, max, null, null))

    @asynchronous
    for example using thread, you can use a thread to wait for something, while the main one continue his normal flow, when the call completes, the thread will manage the return.
    An example is using aio_read() aio_write()


*/


////
#define N 20
#include <semaphore.h>

sem_t* semaphores[N];
int code[20] = {0};

void wait_ch(int x){
    code[x] ++;

    sem_wait(semaphores[x]);
    
    code[x] --;
}

void signal_ch(int y){
    int n;
    for(int i=0; i<code[y]; i++){ 
        sem_post(semaphores[y]);
    }
}

void TA(){

}
void TB(){
    
}
void TC(){
    
}
void TD(){
    
}
void TE(){
    
}

int main(int argc, char* argv[]){
    for(int i=0; i<N; i++){
        sem_init(semaphores[i], 1, 0);
    }
}



////
/*  
    n=3
    p-> f,p -> f1,f2,p1,p2 -> 2 X
    n=2
    p1-> f,p -> f1,f2,p1,p2 -> 2 X
    f1-> f,p -> f1,f2,p1,p2 -> 2 X
    n=1
    p1-> f,p -> f1,f2,p1,p2 -> 2 X
    f1-> f,p -> f1,f2,p1,p2 -> 2 X
    p1-> f,p -> f1,f2,p1,p2 -> 2 X
    f1-> f,p -> f1,f2,p1,p2 -> 2 X
    
    8+4+2 = 14
*/



///////////////
#include <pthread.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

#define ELEV_SPEED 2
#define SIM_TIME 30

using namespace std;

int floor;
int occupied = 0;
mutex mtx;
condition_variable cv;

void *generatePeopleWaiting(void *){
    while(1){
        this_thread::sleep_for(rand()%3+1);
        unique_lock<mutex> lock(mtx);
        
        int r_plan = rand()%3;
        cout << "Person arrived at floor: " << r_plan << endl; 

        if(!occupied && floor != r_plan){
            floor = r_plan;
            occupied = 1;
            cv.notify_all();
        }
        lock.unlock();
    }
}

void *elevator(void *){
    while(1){
        unique_lock<mutex> lock(mtx);
        while(!occupied){
            cv.wait(lock);
        }
        cout << "Going at plan: " << floor << endl;
        
        this_thread::sleep_for(2);
        
        cout << "finished: ";
        
        occupied = 0;
        lock.unlock();
    }
}


int main(int argc, char* argv){
    pthread_t th1, th2;

    srand(time(NULL));
    
    pthread_create(&th1, NULL, generatePeopleWaiting, NULL );
    pthread_create(&th2, NULL, elevator, NULL );

    this_thread::sleep_for(SIM_TIME);

    pthread_cancel(th1);
    pthread_cancel(th2);

    return 0;
}

/**
 * A*{B|C}D^3
*/
sem_t A, BorC, D;

void* TA(void *){
    
    while(1){
        sem_wait(&A);
        int nA = rand();
        for(int i=0; i<nA ; i++){
            cout << "A";
        }
        sem_post(&BorC);
    }
}

void* TB(void *){

    while(1){
        sem_wait(&BorC);
        cout << "B";
        sem_post(&D);
    }
}

void* TC(void *){

    while(1){
        sem_wait(&BorC);
        cout << "C";
        sem_post(&D);
    }
}

void* TD(void *){

    while(1){
        sem_wait(&D);
        for(int i=0; i<3; i++) cout << "D";
        sem_post(&A);
    }
}

int main(int argc, char* argv[]){
    pthread_t a, b, c, d;

    srand(time(NULL));

    sem_init(&A, 0, 1);
    sem_init(&BorC, 0, 0);
    sem_init(&D, 0, 0);

    pthread_create(&a, NULL, TA, NULL);
    pthread_create(&b, NULL, TB, NULL);
    pthread_create(&c, NULL, TC, NULL);
    pthread_create(&d, NULL, TD, NULL);
}