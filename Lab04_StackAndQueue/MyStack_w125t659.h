#ifndef __MYSTACK_H__
#define __MYSTACK_H__

#include <iostream>
#include <algorithm>

#include "MyVector_w125t659.h"

template <typename DataType>
class MyStack : private MyVector<DataType>
{  
  private:
    MyVector<DataType> stackData;

  public:

    // default constructor
    explicit MyStack(size_t initSize = 0)
    {
        stackData.reserve(initSize);    
    }

    // copy constructor
    MyStack(const MyStack & rhs)
    {
        stackData = rhs.stackData;
    }

    // move constructor
    MyStack(MyStack && rhs)
    {
        stackData = std::move(rhs.stackData);
    }

    // destructor
    ~MyStack()
    {
        ;
    }

    // copy assignment
    MyStack & operator= (const MyStack & rhs)
    {
        stackData = rhs.stackData;
        return *this;
    }

    // move assignment
    MyStack & operator= (MyStack && rhs)
    {
        stackData = std::move(rhs.stackData);
        return *this;
    }

    // insert x to the stack
    void push(const DataType & x)
    {
        stackData.push_back(x);
    }

    // insert x to the stack
    void push(DataType && x)
    {
        stackData.push_back(x);
    }

    // remove the last element from the stack
    void pop(void)
    {
        stackData.pop_back();
    }

    // access the last element of the stack
    const DataType & top(void) const
    {
        return stackData.back();
    }

    // check if the stack is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const
    {
        return stackData.empty();
    }

    // access the size of the stack
    size_t size() const
    {
        return stackData.size();
    }

    // access the capacity of the stack
    size_t capacity(void) const 
    {
        return stackData.capacity();
    }
};


#endif // __MYSTACK_H__