#include "iostream" // for cout, cin
#include <array>
#include <cmath>
#include <stdlib.h> // for rand
#include <time.h> 

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