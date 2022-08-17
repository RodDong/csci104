#include <iostream>
#include "rem_dup_lib.h"
using namespace std;

// Add your implementations below

void removeConsecutive(Item* head){
    if(head == nullptr){
        return;
    }
    if(head->next == nullptr){
        return;
    }
    if(head->val == head->next->val){
        Item* temp = head->next;
        head->next = temp->next;
        delete(temp);
        removeConsecutive(head);
    }else{
        removeConsecutive(head->next);
    }
}

Item* concatenate(Item* head1, Item* head2){
    if(head1 == nullptr && head2 == nullptr){
        return nullptr;
    }else if(head1 == nullptr){
        Item* temp = head2->next;
        Item* copy2 = new Item(head2->val, nullptr);
        Item* current  = copy2;
        Item* result = current;
        while(temp!=nullptr){
            current->next = new Item(temp->val, nullptr);
            current = current->next;
            temp = temp->next;
        }
        return result;
    }else if(head2 == nullptr){
        Item* temp = head1->next;
        Item* copy1 = new Item(head1->val, nullptr);
        Item* current  = copy1;
        Item* result = current;
        while(temp!=nullptr){
            current->next = new Item(temp->val, nullptr);
            current = current->next;
            temp = temp->next;
        }
        return result;
    }

    if(head1->next == nullptr){
        head1->next = head2;
    }else{
        head1->next = concatenate(head1->next, head2);
    }
    Item* temp = head1->next;
    Item* copy1 = new Item(head1->val, nullptr);
    Item* current = copy1;
    Item* result = current;
    while(temp!=nullptr){
        current->next = new Item(temp->val, nullptr);
        current = current->next;
        temp = temp->next;
    }
    return result;
}