
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
typedef pair<VertexT,VertexT> VertexTwilling;
struct ComparePair{
    bool operator()(const pairVC& p1, const pairVC& p2){
        return p1.second < p2.second;
    }
};
typedef set<pairVC,ComparePair()> setVC;

typedef DistanceGraph::NeighborT neighbourVector;
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
        // minimum heap
        // if d1 < d2 then the largest element will be at front i.e maximum heap
        return d1 > d2; 
    }
};

// Help functions for a star algo
// creating the path from the start to the destination, the help func for a star algo
void traceback(VertexT start, VertexT destination, std::list<VertexT>& weg,map2V & predecessors);

// a more complex version
bool traceback2(const DistanceGraph& g, VertexT start, VertexT destination, std::list<VertexT>& weg,map2V & predecessors);

// Help func for a star algo
// VertexT_f * isIn(VertexT & vertex, v_VertexTf & vec)
v_VertexTf::iterator isIn(neighbourVector::iterator & iter, v_VertexTf & vec);
// void Dijkstra(const DistanceGraph& g, GraphVisualizer& v, VertexT start, std::vector<CostT>& kostenZumStart) {
//     // ...
// }


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
void outputVertex(VertexT v){cout.width(8); cout << v;}
void outputVertexTwilling(VertexTwilling v){cout << "first: " << v.first<< endl; cout << "second: " << v.second << endl; }
void outputVertexTf(VertexT_f v){cout.width(4); cout << v.first << " : " << v.second.first << "\t" << v.second.second<<endl;}
void Dijkstra(const DistanceGraph& g, VertexT start, std::vector<CostT>& kostenZumStart);

// bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT destination, std::list<VertexT>& weg) 
bool A_star(const DistanceGraph& g, VertexT start, VertexT destination, std::list<VertexT>& weg);

int main()
{
    // Frage Beispielnummer vom User ab
    
    // Lade die zugehoerige Textdatei in einen Graphen
    // PruefeHeuristik
    
    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg
    
    //testing read in files
    ifstream inFile;
    inFile.open("daten/Graph4.dat");
    if(!inFile.is_open())
    {
        cout << "could not find the given files\n";
        exit(FILE_OPEN_ERROR);
    }
    TimeCoordGraph coorG1;
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
    
    for(VertexT v1 = 0; v1 < coorG1.numVertices(); v1++)
    {
        for(VertexT v2 = 0; v2 < coorG1.numVertices(); v2++)
        {
            list<VertexT> weg;
            A_star(coorG1,v1,v2,weg);
            // cout << "after a_star starting : " << v1 << "\t end: " << v2 << endl;
            // for_each(weg.begin(),weg.end(),outputVertex);
            PruefeWeg(4,weg);
        }
    }
    cout << "Test heuristik\n";
    PruefeHeuristik(coorG1);
    inFile.close();
    return 0;
}
 
void traceback(VertexT start, VertexT destination, std::list<VertexT>& weg,map2V & predecessors)
{
    if(!weg.empty())
    {
        cout << "weg already has some elements in traceback!\n";
        exit(INITIALIZING_ERROR);
    }
    // shall we push the start and destination?

    VertexT currentVertex = destination;
    weg.push_front(destination);
    while(currentVertex != start)
    {
        weg.push_front(predecessors[currentVertex]);
        currentVertex = predecessors[currentVertex];
    }
    
}

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

v_VertexTf::iterator isIn(neighbourVector::iterator & iter, v_VertexTf & vec)
{
    // v_VertexTf::iterator viter;
    // for(viter = vec.begin(); viter != vec.end(); viter++)
    // {
    //     if((viter -> first) == vertex)
    //     {
    //         return &(*viter);
    //     }
    // }
    v_VertexTf::iterator viter;
    for(viter = vec.begin(); viter != vec.end(); viter++)
    {
        if((viter -> first) == (iter -> first))
        {
            break;
        }
    }
    return viter;
    // return nullptr;
}

void Dijkstra(const DistanceGraph& g, VertexT start, std::vector<CostT>& kostenZumStart) 
{
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

bool A_star(const DistanceGraph& g, VertexT start, VertexT destination, std::list<VertexT>& weg)
{
    // ...
    // trivial case
    if(start == destination)
    {
        cout << "the start and destination coincide\n";
        // according to PruefeWeg weg containts starting point
        weg.push_front(start);
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
    predecessor[start] = start;
    openlist.push_back(VertexT_f(start,CostTgh(0,g.estimatedCost(start,destination))));


    // cout << "Initializing openlist\n";
    // for_each(openlist.begin(),openlist.end(),outputVertexTf);


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

        // cout << "The bestVertex: " << bestVertex << endl;
        // cout << "The neighbours\n";
        // for_each(neV.begin(),neV.end(),outputVC);
        for(iter = neV.begin(); iter != neV.end(); iter++)
        {
            VertexT currentNode = iter -> first;
            // VertexT_f * p_VertexTf = isIn(currentNode,openlist);
            v_VertexTf::iterator p_VertexTf = isIn(iter,openlist);
            CostT distToNode = firstVTf.second.first + (iter -> second);


            // cout << "The currentNode: " << currentNode << endl;
            // cout << "The distToNode: " << distToNode << endl;


            // first case if the node not ever visited
            if((p_VertexTf == openlist.end()) && predecessor[currentNode] == NOTVISITED)
            {
                // cout << "In the first case\n";
                CostT estiCost = g.estimatedCost(currentNode,destination);
                openlist.push_back(VertexT_f(currentNode,CostTgh(distToNode,estiCost)));
                predecessor[currentNode] = bestVertex;

                // cout << "The predecessor maxtirx\n";
                // for_each(predecessor.begin(),predecessor.end(),outputVertexTwilling);
            }
            else if((p_VertexTf != openlist.end()))
            {
                // cout << "The second case\n";
                CostT distOld = (*p_VertexTf).second.first;
                if(distToNode < distOld)
                {
                    // the value from start to currentNode need to be modified
                    (*p_VertexTf).second.first = distToNode;
                    predecessor[currentNode] = bestVertex;
                }
            }
            else 
            {
                // node in the closelist check for possible updates
                p_VertexTf = isIn(iter,closelist);
                if( p_VertexTf != closelist.end())
                {
                    CostT distOld = (*p_VertexTf).second.first;
                    if(distToNode < distOld)
                    {
                        (*p_VertexTf).second.first = distToNode;
                        VertexT_f vTfNew(*p_VertexTf);
                        // a little bit stupid code
                        // v_VertexTf::iterator iter;
                        // for(iter = closelist.begin(); iter != closelist.end(); iter++)
                        // {
                        //     if((*iter).first == (*p_VertexTf).first)
                        //     closelist.erase(iter);
                        // }
                        closelist.erase(p_VertexTf);
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
        }
        // all the neighbours coverd delete the vertex from the openlist and push it in the closelist
        // cout << "end of the for loop\n";
        // cout << "the closelist\n";

        closelist.push_back(firstVTf);

        // for_each(closelist.begin(),closelist.end(),outputVertexTf);
        openlist.erase(openlist.begin());
        // cout << "The openlist\n";
        // for_each(openlist.begin(),openlist.end(),outputVertexTf);

        // sort the openlist
        make_heap(openlist.begin(),openlist.end(),CompareVertexTf());
        // cout << "After making heap\n";
        // for_each(openlist.begin(),openlist.end(),outputVertexTf);
    }
    // according to the PruefeWeg function weg has the point starting point
    weg.push_front(start);
    return false; // Kein Weg gefunden.
}