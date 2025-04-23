#ifndef _MY_DISJOINTSETS_H_
#define _MY_DISJOINTSETS_H_

#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "MyVector_w125t659.h"

class MyDisjointSets 
{
  public:
    // constructor
	explicit MyDisjointSets(const size_t size)
	{
        theSize = size;
        numSets = size;
        parents = MyVector<int>(size);

        for(size_t i = 0; i < size; ++i){
            parents[i] = -1;
        }
	}

    // destructor
    ~MyDisjointSets(void)
    {
        
    }
	
    // finds the ID of the set that contains x
    // implements path compression
    int find(const int x)
    {
        if(parents[x] < 0){
            return x;
        }
        return parents[x] = find(parents[x]);
    }

    // unions the two sets represented by r1 and r2, respectively
    // (r1 and r2 should be roots)
    // implements the union-by-size approach
	void unionSets(const int r1, const int r2)
	{
        int root1 = find(r1);
        int root2 = find(r2);

        if(root1 == root2){
            return;
        }

        int size1 = -parents[root1];
        int size2 = -parents[root2];

        if(size1 < size2){
            parents[root2] += parents[root1];
            parents[root1] = root2;
        }else{
            parents[root1] += parents[root2];
            parents[root2] = root1;
        }

        --numSets;
	}

    // returns the size of the set that contains x
    size_t setSize(const int x)
    {
        int root = find(x);
        return -parents[root];
    }

    // returns the total number of elements in the disjoint set
    size_t size()
    {
        return theSize;
    }

    // returns the number of disjoint sets
    size_t numDisjointSets()
    {
        return numSets;
    }

  private:
	size_t theSize;             // the number of elements in the disjoint sets
    size_t numSets;             // the number of disjoint sets maintained
    MyVector<int> parents;      // the info array; if the index is not the root, it contains the ID of its parent; otherwise it contains the size of the set as a negative number

};

#endif  // _MY_DISJOINTSETS_H_
