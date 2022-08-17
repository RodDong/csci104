#include "rem_dup_lib.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void deleteList(Item* head);
// helper function for debugging
void outputList(ostream& os, Item* head);

// Add implementation of a readlists function such as:
void readLists(const char* filename, Item*& head1, Item*& head2) {
  ifstream file(filename);
  if (!file.is_open()) {
    cout << "Could not open " << filename << endl;
  }
  string nums;
  int num;
  getline(file, nums);
  istringstream ss1 (nums);
  
  ss1 >> num;
  Item* temp1 = new Item(num, nullptr);
  head1 = temp1;
  while(ss1 >> num){
    temp1->next = new Item(num, nullptr);
    temp1 = temp1->next;
  }
  
  getline(file, nums);
  istringstream ss2 (nums);
  ss2 >> num;
  Item* temp2 = new Item(num, nullptr);
  head2 = temp2;
  while(ss2 >> num){
    temp2->next = new Item(num, nullptr);
    temp2 = temp2->next;
  }

}
// or
// void readLists(char* filename, Item** head1, Item** head2);

// Output function for debug or writing output
void outputList(ostream& os, Item* head) {
    for (Item* temp = head; temp; temp = temp->next) {
        os << temp->val << " ";
    }
    os << endl;
}

// Deallocate memory for a list
void deleteList(Item* head) {
    if (head == nullptr)
        return;
    else {
        deleteList(head->next);
        delete head;
    }
}

// Main test driver
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: ./remdup input_file output_file" << endl;
        return 1;
    }
    Item *head1, *head2;
    readLists(argv[1], head1, head2);

    // Uncomment to verify you read in the right values
    outputList(cout, head1);
    outputList(cout, head2);

    ofstream ofile(argv[2]);
    if (ofile.fail()) {
        cerr << "Couldn't open output file " << argv[2] << endl;
    }

    removeConsecutive(head1);

    // Uncomment to see intermediate results after removeConsecutive
    outputList(ofile, head1);

    Item* head3 = concatenate(head1, head2);
    outputList(ofile, head3);

    ofile.close();

    //cleanup memory
    // deleteList(head1);
    // deleteList(head2);
    deleteList(head3);
    return 0;
}
