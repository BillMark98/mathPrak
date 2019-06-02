
#include "text_visualizer.h"
#include "mygraphvisual.h"
#include "unit.h"
#include "mygraph.h"
#include <set>
#include <list>
#include <deque>
#include <algorithm>
// Ein Graph, der Koordinaten von Knoten speichert.

#define FILE_OPEN_ERROR 4
#define NOTVISITED 10000000
#define FOURTH_CASE 6
#define DRAW 1
using std::set;
using std::vector;
using std::map;
using std::pair;
using std::for_each;
using std::sort;
using std::make_heap;
using std::list;
using std::deque;
typedef set<VertexT> setVertex;
typedef map<VertexT,CostT> mapVertexCost;
typedef pair<VertexT,CostT> pairVC;
typedef pair<VertexT,VertexT> VertexTwilling;
struct ComparePair{
    bool operator()(const pairVC& p1, const pairVC& p2){
        return p1.second <= p2.second;
    }
};
typedef set<pairVC,ComparePair()> setVC;

typedef DistanceGraph::NeighborT neighbourVector;
// typedef neighbourVector::iterator neighborVecIter; global-scope qualifier (leading '::') is not allowed

// // the pair used for saving g and h in the f = g + h for a star algo
// typedef pair<CostT,CostT> CostTgh;
// // the vertex and the corresponding (g,h) value
// typedef pair<VertexT,CostTgh> VertexT_f; 
// map of two vertices
typedef map<VertexT, VertexT> map2V;
// vectors of VertexT_f
// typedef vector<VertexT_f> v_VertexTf;
typedef deque<VertexT_f> v_VertexTf;
struct CompareVertexTf
{
    bool operator()(const VertexT_f & v1, const VertexT_f & v2) const
    {
        CostT d1 = v1.second.first + v1.second.second;
        CostT d2 = v2.second.first + v2.second.second;
        // minimum heap
        // if d1 < d2 then the largest element will be at front i.e maximum heap
        cout << "The VertexT v1: " << v1.first << endl;
        cout << "The VertexT v2: " << v2.first << endl;
        cout << "v1s two values: " << v1.second.first << "\t" << v1.second.second << endl;
        cout << "v2s two values: " << v2.second.first << "\t" << v2.second.second << endl;
        // the problem of the maximums heap
        // is that we can only ensure that the largest of 
        // all elements are at the top
        // but not sure whether the smallest is 
        // at the end
        // because maximumsheap only ensures the father is
        // larger than his two sons,
        // but don't say anything about the relative order of 
        // the two sons
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

// Help func for outputting the weg for a-star algo on the maze
void outWegMaze(const MazeGraph& mz, std::list<VertexT>& weg);

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
void Dijkstra(const DistanceGraph& g, GraphVisualizer& v,VertexT start, std::vector<CostT>& kostenZumStart);

// bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT destination, std::list<VertexT>& weg) 
bool A_star(const DistanceGraph& g, GraphVisualizer& v,VertexT start, VertexT destination, std::list<VertexT>& weg);

int main()
{
    // Frage Beispielnummer vom User ab
    
    // Lade die zugehoerige Textdatei in einen Graphen
    // PruefeHeuristik
    
    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg
    
    //testing read in files
    ifstream inFile;
    // inFile.open("daten/Graph1.dat");
    int Bsp = 6;
    inFile.open("daten/Maze2.dat");
    if(!inFile.is_open())
    {
        cout << "could not find the given files\n";
        exit(FILE_OPEN_ERROR);
    }
    // using DistCoordGraph for 1,2
    // DistSphereGraph for 3
    // TimeCoordGraph for 4
    // MazeGraph for Maze 1 ~ 5
    // Bsp 5   Maze1
    // Bsp 6   Maze2
    // Bsp 7   Maze3
    // Bsp 8   Maze4
    // Bsp 9   Maze5
    MazeGraph coorG1;
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
    // a graph visualizer
    MazeVisualizer v(coorG1,800,600);
    // v.draw_raw();
    // cout << coorG1;

    // cout << "Test in main\n";
    // vector<CellType> vcell;
    // vcell.resize(3);
    // vcell[0] = CellType::Ground;
    // vcell[1] = CellType::Start;
    // vcell[2] = CellType::Wall;
    // for(int i = 0; i < 3; i++)
    // {
    //     if(vcell[i] == CellType::Ground)
    //     {
    //         cout << "Ground\n";
    //     }
    //     if(vcell[i] == CellType::Start)
    //     {
    //         cout << "Start\n";
    //     }
    //     if(vcell[i] == CellType::Wall)
    //     {
    //         cout << "Wall\n";
    //     }
    // }
    // cout << " Testing single celltype\n";
    // CellType test = CellType::Ground;
    // if(test == CellType::Ground)
    // {
    //     cout << "test: Ground\n";
    // }
    // if(test == CellType::Wall)
    // {
    //     cout << "test: Wall\n";
    // }
    // if(test == CellType::Start)
    // {
    //     cout << "test: Start\n";
    // }
    // cout << "Trying to change value\n";
    // test = CellType::Wall;
    // if(test == CellType::Ground)
    // {
    //     cout << "test: Ground\n";
    // }
    // if(test == CellType::Wall)
    // {
    //     cout << "test: Wall\n";
    // }
    // if(test == CellType::Start)
    // {
    //     cout << "test: Start\n";
    // }

    // cout << "get cost from 0 to 1: " << coorG1.cost(0,1) << endl;
    // cout << "cost from 3 to 1:  " << coorG1.cost(3,1) << endl;
    // cout << "cost from 2 to 1:  " << coorG1.cost(2,1) << endl;

    // cout << "Estimated cost from 0 to 1: " << coorG1.estimatedCost(0,1) <<endl;
    // cout << "Estimated cost from 1 to 2: " << coorG1.estimatedCost(1,2) <<endl;
    // cout << "getting neighbour vector of 1\n";
    // DistanceGraph::NeighborT neighbours = coorG1.getNeighbors(1);
    // DistanceGraph::NeighborT::const_iterator iter;
    // for(iter = neighbours.begin(); iter != neighbours.end(); iter++)
    // {
    //     cout << "vertex: " << iter -> first << endl;
    //     cout << "distance: " << iter -> second << endl;
    // }

    // testing a star for maze
    // list<VertexT> weg_maze;
    // VertexT start = coorG1.mzCoord2VertexT(2,2);
    // VertexT end = coorG1.mzCoord2VertexT(5,11);
    // A_star(coorG1,start,end,weg_maze);
    // cout << "finding path \n";
    // outWegMaze(coorG1,weg_maze);


// *************************************************************
// Testing Maze

    // Segmentation fault of the vector<CellType>?
    // because in the constructor
    // before calling setNeighbors, have to resize neighbour_vector
    // vector<CellType> labyrinth = ErzeugeLabyrinth(256,256,15);

    // // cout << "Vector labyrinth successfully created\n";
    // MazeGraph coorG3(256*256,labyrinth,256,256);
    // VertexT start = coorG3.getStart();
    // VertexT dest = coorG3.getDestination();
    // cout << "The start: " << start << " the desti: " << dest << endl;
    // list<VertexT> weg10;
    // A_star(coorG3,v,start,dest,weg10);
    // PruefeWeg(10,weg10);
    // Bsp 5   Maze1
    // Bsp 6   Maze2
    // Bsp 7   Maze3
    // Bsp 8   Maze4
    // Bsp 9   Maze5
    // for ( auto pair : StartZielPaare(10)) 
    // {
    //     auto start = pair.first;
    //     auto goal  = pair.second;
    //     cout << "The start: " << start << " the goal: " << goal << endl;
        
    //     list<VertexT> weg_maze;
    //     A_star(coorG1,v,start,goal,weg_maze);
    //     cout << "The path\n";
    //     outWegMaze(coorG1,weg_maze);
    //     PruefeWeg(10,weg_maze);
    // }


    // for visualizer test
    vector<VertexTwilling> v_stEnd = StartZielPaare(Bsp);
    VertexTwilling StEnd =  v_stEnd[0];
    VertexT start = StEnd.first;
    VertexT end = StEnd.second;
    cout << "The start: " << start << " the goal: " << end << endl;
    list<VertexT> weg_maze;
    v.setStartEnd(start,end);
    A_star(coorG1,v,start,end,weg_maze);
    cout << "The path\n";
    outWegMaze(coorG1,weg_maze);

// *************************************************************


// *************************************************************

// Testing Dijkstra

    // cout << "Testing dijkstra\n";
    // vector<CostT> vecCost(coorG1.numVertices(),0);
    // for(VertexT ve = 0 ; ve < coorG1.numVertices(); ve++)
    // {
        
    //     cout << "\nStarting at " << ve << endl;
    //     Dijkstra(coorG1,v,ve,vecCost);
    //     cout << "\nafter dijkstra\n";
    //     // for_each(vecCost.begin(),vecCost.end(),outputCost);
    //     PruefeDijkstra(4,ve,vecCost);
    // }


// ********************************************************************

    // map<int,int> maptest;
    // maptest[1] = 2;
    // cout << maptest[1] << endl;
    // cout << maptest[0] << endl;

// *************************************************************

// Testing a star algo
    
    // for(VertexT v1 = 0; v1 < coorG1.numVertices(); v1++)
    // {
    //     for(VertexT v2 = 0; v2 < coorG1.numVertices(); v2++)
    //     {
    //         list<VertexT> weg;
    //         A_star(coorG1,v,v1,v2,weg);
    //         // cout << "after a_star starting : " << v1 << "\t end: " << v2 << endl;
    //         // for_each(weg.begin(),weg.end(),outputVertex);
    //         PruefeWeg(Bsp,weg);
    //     }
    // }

// **********************************************************************
// Testing Heuristik
    // cout << "Test heuristik\n";
    // PruefeHeuristik(coorG1);

// **********************************************************************

    // size_t index = 2;
    // int test = -1;
    // size_t me = index - 1;
    // size_t you = index + test;
    // cout << "me: " << me << endl;
    // cout << "you: " << you << endl;

    // size_t in2 = 4;
    // cout << "2 - 4 in size_t: " << index - in2 << endl;
    // size_t res = index - in2;
    // cout << "res: " << res << endl;
    // size_t re2 = abs(index - in2);
    // cout << "re2: " << re2 << endl;
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

void Dijkstra(const DistanceGraph& g, GraphVisualizer& v,VertexT start, std::vector<CostT>& kostenZumStart) 
{
    size_t vertex_number = g.numVertices();
    // setVertex S;
    // S.insert(start);
    // mapVertexCost mVC;
    // setVC sVC; // can't modify existing element so have to delete it and insert a new
    
    neighbourVector vectorVC; // using vector to store VertexT, CostT
    kostenZumStart[start] = 0;
    CostT dist;
#ifdef DRAW
    v.markVertex(start,VertexStatus::Done);
#endif
    for(VertexT i = 0; i < vertex_number; i++)
    {
        if(i != start)
        {
            dist = g.cost(start,i);
            kostenZumStart[i] = dist;
#ifdef DRAW
            if(dist < infty)
            {
                // vertex is connected to start
                
                v.markVertex(i,VertexStatus::InQueue);
                v.markEdge(EdgeT(start,i),EdgeStatus::Visited); 
                
            }
            else
            {
                // still unknown

                v.markVertex(i,VertexStatus::UnknownVertex);
            }
#endif
            // sVC.insert(pairVC(i,dist));
            vectorVC.push_back(pairVC(i,dist));
        }
    }
#ifdef DRAW
    v.draw();
#endif
    sort(vectorVC.begin(),vectorVC.end(),ComparePair());
    // cout << "After initialization the vectorVC\n";
    // for_each(vectorVC.begin(),vectorVC.end(),outputVC);
    // cout << "\nThe vector kostenZumStart\n";
    // for_each(kostenZumStart.begin(),kostenZumStart.end(),outputCost);
    while(!vectorVC.empty())
    {
        // choose the node in sVC s.t D[v] minimal
        sort(vectorVC.begin(),vectorVC.end(),ComparePair());

        // cout << "\nAfter sorting in while loop\n"; 
        // for_each(vectorVC.begin(),vectorVC.end(),outputVC);


        pairVC firstPair(*vectorVC.begin());
#ifdef DRAW
        v.markVertex(firstPair.first,VertexStatus::Active);
#endif
        neighbourVector neV = g.getNeighbors(firstPair.first);
        if(neV.empty())
        {
            // no need to check for updats just delete the first node
            vectorVC.erase(vectorVC.begin()); // may be slow to perform this opertion
            // v.markVertex(firstPair.first,VertexStatus::Done);
            continue;
        }
        // cout << "The chosen node: " << firstPair.first << endl;
        // cout << "its neighbours: \n";
        // for_each(neV.begin(),neV.end(),outputVC);
#ifdef DRAW
        v.draw();
#endif
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
#ifdef DRAW
                    v.markVertex(vectorVC[i].first,VertexStatus::InQueue);
#endif
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
#ifdef DRAW
        v.markVertex(firstPair.first,VertexStatus::Done);
        v.draw();
#endif
    }

}

bool A_star(const DistanceGraph& g,GraphVisualizer& v, VertexT start, VertexT destination, std::list<VertexT>& weg)
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
    cout << "The start: " << start << endl;
    cout << "The desti: " << destination << endl;
    v_VertexTf openlist;
    v_VertexTf closelist;
    map2V predecessor;
    size_t numOfVertices = g.numVertices();
    for(size_t i = 0; i < numOfVertices; i++)
    {
        // initializing the elements of predecessors to -1
        predecessor[i] = NOTVISITED;
#ifdef DRAW
        v.markVertex(i,VertexStatus::UnknownVertex);
#endif
    }
    predecessor[start] = start;
    openlist.push_back(VertexT_f(start,CostTgh(0,g.estimatedCost(start,destination))));


    // cout << "Initializing openlist\n";
    // for_each(openlist.begin(),openlist.end(),outputVertexTf);


    while(!openlist.empty())
    {
        VertexT_f firstVTf(*openlist.begin());
        // VertexT_f firstVTf = openlist.back();
        // cout << "The firstVTf: " << firstVTf.first << endl;
#ifdef DRAW
        v.markVertex(firstVTf.first,VertexStatus::Active);
        v.draw();
#endif
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

            CostT estiCost = g.estimatedCost(currentNode,destination);
            // first case if the node not ever visited
            if((p_VertexTf == openlist.end()) && predecessor[currentNode] == NOTVISITED)
            {
                // cout << "In the first case\n";
                // CostT estiCost = g.estimatedCost(currentNode,destination);
                openlist.push_back(VertexT_f(currentNode,CostTgh(distToNode,estiCost)));
                predecessor[currentNode] = bestVertex;
#ifdef DRAW
                v.updateVertex(currentNode,distToNode,estiCost,bestVertex,VertexStatus::InQueue);
#endif
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
#ifdef DRAW
                    v.updateVertex(currentNode,distToNode,estiCost,bestVertex,VertexStatus::InQueue);
#endif
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
#ifdef DRAW
                        v.updateVertex(currentNode,distToNode,estiCost,bestVertex,VertexStatus::InQueue);
#endif
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
#ifdef DRAW
        v.markVertex(bestVertex,VertexStatus::Done);
#endif
        // cout << "\nThe closelist\n";
        // for_each(closelist.begin(),closelist.end(),outputVertexTf);
        // // openlist.erase(openlist.begin());
        // cout << "The openlist before pop\n";
        // for_each(openlist.begin(),openlist.end(),outputVertexTf);
        // // actually should first erase the begin
        // // the making heap
        

        // openlist.pop_back();
        openlist.erase(openlist.begin());
        // cout << "The openlist after erasing first\n";
        // for_each(openlist.begin(),openlist.end(),outputVertexTf);

        make_heap(openlist.begin(),openlist.end(),CompareVertexTf());
        // cout << "After making heap\n";
        // for_each(openlist.begin(),openlist.end(),outputVertexTf);
#ifdef DRAW        
        v.draw();
#endif
        // sort the openlist
        
    }
    // according to the PruefeWeg function weg has the point starting point
    weg.push_front(start);
    return false; // Kein Weg gefunden.
}

void outWegMaze(const MazeGraph& mz, std::list<VertexT>& weg)
{

    list<VertexT>::const_iterator iter;
    int count = 1;
    for(iter = weg.begin(); iter!= weg.end(); iter++)
    {
        mz.VertexInMzCoord((*iter),cout);
        if(count % 7 == 0)
        {
            cout << endl;
        }
        count++;
    }
    cout << endl;
}