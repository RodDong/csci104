#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "avlbst.h"
using namespace std;

// Enum to represent Variable values and Clause values (results)
//  UNDEC = UNDECided, ZERO = 0/FALSE, ONE = 1/TRUE
enum TruthVal { UNDEC = -1, ZERO = 0, ONE = 1};
// Map from variable ID (always positive) to value
typedef AVLTree<int, TruthVal> VarValueMap;

// Variables (true or negated) that are part of a clause
typedef std::vector<int> Clause;
// All clauses in the CNF formula
typedef std::vector<Clause> ClauseList;

// Prototypes

/**
 * @brief Reads the CNF formula from a file
 * 
 * @param [in]  filename C string containing the filename
 * @param [out] numV Should be set to the number of variables the problem specified
 * @param [out] numC Should be set to the number of clauses the problem specified
 * @param [out] clauses List of clauses where each clause contains the integer list of literals
 * @return true if an error occurs
 * @return false if successful
 */
bool readCNFFile(char* filename,
                 size_t& numV,
                 size_t& numC,
                 ClauseList& clauses);

/**
 * @brief Main entry point of the sat solver
 * 
 * @param [in] v  Current variable ID to assign
 * @param [in] numV  Number of variables total
 * @param [in] varValues Map of variable IDs to TruthVal
 * @param [in] clauses List of clauses from the formula
 * @return true if a solution was found
 * @return false if a solution was not found
 */
bool satSolve(size_t v,
              size_t numV,
              VarValueMap& varValues,
              ClauseList& clauses);

// Feel free to add other prototypes below
//  (a function to evaluate the formula given a set of variable values would
//   likely be useful. It would return ONE=true if all the clauses were satisfied,
//   ZERO=false if any clause evaluated to false), and UNDEC if some clauses were
//   still UNDECIDED due to some variables not being set yet.  To help this task
//   we have provided a function evalClause() to evaluate a single clause.)



// To be completed
bool readCNFFile(char* filename,
                 size_t& numV,
                 size_t& numC,
                 ClauseList& clauses)
{
    // Add your code here
    ifstream ifile(filename);
    if(ifile.fail()) {
        cerr << "Couldn't open file" << endl;
        return 1;
    }
    string in_s;
    while(getline(ifile, in_s)){
        // cout << in_s << '\n' << (int)in_s[0] << endl;
        string a;
        stringstream ss(in_s);
        ss >> a;
        if(a[0]=='p'){
            ss >> a >> numV >> numC;
        }
        else if(a[0]=='-' || (a[0] > '0' && a[0] <= '9')){
            Clause temp;
            int x = stoi(a);
            while(x != 0){
                temp.push_back(x);
                ss >> x;
            }
            clauses.push_back(temp);
        }
    }
    return false;

}

// Complete - Debug routine
void printClauses(const ClauseList& clauses)
{
    // uses C++ 11 ranged loops
    for(const auto& myclause : clauses) {
        for(int varID : myclause) {
            cout << varID << " ";
        }
        cout << endl;
    }
}

// Complete - Evaluates a single clause
TruthVal evalClause(VarValueMap& varValues,
                    Clause& clause)
{
    bool atLeastOneUndecided = false;
    for( int varNum : clause)
    {
        // get value of indicated variable (using tertiary operator)
        TruthVal myVarValue = (varNum < 0) ? varValues[-varNum] : varValues[varNum];

        // check if this variable value makes the clause true
        if((varNum < 0 && myVarValue == ZERO) || (varNum > 0 && myVarValue == ONE)){
            // caluse is satisfied...no need to go further
            return ONE;
        }
        else if( myVarValue == UNDEC) {
            atLeastOneUndecided = true;
        }
    }
    // if clause is not true, we now check if it is undecided or false
    if(true == atLeastOneUndecided ) {
        return UNDEC;
    }
    // clause is false
    return ZERO;

}
bool evalAllClause(VarValueMap& varValues,
                   ClauseList& clauses)
{
    // uses C++ 11 ranged loops
    for(auto& myclause : clauses) 
        if(evalClause(varValues, myclause) != ONE) return false;
    return true;
}

// To be completed - Main recursive backtracking search function
bool satSolve(size_t v,
              size_t numV,
              VarValueMap& varValues,
              ClauseList& clauses)
{
    TruthVal options[2] = {ZERO, ONE};

    if(evalAllClause(varValues, clauses)) return true;
    if(v > numV) return false;

    varValues[v] = options[0];
    if(satSolve(v+1, numV, varValues, clauses)) return true;

    varValues[v] = options[1];
    if(satSolve(v+1, numV, varValues, clauses)) return true;
    
    return false;
}


// Complete (though you can add temporary debugging code)
int main(int argc, char* argv[])
{
    if(argc < 3) {
        cerr << "Please provide the cnf and output filename" << endl;
        return 1;
    }
    size_t numVars, numClauses;
    ClauseList clauses;

    bool status = readCNFFile(argv[1],  numVars, numClauses, clauses);
    if(status) {
        cerr << "Exiting..." << endl;
        return 1;
    }
#ifdef SATDEBUG
    printClauses(clauses);
#endif

    // Declare the variable value map to track our assignment of values
    VarValueMap varValues;
    for(size_t i=1; i <= numVars; i++) {
        varValues.insert(make_pair(i, UNDEC));
    }

    // Attempt to find a solution (using Davis-Putnam backtracking algorithm)
    status = satSolve(1, numVars, varValues, clauses);

    // Output Results
    ofstream ofile(argv[2]);
    if(ofile.fail()) {
        cerr << "Couldn't open file " << argv[2] << endl;
        return 1;
    }
    if(status) {
        for(size_t i=1; i <= numVars; i++) {
            if(varValues[i] != UNDEC) {
                ofile << i << " =  " << varValues[i] << endl;
            }
        }
    }
    else {
        ofile << "No solution" << endl;
    }
    ofile.close();
    return 0;
}
