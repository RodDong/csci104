#include <iostream>
#include <fstream>
#include <sstream>
#include "graphiso.h"


using namespace std;

int main(int argc, char* argv[])
{
	stringstream ss1(
		"a1 a2\n"
		"a2 a1 a3 a4 a5\n"
		"a3 a2\n" 
		"a4 a2\n"
		"a5 a2 a6\n" 
		"a6 a5\n" 
	);
	stringstream ss2(
		"b1 b2\n"
		"b2 b3 b1\n"
		"b3 b4 b5 b6 b2\n"
		"b4 b3\n"
		"b5 b3\n"
		"b6 b3\n"
	);
    Graph g1(ss1);
    Graph g2(ss2);

    VERTEX_ID_MAP_T mapping;
    bool exists = graphIso(g1, g2, mapping);
    if(exists){
        cout << "Isomorphism exists: " << endl;
        // Since we don't have a hash table iterator, we'll just get the 
        // list of vertices and iterate through it, using the hash table
        // to translate to the isomorphic vertex
        VERTEX_LIST_T g1verts = g1.vertices();
        for(const auto& g1v : g1verts){
            cout << "G1: " << g1v << " <=> G2: " << mapping[g1v]  << endl;
        }

    }
    else {
        cout << "No isomorphism exists" << endl;
    }
    return 0;
}