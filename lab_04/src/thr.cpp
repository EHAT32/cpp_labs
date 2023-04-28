#include "iostream" // for cout, cin
#include <array>
#include <cmath>
#include <stdlib.h> // for rand
#include <time.h> 
#include <thread>

float formula_1(const float& x){
    return std::pow(x, 2) - std::pow(x, 2) + x * 4 - x * 5 + x + x;
}

float formula_2(const float& x){
    return x + x;
}

float formula_3(const float& x){
    return formula_1(x) + formula_2(x) - formula_1(x);
}

int main(){
    float x = (float) rand() / RAND_MAX;
    int N = 10000;
    float res_1;
    float res_2;
    float res_3;
    return 0;
}