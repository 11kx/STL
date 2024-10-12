#include <iostream>
#include <string.h>
class String
{
public:
    String(const char *str = nullptr)
    {
        std::cout << "构造函数" << std::endl;
        if (str != nullptr)
        {
            ptr_ = new char[strlen(str) + 1];
            strcpy(ptr_, str);
        }
        else
        {
            ptr_ = new char[1];
            ptr_[0] = '\0';
        }
    }
    String(const String &src)
    {
        std::cout << "拷贝构造函数" << std::endl;
        // 有指针，进行深拷贝
        const int size = strlen(src.ptr_);
        ptr_ = new char[size + 1];
        strcpy(ptr_, src.ptr_);
    }
    String &operator=(const String &src)
    {
        std::cout << "拷贝赋值" << std::endl;
        // 防止自赋值
        if (&src == this)
            return *this;
        // 删除原有数据
        delete[] ptr_;
        // 拷贝过来
        const int size = strlen(src.ptr_);
        ptr_ = new char[size + 1];
        strcpy(ptr_, src.ptr_);
        return *this;
    }
    String(String &&src)
    {
        std::cout << "移动构造" << std::endl;
        ptr_ = src.ptr_;
        src.ptr_ = nullptr;
    }
    String &operator=(String &&src)
    {
        std::cout << "移动赋值" << std::endl;
        // 防止自赋值
        if (&src == this)
            return *this;
        //删除原有数据
        delete[] ptr_;
        //直接使用并将src置空
        ptr_ = src.ptr_;
        src.ptr_ = nullptr;
        return *this;

    }
    ~String()
    {
        std::cout << "析构函数u" << std::endl;
        delete[] ptr_;
        ptr_ = nullptr;
    }
    int length()
    {
        return strlen(ptr_);
    }
    const char *c_str() const
    {
        return ptr_;
    }
    bool operator>(const String &other) const
    {
        return strcmp(this->ptr_, other.ptr_) > 0;
    }
    bool operator<(const String &other) const
    {
        return strcmp(this->ptr_, other.ptr_) < 0;
    }
    bool operator==(const String &other) const
    {
        return strcmp(this->ptr_, other.ptr_) == 0;
    }
    char &operator[](int index)
    {
        if (index < strlen(ptr_))
            return ptr_[index];
    }
    // 给常对象用的，只能读不能写
    const char &operator[](int index) const
    {
        if (index < strlen(ptr_))
            return ptr_[index];
    }
    friend std::ostream &operator<<(std::ostream &out, const String &s);
    friend String operator+(const String &a, const String &b);

public:
    class iterator
    {
    public:
        iterator(char *p = nullptr) : p_(p)
        {
        }
        bool operator!=(const iterator &iter)
        {
            return p_ != iter.p_;
        }
        iterator &operator++()
        {
            ++p_;
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp = *this;
            p_++;
            return tmp;
        }
        char &operator*() const
        {
            return *p_;
        }

    private:
        char *p_;
    };
    iterator begin()
    {
        return iterator(ptr_);
    }
    iterator end()
    {
        return iterator(ptr_ + length());
    }

private:
    char *ptr_;
};
// 这里就特别时候移动构造了，我内部已经申请好了，就是因为生命周期的原因我还要再释放掉再拷贝一个新的。用右值引用来延长生命周期。
String operator+(const String &lhs, const String &rhs)
{
    String result;
    result.ptr_ = new char[strlen(lhs.ptr_) + strlen(rhs.ptr_) + 1];
    // char* p_temp = new char[strlen(lhs.ptr_) + strlen(rhs.ptr_)  + 1];
    strcpy(result.ptr_, lhs.ptr_);
    strcat(result.ptr_, rhs.ptr_);
    // String result = p_temp;
    // delete [] p_temp;
    return result;
}
// 这里就特别时候移动构造了，我内部已经申请好了，就是因为生命周期的原因我还要再释放掉再拷贝一个新的。用右值引用来延长生命周期。
//这里就直接用我这个即将被释放的temp得了
//所有的编译器都会进行一个优化：返回值优化
/*
在函数返回一个对象时，编译器可以直接在调用者的内存中构造这个返回对象，而不创建一个临时对象。
所以移动赋值只优化了一个部分，那就是将即将释放的temp使用上了。
但是再生成一个临时对象，再赋值，这一步被编译器优化了
*/
String GetString(String &str)
{
    String temp(str); // 拷贝构造temp
    return temp;
}

std::ostream &operator<<(std::ostream &out, const String &s)
{
    out << s.ptr_;
    return out;
}
