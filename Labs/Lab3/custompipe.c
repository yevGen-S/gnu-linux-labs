#include <stdio.h>
#include <unistd.h>

#define ERROR_OF_OPENING_PIPE -5
#define ERROR_OF_FORKING -6

int main(int argc, char* argv[]) {

    int fd[2];
    int in = 0;
    int out = 1;

    for (int i = 0; i < argc - 1; i++) {
        if (pipe(fd) == -1) {
            printf(" Error of opening pipe ");
            return ERROR_OF_OPENING_PIPE;
        }

        int child_proc = fork();

        if (child_proc == -1){
            printf("Error of forking");
            return ERROR_OF_FORKING;
        }

        if (child_proc == 0) {
            if (in != 0) {
                dup2(in, 0);
                close(in);
            }

            if (fd[1] != 1) {
                dup2(fd[1], 1);
                close(fd[1]);
            }

            execlp(argv[i], argv[i], 0);
        }
        close(fd[1]);

        in = fd[0];
    }

    if (in != 0) {
        dup2(in, 0);
    }

    if (fd[1]) {
        dup2(out, 1);
    }

    execlp(argv[argc - 1], argv[argc - 1], 0 );

    return 0;
}
