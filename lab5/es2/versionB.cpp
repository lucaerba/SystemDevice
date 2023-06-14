#include <iostream>
#include <string.h>
#include <string>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#define N 20
int main() {
    const char* pathPC = "/tmp/fifoPC";
    const char* pathCP = "/tmp/fifoCP";

    // Genera una chiave univoca per il segmento di memoria condiviso
    key_t key = ftok("hello.txt", 1234);
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    // Crea un nuovo segmento di memoria condiviso
    int shmid = shmget(key, sizeof(char)*(N+1), IPC_CREAT | 0644);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    // Attacca il segmento di memoria condiviso al processo
    char* shared_data = static_cast<char*>(shmat(shmid, nullptr, 0));
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
        mkfifo(pathPC, 0666);
        mkfifo(pathCP, 0666);
        int fd_PC = open(pathPC, O_RDONLY);
        int fd_CP = open(pathCP, O_WRONLY);

        srand(getppid());
        char buff[4];

        while(1){
            read(fd_PC,buff, sizeof (char)*4);
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
                write(fd_CP, "done", sizeof(char) * 4);
            }

        }
        // Rimuovi l'attaccamento del segmento di memoria condiviso
        shmdt(shared_data);
        // Rimuovi il segmento di memoria condiviso
        shmctl(shmid, IPC_RMID, nullptr);

        close(fd_PC);
        close(fd_CP);
    } else {
        mkfifo(pathPC, 0666);
        mkfifo(pathCP, 0666);
        int fd_PC = open(pathPC, O_WRONLY);
        int fd_CP = open(pathCP, O_RDONLY);

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
                write(fd_PC, "done", sizeof(char) * 4);
            }
            read(fd_CP,buff, sizeof (char)*4);
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

        close(fd_PC);
        close(fd_CP);
        // Rimuovi il segmento di memoria condiviso
        shmctl(shmid, IPC_RMID, nullptr);
    }

    return 0;
}