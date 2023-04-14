#include <iostream>
#include <list>

void move_disk(const int& n, const char& from, const char& to, const char& via){
    if (n == 1)
    {
        std::cout << "Move disk 1 from " << from << " to " << to << std::endl;
        return;
    }
    move_disk(n - 1, from, via, to);
    std::cout << "Move disk " << n << " from " << from << " to " << to << std::endl;
    move_disk(n - 1, via, to, from);
}

int main(){
    int n = 8;
    move_disk(n, 'A', 'C', 'B');
    return 0;
}