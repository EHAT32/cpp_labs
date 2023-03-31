#include "iostream"
#include <array>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

using namespace std;

float time_reg(const float& x, const int& N){
    clock_t start = clock();
    for(int i = 0; i < N; i++){
        float calc = pow(x, 2) - pow(x, 2) + pow(x, 4) - pow(x, 5) + x + x;
    }
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The elapsed time is %.2e seconds\n", time);
    return time;
}

int main(){
    // array<float, 10> data;
    // int i = 0;
    string ans = "y";
    while (ans == "y"){
        float x = (float) rand() / RAND_MAX;
        int N;
        std::cout << "Type in the number (as number) of loops: ";
        std::cin >> N;
        if(!std::cin.good())
        {
            std::cout << "This is cannot be interpreted as integer." << endl;
            break;
        }
        time_reg(x, N);
        cout << "Continue? [y/n]: ";
        cin >> ans;
    }
    cout << "Goodbye.\n";
    return 0;
}