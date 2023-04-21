#include "iostream" // for cout, cin
#include <array>
#include <cmath>
#include <stdlib.h> // for rand
#include <time.h> 

float time_reg(const float& x, const int& N){
    clock_t start = clock();
    for(int i = 0; i < N; i++){
        [[maybe_unused]] float calc = pow(x, 2) - pow(x, 2) + pow(x, 4) - pow(x, 5) + x + x;
    }
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The elapsed time is %.10e seconds\n", time);
    return time;
}


int main(){
    return 0;
}