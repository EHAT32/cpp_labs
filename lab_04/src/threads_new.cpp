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