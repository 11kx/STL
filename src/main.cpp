#include <iostream>
#include "priorityQueue.hpp"
int main()
{
   priorityQueue que(5);
   for(int i = 0;i < 10;i++)
   {
        que.push(std::rand() % 100);
   }

   for(int i = 0;i < 10;i++) {
        std::cout << que.top() << std::endl;
        que.pop();
   }

    return 0;
}