#include <iostream>
#include <sstream>
#include <fstream>
#include "stack.h"

using namespace std;

int main(int argc, char* argv[])
{
  if(argc < 3){
    cerr << "Please provide the input and output filenames" << endl;
    return 1;
  }
  // Add your code here
  ifstream file(argv[1]);
  ofstream ofile(argv[2]);
  string nums; 
  int num;
  Stack<int>* stack = new Stack<int>();
  getline(file, nums);
  istringstream ss (nums);
  while(ss>>num){
    stack->push(num);
    if(num > 0){
      stack->pop();
      for(int i = 0; i<num; i++){
        if(stack->empty()){
          break;
        }
        if(stack->top() == -1){
          ofile << "gray" << " ";
        }else{
          ofile << "black" << " ";
        }
        stack->pop();
      }
      ofile << endl;
    }
  }
  for(int i = 0; i<stack->size(); i++){
    cout<<(*stack)[i];
  }

  return 0;
}
