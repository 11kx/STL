#include <iostream>
#include "vector.hpp"

class Test
{
    public:
    Test()
    {
        std::cout << "construct" << std::endl;
    }
    ~Test()
    {
        std::cout << "destory " << std::endl;
    }
};
int main()
{
    Test t1;
    vector<Test> v;
    
    v.push_back(t1);
    v.push_back(t1);
    v.push_back(t1);
    
    v.pop_back();
    std::cout << std::endl;
}