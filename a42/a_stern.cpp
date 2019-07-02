#include "text_visualizer.h"
#include "mygraphvisual.h"
#include "unit.h"
#include "mygraph.h"
#include <set>
#include <list>
#include <algorithm>
#include <deque>
// Ein Graph, der Koordinaten von Knoten speichert.

#define FILE_OPEN_ERROR 4
#define NOTVISITED 10000000
#define FOURTH_CASE 6
#define INVALID_INPUT 11
#define DRAW 1
using std::set;
using std::vector;
using std::map;
using std::pair;
using std::for_each;
using std::sort;
using std::make_heap;
using std::list;
using std::string;
using std::deque;

typedef pair<VertexT,CostT> pairVC;
typedef pair<VertexT,VertexT> VertexTwilling;
struct ComparePair{
    bool operator()(const pairVC& p1, const pairVC& p2){
        return p1.second > p2.second;
    }
};

typedef DistanceGraph::NeighborT neighbourVector;

// the pair used for saving g and h in the f = g + h for a star algo
typedef pair<CostT,CostT> CostTgh;
// the vertex and the corresponding (g,h) value
typedef pair<VertexT,CostTgh> VertexT_f; 
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


// output function
void outputVC(pairVC p1) {cout << p1.first << " : " << p1.second << endl;}
void outputCost(CostT c) {cout.width(8); cout << c;}
void outputVertex(VertexT v){cout.width(8); cout << v;}
void outputVertexTwilling(VertexTwilling v){cout << "first: " << v.first<< endl; cout << "second: " << v.second << endl; }
void outputVertexTf(VertexT_f v){cout.width(4); cout << v.first << " : " << v.second.first << "\t" << v.second.second<<endl;}
void Dijkstra(const DistanceGraph& g, GraphVisualizer& v,VertexT start, std::vector<CostT>& kostenZumStart);

// bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT destination, std::list<VertexT>& weg) 
bool A_star(const DistanceGraph& g, GraphVisualizer& v,VertexT start, VertexT destination, std::list<VertexT>& weg);
bool InputFile(ifstream & inFile, string name);
void TestDijkstra(DistanceGraph & g, GraphVisualizer& v, int Bsp);
void TestAstar(CoordinateGraph & g, int Bsp,unsigned int width = 1500,unsigned int height = 1000,int Mute = 0,int Tests = 0);
void TestAstarMaze(MazeGraph & g,  int Bsp,unsigned int width = 1500,unsigned int height = 1000,int Mute = 0,int Tests = 0);
int main()
{
    // Frage Beispielnummer vom User ab
    cout << "Please enter a number (1 ~ 10)" << endl;
    int input;
    cin >> input;
    cout << "The width of the window:";
    unsigned int width;
    cin >> width;
    cout << "The height of the window:";
    unsigned int height;
    cin >> height;
    cout << "Enter the number of time for muting the draw:";
    int count;
    cin >> count;
    cout << "Enter number of tests want to make(0 for as many as possible)";
    int testsNumber;
    cin >> testsNumber;
    // input = 2;
    // Lade die zugehoerige Textdatei in einen Graphen
    ifstream inFile;
    // int Bsp;
    string name;
    unsigned int seed;
    // a graph visualizer
    TextVisualizer v;
    // for(input = 1; input <= 10; input++)
    // {
        switch(input)
        {
            case 1:
            {
                name = "daten/Graph1.dat";
                DistCoordGraph coorG1;
                InputFile(inFile,name);
                inFile >> coorG1;
                PruefeHeuristik(coorG1);
                // TestDijkstra(coorG1,v,input);
                TestAstar(coorG1,input,width,height,count,testsNumber);
                inFile.close();
                break;
            }
            case 2:
            {
                name = "daten/Graph2.dat";
                // the largest edge is 15 
                // so choose the normierung as 15
                DistCoordGraph coorG2(15);
                InputFile(inFile,name);
                inFile >> coorG2;
                PruefeHeuristik(coorG2);
                // TestDijkstra(coorG2,v,input);
                TestAstar(coorG2,input,width,height,count,testsNumber);
                inFile.close();
                break;
            }
            case 3:
            {
                name = "daten/Graph3.dat";
                DistSphereGraph coorG3;
                InputFile(inFile,name);
                inFile >> coorG3;
                PruefeHeuristik(coorG3);
                // TestDijkstra(coorG3,v,input);
                TestAstar(coorG3,input,width,height,count,testsNumber);
                inFile.close();
                break;
            }
            case 4:
            {
                name = "daten/Graph4.dat";
                TimeCoordGraph coorG4;
                InputFile(inFile,name);
                inFile >> coorG4;
                PruefeHeuristik(coorG4);
                // TestDijkstra(coorG4,v,input);
                TestAstar(coorG4,input,width,height,count,testsNumber);
                inFile.close();
                break;
            }
            case 5:
            {
                name = "daten/Maze1.dat";
                MazeGraph mzG1;
                InputFile(inFile,name);
                inFile >> mzG1;
                PruefeHeuristik(mzG1);
                // TestDijkstra(mzG1,v,input);
                TestAstarMaze(mzG1,input,width,height,count,testsNumber);
                inFile.close();
                break;
            }
            case 6:
            {
                name = "daten/Maze2.dat";
                MazeGraph mzG2;
                InputFile(inFile,name);
                inFile >> mzG2;
                PruefeHeuristik(mzG2);
                // TestDijkstra(mzG2,v,input);
                TestAstarMaze(mzG2,input,width,height,count,testsNumber);
                inFile.close();
                break;
            }
            case 7:
            {
                name = "daten/Maze3.dat";
                MazeGraph mzG3;
                InputFile(inFile,name);
                inFile >> mzG3;
                PruefeHeuristik(mzG3);
                // TestDijkstra(mzG3,v,input);
                TestAstarMaze(mzG3,input,width,height,count,testsNumber);
                inFile.close();
                break;
            }
            case 8:
            {
                name = "daten/Maze4.dat";
                MazeGraph mzG4;
                InputFile(inFile,name);
                inFile >> mzG4;
                PruefeHeuristik(mzG4);
                // TestDijkstra(mzG4,v,input);
                TestAstarMaze(mzG4,input,width,height,count,testsNumber);
                inFile.close();
                break;
            }
            case 9:
            {
                name = "daten/Maze5.dat";
                MazeGraph mzG5;
                InputFile(inFile,name);
                inFile >> mzG5;
                PruefeHeuristik(mzG5);
                // TestDijkstra(mzG5,v,input);
                TestAstarMaze(mzG5,input,width,height,count,testsNumber);
                inFile.close();
                break;
            }
            case 10:
            {
                cout << "Please enter seed\n";
                // cin >> seed;
                seed = 1;
                vector<CellType> labyrinth = ErzeugeLabyrinth(256,256,seed);
                MazeGraph mzG(256*256,labyrinth,256,256);
                MazeVisualizer v1(mzG,width,height,count);
                VertexT start = mzG.getStart();
                VertexT dest = mzG.getDestination();
                cout << "The start: " << start << " the desti: " << dest << endl;
                list<VertexT> weg10;
                v1.setStartEnd(start,dest);
                A_star(mzG,v1,start,dest,weg10);
                PruefeWeg(10,weg10);

                break;
            }
            default:
            {
                cout << "Invalid input \n";
                exit(INVALID_INPUT);
            }
        }
    // }
    
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
    v_VertexTf::iterator viter;
    for(viter = vec.begin(); viter != vec.end(); viter++)
    {
        if((viter -> first) == (iter -> first))
        {
            break;
        }
    }
    return viter;

}

void Dijkstra(const DistanceGraph& g, GraphVisualizer& v,VertexT start, std::vector<CostT>& kostenZumStart) 
{
    size_t vertex_number = g.numVertices();
    // using vector to store VertexT, CostT
    neighbourVector vectorVC; 
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
    while(!vectorVC.empty())
    {
        // choose the node in sVC s.t D[v] minimal
        sort(vectorVC.begin(),vectorVC.end(),ComparePair());
        // pairVC firstPair(*vectorVC.begin());
        pairVC firstPair = vectorVC.back();
#ifdef DRAW
        v.markVertex(firstPair.first,VertexStatus::Active);
#endif
        neighbourVector neV = g.getNeighbors(firstPair.first);
        if(neV.empty())
        {
            // no need to check for updats just delete the first node
            // vectorVC.erase(vectorVC.begin()); // may be slow to perform this opertion
            // v.markVertex(firstPair.first,VertexStatus::Done);
            vectorVC.pop_back();
            continue;
        }
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

        // vectorVC.erase(vectorVC.begin()); // may be slow to perform this opertion
        vectorVC.pop_back();
#ifdef DRAW
        v.markVertex(firstPair.first,VertexStatus::Done);
        v.draw();
#endif
    }

}

