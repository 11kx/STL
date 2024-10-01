
template <typename T>
class vector
{
private:
    T*  first_;     //指向容器的起始位置
    T*  last_;      //指向容器末尾元素的后继位置
    T*  end_;       //指向容器的末尾的后继位置
public:
    vector(int size = 10)
    {
        first_ = new T[size];
        last_ = first_;
        end_ = first_ + size;
    }
    ~vector()
    {
        delete []first_;
        first_ = last_ = end_ = nullptr;
    }
    vector(const vector<T>& other)
    {
        int size = other.end_ - other.first_;
        int len = other.last_ - other.first_;
        first_ = new T[size];
        for(int i = 0;i < len;i++)
        {
            first_[i] = other.first_[i];
        }
    }
    vector& operator=(const vector<T>& other)
    {
        //检查是否为自赋值
        if(other==*this)
            return *this;
        //删除原有数据
        delete[] first_;
        //拷贝
        int size = other.end_ - other.first_;
        int len = other.last_ - other.first_;
        first_ = new T[size];
        for(int i = 0;i < len;i++)
        {
            first_[i] = other.first_[i];
        }
        return *this;
    }
    void push_back(const T& value)
    {
        if(isFull())
            expand();
        *last_++ = value;
        
    }
    void pop_back()
    {
        if(isEmpty())
            return;
        last_--;
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
private:
    void expand()   //二倍扩容
    {
        int size = end_ - first_;
        T* p_temp = new T[size * 2];
        for(int i = 0;i < size;i++)
        {
            p_temp[i] = first_[i];
        }

        delete [] first_;

        first_ = p_temp;
        last_ = first_ + size;
        end_ = first_ + size * 2;
    }
};

