
#include "text_visualizer.h"
#include "unit.h"
#include "mygraph.h"
#include <set>
#include <list>
#include <algorithm>
// Ein Graph, der Koordinaten von Knoten speichert.

#define FILE_OPEN_ERROR 4
#define INITIALIZING_ERROR 5
#define NOTVISITED 10000000
#define FOURTH_CASE 6
using std::set;
using std::vector;
using std::map;
using std::pair;
using std::for_each;
using std::sort;
using std::make_heap;
using std::list;
typedef set<VertexT> setVertex;
typedef map<VertexT,CostT> mapVertexCost;
typedef pair<VertexT,CostT> pairVC;
// typedef neighbourVector::iterator neighborVecIter; global-scope qualifier (leading '::') is not allowed

// the pair used for saving g and h in the f = g + h for a star algo
typedef pair<CostT,CostT> CostTgh;
// the vertex and the corresponding (g,h) value
typedef pair<VertexT,CostTgh> VertexT_f; 
// map of two vertices
typedef map<VertexT, VertexT> map2V;
// vectors of VertexT_f
typedef vector<VertexT_f> v_VertexTf;

struct CompareVertexTf
{
    bool operator()(const VertexT_f & v1, const VertexT_f & v2) const
    {
        CostT d1 = v1.second.first + v1.second.second;
        CostT d2 = v2.second.first + v2.second.second;
        return d1 < d2;
    }
};

// Help functions for a star algo
// creating the path from the start to the destination, the help func for a star algo
void traceback(VertexT start, VertexT destination, std::list<VertexT>& weg,map2V & predecessors)
{
    if(!weg.empty())
    {
        cout << "weg already has some elements in traceback!\n";
        exit(INITIALIZING_ERROR);
    }
    // shall we push the start and destination?

    VertexT currentVertex = destination;
    while(currentVertex != start)
    {
        weg.push_front(predecessors[currentVertex]);
        currentVertex = predecessors[currentVertex];
    }
    
}

// a more complex version
bool traceback2(const DistanceGraph& g, VertexT start, VertexT destination, std::list<VertexT>& weg,map2V & predecessors)
{
    if(!weg.empty())
    {
        cout << "weg already has some elements in traceback!\n";
        exit(INITIALIZING_ERROR);
    }
    // shall we push the start and destination?

    size_t numOfVert = g.numVertices();
    size_t count = 0;
    VertexT currentVertex = destination;
    while(currentVertex != start && count < numOfVert)
    {
        weg.push_front(predecessors[currentVertex]);
        currentVertex = predecessors[currentVertex];
        count++;
    }
    if(count < numOfVert)
    {
        return true;
    }
    else
    {
        cout << "something wrong with the graph or predecessors map\n";
        return false;
    }
    
}

// Help func for a star algo
// std::vector<pair<VertexT,CostT isIn(neighbourVector::iterator & iter, v_VertexTf & vec)
VertexT_f * isIn(VertexT & vertex, v_VertexTf & vec)
{
    v_VertexTf::iterator viter;
    for(viter = vec.begin(); viter != vec.end(); viter++)
    {
        if((viter -> first) == vertex)
        {
            return &(*viter);
        }
    }
    return nullptr;
}

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
void outputVertex(VertexT v){cout.width(4); cout << v;}

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

// bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT destination, std::list<VertexT>& weg) 
bool A_star(const DistanceGraph& g, VertexT start, VertexT destination, std::list<VertexT>& weg)
{
    // ...
    // trivial case
    if(start == destination)
    {
        cout << "the start and destination coincide\n";
        return true;
    }
    v_VertexTf openlist;
    v_VertexTf closelist;
    map2V predecessor;
    size_t numOfVertices = g.numVertices();
    for(size_t i = 0; i < numOfVertices; i++)
    {
        // initializing the elements of predecessors to -1
        predecessor[i] = NOTVISITED;
    }
    openlist.push_back(VertexT_f(start,CostTgh(0,g.estimatedCost(start,destination))));
    while(!openlist.empty())
    {
        VertexT_f firstVTf(*openlist.begin());
        if(firstVTf.first == destination)
        {
            traceback(start,destination,weg,predecessor);
            return true;
        }
        // get all the successors of firstVTf.first
        VertexT bestVertex = firstVTf.first;
        neighbourVector neV = g.getNeighbors(bestVertex);
        // three cases
        neighbourVector::iterator iter;
        for(iter = neV.begin(); iter != neV.end(); iter++)
        {
            VertexT currentNode = iter -> first;
            VertexT_f * p_VertexTf = isIn(currentNode,openlist);
            CostT distToNode = firstVTf.second.first + (iter -> second);
            // first case if the node not ever visited
            if(!p_VertexTf && predecessor[currentNode] == NOTVISITED)
            {
                CostT estiCost = g.estimatedCost(currentNode,destination);
                openlist.push_back(VertexT_f(currentNode,CostTgh(distToNode,estiCost)));
                predecessor[currentNode] = bestVertex;
            }
            else if(p_VertexTf)
            {
                CostT distOld = (*p_VertexTf).second.first;
                if(distToNode < distOld)
                {
                    // the value from start to currentNode need to be modified
                    (*p_VertexTf).second.first = distToNode;
                    predecessor[currentNode] = bestVertex;
                }
            }
            else if(p_VertexTf == isIn(currentNode,closelist))
            {
                // node in the closelist check for possible updates
                CostT distOld = (*p_VertexTf).second.first;
                if(distToNode < distOld)
                {
                    (*p_VertexTf).second.first = distToNode;
                    VertexT_f vTfNew(*p_VertexTf);
                    // a little bit stupid code
                    v_VertexTf::iterator iter;
                    for(iter = closelist.begin(); iter != closelist.end(); iter++)
                    {
                        if((*iter).first == (*p_VertexTf).first)
                        closelist.erase(iter);
                    }
                    
                    openlist.push_back(vTfNew);
                    predecessor[currentNode] = bestVertex;
                }
            }
            else
            {
                // an error
                cout << "The fourth case in A_star algo appears\n";
                exit(FOURTH_CASE);
            }
        }
        // all the neighbours coverd delete the vertex from the openlist and push it in the closelist
        closelist.push_back(firstVTf);
        openlist.erase(openlist.begin());

        // sort the openlist
        make_heap(openlist.begin(),openlist.end(),CompareVertexTf());
    }
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
    inFile.open("daten/Graph2.dat");
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

    // cout << "Testing dijkstra\n";
    // vector<CostT> vecCost(coorG1.numVertices(),0);
    
    
    // for(VertexT ve = 0 ; ve < coorG1.numVertices(); ve++)
    // {
        
    //     cout << "\nStarting at " << ve << endl;
    //     Dijkstra(coorG1,ve,vecCost);
    //     cout << "\nafter dijkstra\n";
    //     for_each(vecCost.begin(),vecCost.end(),outputCost);
    //     PruefeDijkstra(3,ve,vecCost);
    // }

    // map<int,int> maptest;
    // maptest[1] = 2;
    // cout << maptest[1] << endl;
    // cout << maptest[0] << endl;

    // testing a star algo
    list<VertexT> weg;
    A_star(coorG1,1,7,weg);
    cout << "after a_star\n";
    for_each(weg.begin(),weg.end(),outputVertex);
    inFile.close();
    return 0;
}
 
