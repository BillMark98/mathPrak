#include "unit.h"
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

#define WRONG_VERTEX_INDEX 3

using std::cin;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
using std::size_t;
using std::ifstream;
using std::ofstream;

typedef std::pair<double,double> coordinate;
typedef std::map< VertexT,coordinate> VertexCoord;
class CoordinateGraph : public DistanceGraph {
protected:
    // vectors of NeighborT each element is a vector which holds the 
    // information of all the neighbours connected to this specific node. 
    // for example neighbour_vector[0] is a vector of type Neighbor T , we call it vec. then each element of vec
    // is a pair, < index of node, cost to node>
    std::vector<NeighborT> neigbour_vector; 
    // number of edges of the graph
    size_t edgeCount;
    VertexCoord vcMap;
public:
    
    CoordinateGraph(int num_verts= 0)
        : DistanceGraph(num_verts){};
    const NeighborT& getNeighbors( VertexT v) const override;
    
    virtual CostT estimatedCost( VertexT from, VertexT to) const override = 0;
    
    CostT cost( VertexT from, VertexT to) const override;

    friend istream & operator>>(istream & is, CoordinateGraph & coorGraph);
    // naive implementation of output
    friend ostream & operator<<(ostream & os, CoordinateGraph & coorGraph); 
    ~CoordinateGraph(){};
};

class DistCoordGraph: public CoordinateGraph {
public:
    DistCoordGraph(int num_verts= 0)
        : CoordinateGraph(num_verts){};
    CostT estimatedCost( VertexT from, VertexT to) const override;
    ~DistCoordGraph(){};
};