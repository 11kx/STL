#include <iostream>
#include "string.hpp"


int main()
{
    String s2 = "aaa";
    String s5 = s2 + "ccc";
    String s6 = "ddd" + s2;

    std::cout << s5.length() << std::endl;
    std::cout << s6[3] << std::endl;
    if(s5 > s6)
    {
        std::cout << s5 << " > " << s6 << std::endl;
    }
    else
    {
        std::cout << s5 << " < " << s6 << std::endl;
    }
}