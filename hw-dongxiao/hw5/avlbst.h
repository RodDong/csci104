#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new AVLNode will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void adjustParent(AVLNode<Key,Value>* a, AVLNode<Key,Value>* b){
        b->setParent(a->getParent());
        if(a->getParent() != NULL){
            if(a->getParent()->getLeft() == a)
                a->getParent()->setLeft(b);
            else
                a->getParent()->setRight(b);
        }
        else{
            this->root_ = static_cast<Node<Key, Value>*>(b);
        }
        a->setParent(b);
    }
    AVLNode<Key,Value>* LL(AVLNode<Key,Value>* a){
        // std::cout<<"LL in " << a->getKey() << std::endl;
        AVLNode<Key,Value>* b = a->getLeft();
        a->setLeft(b->getRight());
        if(a->getLeft()!=NULL)a->getLeft()->setParent(a);
        b->setRight(a);

        adjustParent(a,b);

        // a->updateBalance(-2);
        // b->updateBalance(-1);
        return b;
    }
    AVLNode<Key,Value>* RR(AVLNode<Key,Value>* a){
        // std::cout<<"RR in " << a->getKey() << std::endl;
        AVLNode<Key,Value>* b = a->getRight();
        a->setRight(b->getLeft());
        if(a->getRight()!=NULL)a->getRight()->setParent(a);
        b->setLeft(a);

        adjustParent(a,b);

        // a->updateBalance(2);
        // b->updateBalance(1);
        return b;
    }
    AVLNode<Key,Value>* LR(AVLNode<Key,Value>* a){
        a->setLeft(RR(a->getLeft()));
        return LL(a);
    }
    AVLNode<Key,Value>* RL(AVLNode<Key,Value>* a){
        a->setRight(LL(a->getRight()));
        return RR(a);
    }
    int8_t UpdateTreeBalance(AVLNode<Key,Value>* a) {
        if(a == NULL) return 0;
        int8_t l = UpdateTreeBalance(a->getLeft());
        int8_t r = UpdateTreeBalance(a->getRight());
        int8_t diff = l - r;
        if(diff == 2){
            if(a->getLeft()->getBalance() == 1)
                a = LL(a);
            else
                a = LR(a);
        }
        else if(diff == -2){
            if(a->getRight()->getBalance() == -1)
                a = RR(a);
            else
                a = RL(a);
        }
        else{

            a->setBalance(diff);
            return std::max(l, r)+1;
        }
        l = UpdateTreeBalance(a->getLeft());
        r = UpdateTreeBalance(a->getRight());
        return std::max(l, r)+1;
    }
    void test(AVLNode<Key,Value>* a) {
        if(a == NULL) return;
        test(a->getLeft());
        std::cout << a->getKey() << "_____" << (int)a->getBalance() << std::endl;
        test(a->getRight());
    }
};

/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // DONE
    AVLNode<Key, Value>* root_ = static_cast<AVLNode<Key, Value>*>(this->root_);
    Key key = new_item.first;
    Value value = new_item.second;
    if(root_ == NULL) {
        this->root_ = static_cast<Node<Key, Value>*>(new AVLNode<Key, Value>(key, value, NULL));
        return;
    }
    AVLNode<Key, Value>* temp = root_;
    while(true){
        if(key < temp->getKey()){
            if(temp->getLeft()==NULL){
                temp->setLeft(new AVLNode<Key, Value>(key, value, temp));
                key = temp->getKey();//debug
                temp = temp->getLeft();
                break;
            }
            temp = temp->getLeft();
        }
        else if(key > temp->getKey()){
            if(temp->getRight()==NULL){
                temp->setRight(new AVLNode<Key, Value>(key, value, temp));
                temp = temp->getRight();
                break;
            }
            temp = temp->getRight();
        }
        else {
            temp->setValue(value);
            return;
        }
    }
    // AVLNode<Key, Value>* temp2 = temp;
    root_ = static_cast<AVLNode<Key, Value>*>(this->root_);
    UpdateTreeBalance(root_);
