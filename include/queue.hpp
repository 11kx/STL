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
        void* operator new(size_t size)
        {
            if(item_pool_ == nullptr)
            {
                item_pool_ = (queItem*)new char [POOLSIZE * sizeof(queItem)];
                //把申请好的位置链接起来
                queItem* p = item_pool_;
                for(;p < item_pool_ + POOLSIZE - 1;p++)
                {
                    p->next_ = p + 1;
                }
                p->next_ = nullptr;
            }
            
            //从对象池里第一个位置拿一个对象给调用者
            queItem* p = item_pool_;
            item_pool_ = item_pool_ ->next_;
            return p;
            
        }
        void operator delete(void* ptr)
        {
            queItem* p = (queItem*) ptr;
            p->next_ = item_pool_;
            item_pool_ = p;
        }
        //采用对象池的形式来构造对象
        static queItem* item_pool_;    //指向池的第一个对象
        static const int POOLSIZE = 1000000;    //对象池的size
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

template <typename T>
typename queue<T>::queItem *queue<T>::queItem::item_pool_ = nullptr;