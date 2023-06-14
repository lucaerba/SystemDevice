#include <iostream>
#include <string.h>
#include <string>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <signal.h>
#include <sys/msg.h>

#define N 20

key_t key;
int shmid;
char* shared_data;

struct Message {
    long mtype;
    char mtext[5];
};

void P1(int pid){
    key_t messageKey = ftok("messages", 'A');
    int msqid = msgget(messageKey, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget");
    }
    Message sms;
    sms.mtype=1;
    strcpy(sms.mtext,"done");
    srand(pid);

    srand(getppid());

    while(1){
        msgrcv(msqid,&sms, 5,2, 0);
        std::cout<<"figlio legge->" << std::endl;
        int cont=0;
        while(*(shared_data+cont)!=3){
            if(*(shared_data+cont)==32||*(shared_data+cont)==10)
                std::cout<<(char)*(shared_data+cont);
            else
                std::cout<<(char)(*(shared_data+cont)-32);
            cont++;
        }
        std::cout<<std::endl;
        int n = rand() % N;
        if (n == 0) {
            kill(getppid(), SIGKILL);
            break;
        } else {
            char* message=(char*) malloc((n+1)*sizeof(char));
            for (int i = 0; i < n; i++) {
                int k = rand() % (124 - 97 + 1) + 97;
                if (k == 123)
                    k = 32;//space
                else if (k == 124)
                    k = 10;//new line
                message[i]=((char) k);
            }
            message[n]=3;
            strncpy(shared_data, message, sizeof(char)*(n+1));
            std::cout<<"figlio genera->" << message << std::endl;
            free(message);

            sms.mtype=1;
            msgsnd(msqid,&sms, 5, 0);
        }
    }

    // Rimuovi l'attaccamento del segmento di memoria condiviso
    shmdt(shared_data);
    // Rimuovi il segmento di memoria condiviso
    shmctl(shmid, IPC_RMID, nullptr);

    msgctl(msqid, IPC_RMID, NULL);
}

void P2(int pid){
    key_t messageKey = ftok("messages", 'A');
    int msqid = msgget(messageKey, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget");
    }
    Message sms;
    sms.mtype=2;
    strcpy(sms.mtext,"done");
    srand(pid);
    char buff[4];
    while(1){
        int n = rand() % N;
        if (n == 0) {
            kill(pid, SIGKILL);
            break;
        } else {
            char* message=(char*) malloc((n+1)*sizeof(char));
            for (int i = 0; i < n; i++) {
                int k = rand() % (124 - 97 + 1) + 97;
                if (k == 123)
                    k = 32;//space
                else if (k == 124)
                    k = 10;//new line
                message[i]=((char) k);
            }
            message[n]=3;
            strncpy(shared_data, message, sizeof(char)*(n+1));
            std::cout<<"padre genera->" << message << std::endl;
            free(message);

            sms.mtype=2;
            msgsnd(msqid,&sms, 5, 0);
        }
        msgrcv(msqid,&sms, 5,1, 0);
        std::cout<<"padre legge->" << std::endl;
        int cont=0;
        while(*(shared_data+cont)!=3){
            if(*(shared_data+cont)==32||*(shared_data+cont)==10)
                std::cout<<(char)*(shared_data+cont);
            else
                std::cout<<(char)(*(shared_data+cont)-32);
            cont++;
        }
        std::cout<<std::endl;
    }



    // Rimuovi l'attaccamento del segmento di memoria condiviso
    shmdt(shared_data);
    msgctl(msqid, IPC_RMID, NULL);
    // Rimuovi il segmento di memoria condiviso
    shmctl(shmid, IPC_RMID, nullptr);

}

int main() {

    // Genera una chiave univoca per il segmento di memoria condiviso
    key = ftok("hello.txt", 1234);
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    // Crea un nuovo segmento di memoria condiviso
    shmid = shmget(key, sizeof(char)*(N+1), IPC_CREAT | 0644);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    // Attacca il segmento di memoria condiviso al processo
    shared_data = static_cast<char*>(shmat(shmid, nullptr, 0));
    if (shared_data == reinterpret_cast<char*>(-1)) {
        perror("shmat");
        return 1;
    }

    // Esegui il processo figlio
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        P1(pid);
    } else {
        P2(pid);
    }
    return 0;
}