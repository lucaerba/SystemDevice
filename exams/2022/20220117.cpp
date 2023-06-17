

/// FIFO

struct record_s
{
    int id;
    char s1[N], s2[N];
    float f;
} record_t;

int fdPW, fdPR;

int main(int argc, char *argv[])
{
    char pathPW[] = "./fifoPW";
    char pathPR[] = "./fifoPR";

    mkfifo(pathPW, 0666);
    mkfifo(pathPR, 0666);
    // boht read and write
    fdPW = open(pathPW, O_RDWR);
    fdPR = open(pathPR, O_RDWR);

    if (fork() > 0)
    {
        PW();
    }
    else
    {
        PR();
    }

    return 0;
}

void PW()
{
    char okres[5];

    while (1)
    {
        record_t newr;

        generateLine(&newr);

        write(fdPW, &newr, sizeof(record_t));
        read(fdPR, okres, sizeof(char) * 5);
    }
}

void PR()
{
    char okres[] = "fatto\0";

    while (1)
    {
        record_t newr;

        read(fdPW, &newr, sizeof(record_t));

        write(fdPR, okres, sizeof(char) * 5);
    }
}

struct record_s
{
    int id;
    char s1[N], s2[N];
    float f;
} record_t;

struct MessagePW
{
    long mtype;
    record_t product;
};

struct MessagePR
{
    long mtype;
    char text[5];
};
/// msgQueue
void PR()
{
    key_t key = ftok("./messages", 42);
    int msgqid = msgget(key, IPC_CREAT | 0666);

    MessagePW m;
    m.mtype = 1;
    m2.mtype = 10;

    while (1)
    {

        msgrcv(msgqid, &m, sizeof(record_t), 0);
        print(m.product);
    }
}

void PW()
{
    key_t key = ftok("./messages", 42);
    int msgqid = msgget(key, IPC_CREAT | 0666);

    MessagePW m;
    m.mtype = 1;
    m2.mtype = 10;

    while (1)
    {

        generateProduct(&(m.product));

        msgsnd(msgqid, &m, sizeof(record_t), 0);
    }
}

int main(int argc, char *arv[])
{
}

///condition variable 

#include <thread>
#include <condition_variable>
#include <mutex>

using namespace std;

int wallet = 3;

condition_variable cv;
mutex m;

void pay(int pay_amount){
    unique_lock<mutex> lock(m);

    while(wallet < pay_amount){
        cv.wait(lock);
    }
    
    //atomic
    wallet -= pay_amount;
    
    lock.unlock();
}

void top_up_wallet(){
    unique_lock<mutex> lock(m);
    
    //atomic
    wallet += 10;
    
    cv.notify_one();
    lock.unlock();

}

int main(){   
    pthread_cond_init();
    thread a(pay, 5);
    thread b(top_up_wallet);
    a.join();
    b.join();
    return 0;
}


#include <pthread.h>
#include <semaphore.h>
//T1 --> T2, T3, T3 --> T4 --> T1

sem_t sem1, sem23, sem4; //1, 0, 0 as init
////cycle
void* T3(){
    while (1){
        sem_wait(&sem23);

        //do T3 things

        sem_post(&sem4);
    }
}

void* T2(){
    while (1){
        sem_wait(&sem23);

        //do T2 things

        sem_post(&sem4);
    }
    
}

void* T1(){
    while(1){
        sem_wait(&sem1);

        //do T1 things

        sem_post(&sem23);
        sem_post(&sem23);
        sem_post(&sem23);

    }
}

void* T4(){
    while (1){
        sem_wait(&sem4);
        sem_wait(&sem4);
        sem_wait(&sem4);

        //do T4 things

        sem_post(&sem1);
    }
    
}


////

#include <stdlib.h>
#include <time.h>
#include <thread>
#include <mutex>

#define NPLACES 5
#define TIME_SIM 10

int carsInPark = 0;
mutex m;
using namespace std;

void *carProducer(){
    while(1){
        this_thread::sleep_for(rand()%3+1);

        unique_lock<mutex> lock(m);
        if(carsInPark < NPLACES)
            carsInPark ++;

        lock.unlock();
    }
}

void *carLeaving(){
    while(1){
        this_thread::sleep_for(rand()%3+1);

        unique_lock<mutex> lock(m);
        if(carsInPark > 0)
            carsInPark --;
            
        lock.unlock();
    }
}

int main(int argc, char* argv[]){
    srand(time(NULL));

}


///////////Reading op
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define N 20

using namespace std;

struct record_s {
 int id;
 char s1[N], s2[N];
 float f;
};

int main(int argc, char* argv[]){
    int n;
    record_s record;

    int fd = open("file.bin", O_RDWR, 0666);
    cin >> n;

    lseek(fd, (n-1)*sizeof(record_s), SEEK_SET);

    read(fd, &record, sizeof(record_s));

    char tmp [N];

    strcpy(tmp, record.s1);
    strcpy(record.s1, record.s2);
    strcpy(record.s2, tmp);

    lseek(fd, -sizeof(record_s), SEEK_CUR);
    write(fd, &record, sizeof(record_s));

    close(fd);
}


