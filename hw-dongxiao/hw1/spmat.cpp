#include "spmat.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
using namespace std;

#ifdef SPARSE_DEBUG
#include "spmat-debug.h"
// global instance of the debug helper
SparseMatrixDebugHelper debug;
#endif

// Define and initialize the constant static member npos here

size_t const SparseMatrix::npos = (size_t)-1;

// ----------------------------------------------------------------------------
// ------------------ Coord struct Implementation -----------------------------
// ----------------------------------------------------------------------------

// Constructor
// COMPLETE
SparseMatrix::Coord::Coord(size_t row, size_t col) : r(row), c(col) {}

// operator[] accessors
// To be completed - Must adhere it requirements in the .h
size_t& SparseMatrix::Coord::operator[](size_t index) {
    if (index >= INVALID_DIM) {
        throw std::out_of_range("out of range");
    } else if (index == ROW) {
        return r;
    } else if (index == COL) {
        return c;
    }
}

size_t const& SparseMatrix::Coord::operator[](size_t index) const {
    if (index >= INVALID_DIM) {
        throw std::out_of_range("out of range");
    } else if (index == ROW) {
        return r;
    } else if (index == COL) {
        return c;
    }
}

// operator== comparison function
// To be completed - Must adhere it requirements in the .h
bool SparseMatrix::Coord::operator==(const Coord& rhs) const {
    if (this->c == rhs.c && this->r == rhs.c) {
        return true;
    } else {
        return false;
    }
}

// operator != comparison function
// COMPLETE
bool SparseMatrix::Coord::operator!=(const Coord& rhs) const { return !operator==(rhs); }

// ----------------------------------------------------------------------------
// ------------------ SparseItem struct Implementation ------------------------
// ----------------------------------------------------------------------------

// Constructor
// COMPLETE
SparseMatrix::SparseItem::SparseItem(Coord coord, double v) {
    this->val = v;
    this->coord = coord;
    this->prev[ROW] = this->prev[COL] = nullptr;
    this->next[ROW] = this->next[COL] = nullptr;
}

// ----------------------------------------------------------------------------
// ----------------- SparseMatrix class Implementation ------------------------
// ----------------------------------------------------------------------------

// Constructor
// To be completed - Must adhere to requirements in the .h
SparseMatrix::SparseMatrix(size_t n) : /* Add code as needed */ n_(n) {

    if (0U == n_) {
        throw std::invalid_argument("n must be greater than 0");
    }
    /* Add code as needed */

    SparseItem** rows = new SparseItem*[n];
    SparseItem** cols = new SparseItem*[n];
    //ROW init
    for(int i = 0; i < n; i++){
        SparseItem* headR = nullptr;
        rows[i] = headR;
    }

    //COL init
    for(int i = 0; i < n; i++){
        SparseItem* headC = nullptr;
        cols[i] = headC;
    }
    lists_[ROW] = rows;
    lists_[COL] = cols;
}

// Destructor
// To be completed
SparseMatrix::~SparseMatrix() {
    for(int i = 0; i<n_; i++){
        SparseItem* headR = lists_[ROW][i];
        while(headR!=nullptr){
            SparseItem* temp = headR;
            deleteNode(temp);
            headR = lists_[ROW][i];
        }
    }
    delete(lists_[ROW]);
    delete(lists_[COL]);

}

// COMPLETE
SparseMatrix::SparseItem* SparseMatrix::createSparseItem(const SparseMatrix::Coord& coord, double val) {
    SparseItem* ptr = new SparseItem(coord, val);
#ifdef SPARSE_DEBUG
    debug.addItem(ptr);
#endif
    return ptr;
}

// COMPLETE
SparseMatrix::SparseItem* SparseMatrix::lowerBound(size_t list, Coord target_coord) const {
    SparseItem* head = this->lists_[list][target_coord[list]];
    size_t target_index = target_coord[1 - list];

    if (head == nullptr) {
        return nullptr;
    } else if (head->coord[1 - list] > target_index) {
        return nullptr;
    } else {
        while (head->next[list] != nullptr && head->next[list]->coord[1 - list] <= target_index) {
            head = head->next[list];
        }
        return head;
    }
}

