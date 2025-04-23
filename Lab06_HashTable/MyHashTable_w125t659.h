#ifndef __MYHASHTABLE_H__
#define __MYHASHTABLE_H__

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>

#include "MyVector_w125t659.h"
#include "MyLinkedList_w125t659.h"

static const long long uh_param_a = 53;       // universal hash function parameter a
static const long long uh_param_b = 97;       // universal hash function parameter b
static const long long prime_digits = 19;     // parameter used for finding a Mersenne prime
static const long long mersenne_prime = (1 << prime_digits) - 1;  // the Mersenne prime for universal hashing

// fast calculation of (n modulo mersenne_prime)
long long fastMersenneModulo(const long long n)
{
    long long res = n;
    while(res >= mersenne_prime){
        res = (res & mersenne_prime) + (res >> prime_digits);
    }

    return res == mersenne_prime ? 0 : res;
}

// definition of the template hash function class
template <typename KeyType>
class HashFunc
{
  public:
    long long univHash(const KeyType key, const long long table_size) const;
};

// the hash function class that supports the hashing of the "long long" data type
template <>
class HashFunc<long long>
{
  public:
    long long univHash(const long long key, const long long table_size) const
    {
        long long hv = fastMersenneModulo(static_cast<long long>(uh_param_a * key + uh_param_b));
        hv = hv % table_size;
        return hv;
    }
};

// the has function class that supports the hashing of the "std::string" data type
template <>
class HashFunc<std::string>
{
  private:
    const int param_base = 37;      // the base used for inflating each character
  public:    
    long long univHash(const std::string& key, const long long table_size) const
    {
        long long hv = 0;
        for(size_t i = 0; i < key.length(); ++ i)
        {
            hv = param_base * hv + static_cast<long long>(key[i]);
        }
        hv = fastMersenneModulo(static_cast<long long>(uh_param_a * hv + uh_param_b));
        hv = hv % table_size;
        return hv;
    }
};

// definition of the template hashed object class
template <typename KeyType, typename ValueType>
class HashedObj
{
  public:

    KeyType key;
    ValueType value;

    HashedObj()
    {
        return;
    }

    HashedObj(const KeyType& k, const ValueType& v) :
        key(k),
        value(v)
    {
        return;
    }
  
    HashedObj(KeyType && k, ValueType && v) :
        key(std::move(k)),
        value(std::move(v))
    {
        return;
    }

    bool operator==(const HashedObj<KeyType, ValueType>& rhs)
    {
        return (key == rhs.key);
    }    

    bool operator!=(const HashedObj<KeyType, ValueType>& rhs)
    {
        return !(*this == rhs);
    }   

};

template <typename KeyType, typename ValueType>
class MyHashTable
{ 
  private:
    size_t theSize; // the number of data elements stored in the hash table
    MyVector<MyLinkedList<HashedObj<KeyType, ValueType> >* > hash_table;    // the hash table implementing the separate chaining approach
    MyVector<size_t> primes;    // a set of precomputed and sorted prime numbers

    // pre-calculate a set of primes using the sieve of Eratosthenes algorithm
    // will be called if table doubling requires a larger prime number for table size
    // expected to update the private member "primes"
    void preCalPrimes(const size_t n)
    {
        // create new array with size n^2
        size_t limit = n * 2;
        std::vector<bool> is_prime(limit, true);
        is_prime[0] = is_prime[1] = false;

        // check each number until the limit
        for (size_t i = 2; i * i <= limit; i++){
            // mark multiples as false
            if(is_prime[i]){
                // i^2 is lowest possible uncounted multiple
                for (size_t j = i * i; j < limit; j += i){
                    is_prime[j] = false;
                }
            }
        }

        // clear any previously stored primes
        primes = MyVector<size_t>();

        // convert boolean array to numbers
        for (size_t i = 2; i <= limit; i++){
            if (is_prime[i]){
                primes.push_back(i);
            }
        }

        // for (size_t prime : primes){
        //     std::cout << prime << " ";
        // }
        // std::cout << "" << std::endl;
    }

    // finding the smallest prime that is larger than or equal to n
    // should perform binary search against the private member "primes"
    size_t nextPrime(const size_t n)
    {
        // calculate more primes if necessary
        if (primes.empty() || primes.back() < n)
            preCalPrimes(n);

        // binary search algorithm
        size_t left = 0;
        size_t right = primes.size() - 1;

        while (left < right){
            size_t mid = left + (right - left) / 2;
            if (primes[mid] < n){
                left = mid + 1; 
            }else{
                right = mid;
            }
        }

        return primes[left];
    }

    // finds the MyLinkedList itertor that corresponds to the hashed object that has the specified key
    // returns the end() iterator if not found
    typename MyLinkedList<HashedObj<KeyType, ValueType>>::iterator find(const KeyType& key)
    {
        size_t index = HashFunc<KeyType>().univHash(key, hash_table.size());
        auto& list = hash_table[index];

        for (auto it = list->begin(); it != list->end(); ++it) {
            if ((*it).key == key) {  // Use *it, which internally calls operator*()
                return it;
            }
        }

        return list->end();
    }

