#include <iostream>
#include <ostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cmath>
#include <time.h>

float formula_1(const float& x){
    return std::pow(x, 2) - std::pow(x, 2) + x * 4 - x * 5 + x + x;
}

float formula_2(const float& x){
    return x + x;
}

int main() {
    float x = 1.337;
    int n = 10000;
    float buf[n];

    clock_t start = clock();

    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        std::cerr << "Failed to create pipes." << std::endl;
        return 1;
    }
    pid_t pid1 = fork();
    if (pid1 == -1) {
        std::cerr << "Failed to fork first child process." << std::endl;
        return 1;
    } else if (pid1 == 0) {
        // First child process
        
        float calc_1[n];
        for (int i = 0; i < n; i++) {
            close(pipe1[0]); // Close read end of pipe 1
            close(pipe2[0]); // Close read end of pipe 2
            close(pipe2[1]); // Close write end of pipe 2
            calc_1[i] = formula_1(x);
            write(pipe1[1], calc_1, sizeof(calc_1)); // Write result to pipe 1
            close(pipe1[1]); // Close write end of pipe 1
        }
        return 0;
    }
    pid_t pid2 = fork();
    if (pid2 == -1) {
        std::cerr << "Failed to fork second child process." << std::endl;
        return 1;
    } else if (pid2 == 0) {
        // Second child process
        float calc_2[n];
        for (int i = 0; i < n; i++) {
            close(pipe2[0]); // Close read end of pipe 2
            close(pipe1[0]); // Close read end of pipe 1
            close(pipe2[1]); // Close write end of pipe 2
            calc_2[i] = formula_2(x);
            write(pipe2[1], calc_2, sizeof(calc_2)); // Write result to pipe 2
            close(pipe2[1]); // Close write end of pipe 2

        }
        return 0;
    }
    // Parent process
    close(pipe1[0]); // Close read end of pipe 1
    close(pipe1[1]); // Close write end of pipe 1
    close(pipe2[1]); // Close write end of pipe 2
    float  calc_3;
    float calc_2[n];
    read(pipe2[0], calc_2, sizeof(calc_2)); // Read result from pipe 2
    float calc_1[n];
    read(pipe1[0], calc_1, sizeof(calc_1)); // Read result from pipe 1
    for (int i = 0; i < n; i++) {
        calc_3 = calc_1[i] + calc_2[i] - calc_1[i];
    }
    // Wait for child processes to exit
    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    std:: cout << "for n="<<n<< " with processes time is: " << seconds << " s." << std::endl;

    return 0;
}