// bool A_star(const DistanceGraph& g,GraphVisualizer& v, VertexT start, VertexT destination, std::list<VertexT>& weg)
// {
//     // ...
//     // trivial case
//     if(start == destination)
//     {
//         cout << "the start and destination coincide\n";
//         // according to PruefeWeg weg containts starting point
//         weg.push_front(start);
//         return true;
//     }
//     v_VertexTf openlist;
//     v_VertexTf closelist;
//     map2V predecessor;
//     size_t numOfVertices = g.numVertices();
//     for(size_t i = 0; i < numOfVertices; i++)
//     {
//         // initializing the elements of predecessors to -1
//         predecessor[i] = NOTVISITED;
// #ifdef DRAW
//         v.markVertex(i,VertexStatus::UnknownVertex);
// #endif
//     }
//     predecessor[start] = start;
//     openlist.push_back(VertexT_f(start,CostTgh(0,g.estimatedCost(start,destination))));


//     while(!openlist.empty())
//     {
//         VertexT_f firstVTf(*openlist.begin());
//         // VertexT_f firstVTf = openlist.back();
// #ifdef DRAW
//         v.markVertex(firstVTf.first,VertexStatus::Active);
//         v.draw();
// #endif
//         if(firstVTf.first == destination)
//         {
//             traceback(start,destination,weg,predecessor);
//             return true;
//         }
//         // get all the successors of firstVTf.first
//         VertexT bestVertex = firstVTf.first;
//         neighbourVector neV = g.getNeighbors(bestVertex);
//         // three cases
//         neighbourVector::iterator iter;

//         for(iter = neV.begin(); iter != neV.end(); iter++)
//         {
//             VertexT currentNode = iter -> first;
//             // VertexT_f * p_VertexTf = isIn(currentNode,openlist);
//             v_VertexTf::iterator p_VertexTf = isIn(iter,openlist);
//             CostT distToNode = firstVTf.second.first + (iter -> second);


//             CostT estiCost = g.estimatedCost(currentNode,destination);
//             // first case if the node not ever visited
//             if((p_VertexTf == openlist.end()) && predecessor[currentNode] == NOTVISITED)
//             {

//                 openlist.push_back(VertexT_f(currentNode,CostTgh(distToNode,estiCost)));
//                 predecessor[currentNode] = bestVertex;
// #ifdef DRAW
//                 v.updateVertex(currentNode,distToNode,estiCost,bestVertex,VertexStatus::InQueue);
// #endif
                
//             }
//             else if((p_VertexTf != openlist.end()))
//             {
//                 // cout << "The second case\n";
//                 CostT distOld = (*p_VertexTf).second.first;
//                 if(distToNode < distOld)
//                 {
//                     // the value from start to currentNode need to be modified
//                     (*p_VertexTf).second.first = distToNode;
//                     predecessor[currentNode] = bestVertex;
// #ifdef DRAW
//                     v.updateVertex(currentNode,distToNode,estiCost,bestVertex,VertexStatus::InQueue);
// #endif
//                 }
//             }
//             else 
//             {
//                 // node in the closelist check for possible updates
//                 p_VertexTf = isIn(iter,closelist);
//                 if( p_VertexTf != closelist.end())
//                 {
//                     CostT distOld = (*p_VertexTf).second.first;
//                     if(distToNode < distOld)
//                     {
//                         (*p_VertexTf).second.first = distToNode;
//                         VertexT_f vTfNew(*p_VertexTf);
                        
//                         closelist.erase(p_VertexTf);
//                         openlist.push_back(vTfNew);
//                         predecessor[currentNode] = bestVertex;
// #ifdef DRAW
//                         v.updateVertex(currentNode,distToNode,estiCost,bestVertex,VertexStatus::InQueue);
// #endif
//                     }
//                 }
//                 else
//                 {
//                     // an error 
//                     cout << "The fourth case in A_star algo appears\n";
//                     exit(FOURTH_CASE);
//                 }
//             }
//         }
//         closelist.push_back(firstVTf);
// #ifdef DRAW
//         v.markVertex(bestVertex,VertexStatus::Done);
// #endif
       
//         openlist.erase(openlist.begin());
//         // openlist.pop_back();
        
// #ifdef DRAW        
//         v.draw();
// #endif
//         // sort the openlist
//         make_heap(openlist.begin(),openlist.end(),CompareVertexTf());

//     }
//     // according to the PruefeWeg function weg has the point starting point
//     weg.push_front(start);
//     return false; // Kein Weg gefunden.
// }











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
        v.markVertex(firstVTf.first,VertexStatus::Done);
        // at the first loop the start will be extracted 
        // in this case predecessors[start] is not well defined
        if(firstVTf.first != start)
        {

             v.markEdge(EdgeT(predecessor[firstVTf.first],firstVTf.first),EdgeStatus::Optimal);
        }
       
        v.draw();
