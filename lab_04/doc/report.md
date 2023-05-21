# ЛР #4: [C++ & UNIX]: C++ PROCESSES / THREADS

## Цель

Познакомить студента с принципами параллельных вычислений. Составить несколько программ в простейшими вычислительными действиями, чтобы освоить принципы параллельных вычислений (когда одни алгоритмы зависят / не зависят от других).

## **1. [С++ SEQUENCE] Последовательные вычисления**

Требуется последовательно выполнить вычисления по формуле 1, вычисления по формуле 2, после чего выполнить вычисления по формуле 3, которые выглядят следующим образом: результат вычислений 1 + результат вычислений 2 – результат вычислений 1
Выполнить последовательно на 10 000 итераций и 100 000 итераций
Формула 1: f(x) = x ^2- x ^2+ x *4- x *5+ x + x
Формула 2: f(x) = x + x
Вывести длительность выполнения всех 10 000 итераций и 100 000 итераций в сек.

### Решение

```
float formula_1(const float& x){
    return std::pow(x, 2) - std::pow(x, 2) + x * 4 - x * 5 + x + x;
}

float formula_2(const float& x){
    return x + x;
}

float time_reg(const float& x, const int& N){
    clock_t start = clock();
    for(int i = 0; i < N; i++){
        [[maybe_unused]] float calc_1 = formula_1(x);
        [[maybe_unused]] float calc_2 = formula_2(x);
        [[maybe_unused]] float calc_3 = calc_1 - calc_2 + calc_1;

    }
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("For N = %i the elapsed time is %.2e seconds\n", N, time);
    return time;
}

int main(){
    
    float x = (float) rand() / RAND_MAX;

    time_reg(x, 10000);
    time_reg(x, 100000);

    return 0;
}
```

## **2. [C++ THREADS] Параллельные вычисления через потоки**

Требуется параллельно (насколько возможно с помощью потоков) выполнить вычисления по формуле 1, вычисления по формуле 2, после чего выполнить вычисления по формуле 3, которые выглядят следующим образом: результат вычислений 1 + результат вычислений 2 – результат вычислений 1
Выполнить последовательно на 10 000 итераций и 100 000 итераций

Формула 1: f(x) = x ^2- x ^2+ x *4- x *5+ x + x

Формула 2: f(x) = x + x
Вывести длительность выполнения всех 10 000 итераций и 100 000 итераций в сек. в разбивке по шагам вычислений 1, 2 и 3

### Решение

```
 #include <iostream> //standart
#include <cmath> //for pow()
#include <time.h>
#include <stdlib.h> // random number from _start to _end
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>

using namespace std;

float formula_1(const float& x){
    return std::pow(x, 2) - std::pow(x, 2) + x * 4 - x * 5 + x + x;
}

float formula_2(const float& x){
    return x + x;
}


int main()
{
float x = 1;
int n = 100000;
float calc_1[n];
float calc_2[n];
std::mutex m;
clock_t start = clock();


thread th1([&calc_1, &x, &n, &m]()
{   
    for(int i = 0; i<n; i++)
    {
        m.lock();
        calc_1[i] = formula_1(x);
        m.unlock();
    }
});

thread th([&calc_2, &x, &n, &m]()
{   
    for(int i = 0; i < n; i++)
    {
        m.lock();
        calc_2[i] =  formula_2(x);
        m.unlock();
    }
});

 [[maybe_unused]] float calc_3; 

for(int i = 0; i<n; i++)
{
    calc_3 = calc_1[i] + calc_2[i] - calc_1[i];
}
th1.join();
th.join();
clock_t end = clock(); // time of end in flops
double seconds = (double)(end - start) / CLOCKS_PER_SEC;
printf("For N = %i the elapsed time is %.2e seconds\n", n, seconds);
return 0;
}
```

## **3. [C++ PROCESS] Параллельные вычисления через процессы**

Требуется параллельно (насколько возможно с помощью процессов) выполнить вычисления по формуле 1, вычисления по формуле 2, после чего выполнить вычисления по формуле 3, которые выглядят следующим образом: результат вычислений 1 + результат вычислений 2 – результат вычислений 1
Выполнить последовательно на 10 000 итераций и 100 000 итераций

Формула 1: f(x) = x ^2- x ^2+ x *4- x *5+ x + x

Формула 2: f(x) = x + x

Вывести длительность выполнения всех 10 000 итераций и 100 000 итераций в сек. в разбивке по шагам вычислений 1, 2 и 3

### Решение

```
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
```

## Результаты:

Последоваельное выполнение:

N = 10000: 1 мс

N = 100000: 6 мс

Потоки:

N = 10000: 7 мс

N = 100000: 14 мс

Если создавать потоки внутри итерации:

N = 10000: 7.7 с

N = 100000: 85 с

Процессы:

N = 10000: 0,25 мс

N = 100000: 0,66 мс



