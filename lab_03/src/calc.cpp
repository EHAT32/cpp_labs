#include <cmath>
#include <iostream>
#include <string>

int main(int argc, char **argv){
    float x1 = std::stof(argv[1]);
    char op = *argv[2];
    float x2 = std::stof(argv[3]);

    switch (op) {
    case '+':
        std::cout << x1 + x2 << std::endl;
        break;
    case '-':
        std::cout << x1 - x2 << std::endl;
        break;
    case '^':
        std::cout << std::pow(x1, x2) << std::endl;
        break;
    default:
        std::cout << "Only +, - and ^ operations are implemented" << std::endl;
    }
    return 0;
}