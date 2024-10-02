#include <iostream>
#include "vector.hpp"

int main()
{
    vector<int> v;
    for (int i = 0; i < 20; i++)
    {
        v.push_back(random() % 100);
    }
    for (auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    auto it = v.end();
    v.pop_back();
    std::cout << std::boolalpha << (bool)(it != v.end()) << std::endl;

    auto fir = v.begin();
    fir = v.insert(fir, 20);

    for (auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    
    for(;fir != v.end();)
    {
         v.erase(fir);
    }
    

    for (auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

}