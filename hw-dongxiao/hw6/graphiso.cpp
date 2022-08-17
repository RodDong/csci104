#include <iostream>
#include <sstream>
#include "graphiso.h"

using namespace std;

// ================= Complete - Begin Graph class implementation ===================
Graph::Graph(std::istream& istr)
{
    string aline;
    while(getline(istr,aline))
    {
        istringstream iss(aline);
        string u, v;
        if(iss >> u){
            VERTEX_SET_T neighbors;
            while(iss >> v)
            {
                neighbors.insert(v);
            }
            adj_.insert(std::make_pair(u,neighbors));
        }
    }
}


bool Graph::edgeExists(const VERTEX_T& u, const VERTEX_T& v) const
{
    if((adj_.find(u) != adj_.end()) && (adj_.at(u).find(v) != adj_.at(u).end()))
    {
        return true;
    }
    return false;
}
const VERTEX_SET_T& Graph::neighbors(const VERTEX_T& v) const
{
    if(adj_.find(v) == adj_.end()){
        throw std::invalid_argument("Neighbors: invalid vertex - not in  map");
    }
    return adj_.at(v);
}
VERTEX_LIST_T Graph::vertices() const
{
    VERTEX_LIST_T verts;
    for(const auto& p : adj_)
    {
        verts.push_back(p.first);
    }
    return verts;
}
// ================= Complete - End Graph class implementation ===================

// Prototype and helper functions here

// To be completed

bool degIsConsistent(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping) {
    VERTEX_LIST_T g1verts = g1.vertices();
    for(const auto& g1u : g1verts)
    {
        // Check mappings for necessary vertices to see if there is any violation
        // and return false
        // deg
        if(mapping.find(g1u)!=nullptr && g1.neighbors(g1u).size() != g2.neighbors(mapping[g1u]).size()) return false;

    }
    return true;
}
bool isConsistent(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping)
{
    // Feel free to change or remove this starter code
    VERTEX_LIST_T g1verts = g1.vertices();
    for(const auto& g1u : g1verts)
    {
        // Check mappings for necessary vertices to see if there is any violation
        // and return false
        // edge
        for(const auto& g1n : g1.neighbors(g1u))
            if(mapping.find(g1n)!=nullptr && mapping.find(g1u)!=nullptr && !g2.edgeExists(mapping[g1u], mapping[g1n])) return false;
        
    }
    return true;
}

// Add any helper functions you deem useful

// To be completed
bool graphIso(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping)
{
    VERTEX_LIST_T g1verts = g1.vertices();
    if(g1verts.size() != g2.vertices().size())
    {
        return false;
    }
    else if(!degIsConsistent(g1, g2, mapping)) return false;
    else if(!isConsistent(g1, g2, mapping)) return false;
    // Add code here
    map< VERTEX_T, bool > temp;
    VERTEX_T visit;
    
    for(const auto& g1u : g1verts){
        if(mapping.find(g1u)==nullptr)
            visit = g1u;
        else
            temp[mapping[g1u]] = true;
    }

    if(temp.size() == g1verts.size()) {
        // for(const auto& g1u : g1verts){
        //     cout << g1u << ' ' << mapping[g1u] << endl;
        // }
        // cout << endl;
        return true;
        // return false;
    }
    for(const auto& g2u : g2.vertices()){
        if(temp.find(g2u) == temp.end()){
            mapping.insert({visit,g2u});
            if(graphIso(g1, g2, mapping)) return true;
            
            mapping.remove(visit);
        }
    }
    // Delete this and return the correct value...
    // This is just placeholder to allow compilation
    return false;
}

