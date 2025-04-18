#ifndef __MYBINARYHEAP_H__
#define __MYBINARYHEAP_H__

#include <iostream>
#include <cstdlib>

#include "MyVector_w125t659.h"

// ComparableType should be comparable (<, >, ==, >=, <= operators implemented)
// ComparableType should also have +, +=, -, -= operators implemented to support priority adjustment

template <typename ComparableType>
class MyBinaryHeap 
{
  private:
	MyVector<ComparableType> data;  // the array that holds the data elements

    // moves the data element at the pth position of the array up
    void percolateUp(const size_t p) 
    {
        // save initial data to swap
        size_t i = p;
        ComparableType temp = data[i];

        while(i > 1 && temp > data[i/2]){
            // move parent down
            data[i] = data[i/2];
            // move index upwards
            i /= 2;
        }

        // set found index 
        data[i] = temp;
    }

    // moves the data element at the pth position of the array down
    void percolateDown(const size_t p) 
    {
        size_t i = p;
        size_t leftChild = i * 2;
        size_t rightChild = i * 2 + 1;

        while(leftChild < data.size()){

            // find which child is larger
            size_t largerChild = leftChild;

            if(rightChild < data.size() && data[rightChild] > data[leftChild]){
                largerChild = rightChild;
            }

            // ensure data should be swapped by max heap logic
            if (data[i] >= data[largerChild]) {
                break;
            }

            std::swap(data[i], data[largerChild]);

            // move index, then loop
            i = largerChild;
            leftChild = i * 2;
            rightChild = i * 2 + 1;
        }
    }

    // reorders the data elements in the array to ensure heap property
    void buildHeap() 
    {
        // start from last non-leaf node
        for(size_t i = data.size() / 2; i > 0; --i){
            percolateDown(i);
        }
    }	

  public: 

    // default constructor
    explicit MyBinaryHeap() :
        data(1)     // reserve data[0]
    {
        // no extra code necessary
    }

    // constructor from a set of data elements
    MyBinaryHeap(const MyVector<ComparableType>& items) : 
        data(1)     // reserve data[0]
    {
        // move items from parameter to data
        for(size_t i = 0; i < items.size(); ++i){
            data.push_back(items[i]);
        }

        // resort data to make it max heap
        buildHeap();
    }

    // copy constructor
    MyBinaryHeap(const MyBinaryHeap<ComparableType>& rhs)
    {
        data = rhs.data;
    }

    // move constructor
    MyBinaryHeap(MyBinaryHeap<ComparableType> && rhs)
    {
        data = std::move(rhs.data);

        rhs.data.clear();
    }

    // copy assignment
    MyBinaryHeap& operator=(const MyBinaryHeap<ComparableType>& rhs)   
    {
        if (this != &rhs){
            data = rhs.data;
        }

        return *this;
    }

    // move assignment
    MyBinaryHeap& operator=(MyBinaryHeap<ComparableType> && rhs)
    {
        if (this != &rhs){
            data = std::move(rhs.data);
            rhs.data.clear();
        }

        return *this;
    }

    // inserts x into the priority queue (copy)
	void enqueue(const ComparableType& x)
    {
        data.push_back(x);

        percolateUp(data.size() - 1);
    } 

    // inserts x into the priority queue (move)
    void enqueue(ComparableType && x)
    {
        data.push_back(std::move(x));

        percolateUp(data.size() - 1);
    }

    // accesses the data element with the highest priority
	const ComparableType& front()
    {
        if(data.size() <= 1){
            throw std::underflow_error("Heap is empty");
        }

        return data[1];
    } 

    // deletes the data element with the highest priority from the queue
    void dequeue()
    {
        if (data.size() > 1){
            // replace root with last element
            std::swap(data[1], data[data.size() - 1]);

            // remove previous root
            data.pop_back();

            // fix heap
            percolateDown(1);
        }
    }

    // verifies whether the array satisfies the heap property
    bool verifyHeapProperty(void)
    {
        // loop for every element in data
        for(size_t i = 1; i < data.size(); ++i){
            size_t leftChild = i * 2;
            size_t rightChild = i * 2 + 1;

            bool leftWrong = leftChild < data.size() && data[i] < data[leftChild];
            bool rightWrong = rightChild < data.size() && data[i] < data[rightChild];

            // if either child is wrong, 
            if(leftWrong || rightWrong){
                return false;
            }
        }

        return true;
    }

    // disrupts heap property by random shuffling
    void disruptHeapProperty(void)
    {
        if(data.size() <= 3)
            return;
        for(size_t i = 0; i < 1000; ++ i)
        {
            size_t p = 1 + ((int) rand()) % (data.size() - 1);
            size_t q = 1 + ((int) rand()) % (data.size() - 1);
            std::swap(data[p], data[q]);
        }
        return;
    }

    // merges two heaps; the second heap can be destructed after the merge
    MyBinaryHeap& merge(MyBinaryHeap<ComparableType> && rhs) 
    {
        // move rhs.data to data
        for(size_t i = 1; i < rhs.data.size(); ++i){
            data.push_back(std::move(rhs.data[i]));
        }

        // clear rhs
        rhs.data.resize(1);

        // fix max heap
        buildHeap();

        return *this;
    }

    // increases the priority measure of an element at a specific position and reorder the heap
	void increaseKey(const size_t p, const unsigned int d)
    {    
        // increase data at index p
        data[p] += d;

        // rearrange to retain heap
        percolateUp(p);
    }

    // decreases the priority measure of an element at a specific position and reorder the heap
    // if the current priority is smaller than the requested decrement, assign priority 0
	void decreaseKey(const size_t p, const unsigned int d) 
    {
        // decrease data at index p
        if(data[p] < d){
            data[p] = 0;
        }else{
            data[p] -= d;
        }

        // rearrange to retain heap
        percolateDown(p);
    }

    // checks whether the heap is empty
    bool empty()
    {
        return data.size() <= 1;
    }

    // removes all data elements from the heap
    void clear()
    {
        data.clear();
    }

    // returns the size of the heap
    size_t size()
    {
        return data.size() - 1;
    }

    // prints the data in the array
    void print() 
    {
        const char delim = '\t';
        for (size_t i = 1; i < data.size(); ++ i) {
            std::cout << data[i] << delim;
        }
        std::cout << std::endl;
        return;
    }

};

#endif // __MYBINARYHEAP_H__