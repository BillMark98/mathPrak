#ifndef MYGRAPHVISUALIZER_H
#define MYGRAPHVISUALIZER_H


#include "unit.h"
#include "mygraph.h"
#include <SFML/Graphics.hpp>
#include <string>

#define FONT_OPEN_ERROR 20
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



#endif