    // rehashes all data elements in the hash table into a new hash table with new_size
    // note that the new_size can be either smaller or larger than the existing size
    void rehash(const size_t new_size)
    {
        // Create a new table with the correct size and initialize it
        MyVector<MyLinkedList<HashedObj<KeyType, ValueType>>*> new_table(new_size);

        // Initialize the new table's linked lists
        for (size_t i = 0; i < new_size; ++i) {
            new_table[i] = new MyLinkedList<HashedObj<KeyType, ValueType>>();
        }

        size_t old_size = theSize;

        // Rehash all elements into the new table
        for (size_t i = 0; i < hash_table.size(); ++i) {
            if (hash_table[i]) {
                for (const auto& item : *hash_table[i]) {
                    size_t new_index = HashFunc<KeyType>().univHash(item.key, new_size);
                    new_table[new_index]->push_back(item);
                }
                delete hash_table[i];  // Free old memory
            }
        }

        // Assign new table and update the size
        hash_table = std::move(new_table);
        theSize = old_size;
    }

    // doubles the size of the table and perform rehashing
    // the new table size should be the smallest prime that is larger than the expected new table size (double of the old size)
    void doubleTable()
    {
        size_t new_size = nextPrime(2 * hash_table.capacity() + 1);
        this->rehash(new_size);
    }

    // halves the size of the table and perform rehahsing
    // the new table size should be the smallest prime that is larger than the expected new table size (half of the old size)
    void halveTable()
    {
        size_t new_size = nextPrime(ceil(hash_table.capacity() / 2));
        this->rehash(new_size);
    }

  public:

    // the default constructor; allocate memory if necessary
    explicit MyHashTable(const size_t init_size = 3)
    {
        size_t table_size = nextPrime(init_size);
        hash_table.resize(table_size);

        theSize = 0;

        for (size_t i = 0; i < table_size; ++i) {
            hash_table[i] = new MyLinkedList<HashedObj<KeyType, ValueType>>();
        }
    }

    // the default destructor; collect memory if necessary
    ~MyHashTable()
    {
        for (size_t i = 0; i < hash_table.size(); ++i) {
            delete hash_table[i];  // Free each MyLinkedList object
        }
    }

    // checks if the hash tabel contains the given key
    bool contains(const KeyType& key)
    {
        auto it = find(key);
        size_t index = HashFunc<KeyType>().univHash(key, hash_table.size());
        bool contained = (it != hash_table[index]->end());

        return contained;
    }

    // retrieves the data element that has the specified key
    // returns true if the key is contained in the hash table
    // return false otherwise
    bool retrieve(const KeyType& key, HashedObj<KeyType, ValueType>& data)
    {
        if(contains(key)){
            // key found; update data and return true
            auto it = find(key);
            data = *it;
            return true;
        }

        // key not found; return false
        return false;
    }

    // inserts the given data element into the hash table (copy)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(const HashedObj<KeyType, ValueType>& x)
    {
        // check redundancy
        if(contains(x.key))
            return false;
        
        // insert element into hash table
        size_t index = HashFunc<KeyType>().univHash(x.key, hash_table.size());
        hash_table[index]->push_back(x);

        theSize++;

        double loadFactor = (static_cast<double>(size()) / capacity());
        if(loadFactor > 0.5)
            doubleTable();

        if(capacity() == 7)
            doubleTable();

        return true;
    }

    // inserts the given data element into the hash table (move)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(HashedObj<KeyType, ValueType> && x)
    {
        // check redundancy
        if(contains(x.key))
            return false;
        
        // insert element into hash table
        size_t index = HashFunc<KeyType>().univHash(x.key, hash_table.size());
        hash_table[index]->push_back(std::move(x));

        theSize++;


        double loadFactor = (static_cast<double>(size()) / capacity());
        if(loadFactor > 0.5)
            doubleTable();

        if(capacity() == 7)
            doubleTable();

        return true;
    }

    // removes the data element that has the key from the hash table
    // returns true if the key is contained in the hash table
    // returns false otherwise
    bool remove(const KeyType& key)
    {
        // find key's bucket
        size_t index = HashFunc<KeyType>().univHash(key, hash_table.size());

        // check if key is in list
        auto it = find(key);
        if(it == hash_table[index]->end())
            // key not found
            return false;

        // remove key
        hash_table[index]->erase(it);
        
        theSize--;

        double loadFactor = (static_cast<double>(size()) / capacity());
        if(loadFactor < (0.125))
            halveTable();

        return true;
    }

    // returns the number of data elements stored in the hash table
    size_t size()
    {
        return theSize;
    }

    // returns the capacity of the hash table
    size_t capacity()
    {
        return hash_table.capacity();
    }

};


#endif // __MYHASHTABLE_H__
