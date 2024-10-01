#include <iostream>
#include <string.h>
class String
{
public:
    String(const char* str = nullptr)
    {
        std::cout << "构造函数" << std::endl;
        if(str != nullptr)
        {
            ptr_ = new char[strlen(str) + 1];
            strcpy(ptr_,str);
        }
        else
        {
            ptr_ = new char[1];
            ptr_[0] = '\0';
        }
    }
    String(const String& src)
    {
        std::cout << "拷贝构造函数" << std::endl;
        //有指针，进行深拷贝
        const int size = strlen(src.ptr_);
        ptr_ = new char[size + 1];
        strcpy(ptr_,src.ptr_);
        
    }
    String& operator=(const String& src)
    {
        std::cout << "拷贝赋值" << std::endl;
        //防止自赋值
        if(&src == this)
            return *this;
        //删除原有数据
        delete[] ptr_;
        //拷贝过来
        const int size = strlen(src.ptr_);
        ptr_ = new char[size + 1];
        strcpy(ptr_,src.ptr_);
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
    const char* c_str()
    {
        return ptr_;
    }
    bool operator>(const String& other) const
    {
        return strcmp(this->ptr_,other.ptr_) > 0;
    }
    bool operator<(const String& other) const
    {
        return strcmp(this->ptr_,other.ptr_) < 0;
    }
    bool operator==(const String& other) const
    {
        return strcmp(this->ptr_,other.ptr_) == 0;
    }
    char& operator[](int index)
    {
        if(index < strlen(ptr_))
            return ptr_[index];
    }
    //给常对象用的，只能读不能写
    const char& operator[](int index) const
    {
        if(index < strlen(ptr_))
            return ptr_[index];
    }
    friend std::ostream& operator<<(std::ostream& out,const String& s);
    friend String operator+(const String& a,const String& b);
private:
    char* ptr_;
};
//这里就特别时候移动构造了，我内部已经申请好了，就是因为生命周期的原因我还要再释放掉再拷贝一个新的。用右值引用来延长生命周期。
String operator+(const String& lhs,const String& rhs)
{
    char* p_temp = new char[strlen(lhs.ptr_) + strlen(rhs.ptr_)  + 2];
    strcpy(p_temp,lhs.ptr_);
    strcat(p_temp,rhs.ptr_);
    String result = p_temp;
    delete [] p_temp;
    return result;
}

std::ostream& operator<<(std::ostream& out,const String& s)
{
    out << s.ptr_;
    return out;
} 
