#ifndef STACK_H
#define STACK_H

/**
 * Your Stack<T> class must be templated and
 * privately inherit from LList<T>. All
 * operations must run in O(1).
 *
 * Both pop() and top() must throw std::underflow_error
 * if the stack is empty.
 */
#include "llist.h"
#include <stdexcept>

using namespace std;



template<typename T>
class Stack : public LList<T> // complete the declaration and implementation
{


public:
    size_t size() const;
    bool empty() const;
    T const& top() const;
    void push(const T& item);
    void pop();
    void print(typename LList<T>::Item* head);
};

#endif



template<typename T>
size_t Stack<T>::size() const {
    return LList<T>::size();
}

template<typename T>
bool Stack<T>::empty() const {
    return LList<T>::size() == 0;
}

template<typename T>
T const& Stack<T>::top() const {
    if (empty()) {
        throw underflow_error("The Stack is empty");
    }
    return (*this)[0];
}

template<typename T>
void Stack<T>::push(const T& item) {
    
    LList<T>::push_front(item);

}

template<typename T>
void Stack<T>::pop() {
    if (empty()) {
        throw underflow_error("The Stack is empty");
    }
    
    LList<T>::remove(0);
}