// COMPLETED
double SparseMatrix::get(const Coord& coord) const {
    if (coord[ROW] >= n_ || coord[COL] >= n_) {
        throw std::invalid_argument("Bad coordinates");
    }
    SparseItem* head = lowerBound(ROW, coord);
    if (nullptr == head || head->coord[COL] != coord[COL]) {
        return 0;
    }
    return head->val;
}

// COMPLETED
void SparseMatrix::print(std::ostream& os) const {
    os << setprecision(7);
    for (size_t r = 0; r < this->n_; r++) {
        SparseItem* ptr = this->lists_[ROW][r];
        size_t col = 0;
        while (ptr != NULL) {
            while (col < ptr->coord[COL]) {
                os << setw(8) << 0;
                col++;
            }
            os << setw(8) << ptr->val;
            col++;
            ptr = ptr->next[ROW];
        }
        while (col < this->n_) {
            os << setw(8) << 0;
            col++;
        }
        os << endl;
    }
}

// To be completed - Must adhere it requirements in the .h
void SparseMatrix::deleteNode(SparseItem* node) {
    if (nullptr == node) {
        throw std::invalid_argument("argument must not be null");
    }
    /* Add necessary code to update all other pointers */
    /* Note: calling delete is provided for you below  */
    SparseItem*& headR = lists_[ROW][node->coord[ROW]];
    SparseItem*& headC = lists_[COL][node->coord[COL]];

    if(node->prev[ROW] == nullptr){
        headR = node->next[ROW];
        if(node->next[ROW]!=nullptr){
            node->next[ROW]->prev[ROW] = nullptr;
        }
        
    }else{
        node->prev[ROW] = node->next[ROW];
        if(node->next[ROW]!=nullptr){
            node->next[ROW]->prev[ROW] = node->prev[ROW];
        }
    }
    
    if(node->prev[COL] == nullptr){
        headC = node->next[COL];
        if(node->next[COL]!=nullptr){
            node->next[COL]->prev[COL] = nullptr;
        }
        
    }else{
        node->prev[COL] = node->next[COL];
        if(node->next[COL]!=nullptr){
            node->next[COL]->prev[COL] = node->prev[COL];
        }
    }
    //delete(node);
    /* This code should not be altered and should end this function */
#ifdef SPARSE_DEBUG
    debug.deleteItem(node);
#endif
    delete node;
}

// To be completed - Must adhere it requirements in the .h
void SparseMatrix::set(const Coord& coord, double val) {
    SparseItem*& headR = lists_[ROW][coord[ROW]];
    SparseItem*& headC = lists_[COL][coord[COL]];

    SparseItem* p = headR;
    SparseItem* dR = new SparseItem({0,0}, -1);
    dR->next[ROW] = headR;
    SparseItem* pPrev = dR;
    bool flag = false;


    // SparseItem* q = headC->next[COL];
    // SparseItem* qPrev = headC;
    SparseItem* newNode = new SparseItem(coord, val);
    //SparseItem* targetItem = lowerBound(ROW, coord);

    //ROW Operations
    //insert when empty
    if(p == nullptr){
        headR = newNode;

    }else{

        while(p!=nullptr && p->coord[COL]<coord[COL]){
            p = p->next[ROW];
            pPrev = pPrev->next[ROW];
        }

        if(p == nullptr){
            //insert at end
            pPrev->next[ROW] = newNode;
            newNode->prev[ROW] = pPrev;
        }else{
            if(p->coord[COL]>coord[COL]){
                if(p == headR){
                    newNode->next[ROW] = p;
                    p->prev[ROW] = newNode;
                    headR = newNode;
                }else{
                    pPrev->next[ROW] = newNode;
                    newNode->prev[ROW] = pPrev;
                    newNode->next[ROW] = p;
                    p->prev[ROW] = newNode;
                }
            }else{
                flag = true;
            }
                
        }
    }

    //COL operations

    SparseItem* q = headC;
    SparseItem* dC = new SparseItem({0,0}, -1);
    dC->next[COL] = headC;
    SparseItem* qPrev = dC;

    if(q == nullptr){
        headC = newNode;
    }else{
        while(q!=nullptr && q->coord[ROW]<coord[ROW]){
            q = q->next[COL];
            qPrev = qPrev->next[COL];
        }

        if(q == nullptr){
            //insert at end
            qPrev->next[COL] = newNode;
            newNode->prev[COL] = qPrev;

        }else{
            if(q->coord[ROW]>coord[ROW]){
                if(q == headC){
                    
                    newNode->next[COL] = q;
                    q->prev[COL] = newNode;
                    headC = newNode;
                }else{
                    qPrev->next[COL] = newNode;
                    newNode->prev[COL] = qPrev;
                    newNode->next[COL] = q;
                    q->prev[COL] = newNode;
                }
            }else{
                if(flag){
                    p->val = val;
                }
            }
                
        }
    }
    delete(dR);
    delete(dC);

/* Leave these as the last lines of this function */
#ifdef SPARSE_DEBUG
    try {
        debug.checkConsistency(this);
    } catch (std::logic_error& e) {
        cerr << e.what();
    }
#endif
}

