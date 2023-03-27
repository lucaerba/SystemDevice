#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <vector>

using namespace std;

struct Param{
    int n;
    vector<pthread_t> ptid_father;
};
void* thread_fun(void * arg){
    
    pthread_t mypt1, mypt2; 
    Param p1;
    Param p2;
    Param myp = *((Param*)arg);

    if(!myp.n){
        //print and return
        for(auto p: myp.ptid_father)
            printf("%ld ", p);

        printf("\n");
        return 0;
    }
    p1.ptid_father = myp.ptid_father;
    p2.ptid_father = myp.ptid_father;
    p1.n = myp.n-1;
    p2.n = myp.n-1;
    p1.ptid_father.push_back(pthread_self());
    p2.ptid_father.push_back(pthread_self());
    int ptid1 = pthread_create(&mypt1, NULL, &thread_fun, &p1);
    int ptid2 = pthread_create(&mypt2, NULL, &thread_fun, &p2);
    pthread_join(mypt1, NULL);
    pthread_join(mypt2, NULL);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    
    pthread_t mypt1, mypt2; 
    Param p1;
    Param p2;
    int n = atoi(argv[1]);

    if(argc != 2) return -1;
    p1.ptid_father.push_back(pthread_self());
    p2.ptid_father.push_back(pthread_self());
    p1.n = n-1;
    p2.n = n-1;

    int ptid1 = pthread_create(&mypt1, NULL, &thread_fun, &p1);
    int ptid2 = pthread_create(&mypt2, NULL, &thread_fun, &p2);
    pthread_join(mypt1, NULL);
    pthread_join(mypt2, NULL);
    
    return 0;
}