//    while(temp != root_){
//        // if(temp == temp->getParent()->getLeft())
//        //     temp->getParent()->updateBalance(1);
//        // else
//        //     temp->getParent()->updateBalance(-1);
//        temp = temp->getParent();
//        if(temp->getBalance() == 2){
//            if(temp->getLeft()->getBalance() == 1)
//                temp = LL(temp);
//            else
//                temp = LR(temp);
//            break;
//        }
//        else if(temp->getBalance() == -2){
//            if(temp->getRight()->getBalance() == -1)
//                temp = RR(temp);
//            else
//                temp = RL(temp);
//            break;
//        }
//    }
    // test(root_);
    // while(temp2 != NULL){
    //     int8_t old = temp2->getBalance();
    //     if(temp2->getBalance() == 2){
    //         if(temp2->getLeft()->getBalance() == 1)
    //             temp2 = LL(temp2);
    //         else
    //             temp2 = LR(temp2);
    //     }
    //     else if(temp2->getBalance() == -2){
    //         if(temp2->getRight()->getBalance() == -1)
    //             temp2 = RR(temp2);
    //         else
    //             temp2 = RL(temp2);
    //     }
    //     if(temp2->getBalance() != old){
    //         temp = temp2;
    //         root_ = static_cast<AVLNode<Key, Value>*>(this->root_);
    //         while(temp != root_){
    //             if(temp == temp->getParent()->getLeft())
    //                 temp->getParent()->updateBalance(-1);
    //             else
    //                 temp->getParent()->updateBalance(1);
    //             temp = temp->getParent();
    //         }
    //         test(root_);
    //     }
    //     temp2 = temp2->getParent();
    // }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // DONE
    AVLNode<Key, Value>* root_ = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value> *temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key)), *temp2;
    if(temp == NULL) return;
    if(temp->getLeft()!=NULL && temp->getRight()!=NULL){
        nodeSwap(temp, static_cast<AVLNode<Key, Value>*>(this->predecessor(temp)));
        root_ = static_cast<AVLNode<Key, Value>*>(this->root_);
    }
    if(temp == root_){
        if(root_->getLeft() != NULL)
            this->root_ = static_cast<Node<Key, Value>*>(root_->getLeft());
        else
            this->root_ = static_cast<Node<Key, Value>*>(root_->getRight());
    }
    else{

        temp2 = temp->getParent();
        if(temp == temp2->getLeft()){
            if(temp->getLeft() != NULL){
                temp2->setLeft(temp->getLeft());
                temp->getLeft()->setParent(temp2);
                temp2 = temp2->getLeft();
            }
            else if(temp->getRight() != NULL){
                temp2->setLeft(temp->getRight());
                temp->getRight()->setParent(temp2);
                temp2 = temp2->getLeft();
            }
            else
                temp2->setLeft(NULL);
        }
        else{
            if(temp->getLeft() != NULL){
                temp2->setRight(temp->getLeft());
                temp->getLeft()->setParent(temp2);
                temp2 = temp2->getRight();
            }
            else if(temp->getRight() != NULL){
                temp2->setRight(temp->getRight());
                temp->getRight()->setParent(temp2);
                temp2 = temp2->getRight();
            }
            else
                temp2->setRight(NULL);
        }
    }
    delete temp;
    root_ = static_cast<AVLNode<Key, Value>*>(this->root_);
    UpdateTreeBalance(root_);

//    while(temp2 != NULL){
//        if(temp2->getBalance() == 2){
//            if(temp2->getLeft()->getBalance() == 1)
//                temp2 = LL(temp2);
//            else
//                temp2 = LR(temp2);
//            root_ = static_cast<AVLNode<Key, Value>*>(this->root_);
//            UpdateTreeBalance(root_);
//        }
//        else if(temp2->getBalance() == -2){
//            if(temp2->getRight()->getBalance() == -1)
//                temp2 = RR(temp2);
//            else
//                temp2 = RL(temp2);
//            root_ = static_cast<AVLNode<Key, Value>*>(this->root_);
//            UpdateTreeBalance(root_);
//        }
//        temp2 = temp2->getParent();
//    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
