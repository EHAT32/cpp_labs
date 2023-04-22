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
    printf("The elapsed time is %.10e seconds\n", time);
    return time;
}



int main(){
    std::string ans = "y";
    while (ans == "y"){
        float x = (float) rand() / RAND_MAX;
        int N;
        std::cout << "Type in the number (as number) of loops: ";
        std::cin >> N;
        if(!std::cin.good())
        {
            std::cout << "This is cannot be interpreted as integer." << std::endl;
            break;
        }
        time_reg(x, N);
        std::cout << "Continue? [y/n]: ";
        std::cin >> ans;
    }
    std::cout << "Goodbye.\n";

    return 0;
}