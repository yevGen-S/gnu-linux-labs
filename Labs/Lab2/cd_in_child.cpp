#include <cstdio>
#include <sys/wait.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

using namespace std;



int main() {
    printf("Parent process is in \n");
    system("pwd");
    printf("\n");

    if (fork() == 0) {
        printf("Child process is in \n");
        system("pwd");
        printf("\n");

        system("cd /tmp");

        printf("Now child process is in \n");
        system("pwd");
        printf("\n");

        exit(0);
    }
    else {
        wait(0);
        printf("Now parent process is in ");
        system("pwd");
        printf("\n");

        exit(0);
    }

    return 0;
}












//Пункт 4
//int main() {
//    printf("Parent process is in \n");
//    system("pwd");
//    printf("\n");
//
//    if (fork() == 0) {
//        printf("Child process is in \n");
//        system("pwd");
//        printf("\n");
//
//        system("cd /tmp");
//
//        printf("Now child process is in \n");
//        system("pwd");
//        printf("\n");
//
//        exit(0);
//    }
//    else {
//        wait(0);
//        printf("Now parent process is in ");
//        system("pwd");
//        printf("\n");
//
//        exit(0);
//    }
//
//    return 0;
//}
