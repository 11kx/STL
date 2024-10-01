#include <iostream>
#include "string.hpp"


int main()
{
    String s1 = "hello world!";
    auto iter = s1.begin();
    for(auto c : s1)
    {
        std::cout << c;
    }
}