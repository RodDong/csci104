#include "internal-range.h"
#include <stack>
using namespace std;


// You may add any prototypes of helper functions here


// Now implement the sumInternalRange function and any necessary helpers

int findDepth(Node* node){
    int depth = 0;
    if(node->parent != nullptr){
        return 1+findDepth(node->parent);
    }else{
        return 0;
    } 
}

int sumInternalRange(Node * root, int depth, int min, int max){
    int count = 0;
    Node* current = root;
    stack<Node*> stack;
    //check root
    if(current == nullptr){
        return 0;
    }

    while(current!= nullptr || !stack.empty()){
        while(current != nullptr){
            stack.push(current);
            current = current->left;
        }

        current = stack.top();
        stack.pop();
        if(current->key <= max && current->key >= min && findDepth(current) < depth){
            count++;
        }
        current = current -> right;

    }
    return count;
}

