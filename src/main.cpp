#include <iostream>
#include "queue.hpp"
int main()
{
    queue<int> q;

    for(int i = 0;i < 100000;i++)
    {
        q.push(i);
        q.pop();
    }

    return 0;
}