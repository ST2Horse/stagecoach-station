#pragma once
//我是按照没有哨兵节点搞的
/*模板节点结构体*/
template <typename T>   //考虑到结构体类型的不同
struct Node
{
    T data;                                 //存储普遍T数据
    Node* next;                             //指向下一个节点
    Node(T val): data(val),next(nullptr){}  //构造函数
};



/*创建链表同时初始化头节点*/
template <typename T>
Node<T>* createlinkedlist(T value)
{
    return new Node<T>(value);  //创建普遍T节点
}

/*添加节点到链表结尾*/
template <typename T>
void addNode(Node<T>*& head, T value)      //使用引用避免指针的复杂输入
{
    Node<T>* newNode = new Node<T>(value);
    if (head == nullptr)                   //头节点为空时
    {
        head = newNode;                    //创建节点
        return;
    }
    Node<T>* temp = head;                 //头节点不为空
    while (temp->next != nullptr)         //从头遍历到next为空节点
    {        
        temp = temp->next;              
    }
    temp->next = newNode;                 //尾部添加节点
}

/*删除节点*/
template <typename T>
void delNode(Node<T>*& head, T value)  //使用引用避免指针的复杂输入
{
    if(head == nullptr) return;        //空链表直接返回

    if(head->data == value)            //删除头节点
    {                               
        Node<T>* temp = head;          //创建临时节点
        head = head->next;             //覆盖头节点
        delete temp;                   //删除目标节点
        return;
    }

    Node<T>* temp = head;              //删除中间和尾节点
    while (temp->next != nullptr && temp->next->data != value)
    {                                  //遍历至删除节点的前一个
        temp = temp->next;
    }

    if (temp->next != nullptr)         // 找到前一位，且目标节点且不为空
    {  
        Node<T>* deltemp = temp->next; //创建临时节点
        temp->next = temp->next->next; //覆盖目标节点
        delete deltemp;                //删除目标节点
    }
    return;
}