// To be completed - Must adhere it requirements in the .h
//   Be sure to meet the run-time requirements
double SparseMatrix::sumDim(const Coord& coord) const {
    size_t r = coord[ROW];
    size_t c = coord[COL];
    double sum = 0;
    
    if(r==npos && c==npos){
        throw std::invalid_argument("Invalid argument");
    }else if(r==npos||c==npos){
        if(r == npos){
            if(c>=n_){
                throw std::out_of_range("out of range");
            }else{
                SparseItem*& headC = lists_[COL][coord[COL]];
                SparseItem* p = headC;
                while(p!=nullptr && p->coord[ROW]<=coord[ROW]){
                    sum+=p->val;
                    p=p->next[ROW];
                }
            }
        }else{
            if(r>=n_){
                throw std::out_of_range("out of range");
            }else{
                SparseItem*& headR = lists_[ROW][coord[ROW]];
                SparseItem* q = headR;
                while(q!=nullptr && q->coord[COL]<=coord[COL]){
                    sum+=q->val;
                    q=q->next[COL];
                }
            }
        }
    }else{
        throw std::invalid_argument("Invalid argument");
    }

    return sum;
}

// To be completed - Must adhere it requirements in the .h
//   Be sure to meet the run-time requirements
void SparseMatrix::copyDim(const Coord& srcCoord, const Coord& dstCoord) {
    // Ignore self-copy - leave these as the first lines of the function
    if (dstCoord == srcCoord) {
        return;
    }

    
    
    if((srcCoord[ROW] == npos && srcCoord[COL] == npos)||(dstCoord[ROW] == npos && dstCoord[COL] == npos)){
        throw std::invalid_argument("Invalid argument");
    }else if((srcCoord[ROW] != npos && srcCoord[COL] != npos)||(dstCoord[ROW] != npos && dstCoord[COL] != npos)){        
        throw std::invalid_argument("Invalid argument");
    }else if(srcCoord[ROW] != npos){
        
        SparseItem* headsrcR = lists_[ROW][srcCoord[ROW]];
        if(dstCoord[ROW]!=npos){
            //delete dest nodes
            SparseItem* headdstR = lists_[ROW][dstCoord[ROW]];
            while(headdstR!=nullptr){
                SparseItem* temp = headdstR;
                deleteNode(temp);
                headdstR = lists_[ROW][dstCoord[ROW]];
            }
            //set dest nodes to src nodes
            while(headsrcR!=nullptr){
                set({dstCoord[ROW],headsrcR->coord[COL]}, headsrcR->val);
                headsrcR = headsrcR->next[ROW];
            }
        }else{
 
            // //copy src row
            // int i = 0;
            // SparseItem* copy[dstCoord[COL]];
            // SparseItem* temp = headsrcR;
            // while(headsrcR !=nullptr){
            //     copy[i] = temp;
            //     temp = temp->next[ROW];
            // }
            //delete dest nodes
            SparseItem* headdstC = lists_[COL][dstCoord[COL]];
            
            while(headdstC!=nullptr){
                SparseItem* temp1 = headdstC;
                deleteNode(temp1);
                headdstC = lists_[COL][dstCoord[COL]];
            }
            //set dest nodes to src nodes
            // while(headsrcR!=nullptr){
            //     set({headsrcR->coord[ROW],dstCoord[COL]}, headsrcR->val);
            //     headsrcR = headsrcR->next[ROW];
            // }
        }

    }

    // Add code to check for exception cases and, if valid, perform the copy
    // of the indicated dimension

/* Leave these as the last lines of this function */
#ifdef SPARSE_DEBUG
    try {
        debug.checkConsistency(this);
    } catch (std::logic_error& e) {
        cerr << e.what();
    }
#endif
}
