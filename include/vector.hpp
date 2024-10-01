#include <iostream>
/*
空间配置器
allocator做四件事 内存开辟/内存释放 对象构造/对象析构
*/
template <typename T>
class Allocator
{
public:
    //内存开辟
    T* allocate(int size)
    {
        return (T*)malloc(sizeof(T) * size);
    }
    //内存释放
    void deallocate(T* p)
    {
        free(p);
        p = nullptr;
    }
    //对象构造
    void construct(T* p,const T& value)
    {
        //在p位置，用传入的value，将T构造出来.所以也就将分配内存和构造对象分开了。
        new (p) T(value);
    }
    //对象析构
    void destory(T* p)
    {
        p->~T();
    }
};

template <typename T,typename Alloc = Allocator<T>>
class vector
{
private:
    T*  first_;     //指向容器的起始位置
    T*  last_;      //指向容器末尾元素的后继位置
    T*  end_;       //指向容器的末尾的后继位置
    Alloc alloc;     //空间配置器，负责内存的申请和释放，对象的构造和析构。
public:
    vector(int size = 10)
    {
        //若传入的是对象，则应该将申请内存空间，和对象的构造分开来处理
        //first_ = new T[size];
        first_ = alloc.allocate(size);
        last_ = first_;
        end_ = first_ + size;
    }
    ~vector()
    {
        //析构容器的有效元素，然后删除掉first指向的堆内存。
        //delete []first_;
        for(T* p = first_;p != last_;p++)
        {
            alloc.destory(p);   //把从first到last中的有效元素进行析构。
        }

        //将整个内存块释放
        alloc.deallocate(first_);
        
        first_ = last_ = end_ = nullptr;
    }
    vector(const vector<T>& other)
    {
        int size = other.end_ - other.first_;
        int len = other.last_ - other.first_;

        first_ = alloc.allocate(size);
        for(int i = 0;i < len;i++)
        {
            //first_[i] = other.first_[i];
            alloc.construct(first_[i],other.first_[i]);
        }
    }
    vector& operator=(const vector<T>& other)
    {
        //检查是否为自赋值
        if(other==*this)
            return *this;

        //删除原有数据
        for(T* p = first_;p != last_;p++)
        {
            alloc.destory(p);
        }
        //将整个内存块释放
        alloc.deallocate(first_);

        //拷贝
        int size = other.end_ - other.first_;
        int len = other.last_ - other.first_;
        first_ = alloc.allocate(size);
        for(int i = 0;i < len;i++)
        {
            //first_[i] = other.first_[i];
            alloc.construct(first_[i],other.first_[i]);
        }
        return *this;
    }
    void push_back(const T& value)
    {
        if(isFull())
            expand();
        alloc.construct(last_,value);
        last_++;
        
    }
    //只需要析构对象，而不能释放容器的内存。
    void pop_back()
    {
        if(isEmpty())
            return;
        --last_;
        alloc.destory(last_);
    }
    const T& back()
    {
        return *(last_ - 1);
    }
    bool isFull()
    {
        return last_ == end_;
    }
    bool isEmpty()
    {
        return first_ == last_;
    }
    size_t size()
    {
        return last_ - first_;
    }
    T& operator[](int index)
    {
        return first_[index];
    }
    class iterator
    {
    public:
        iterator(T* p = nullptr)
        :p_(p)
        {}
        bool operator!=(const iterator& other)
        {
            return p_ != other.p_;
        }
        void operator++()
        {
            p_++;
        }
        T& operator*()
        {
            return *p_;
        }
    private:
        T* p_;
    };
    iterator begin()
    {
        return iterator(first_);
    }
    iterator end()
    {
        return iterator(last_);
    }
private:
    void expand()   //二倍扩容
    {
        int size = end_ - first_;
        T* p_temp = alloc.allocate(size * 2);
        for(int i = 0;i < size;i++)
        {
            //p_temp[i] = first_[i];
            alloc.construct(p_temp + i, first_[i]);
        }

        for(T* p = first_;p != last_;p++)
        {
            alloc.destory(p);
        }
        //将整个内存块释放
        alloc.deallocate(first_);

        first_ = p_temp;
        last_ = first_ + size;
        end_ = first_ + size * 2;
    }
};

