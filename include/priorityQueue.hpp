#include <iostream>
#include <functional>
class priorityQueue
{
public:
    using Com = std::function<bool(int,int)>;
    priorityQueue(int cap = 20,Com com = std::greater<int>())
                    :size_(0)
                    ,cap_(cap)
                    ,com_(com)
    {
        que_ = new int[cap_];
    }
    ~priorityQueue()
    {
        delete []que_;
        que_ = nullptr;
    }
    //push 插入元素
    void push(int value)
    {
        if(size_ == 0) {
            que_[size_] = value;
            size_++;
            return;
        }
        if(size_ == cap_) {
            expand(2 * cap_);
        }
        que_[size_] = value;
        shiftUp(size_,value);
        size_++;
    }
    //pop 根节点，最大的元素出队列
    void pop()
    {
        if(size_ == 0) {
            return;
        }
        //J将根节点出队列，然后将末尾元素放入根节点位置，进行下沉操作
        que_[0] = que_[size_ - 1];
        shiftDown(0,que_[0]);
        size_--;
    }
    //size
    int size() const {
        return size_;
    }
    //empty
    bool empty() const {
        return size_ == 0;
    }
    //top
    int top() const {
        return que_[0];
    }
    //shiftUp元素上浮操作
    void shiftUp(int i,int value)
    {
        //末尾节点不停的跟父节点进行比较,最终找到合适的位置将value放入，而不是不停的交换
        while(i > 0)
        {
            int father = (i - 1) / 2;
            if(com_(value,que_[father])) {
                que_[i] = que_[father];
                i = father;
            }
            else {
                break;
            }
        }
        que_[i] = value;
    }
    //shiftDown 元素下沉操作
    void shiftDown(int i,int value)
    {
        // i下沉不能超过最后一个有孩子的节点
        while(i < size_ / 2) {
        // 与左右孩子分别相比，将大的那个上浮。然后跟新i
            int child = i * 2 + 1;
            if(child < size_ - 1 && com_(que_[child + 1],que_[child])) {
                //右孩子大
                child = child + 1;
            }

            if(com_(que_[child],value)) {
                que_[i] = que_[child];
                i = child;
            } else {
                break;
            }
        }
        que_[i] = value;
    }
private:
    void expand(int cap)
    {
        int* temp = new int [cap];
        for(int i = 0;i < size_;i++)
        {
            temp[i] = que_[i];
        } 
        delete[] que_;
        que_ = temp;
    }
private:
    int* que_;  //动态扩容的数组
    int size_;  //优先队列的元素个数
    int cap_;   //优先队列的容量
    Com com_;   //比较器对象
};

