#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
// for pair
#include <functional>
#include <string>

using namespace std;

// typedef for X,Y pair
typedef std::pair<size_t, size_t> XYPair;

// Utility Prototypes
bool operator==(const XYPair& p1, const XYPair& p2);
void printPaths(const std::vector<std::vector<XYPair> >& results);

// Utility function to compare XY points
bool operator==(const XYPair& p1, const XYPair& p2)
{
  return p1.first == p2.first && p1.second == p2.second;
}


// Primary street grid function prototype
std::vector<std::vector<XYPair> > gridpaths(const XYPair& inter, const XYPair& final);

// Prototype any helper functions here
void gridpathshelper(
    const XYPair& start,  
    const XYPair& final,
    std::vector<std::vector<XYPair> > &result,
    std::vector<XYPair> &now){
    if(start.first > final.first || start.second > final.second)
        return;
    else if(start == final){
        result.push_back(now);
        return;
    }
    now.push_back({start.first+1, start.second});
    gridpathshelper({start.first+1, start.second}, final, result, now);
    now.pop_back();
    now.push_back({start.first, start.second+1});
    gridpathshelper({start.first, start.second+1}, final, result, now);
    now.pop_back();

}

// Implement your gridpaths and any helper functions below
std::vector<std::vector<XYPair> > gridpaths(
    const XYPair& inter, 
    const XYPair& final)
{

    std::vector<XYPair> temp;
    temp.push_back({0, 0});
    std::vector<std::vector<XYPair> > a;
    std::vector<std::vector<XYPair> > b;
    std::vector<std::vector<XYPair> > c;
    if(final == XYPair(0,0)) return c;
    gridpathshelper({0,0}, inter, a, temp);
    temp.pop_back();
    gridpathshelper(inter, final, b, temp);
    for(vector<std::vector<XYPair> >::iterator ita=a.begin(); ita != a.end(); ++ita)
        for(vector<std::vector<XYPair> >::iterator itb=b.begin(); itb != b.end(); ++itb){
            std::vector<XYPair> res;
            res.insert(res.end(),ita->begin(),ita->end());
            res.insert(res.end(),itb->begin(),itb->end());
            c.push_back(res);
        }
    return c;
}




// Complete - but can be changed for debugging purposes
int main(int argc, char* argv[])
{
  // Default to intermediate point of 2,3 and final location of 3,4
  size_t ix = 2, iy = 3, fx = 3, fy = 4;
  if(argc >= 5) {
    ix = atoi(argv[1]);
    iy = atoi(argv[2]);
    fx = atoi(argv[3]);
    fy = atoi(argv[4]);
  }
  vector<vector<XYPair> > results;
  results = gridpaths({ix,iy},{fx,fy});  
  printPaths(results);

  return 0;
}

// Prints the results in a clean fashion for human consumption / debugging
void printPaths(const vector<vector<XYPair> >& results)
{
  for( const auto& path : results) {
    for( size_t i = 0; i < path.size()-1; i++ ) 
    {      
      cout << path[i].first << "," << path[i].second << " -> ";
    }
    cout << path.back().first << "," << path.back().second << endl;
  }
  cout << results.size() << " solutions." << endl;

}
