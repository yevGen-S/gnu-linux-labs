#include <cstdio>
#include <sys/wait.h>
#include <cstdlib>
#include <iostream>

using namespace std;


//Пункт 5

int main() {
    cout << "Parent process opening file" << endl;
    FILE* file = fopen("./input.txt", "r");

    if (file == nullptr) {
        fprintf(stderr, " Error of opening file");
    }

    char buff[250];

    if (fork() == 0) {
        cout << "Child process created" << endl;

        fgets(buff, 250, file);
        cout << buff;

        cout << endl;
        exit(0);
    }
    else {
        wait(0);
        cout << "Parent process in work" << endl;

        fgets(buff, 250, file);
        cout << buff;

        cout << endl;
        exit(0);
    }

    return 0;
}













