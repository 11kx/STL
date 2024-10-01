#include <iostream>
#include "vector.hpp"


int main()
{
    vector<int> v;
    for(int i = 0;i < 20 ;i ++)
    {
        v.push_back(random()%100);
    }

    int size = v.size();
    for(int i = 0;i < size;i++)
    {
        std::cout << v[i] ;
    }
std::cout  << std::endl;
    vector<int>::iterator iter = v.begin();
    for(;iter != v.end();++iter)
    {
        std::cout << *iter ;
    }
    std::cout  << std::endl;
    for(auto i : v)
    {
        std::cout << i ;
    }
  
}