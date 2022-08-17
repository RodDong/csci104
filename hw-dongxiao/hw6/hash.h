#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        const HASH_INDEX_T NUM_W = 5;
        const HASH_INDEX_T NUM_SHORT = 6;
        const HASH_INDEX_T VALUE_NULL = '.';
        const HASH_INDEX_T _36[6] = {1, 36, 1296, 46656, 1679616, 60466176};
        

        HASH_INDEX_T w[5] = {0};
        HASH_INDEX_T res=0;
        std::string temp2(NUM_SHORT*NUM_W - k.size(), VALUE_NULL), temp = k;
        temp = temp2 + temp;
        // std::cout << temp << std::endl;
        for(HASH_INDEX_T w_c=0; w_c < NUM_W; ++w_c){
            HASH_INDEX_T base_loc = w_c * NUM_SHORT;
            for(HASH_INDEX_T s_c=0; s_c < NUM_SHORT; ++s_c){
                HASH_INDEX_T loc = base_loc + s_c;
                if(temp[loc] != VALUE_NULL) {
                    // std:: cout << temp[loc] << ' ' << letterDigitToNumber(temp[loc]) << ' ' << _36[NUM_SHORT - s_c - 1] << std::endl;
                    w[w_c] += letterDigitToNumber(temp[loc]) 
                                * _36[NUM_SHORT - s_c - 1];
                }
            }

            std::cout << "w[" << w_c << "] = " << w[w_c] << std::endl;
            res += rValues[w_c]*w[w_c];
        }
        return res;

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if(letter >= 'A' && letter <= 'Z')
            return letter - 'A';
        else if(letter >= 'a')
            return letter - 'a';
        return letter - '0' + 26;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
