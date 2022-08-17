#ifndef HEAP_H
#define HEAP_H
#include <functional>
#include <stdexcept>
#include <vector>
#include <iostream>
template <typename T, typename PComparator = std::less<T> >
class Heap
{
public:
    /**
    * @brief Construct a new Heap object
    *
    * @param m ary-ness of heap tree (default to 2)
    * @param c binary predicate function/functor that takes two items
    *          as an argument and returns a bool if the first argument has
    *          priority over the second.
    */
    Heap(int m=2, PComparator c = PComparator());

    /**
    * @brief Destroy the Heap object
    *
    */
    ~Heap();

    /**
    * @brief Push an item to the heap
    *
    * @param item item to heap
    */
    void push(const T& item);

    /**
    * @brief Returns the top (priority) item
    *
    * @return T const& top priority item
    * @throw std::underflow_error if the heap is empty
    */
    T const & top() const;

    /**
    * @brief Remove the top priority item
    *
    * @throw std::underflow_error if the heap is empty
    */
    void pop();

    /// returns true if the heap is empty

    /**
    * @brief Returns true if the heap is empty
    *
    */
    bool empty() const;
private:
    /// Add whatever helper functions and data members you need below
    std::vector<T> data;
    int h;
    int ary;
    PComparator cmp;

};

// Add implementation of member functions here

template <typename T, typename PComparator>
Heap<T,PComparator>::Heap(int m, PComparator c) {
    h = 0;
    ary = m;
    cmp = c;

}
template <typename T, typename PComparator>
Heap<T,PComparator>::~Heap()
{
    data.clear();
}
template <typename T, typename PComparator>
void Heap<T,PComparator>::push(const T& item)
{
    data.push_back(item);
    int i = data.size() - 1;
    int parent = (i-1)/ary;
    while(parent >= 0){
        if(cmp(data[i], data[parent])) {
            std::swap(data[i], data[parent]);
            i = parent;
            parent = (i-1)/ary;
        }
        else break;
    }
}
template <typename T, typename PComparator>
bool Heap<T,PComparator>::empty() const
{
    return data.size()==0?true:false;
}



// We will start top() for you to handle the case of
// calling top on an empty heap
template <typename T, typename PComparator>
T const & Heap<T,PComparator>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()){
    // ================================
    // throw the appropriate exception
    // ================================
        throw std::out_of_range("have no element.");


    }
    // If we get here we know the heap has at least 1 item
    // Add code to return the top element

    return data[0];

}


// We will start pop() for you to handle the case of
// calling top on an empty heap
template <typename T, typename PComparator>
void Heap<T,PComparator>::pop()
{
    if(empty()){
    // ================================
    // throw the appropriate exception
    // ================================
        throw std::out_of_range("have no element.");


    }
    data[0] = data.back();
    data.pop_back();
    int loc = 0;
    while(true){
        int _i = loc*ary+1;
        if((size_t)(_i)>=data.size())
            return;
        T _t = data[_i];
        for(int i=2;i<=ary;++i)
            if((size_t)(loc*ary+i)<data.size() && cmp(data[loc*ary+i], _t)){
                _i = loc*ary+i;
                _t = data[_i];
            }
        if(cmp(data[_i], data[loc])) {
            std::swap(data[_i], data[loc]);
            loc = _i;
        }
        else return;

    }
}



#endif

