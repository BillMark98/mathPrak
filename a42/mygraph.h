#ifndef MYGRAPH_H
#define MYGRAPH_H

#include "unit.h"
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>




#define WRONG_VERTEX_INDEX 3
#define INITIALIZING_ERROR 5
#define WRONG_POSITION 13
#define INVALID_SIZE 8
#define READ_ERROR 9
#define GRAPH_EMPTY 10
#define NOTFOUND 10000000
using std::cin;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
using std::size_t;
using std::ifstream;
using std::ofstream;
using std::pair;
using std::map;
using std::vector;
// the earth radius 6371 km
#define EARTH_RADIUS 6371
#define pi 3.1415926
// estimated car speed 1000 km/h
#define CAR_SPEED 200
#define Hour2Min 60
typedef std::pair<double,double> coordinate;
typedef std::map< VertexT,coordinate> VertexCoord;
typedef std::pair<size_t,size_t> mzCoord;

typedef std::pair<VertexT, CellType> p_vertexCell;
// vectors of pair of vertex and cell type
typedef std::vector<p_vertexCell> v_vertexCell;
typedef std::vector<CellType> v_Cell;


// the pair used for saving g and h in the f = g + h for a star algo
typedef pair<CostT,CostT> CostTgh;
// the vertex and the corresponding (g,h) value
typedef pair<VertexT,CostTgh> VertexT_f; 
typedef std::pair<VertexStatus,CostTgh> VertexInfo;
typedef std::vector<VertexInfo> v_vtInfo;

// CoordinateGraph be a ABC or just a class?
// if ABC then RouteVisualiser can't have a object of this type
class CoordinateGraph : public DistanceGraph {
protected:
    // vectors of NeighborT each element is a vector which holds the 
    // information of all the neighbours connected to this specific node. 
    // for example neighbour_vector[0] is a vector of type Neighbor T , we call it vec. then each element of vec
    // is a pair, < index of node, cost to node>

    // actually we have to create such vectors to hold 
    // because the api getNeighbors return a const NeighborT& 
    // so the vector has to be a member element
    // especially we cant create it inside getNeighbors temporarily
    std::vector<NeighborT> neigbour_vector; 
    // number of edges of the graph
    size_t edgeCount;
    VertexCoord vcMap;
public:
    
    CoordinateGraph(int num_verts= 0)
        : DistanceGraph(num_verts){};
    const NeighborT& getNeighbors( VertexT v) const override;
    
    // virtual CostT estimatedCost( VertexT from, VertexT to) const override = 0;
    // change the coordinateGraph to a class instead of ABC for the purpose of 
    // being able to create an object for the RouteVisualizer
    virtual CostT estimatedCost( VertexT from, VertexT to) const override {return cost(from,to);}
    
    CostT cost( VertexT from, VertexT to) const override;

    friend istream & operator>>(istream & is, CoordinateGraph & coorGraph);
    // naive implementation of output
    friend ostream & operator<<(ostream & os, CoordinateGraph & coorGraph); 
    double getMaxXcoord() const;
    double getMinXcoord() const;
    double getMaxYcoord() const;
    double getMinYcoord() const;
    coordinate getCoordinate(VertexT v) const {return vcMap.at(v);}
    ~CoordinateGraph(){};
};

class DistCoordGraph: public CoordinateGraph {
protected:
    // for the heuristik 
    int Normierung; 
public:
    DistCoordGraph(int normierung = 1,int num_verts= 0)
        : CoordinateGraph(num_verts),Normierung(normierung){};
    CostT estimatedCost( VertexT from, VertexT to) const override;
    ~DistCoordGraph(){};
};
// the graph on a sphere given by a radius
class DistSphereGraph: public CoordinateGraph {
protected:
    double radius;
public:
    DistSphereGraph(int num_verts = 0,double R = EARTH_RADIUS)
        : CoordinateGraph(num_verts),radius(R){};
    CostT estimatedCost(VertexT from, VertexT to) const override;
    ~DistSphereGraph(){};
    double deg2rad(double deg) const{ return deg * pi / 180;};
};
class TimeCoordGraph: public CoordinateGraph {
protected:
    double radius;
    double vehicleSpeed;
public:
    TimeCoordGraph(int num_verts = 0,double R = EARTH_RADIUS,double v = CAR_SPEED)
        : CoordinateGraph(num_verts),radius(R),vehicleSpeed(v){};
    CostT estimatedCost(VertexT from, VertexT to) const override;
    ~TimeCoordGraph(){};
    double deg2rad(double deg) const{ return deg * pi / 180;};
};


// the maze graph
class MazeGraph : public DistanceGraph
{
protected:
    v_Cell v_vC;
    // size_t edgeCount;
    size_t width;
    size_t height;
    
    std::vector<NeighborT> neighbour_vector; 
public:
    MazeGraph(int num = 0)
        : DistanceGraph(num){};
    MazeGraph(int num,v_Cell & v_C,int breite, int hoehe);
    // get the start
    VertexT getStart() const;
    // get the destination
    VertexT getDestination() const;
    // get the maze size information: the width
    size_t getWidth() const{ return width;}
    // get the maze size information: the height
    size_t getHeight() const{return height;}
    const NeighborT& getNeighbors( VertexT v) const override;
    void setNeighbors();
    CostT estimatedCost( VertexT from, VertexT to) const override;
    CostT cost( VertexT from, VertexT to) const override;
    bool isEmpty() const {return height < 1 || width < 1 || vertexCount <= 0;}
    // bool isValid(VertexT v) const{ return v >= 0 && v < vertexCount;}
    bool isValid(VertexT v) const{ return v < vertexCount;}
    bool isBorder(VertexT v) const;
    bool isLeftBorder(VertexT v) const;
    bool isRightBorder(VertexT v) const;
    bool isUpBorder(VertexT v) const;
    bool isCorner(VertexT v) const;
    bool isUpCorner(VertexT v) const;

    // for the graph visualizer
    bool setVectInfo(v_vtInfo & vtInfo)const;

    // give back the type of the vertex

    CellType getCellType(VertexT &v) const{return v_vC[v];}

    mzCoord Vertex2mzCoord(VertexT v) const;
    // mzCoord in primitive version, i.e two size_t
    VertexT mzCoord2VertexT(size_t w, size_t h) const;
    // testing the validity of the mzCoordinate
    bool ismzCodValid(size_t w, size_t h) const;
    // giving back the value of the v_vC using the mzCoord
    const CellType& at(size_t w, size_t h) const;
    CellType& operator()(size_t w, size_t h);
    // outputting the Vertex v in mzcoordinate
    ostream & VertexInMzCoord(const VertexT &v,ostream & os) const;
    friend istream & operator>>(istream & is, MazeGraph & mz);
    friend ostream & operator<<(ostream & os, MazeGraph & mz);

};

#endif