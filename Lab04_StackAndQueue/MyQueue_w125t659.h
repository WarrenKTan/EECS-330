#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__

#include <iostream>
#include <algorithm>

#include "MyVector_w125t659.h"

template <typename DataType>
class MyQueue : private MyVector<DataType>
{  
  private:
    size_t dataStart, dataEnd;
    MyVector<DataType> queueData;

    // changes the size of the array to newSize
    void resize(size_t newSize)
    {
        reserve(newSize * 2);
    }

    // requests for newCapacity amount of space
    void reserve(size_t newCapacity)
    {
        if(queueData.capacity() > newCapacity){
            return;
        }

        MyVector<DataType> newQueue(newCapacity);
        
        for(size_t i = dataStart; i != dataEnd; i++){
            newQueue.push_back(queueData[i]);
        }

        std::swap(queueData, newQueue);
    }

  public:

    // default constructor
    explicit MyQueue(size_t initSize = 0)
    {
        dataStart = 0;
        dataEnd = initSize;
        queueData.reserve(initSize);
    }

    // copy constructor
    MyQueue(const MyQueue & rhs)
    {
        *this = rhs;
    }

    // move constructor
    MyQueue(MyQueue && rhs)
    {
        queueData = std::move(rhs.queueData);
        dataStart = rhs.dataStart;
        dataEnd = rhs.dataEnd;
        
        rhs.dataStart = 0;
        rhs.dataEnd = 0;
    }

    // destructor
    ~MyQueue()
    {
        ;
    }

    // copy assignment
    MyQueue & operator= (const MyQueue & rhs)
    {
        queueData = rhs.queueData;
        dataStart = rhs.dataStart;
        dataEnd = rhs.dataEnd;
        return *this;
    }

    // move assignment
    MyQueue & operator= (MyQueue && rhs)
    {
        queueData = std::move(rhs.queueData);
        dataStart = rhs.dataStart;
        dataEnd = rhs.dataEnd;
        
        rhs.dataStart = 0;
        rhs.dataEnd = 0;

        return *this;
    }

    // insert x into the queue
    void enqueue(const DataType & x)
    {
        if(dataEnd == queueData.capacity()){
            reserve(queueData.capacity() * 2);
        }

        queueData.push_back(x);
        dataEnd++;
    }

    // insert x into the queue
    void enqueue(DataType && x)
    {
        if(dataEnd == queueData.capacity()){
            reserve(queueData.capacity() * 2);
        }

        queueData.push_back(std::move(x));
        dataEnd++;
    }

    // remove the first element from the queue
    void dequeue(void)
    {
        if (!empty()) {
            dataStart++;
            // Optional: shift data back to the front if too much unused space
            if (dataStart > queueData.size() / 2) {
                MyVector<DataType> newQueue;
                for (size_t i = dataStart; i < dataEnd; ++i) {
                    newQueue.push_back(queueData[i]);
                }
                std::swap(queueData, newQueue);
                dataEnd = dataEnd - dataStart;
                dataStart = 0;
            }
        }
    }

    // access the first element of the queue
    const DataType & front(void) const
    {
        return queueData[dataStart];
    }

    // check if the queue is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const
    {
        return dataStart == dataEnd;
    }

    // access the size of the queue
    size_t size() const
    {
        return dataEnd - dataStart;
    }

    // access the capacity of the queue
    size_t capacity(void) const 
    {
        return queueData.capacity();
    }

};


#endif // __MYQUEUE_H__