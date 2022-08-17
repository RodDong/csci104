#include "ht.h"
#include "hash.h"
#include <unordered_map>
#include <iostream>
#include <utility>
#include <string>
#include <sstream>
#include <functional>
#include <set>
using namespace std;
int main()
{
    double loadfactor = 0.6;
    const vector<int> sizemap = 
    {
        11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421
    };
    HashTable<int, int, LinearProber, IntHash2, equal_to<int>> ht(loadfactor, LinearProber(), IntHash2());   
    set<pair<int, int> > items;
    for(size_t i = 0;i<sizemap.size()-1;i++){
        int min = (int)floor(sizemap[i]*loadfactor);
        for(int j = 0; j<=min/2;j++){
            pair<int,int> pair(j,j);
            //insert then delete
            ht.insert(pair);
            ht.remove(j);
        }
        //reinsert the same pair, loc should change
        for(int j = 0; j<(min-(min/2));j++){
            pair<int,int> pair(j,j);
            //insert again
            ht.insert(pair);
            items.insert(pair);
            EXPECT_EQ(ht.probe(j),(min/2)+1+j);
        }
    // if( ht.find("hi1") != nullptr ){
    //     cout << "Found hi1" << endl;
    //     ht["hi1"] += 1;
    //     cout << "Incremented hi1's value to: " << ht["hi1"] << endl;
    // }
    // if( ht.find("doesnotexist") == nullptr ){
    //     cout << "Did not find: doesnotexist" << endl;
    // }
    // cout << "HT size: " << ht.size() << endl;
    // ht.remove("hi7");
    // ht.remove("hi9");
    // cout << "HT size: " << ht.size() << endl;
    // if( ht.find("hi9") != nullptr ){
    //     cout << "Found hi9" << endl;
    // }
    // else {
    //     cout << "Did not find hi9" << endl;
    // }
    // ht.insert({"hi7",17});
    // cout << "size: " << ht.size() << endl;

    return 0;
}
