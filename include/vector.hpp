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
    Alloc alloc_;     //空间配置器，负责内存的申请和释放，对象的构造和析构。
public:
    vector(int size = 10)
    {
        //若传入的是对象，则应该将申请内存空间，和对象的构造分开来处理
        //first_ = new T[size];
        first_ = alloc_.allocate(size);
        last_ = first_;
        end_ = first_ + size;
    }
    ~vector()
    {
        //析构容器的有效元素，然后删除掉first指向的堆内存。
        //delete []first_;
        for(T* p = first_;p != last_;p++)
        {
            alloc_.destory(p);   //把从first到last中的有效元素进行析构。
        }

        //将整个内存块释放
        alloc_.deallocate(first_);
        
        first_ = last_ = end_ = nullptr;
    }
    vector(const vector<T>& other)
    {
        int size = other.end_ - other.first_;
        int len = other.last_ - other.first_;

        first_ = alloc_.allocate(size);
        for(int i = 0;i < len;i++)
        {
            //first_[i] = other.first_[i];
            alloc_.construct(first_[i],other.first_[i]);
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
            alloc_.destory(p);
        }
        //将整个内存块释放
        alloc_.deallocate(first_);

        //拷贝
        int size = other.end_ - other.first_;
        int len = other.last_ - other.first_;
        first_ = alloc_.allocate(size);
        for(int i = 0;i < len;i++)
        {
            //first_[i] = other.first_[i];
            alloc_.construct(first_[i],other.first_[i]);
        }
        return *this;
    }
    void push_back(const T& value)
    {
        if(isFull())
            expand();
        alloc_.construct(last_,value);
        last_++;
        
    }
    //只需要析构对象，而不能释放容器的内存。
    void pop_back()
    {
        if(isEmpty())
            return;
        //使迭代器失效
        verify(last_-1,last_);
        --last_;
        alloc_.destory(last_);
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
        friend class vector<T,Allocator<T>>;
        friend struct IteratorBase;
        
        iterator(vector<T,Alloc>* v_ptr = nullptr,T* p = nullptr)
        :v_ptr_(v_ptr)
        ,p_(p)
        {
            //新生成了一个迭代器，指向容器的某一个位置
            //将其插入链表
            IteratorBase* itb = new IteratorBase(this,v_ptr_->head_.next_);
            v_ptr_->head_.next_ = itb;
        }
        bool operator!=(const iterator& other)
        {
            if(v_ptr_ == nullptr || v_ptr_ != other.v_ptr_)
            {
                throw("iterator incompatable");
            }
            return p_ != other.p_;
        }
        void operator++()
        {
            if(v_ptr_ == nullptr)
            {
                throw("iterator is nullptr");
            }
            p_++;
        }
        T& operator*()
        {
            if(v_ptr_ == nullptr)
            {
                throw("iterator is nullptr");
            }
            return *p_;
        }
    private:
        //当前迭代器是哪个容器的迭代器
        vector<T,Alloc>* v_ptr_;    //检查迭代器是否有效就是用的这个
        //指向当前容器底层的数据结构
        T* p_;
    };
    iterator begin()
    {
        return iterator(this,first_);
    }
    iterator end()
    {
        return iterator(this,last_);
    }
    iterator insert(iterator iter,const T value)
    {
        /*
        1.不考虑扩容情况
        2.不考虑iter不合法情况
        */
       //先将从iter位置到last的迭代器失效
        verify(iter.p_ - 1,last_);
        T* p = last_;
       //将容器内元素一个一个往后移，挨个拷贝构造和析构
       while (p > iter.p_)
       {
            alloc_.construct(p,*(p - 1));
            alloc_.destory(p - 1);
            --p;
       }
       //在指定位置插入
       alloc_.construct(p,value);
       last_++;
       return iterator(this, p);
    }
    iterator erase(iterator iter)
    {
        //将迭代器失效
        verify(iter.p_ - 1,last_);
        T* p = iter.p_;
       //将容器内元素一个一个往前移，先析构再拷贝
       while (p < last_ - 1)
       {
            alloc_.destory(p);
            alloc_.construct(p,*(p + 1));
            
            ++p;
       }
       //析构掉最后一个位置
       alloc_.destory(p);
       last_--;
       return iterator(this, iter.p_);
    }
private:
    //用于存储容器的迭代器的链表，用户获取迭代器时就将新生成的迭代器插入链表，需要将迭代器失效时就就在链表中将其置空！
    struct IteratorBase
    {
        IteratorBase(iterator* c = nullptr,IteratorBase* n = nullptr)
        :cur_(c)
        ,next_(n)
        {

        }
        iterator* cur_;
        IteratorBase* next_;
    };
    IteratorBase head_;
    //将first到last范围内的迭代器失效
    void verify(T* first,T* last)
    {
        IteratorBase *pre = &this->head_;
        IteratorBase* it = this->head_.next_;
        while (it != nullptr)
        {
            if(it->cur_->p_ > first && it->cur_->p_ <= last)
            {
                //迭代器失效
                it->cur_->v_ptr_ = nullptr;
                //删除当前迭代器节点
                pre->next_ = it->next_;
                delete it;
                it = pre->next_;
            }
            else
            {
                pre = it;
                it = it ->next_;
            }
        }
    
    }
    void expand()   //二倍扩容
    {
        int size = end_ - first_;
        T* p_temp = alloc_.allocate(size * 2);
        for(int i = 0;i < size;i++)
        {
            //p_temp[i] = first_[i];
            alloc_.construct(p_temp + i, first_[i]);
        }

        for(T* p = first_;p != last_;p++)
        {
            alloc_.destory(p);
        }
        //将整个内存块释放
        alloc_.deallocate(first_);

        first_ = p_temp;
        last_ = first_ + size;
        end_ = first_ + size * 2;
    }
};

