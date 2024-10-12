#include <iostream>
#include "string.hpp"
int main()
{
    String str1 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    String str2;
    str2 = GetString(str1);
    std::cout << str2.c_str() << std::endl;

    return 0;
}