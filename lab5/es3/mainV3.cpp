#include <iostream>
#include <string.h>
#include <string>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <sys/select.h>

#define N 20
#define WAIT_TIME_1 3
#define WAIT_TIME_2 6
#define STR_NUM 3
#define STR_SIZE 5

int main()
{
    int pipePC1[2], pipePC2[2];
    if (pipe(pipePC1) == -1)
    {
        perror("pipe");
        return 1;
    }
    if (pipe(pipePC2) == -1)
    {
        perror("pipe");
        return 1;
    }

    // Esegui il processo figlio
    pid_t pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid1 == 0)
    {

        // FIGLIO 1

        close(pipePC1[0]); // close read
        srand(getppid() + 23);
        while (true) {
            sleep(WAIT_TIME_1);

            char *message = (char *)malloc((STR_SIZE + 1) * sizeof(char));
            for (int i = 0; i < STR_SIZE; i++) {
                int k = rand() % (124 - 97 + 1) + 97;
                if (k == 123)
                    k = 32; // space
                else if (k == 124)
                    k = 10; // new line
                message[i] = ((char)k);
            }
            message[STR_SIZE] = 3;
            std::cout <<"figlio1 "<<message << std::endl;
            write(pipePC1[1], message, sizeof(message));
            free(message);
        }
    }
    else {

        pid_t pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            return 1;
        }

        if (pid2 == 0) {
            // FIGLIO 2

            close(pipePC2[0]); // close read
            srand(getppid() + 77);
            while (true) {
                sleep(WAIT_TIME_2);

                char *message = (char *)malloc((STR_SIZE + 1) * sizeof(char));
                for (int i = 0; i < STR_SIZE; i++)
                {
                    int k = rand() % (124 - 97 + 1) + 97;
                    if (k == 123)
                        k = 32; // space
                    else if (k == 124)
                        k = 10; // new line
                    message[i] = ((char)k);
                }
                message[STR_SIZE] = 3;
                std::cout <<"figlio2 "<<message << std::endl;
                write(pipePC2[1], message, sizeof(message));
                free(message);
            }
        }
        else {
            // PADRE

            fd_set set;
            int maxFd = std::max(pipePC1[0], pipePC2[0])+1;

            FD_ZERO(&set);
            FD_SET(pipePC1[0], &set);
            //FD_SET(pipePC1[1], &set);
            FD_SET(pipePC2[0], &set);
            //FD_SET(pipePC2[1], &set);

            //int maxFd2 = std::max(pipePC2[0], pipePC2[1]);
            //int maxFdfinal = std::max(maxFd, maxFd2) + 1;
            close(pipePC1[1]); // close write
            close(pipePC2[1]); // close write
            while (1)
            {

                FD_ZERO(&set);
                FD_SET(pipePC1[0], &set);
                //FD_SET(pipePC1[1], &set);
                FD_SET(pipePC2[0], &set);
                //FD_SET(pipePC2[1], &set);

                if (select(maxFd, &set, NULL, NULL, NULL) == -1) {
                    std::cerr << "Error in select" << std::endl;
                    exit(1);
                }

                //sleep(5);
                char *message = (char *)malloc((STR_SIZE + 1) * sizeof(char));

                if (FD_ISSET(pipePC1[0], &set)) {
                    read(pipePC1[0], message, sizeof(message));
                    std::cout <<"padre da 1 "<<message << std::endl;
                }
                if (FD_ISSET(pipePC2[0], &set)) {
                    read(pipePC2[0], message, sizeof(message));
                    std::cout <<"padre da 2 "<<message << std::endl;
                }

                free(message);
            }
        }
    }
    return 0;
}   