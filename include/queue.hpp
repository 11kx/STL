template <typename T>
class queue
{
private:
    struct queItem
    {
        queItem(T data = T(), queItem *next = nullptr)
            : value_(data), next_(next)
        {
        }
        T value_;
        queItem *next_;
    };
    queItem *front_; // 指向队列第一个节点
    queItem *rear_;  // 指向队尾
public:
    queue()
    {
        front_ = rear_ = new queItem();
    }
    ~queue()
    {
        queItem* p_item = front_;
        while (p_item->next_ != nullptr)
        {
            queItem* tmp = p_item;
            p_item = p_item->next_;
            delete tmp;
        }
        //删除最后一个节点
        delete p_item;        
    }
    //从队尾插入
    void push(const T& value)
    {
        queItem* p_item = new queItem(value);
        rear_->next_ = p_item;
        rear_ = p_item;
    }
    //从队首弹出
    void pop()
    {
        if(empty())
        {
            throw("The queue is empty");
        }
        queItem* p_item = front_->next_;
        front_->next_ = p_item->next_;
        if(front_->next_ == nullptr)
        {
            rear_ = front_;
        }
        delete p_item;
    }
    T front()
    {
        if(empty())
        {
            throw("The queue is empty");
        }
        return front_->next_->value_;
    }
    bool empty()
    {
        return front_ == rear_;
    }
};