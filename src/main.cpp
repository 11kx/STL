#include <iostream>
#include "vector.hpp"

int main()
{
    vector<int> v;
    for(int i =0;i < 20;i++)
    {
        v.push_back(random()%100);
    }
    

    for(int i =0;i < 20;i++)
    {
        std::cout << v.back() << " ";
        v.pop_back();
    }
    std::cout << std::endl;
}