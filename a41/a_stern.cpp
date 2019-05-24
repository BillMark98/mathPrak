
#include "text_visualizer.h"
#include "unit.h"
#include "mygraph.h"
#include <set>
#include <algorithm>
// Ein Graph, der Koordinaten von Knoten speichert.

#define FILE_OPEN_ERROR 4
using std::set;
using std::vector;
using std::map;
using std::pair;
using std::for_each;
using std::sort;
typedef set<VertexT> setVertex;
typedef map<VertexT,CostT> mapVertexCost;
typedef pair<VertexT,CostT> pairVC;

// void Dijkstra(const DistanceGraph& g, GraphVisualizer& v, VertexT start, std::vector<CostT>& kostenZumStart) {
//     // ...
// }
struct ComparePair{
    bool operator()(const pairVC& p1, const pairVC& p2){
        return p1.second < p2.second;
    }
};
typedef set<pairVC,ComparePair> setVC;

typedef DistanceGraph::NeighborT neighbourVector;
// class VertexCompare
// {
//     private:
//         const vector<CostT> & vectorcost;
//     public:
//         VertexCompare(vector<CostT> & vc) : vectorcost(vc){};
//         VertexCompare(const vector<CostT> &vc) : vectorcost(vc){};
//         bool operator() (const VertexT & v1, const VertexT & v2) const {
//             return vectorcost[v1] < vectorcost[v2];
//         }
// };

// output function
void outputVC(pairVC p1) {cout << p1.first << " : " << p1.second << endl;}
void outputCost(CostT c) {cout.width(8); cout << c;}


void Dijkstra(const DistanceGraph& g, VertexT start, std::vector<CostT>& kostenZumStart) {
    size_t vertex_number = g.numVertices();
    // setVertex S;
    // S.insert(start);
    // mapVertexCost mVC;
    // setVC sVC; // can't modify existing element so have to delete it and insert a new
    
    neighbourVector vectorVC; // using vector to store VertexT, CostT
    kostenZumStart[start] = 0;
    CostT dist;
    for(VertexT i = 0; i < vertex_number; i++)
    {
        if(i != start)
        {
            dist = g.cost(start,i);
            kostenZumStart[i] = dist;
            // sVC.insert(pairVC(i,dist));
            vectorVC.push_back(pairVC(i,dist));
        }
    }
    sort(vectorVC.begin(),vectorVC.end(),ComparePair());
    cout << "After initialization the vectorVC\n";
    for_each(vectorVC.begin(),vectorVC.end(),outputVC);
    cout << "\nThe vector kostenZumStart\n";
    for_each(kostenZumStart.begin(),kostenZumStart.end(),outputCost);
    while(!vectorVC.empty())
    {
        // choose the node in sVC s.t D[v] minimal
        sort(vectorVC.begin(),vectorVC.end(),ComparePair());

        // cout << "\nAfter sorting in while loop\n"; 
        // for_each(vectorVC.begin(),vectorVC.end(),outputVC);


        pairVC firstPair(*vectorVC.begin());
        neighbourVector neV = g.getNeighbors(firstPair.first);
        if(neV.empty())
        {
            // no need to check for updats just delete the first node
            vectorVC.erase(vectorVC.begin()); // may be slow to perform this opertion
            continue;
        }
        // cout << "The chosen node: " << firstPair.first << endl;
        // cout << "its neighbours: \n";
        // for_each(neV.begin(),neV.end(),outputVC);


        neighbourVector::const_iterator neV_iter;
        size_t sizeVec = vectorVC.size();
        CostT dmin = kostenZumStart[firstPair.first];
        for(neV_iter = neV.begin(); neV_iter != neV.end(); neV_iter++)
        {
            for(size_t i = 0; i < sizeVec; i++)
            {
                if(vectorVC[i].first == (neV_iter -> first))
                {
                    CostT distold = vectorVC[i].second;
                    CostT distnew = dmin + neV_iter -> second;
                    if(distnew < distold)
                    {
                        vectorVC[i].second = distnew;
                        kostenZumStart[vectorVC[i].first] = distnew;
                    }

                }
            }
        }

        // cout << "End of two for loop\n";
        // cout << "The kostenZumStart\n";
        // for_each(kostenZumStart.begin(),kostenZumStart.end(),outputCost);

        vectorVC.erase(vectorVC.begin()); // may be slow to perform this opertion
    }

}

bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
    // ...
    return false; // Kein Weg gefunden.
}

int main()
{
    // Frage Beispielnummer vom User ab
    
    // Lade die zugehoerige Textdatei in einen Graphen
    // PruefeHeuristik
    
    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg
    
    //testing read in files
    ifstream inFile;
    inFile.open("daten/Graph3.dat");
    if(!inFile.is_open())
    {
        cout << "could not find the given files\n";
        exit(FILE_OPEN_ERROR);
    }
    DistCoordGraph coorG1;
    inFile >> coorG1;
    
    if(inFile.eof())
    {
        cout << "end of file reached.\n";
    }
    else if(inFile.fail())
    {
        cout << "Input terminated by data mismatch.\n";
    }
    else
    {
        cout << "Input terminated by unkown reasons.\n";
    }

    cout << coorG1;
    // cout << "get cost from 0 to 1: " << coorG1.cost(0,1) << endl;
    // cout << "cost from 3 to 1:  " << coorG1.cost(3,1) << endl;
    // cout << "cost from 2 to 1:  " << coorG1.cost(2,1) << endl;

    // cout << "Estimated cost from 0 to 1: " << coorG1.estimatedCost(0,1) <<endl;
    // cout << "Estimated cost from 1 to 2: " << coorG1.estimatedCost(1,2) <<endl;
    // cout << "getting neighbour vector of 1\n";
    DistanceGraph::NeighborT neighbours = coorG1.getNeighbors(1);
    DistanceGraph::NeighborT::const_iterator iter;
    for(iter = neighbours.begin(); iter != neighbours.end(); iter++)
    {
        cout << "vertex: " << iter -> first << endl;
        cout << "distance: " << iter -> second << endl;
    }

    cout << "Testing dijkstra\n";
    vector<CostT> vecCost(coorG1.numVertices(),0);
    
    
    for(VertexT ve = 0 ; ve < coorG1.numVertices(); ve++)
    {
        
        cout << "\nStarting at " << ve << endl;
        Dijkstra(coorG1,ve,vecCost);
        cout << "\nafter dijkstra\n";
        for_each(vecCost.begin(),vecCost.end(),outputCost);
        PruefeDijkstra(3,ve,vecCost);
    }
    return 0;
}
 
