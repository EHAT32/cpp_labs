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
#include "iostream" // for cout, cin
#include <array>
#include <cmath>
#include <stdlib.h> // for rand
#include <time.h> 
#include <thread>
#include <mutex>

float formula_1(const float& x){
    return std::pow(x, 2) - std::pow(x, 2) + x * 4 - x * 5 + x + x;
}

float formula_2(const float& x){
    return x + x;
}

float formula_3(const float& x){
    return formula_1(x) + formula_2(x) - formula_1(x);
}

float time_reg(const float& x, const int& N){
    clock_t start = clock();
    for(int i = 0; i < N; i++){
        float calc_1;
        float calc_2;
        std::thread th1([&calc_1, &x]()
        {
            calc_1 = formula_1(x);
        });
        std::thread th2([&calc_2, &x]()
        {
            calc_2 = formula_2(x);
        });
        th1.join();
        th2.join();
        [[maybe_unused]] float calc_3 = calc_1 + calc_2 - calc_1; 
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

## **3. [C++ PROCESS] Параллельные вычисления через процессы**

Требуется параллельно (насколько возможно с помощью процессов) выполнить вычисления по формуле 1, вычисления по формуле 2, после чего выполнить вычисления по формуле 3, которые выглядят следующим образом: результат вычислений 1 + результат вычислений 2 – результат вычислений 1
Выполнить последовательно на 10 000 итераций и 100 000 итераций

Формула 1: f(x) = x ^2- x ^2+ x *4- x *5+ x + x

Формула 2: f(x) = x + x

Вывести длительность выполнения всех 10 000 итераций и 100 000 итераций в сек. в разбивке по шагам вычислений 1, 2 и 3

### Решение

