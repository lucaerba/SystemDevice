#include <iostream>
#include <string.h>
#include <string>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <signal.h>
#define N 20
int main() {
    int pipePC1[2], pipePC2[2];
    if (pipe(pipePC1) == -1) {
        perror("pipe");
        return 1;
    }
    if (pipe(pipePC2) == -1) {
        perror("pipe");
        return 1;
    }

    // Esegui il processo figlio
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) {

        // Esegui il processo figlio
        pid_t pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            return 1;
        }

        srand(getppid());
        close(pipePC1[1]);//close write
        close(pipePC2[1]);//close write
        char buff[4];

        while(1){
            read(pipePC[0],buff, sizeof (char)*4);
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
                write(pipeCP[1], "done", sizeof(char) * 4);
            }

        }

        // Rimuovi l'attaccamento del segmento di memoria condiviso
        shmdt(shared_data);
        // Rimuovi il segmento di memoria condiviso
        shmctl(shmid, IPC_RMID, nullptr);

        close(pipePC[0]);
        close(pipeCP[1]);
    } else {

        srand(pid);
        close(pipePC[0]);//close read
        close(pipeCP[1]);//close write
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
                write(pipePC[1], "done", sizeof(char) * 4);
            }
            read(pipeCP[0],buff, sizeof (char)*4);
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

        close(pipePC[1]);
        close(pipeCP[0]);
        // Rimuovi il segmento di memoria condiviso
        shmctl(shmid, IPC_RMID, nullptr);
    }

    return 0;
}