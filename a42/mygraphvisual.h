#ifndef MYGRAPHVISUALIZER_H
#define MYGRAPHVISUALIZER_H


#include "unit.h"
#include "mygraph.h"
#include <SFML/Graphics.hpp>
#include <string>

#define FONT_OPEN_ERROR 20
#define NO_PATH_FOUND 21
#define WRONT_EDGE_STATUS 22
#define ON_PATH 5000000
#define NOTVISITED 10000000
// the relative size of arrow of an edge of a directed graph
// compared with the radius of the vertex
#define ARROW_SCALE 1
#define TEXT_SCALE 1
// the relative dist from the text showing the weight of the edge
// to the edge itself, compared with the radius of the vertex
#define TEXT_TO_LINE_SCALE 1
// the precision of the decimal
#define PRECISION 2
// the side factor a smaller than 1/2 value
// the min of vertices of a graph xMin will be 
// mapped to SIDE_FACTOR * windowWidth and xMax will be
// mapped to (1-SIDE_FACTOR) * windowWidth
#define SIDE_FACTOR 1/8
using std::pair;
using std::string;

// // the pair used for saving g and h in the f = g + h for a star algo
// typedef pair<CostT,CostT> CostTgh;
// // the vertex and the corresponding (g,h) value
// typedef pair<VertexT,CostTgh> VertexT_f; 
// typedef std::pair<VertexStatus,CostTgh> VertexInfo;
// typedef std::vector<VertexInfo> v_vtInfo;


using sf::RenderWindow;
// the size of the shape
typedef pair<float, float> shapeSize;
typedef map<string,sf::Color> mapRGB;
typedef pair<CostT,EdgeStatus> edgeInfo;
typedef pair<EdgeT,edgeInfo> edgeInfoPair;
// edge from one node to another VertexT with edgeInfo
typedef pair<VertexT,edgeInfo> sourceEdgeInfo;
typedef vector<edgeInfoPair> v_edgeInfoPair;
typedef map<VertexT,sourceEdgeInfo> mpVertexEdgeInfo;
typedef vector<sourceEdgeInfo> vectVertexEdgeInfo;
// vector, each element is a vector storing the edge goes
// out of the vertex with corresponding edge information (CostT,EdgeStatus)
typedef vector<vectVertexEdgeInfo> v_edgeInfo;
class MazeVisualizer : public GraphVisualizer
{
    protected:
        sf::RenderWindow mainWindow;
        MazeGraph & mzGraph;
        VertexT start;
        VertexT destination;
        v_vtInfo vectInfo;
        // the size of the window
        unsigned int windowWidth;
        unsigned int windowHeight;

        // the size of the maze
        size_t mzWidth;
        size_t mzHeight;
        // the size of the rectangular shape
        shapeSize rectShape;
        // the colors:
        static mapRGB colormap;
        size_t charsize;
        vector<VertexT> predecessors;
    public:
        MazeVisualizer(MazeGraph & mz, VertexT & st, VertexT & end,unsigned int modeWidth = 800,unsigned int modeHeight = 600);
        MazeVisualizer(MazeGraph & mz,unsigned int modeWidth = 800,unsigned int modeHeight = 600);
        ~MazeVisualizer(){}

        void setStartEnd(VertexT & st, VertexT & end);
        // set the vectInfo
        void setVecInfo();

        // set the vectInfo for the current node
        void setVertexInfo(VertexT,VertexStatus,CostT g,CostT h);
        // set the cost g h
        void setVertexGH(VertexT,CostT g, CostT h);

        // Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
        void markVertex(VertexT vertex, VertexStatus status);
        
        // Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
        void markEdge(EdgeT e, EdgeStatus status);
        
        // Aktualisiere jegliche Daten eines Knotens.
        void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status);
        
        
        // Zeichne den aktuellen Zustand des Graphen.
        void draw();
        // draw the protetype of the maze
        void draw_raw();

        // draw help function
        // void drawVertex(size_t i, size_t j, )

};


class RouteVisualizer : public GraphVisualizer
{
    protected:
        sf::RenderWindow mainWindow;
        CoordinateGraph & coorG;
        VertexT start;
        VertexT destination;
        v_vtInfo vectInfo;
        // // a map for the edge info
        // // the key is the VertexT the value is a vector
        // // saving each edge goes out of that node with the corresponding
        // // edge info (CostT, EdgeStatus)
        // mpVertexEdgeInfo mpvE;
        v_edgeInfo v_eI;
        // the size of the window
        unsigned int windowWidth;
        unsigned int windowHeight;

        // the parameter of the affine transformation for
        // the coordinate of the vertices
        // a_x * Vertex_x + b_x = GraphVertex_x
        // a_y * Vertex_y + b_y = GraphVertex_y
        // the size of the rectangular shape
        double a_x;
        double a_y;
        double b_x;
        double b_y;
        // the radius of the vertex circle shape
        float circShape;
        // the colors:
        static mapRGB colormap;
        size_t charsize;
        // size_t textsize;
        vector<VertexT> predecessors;
    public:
        RouteVisualizer(CoordinateGraph & cg, VertexT & st, VertexT & end,unsigned int modeWidth = 800,unsigned int modeHeight = 600);
        RouteVisualizer(CoordinateGraph & cg,unsigned int modeWidth = 800,unsigned int modeHeight = 600);
        void InitializeVectEI();
        void setStartEnd(VertexT & st, VertexT & end);
        // set the vectInfo
        void setVecInfo();
        
        void AffineIntialize();
        void CircInitialize();
        // calculate the position on the graph
        sf::Vector2f getPosition(const VertexT & v) const;
        // get the graph visual coordinate giving back the pair<double,double>
        coordinate getGraphVisPosition(const VertexT &v) const;
        // set the vectInfo for the current node
        void setVertexInfo(VertexT,VertexStatus,CostT g,CostT h);
        // set the cost g h
        void setVertexGH(VertexT,CostT g, CostT h);

        // test whether there is an edge from vertex from to vertex to
        bool IsEdge(const VertexT & from, const VertexT & to) const;
        // Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
        void markVertex(VertexT vertex, VertexStatus status);
        
        // Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
        void markEdge(EdgeT e, EdgeStatus status);
        
        // Aktualisiere jegliche Daten eines Knotens.
        void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status);
        
        // draw an edge between two vertices
        void drawEdge(VertexT from, VertexT to,CostT cost,EdgeStatus eds = EdgeStatus::UnknownEdge);
        // Zeichne den aktuellen Zustand des Graphen.
        void draw();
        // draw the protetype of the maze
        void draw_raw();
};
#endif