#endif
        if(firstVTf.first == destination)
        {
            traceback(start,destination,weg,predecessor);
// #ifdef DRAW
//             // already marked the destination above 
//             // after the while loop
//             // can't call draw again, because
//             // now the predecessors have to enter value 
//             // at predecessors[ON_PATH] ---> Segmentation fault
//             v.markVertex(firstVTf.first,VertexStatus::Done);
//             v.draw();
// #endif
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
            // checking whether the currentNode is updated
            // if not need extra v.markVertex to change it to InQueue
            bool updated = false; 
            VertexT currentNode = iter -> first;
            // VertexT_f * p_VertexTf = isIn(currentNode,openlist);
            v_VertexTf::iterator p_VertexTf = isIn(iter,openlist);
            CostT distToNode = firstVTf.second.first + (iter -> second);
#ifdef DRAW
            v.markVertex(currentNode,VertexStatus::Active);
            v.markEdge(EdgeT(bestVertex,currentNode),EdgeStatus::Active);
            v.draw();
#endif

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
               v.markEdge(EdgeT(bestVertex,currentNode),EdgeStatus::Visited);
               v.updateVertex(currentNode,distToNode,estiCost,bestVertex,VertexStatus::InQueue);
                updated = true;
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
                    v.markEdge(EdgeT(bestVertex,currentNode),EdgeStatus::Visited);
                    v.updateVertex(currentNode,distToNode,estiCost,bestVertex,VertexStatus::InQueue);
                    updated = true;
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
                        v.markEdge(EdgeT(bestVertex,currentNode),EdgeStatus::Visited);
                        v.updateVertex(currentNode,distToNode,estiCost,bestVertex,VertexStatus::InQueue);
                        updated = true;
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
#ifdef DRAW
            if(!updated)
            {
                v.markEdge(EdgeT(bestVertex,currentNode),EdgeStatus::Visited);
                v.markVertex(currentNode,VertexStatus::InQueue);
                v.draw();
            }
#endif
        }
        // all the neighbours coverd delete the vertex from the openlist and push it in the closelist
        // cout << "end of the for loop\n";
        // cout << "the closelist\n";

        closelist.push_back(firstVTf);
// #ifdef DRAW
//         v.markVertex(bestVertex,VertexStatus::Done);
// #endif
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

bool InputFile(ifstream & inFile, string name)
{
    inFile.open(name);
    if(!inFile.is_open())
    {
        cout << "could not find the given files\n";
        exit(FILE_OPEN_ERROR);
    }
    if(inFile.eof())
    {
        cout << "end of file reached.\n";
        return true;
    }
    else if(inFile.fail())
    {
        cout << "Input terminated by data mismatch.\n";
        return false;
    }
    else
    {
        cout << "Input terminated by unkown reasons.\n";
        return false;
    }
}

void TestDijkstra(DistanceGraph & g, GraphVisualizer& v, int Bsp)
{
    cout << "Testing dijkstra\n";
    vector<CostT> vecCost(g.numVertices(),0);
    for(VertexT ve = 0 ; ve < g.numVertices(); ve++)
    {
        
        cout << "\nStarting at " << ve << endl;
        Dijkstra(g,v,ve,vecCost);
        cout << "\nafter dijkstra\n";
        PruefeDijkstra(Bsp,ve,vecCost);
    }
}
void TestAstar(CoordinateGraph & coorG1, int Bsp,unsigned int width,unsigned int height,int Mute,int Tests)
{
    int count = 0;
    for(VertexT v1 = 0; v1 < coorG1.numVertices(); v1++)
    {
        for(VertexT v2 = 0; v2 < coorG1.numVertices(); v2++)
        {

            count++;
            list<VertexT> weg;
            RouteVisualizer rv(coorG1,width,height,Mute);
            rv.setStartEnd(v1,v2);
            A_star(coorG1,rv,v1,v2,weg);
            cout << "after a_star starting : " << v1 << "\t end: " << v2 << endl;
            for_each(weg.begin(),weg.end(),outputVertex);
            PruefeWeg(Bsp,weg);
            if(Tests && count < Tests)
            {
                count++;
            }
            else
            {
                return;
            }
            
        }
    }

}
void TestAstarMaze(MazeGraph & coorG2,  int Bsp,unsigned int width,unsigned int height,int Mute,int Tests)
{
    int count = 0;
    for ( auto pair : StartZielPaare(Bsp)) 
    {
        auto start = pair.first;
        auto goal  = pair.second;
        cout << "The start: " << start << " the goal: " << goal << endl;
        list<VertexT> weg_maze;
        MazeVisualizer v(coorG2,width,height,Mute);
        v.setStartEnd(start,goal);
        A_star(coorG2,v,start,goal,weg_maze);
        cout << "The path\n";
        // outWegMaze(coorG2,weg_maze);
        PruefeWeg(Bsp,weg_maze);
        if(Tests && count < Tests)
        {
            count++;
        }
        else
        {
            return;
        }
    }
}
