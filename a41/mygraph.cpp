#include "mygraph.h"

const DistanceGraph::NeighborT& CoordinateGraph::getNeighbors( VertexT v) const
{
    if(v < 0 || v > vertexCount)
    {
        cout << "Wrong index of vertex in CoordinateGraph::getNeighbors\n";
        exit(WRONG_VERTEX_INDEX);
    }
    return neigbour_vector[v];
}
istream & operator>>(istream & is, CoordinateGraph & coorGraph)
{
    is >> coorGraph.vertexCount; // read in vertexCount
    coorGraph.neigbour_vector.resize(coorGraph.vertexCount);
    is >> coorGraph.edgeCount; 
    size_t number = coorGraph.edgeCount;
    size_t node;
    // cout << "successfully reading the two variables\n";
    for(size_t i = 0; i < number; i++)
    {
        is >> node;
        if(node < 0 || node > coorGraph.vertexCount)
        {
            cout << "the given index of node is out of range in operator >> of coordinateGraph .\n";
            exit(WRONG_VERTEX_INDEX);
        }
        VertexT vertex;
        CostT costs;
        is >> vertex;
        is >> costs;
        DistanceGraph::LocalEdgeT localedge(vertex,costs);
        coorGraph.neigbour_vector[node].push_back(localedge);
    }
    // cout << "succesfully reading vertexes\n";
    // read in coordinates of each node
    number = coorGraph.vertexCount;
    double x,y;
    for(size_t i = 0; i < number; i++)
    {
        is >> x >> y;
        coorGraph.vcMap[i] = coordinate(x,y);
    }
    // cout << "successfully reading edges\n";
    return is;
}
ostream & operator<<(ostream & os, CoordinateGraph & coorGraph)
{
    size_t number = coorGraph.vertexCount;
    cout << "The vertexCount is " << number << endl;
    for(size_t i = 0; i < number; i++)
    {
        os << "************************\n";
        os << "The node " << i << " 's neighors:\n";
        os << "vertex\t\t\t costs\n";
        DistanceGraph::NeighborT::const_iterator iter;
        for(iter = coorGraph.neigbour_vector[i].begin(); iter != coorGraph.neigbour_vector[i].end(); iter++)
        {
            os.width(6);
            os << iter -> first;
            os.width(15);
            os << iter -> second <<endl;
        }
        os << "The node " << i << " 's coordinate\n";
        os.width(6);
        os << coorGraph.vcMap[i].first;
        os.width(15);
        os << coorGraph.vcMap[i].second<<endl;
    }
    return os;

}
    

    
CostT CoordinateGraph::cost( VertexT from, VertexT to) const
{
    if(from < 0 || from > vertexCount || to < 0 || to > vertexCount)
    {
        cout << "Wrong index of vertex in CoGraph:Cos\n";
        exit(WRONG_VERTEX_INDEX);
    }
    DistanceGraph::NeighborT::const_iterator iter;
    for(iter = neigbour_vector[from].begin(); iter != neigbour_vector[from].end(); iter++)
    {
        if(iter -> first == to)
        {
            return iter -> second;
        }
    }
    return infty;
}

CostT DistCoordGraph::estimatedCost( VertexT from, VertexT to) const
{
    if(from < 0 || from > vertexCount || to < 0 || to > vertexCount)
    {
        cout << "Wrong index of vertex in CoGraph:estiCos\n";
        exit(WRONG_VERTEX_INDEX);
    }
    // naive estimation the euclidean distance between the two nodes
    double fromx = vcMap.at(from).first; // can't use [] operator here because expected is a const pair
    // no operator "[]" matches these operands -- operand types are: const VertexCoord [ VertexT ]
    double fromy = vcMap.at(from).second;
    double tox = vcMap.at(to).first;
    double toy = vcMap.at(to).second;
    double distance = sqrt(pow((fromx - tox),2) + pow((fromy - toy),2));
    return distance;

}