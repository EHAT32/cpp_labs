#include <iostream>
#include <iterator>
#include <list>
#include <cstring>
#include <string.h>
#include <string>

void move_disk(const int& n, const char& from, const char& to, const char& via, std::list<std::string>& list){
    if (n == 1)
    {
        //std::cout << "Move disk 1 from " << from << " to " << to << std::endl;
        std::string s;
        s.push_back('1');
        s.push_back(from);
        s.push_back(to);
        list.push_back(s);
        s.clear();
        return;
    }
    move_disk(n - 1, from, via, to, list);
    //std::cout << "Move disk " << n << " from " << from << " to " << to << std::endl;
    std::string s;
    s.push_back(char(n + 48));
    s.push_back(from);
    s.push_back(to);
    list.push_back(s);
    s.clear();
    move_disk(n - 1, via, to, from, list);
}

template<typename T>
void show_list(std::list<T> l) {
    for (const auto &item : l) {
        std::cout << item << std::endl;
    }
    std::cout << std::endl;
}

int main(){
    std::list<std::string> moves;
    int n = 8;
    move_disk(n, 'A', 'C', 'B', moves);
    show_list(moves);
    moves.clear();
    return 0;
}