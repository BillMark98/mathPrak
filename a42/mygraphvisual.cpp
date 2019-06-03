#include "mygraphvisual.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
// using DistanceGraph::NeighborT;
mapRGB MazeVisualizer::colormap = 
{
    {"LightBlack",sf::Color(150,150,150)},
    {"Black",sf::Color(0,0,0)},
    {"White",sf::Color::White},
    {"Start",sf::Color(250,180,50)},
    {"Destination",sf::Color(240,190,45)},
    {"InQueue",sf::Color(100,100,150)},
    {"Done",sf::Color(10,100,10)},
    {"Active",sf::Color(255,10,10)},
    {"Route",sf::Color(250,250,100)},
    {"Text",sf::Color(135,250,250)}
};
MazeVisualizer::MazeVisualizer(MazeGraph & mz, VertexT & st, VertexT & end,unsigned int modeWidth,unsigned int modeHeight)
: mainWindow(sf::VideoMode(modeWidth,modeHeight),"mywindow"),mzGraph(mz),start(st),destination(end),windowWidth(modeWidth),
windowHeight(modeHeight),predecessors(mzGraph.numVertices(),NOTVISITED)
    
{
    mainWindow.clear(colormap["LightBlack"]);
    vectInfo.resize(mzGraph.numVertices());
    mzWidth = mzGraph.getWidth();
    mzHeight = mzGraph.getHeight();
    float len = windowWidth / mzWidth;
    float hei = windowHeight / mzHeight;
    rectShape = shapeSize(len,hei);
    charsize = (size_t)MIN(rectShape.first,rectShape.second);

}
MazeVisualizer::MazeVisualizer(MazeGraph & mz,unsigned int modeWidth,unsigned int modeHeight)
: mainWindow(sf::VideoMode(modeWidth,modeHeight),"mywindow"),mzGraph(mz),windowWidth(modeWidth),
    windowHeight(modeHeight),predecessors(mzGraph.numVertices(),NOTVISITED)
{
    mainWindow.clear(colormap["LightBlack"]);
    vectInfo.resize(mzGraph.numVertices());
    mzWidth = mzGraph.getWidth();
    mzHeight = mzGraph.getHeight();
    float len = windowWidth / mzWidth;
    float hei = windowHeight / mzHeight;
    rectShape = shapeSize(len,hei);
    charsize = (size_t)MIN(rectShape.first,rectShape.second);
}
void MazeVisualizer::setVecInfo()
{

}
void MazeVisualizer::setStartEnd(VertexT & st, VertexT & end)
{
    start = st;
    destination = end;
    // the vectInfo need to be remodified.
    vectInfo.clear();
    vectInfo.resize(mzGraph.numVertices());
    predecessors.clear();
    predecessors.resize(mzGraph.numVertices(),NOTVISITED);
}

void MazeVisualizer::setVertexInfo(VertexT v,VertexStatus st,CostT g,CostT h)
{
    if(!mzGraph.isValid(v))
    {
        cout << "Invalid index cant set VertexInfo\n";
        exit(WRONG_VERTEX_INDEX);
    }
    vectInfo[v].first = st;
    vectInfo[v].second.first = g;
    vectInfo[v].second.second = h;
}
void MazeVisualizer::setVertexGH(VertexT v,CostT g, CostT h)
{
    // Assuming the index is correct
    vectInfo[v].second.first = g;
    vectInfo[v].second.second = h;
}
    // Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
void MazeVisualizer::markVertex(VertexT vertex, VertexStatus status)
{
    vectInfo[vertex].first = status;
}

// Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
void MazeVisualizer::markEdge(EdgeT e, EdgeStatus status)
{
    
}

// // Aktualisiere jegliche Daten eines Knotens.
void MazeVisualizer::updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status)
{
    if(!mzGraph.isValid(vertex))
    {
        cout << "Invalid index cant set VertexInfo\n";
        exit(WRONG_VERTEX_INDEX);
    }
    vectInfo[vertex].first = status;
    vectInfo[vertex].second.first = cost;
    vectInfo[vertex].second.second = estimate;
    predecessors[vertex] = parent;
    draw();
}

// // Zeichne den aktuellen Zustand des Graphen.
void MazeVisualizer::draw()
{
    bool PathFound = false;
    // Combined with the a-star algo
    // this is the case when destination is the top
    // of the openlist, i.e the algo terminates and have found
    // the path
    if(vectInfo[destination].first == VertexStatus::Done)
    {
        PathFound = true;
        VertexT platform = destination;
        size_t count = 0;
        size_t UpperBound = mzGraph.numVertices();
        while(platform != start && (count < UpperBound))
        {
            // save the next hop
            VertexT temp = predecessors[platform];
            predecessors[platform] = ON_PATH;
            platform = temp;
            count++;
        }
        if(count >= UpperBound)
        {
            cout << "Error, no path from start to end\n";
            exit(NO_PATH_FOUND);
        }
    }
    sf::Font font;
    if(!font.loadFromFile("font/BebasNeue-Regular.ttf"))
    {
        cout << "the file could not found\n";
        exit(FONT_OPEN_ERROR);
    }
    // charsize > 1 so the (g,h) and other texts will be displayed
    if(charsize > 1)
    {
        for(size_t i = 1; i <= mzWidth; i++)
        {
            for(size_t j = 1; j <= mzHeight; j++)
            {
                VertexT currentNode = mzGraph.mzCoord2VertexT(i,j);
                // in case a wall draw rectangular
                if(mzGraph(i,j) == CellType::Wall)
                {
                    sf::RectangleShape rect(sf::Vector2f(rectShape.first,rectShape.second));
                    rect.setFillColor(colormap["Black"]);
                    float widthCoor = (i-1) * rectShape.first;
                    float heightCoor = (j-1)* rectShape.second;
                    rect.setPosition(sf::Vector2f(widthCoor,heightCoor));
                    mainWindow.draw(rect);
                }
                // the current node is the start
                else if(currentNode == start)
                {
                    sf::RectangleShape rect(sf::Vector2f(rectShape.first,rectShape.second));
                    rect.setFillColor(colormap["Start"]);
                    float widthCoor = (i-1) * rectShape.first;
                    float heightCoor = (j-1)* rectShape.second;
                    rect.setPosition(sf::Vector2f(widthCoor,heightCoor));
                    mainWindow.draw(rect);
                    sf::Text text;
                    // have to set Font or else the text wont be displayed
                    text.setFont(font);
                    size_t charsi = charsize/2;
                    // cout << "charsize is: " << charsi << endl;
                    // if(charsize == 0)
                    // {
                    //     cout << "The char size is too small\n";
                    //     cout << "Using size = 1\n";
                    //     charsize = 1;
                    // }
                    
                    text.setCharacterSize(charsi);
                    text.setString("Start");
                    text.setPosition(sf::Vector2f(widthCoor,heightCoor));
                    text.setFillColor(colormap["Text"]);
                    mainWindow.draw(text);
                }
                else if(currentNode == destination)
                {
                    sf::RectangleShape rect(sf::Vector2f(rectShape.first,rectShape.second));
                    rect.setFillColor(colormap["Destination"]);
                    float widthCoor = (i-1) * rectShape.first;
                    float heightCoor = (j-1)* rectShape.second;
                    rect.setPosition(sf::Vector2f(widthCoor,heightCoor));
                    mainWindow.draw(rect);
                    sf::Text text;
                    text.setFont(font);
                    size_t charsi = charsize/2;
                    // cout << "charsize is: " << charsi << endl;
                    // if(charsize == 0)
                    // {
                    //     cout << "The char size is too small\n";
                    //     cout << "Using size = 1\n";
                    //     charsize = 1;
                    // }
                    text.setCharacterSize(charsi);
                    text.setString("Goal");
                    text.setPosition(sf::Vector2f(widthCoor,heightCoor));
                    text.setFillColor(colormap["Text"]);
                    mainWindow.draw(text);
                    
                }
                else 
                {

                    sf::RectangleShape rect(sf::Vector2f(rectShape.first,rectShape.second));
                    if(predecessors[currentNode] == ON_PATH)
                    {
                        rect.setFillColor(colormap["Route"]);
                    }
                    else if(vectInfo[currentNode].first == VertexStatus::InQueue)
                    {
                        rect.setFillColor(colormap["InQueue"]);
                    }
                    else if(vectInfo[currentNode].first == VertexStatus::Active)
                    {
                        rect.setFillColor(colormap["Active"]);
                    }
                    else if(vectInfo[currentNode].first == VertexStatus::Done)
                    {
                        rect.setFillColor(colormap["Done"]);
                    }
                    float widthCoor = (i-1) * rectShape.first;
                    float heightCoor = (j-1)* rectShape.second;
                    rect.setPosition(sf::Vector2f(widthCoor,heightCoor));
                    mainWindow.draw(rect);
                    sf::Text text;
                    text.setFont(font);
                    size_t charsi = charsize/2;
                    // cout << "charsize is: " << charsize << endl;
                    // if(charsize == 0)
                    // {
                    //     cout << "The char size is too small\n";
                    //     cout << "Using size = 1\n";
                    //     charsize = 1;
                    // }
                    text.setCharacterSize(charsi);
                    CostT gdist = vectInfo[currentNode].second.first;
                    CostT hdist = vectInfo[currentNode].second.second;
                    string g1 = std::to_string((size_t)(gdist));
                    string g2 = std::to_string((size_t)hdist);
                    string output = "(" + g1 + "," + g2 + ")";
                    text.setString(output);
                    text.setPosition(sf::Vector2f(widthCoor,heightCoor));
                    text.setFillColor(colormap["Text"]);
                    mainWindow.draw(text);
                }
            }
        }
    }
    else
    {
        // just display the shape
        for(size_t i = 1; i <= mzWidth; i++)
        {
            for(size_t j = 1; j <= mzHeight; j++)
            {
                VertexT currentNode = mzGraph.mzCoord2VertexT(i,j);
                // in case a wall draw rectangular
                if(mzGraph(i,j) == CellType::Wall)
                {
                    sf::RectangleShape rect(sf::Vector2f(rectShape.first,rectShape.second));
                    rect.setFillColor(colormap["Black"]);
                    float widthCoor = (i-1) * rectShape.first;
                    float heightCoor = (j-1)* rectShape.second;
                    rect.setPosition(sf::Vector2f(widthCoor,heightCoor));
                    mainWindow.draw(rect);
                }
                // the current node is the start
                else if(currentNode == start)
                {
                    sf::RectangleShape rect(sf::Vector2f(rectShape.first,rectShape.second));
                    rect.setFillColor(colormap["Start"]);
                    float widthCoor = (i-1) * rectShape.first;
                    float heightCoor = (j-1)* rectShape.second;
                    rect.setPosition(sf::Vector2f(widthCoor,heightCoor));
                    mainWindow.draw(rect);
                }
                else if(currentNode == destination)
                {
                    sf::RectangleShape rect(sf::Vector2f(rectShape.first,rectShape.second));
                    rect.setFillColor(colormap["Destination"]);
                    float widthCoor = (i-1) * rectShape.first;
                    float heightCoor = (j-1)* rectShape.second;
                    rect.setPosition(sf::Vector2f(widthCoor,heightCoor));
                    mainWindow.draw(rect);
                }
                else 
                {

                    sf::RectangleShape rect(sf::Vector2f(rectShape.first,rectShape.second));
                    if(predecessors[currentNode] == ON_PATH)
                    {
                        rect.setFillColor(colormap["Route"]);
                    }
                    else if(vectInfo[currentNode].first == VertexStatus::InQueue)
                    {
                        rect.setFillColor(colormap["InQueue"]);
                    }
                    else if(vectInfo[currentNode].first == VertexStatus::Active)
                    {
                        rect.setFillColor(colormap["Active"]);
                    }
                    else if(vectInfo[currentNode].first == VertexStatus::Done)
                    {
                        rect.setFillColor(colormap["Done"]);
                    }
                    float widthCoor = (i-1) * rectShape.first;
                    float heightCoor = (j-1)* rectShape.second;
                    rect.setPosition(sf::Vector2f(widthCoor,heightCoor));
                    mainWindow.draw(rect);
                }
            }
        }
    }
    if(!PathFound)
    {
        mainWindow.display();
        sf::sleep(sf::seconds(0.01));
        // sf::sleep(sf::microseconds(10));
    }
    else
    {
        // display the window until user close it
        mainWindow.display();
        // sf::sleep(sf::seconds(2));
        // cout << "sleep ends, now the while loop\n";
        while(mainWindow.isOpen())
        {
            sf::Event event;
            while (mainWindow.pollEvent(event)) // event loop
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    mainWindow.close();
            }
        }
    }
    
}

void MazeVisualizer::draw_raw()
{
    // while (mainWindow.isOpen())
    // {
    //     // check all the window's events that were triggered since the last iteration of the loop
    //     sf::Event event;
    //     while (mainWindow.pollEvent(event))
    //     {
    //         // "close requested" event: we close the window
    //         if (event.type == sf::Event::Closed)
    //             mainWindow.close();
    //     }
    for(size_t i = 1; i <= mzWidth; i++)
    {
        for(size_t j = 1; j <= mzHeight; j++)
        {
            // in case a wall draw rectangular
            if(mzGraph(i,j) == CellType::Wall)
            {
                sf::RectangleShape rect(sf::Vector2f(rectShape.first,rectShape.second));
                rect.setFillColor(colormap["Black"]);
                float widthCoor = (i-1) * rectShape.first;
                float heightCoor = (j-1)* rectShape.second;
                rect.setPosition(sf::Vector2f(widthCoor,heightCoor));
                // mainWindow.draw(rect);
                sf::Text text;

            }
        }
    }
    mainWindow.display();
    sf::sleep(sf::seconds(2));
    // }
}

mapRGB RouteVisualizer::colormap = 
{
    {"LightBlack",sf::Color(150,150,150)},
    {"LightWhite",sf::Color(240,240,240)},
    {"Black",sf::Color(0,0,0)},
    {"White",sf::Color::White},
    {"Start",sf::Color(250,180,50)},
    {"Destination",sf::Color(240,190,45)},
    {"InQueue",sf::Color(100,100,150)},
    {"Done",sf::Color(10,100,10)},
    {"Active",sf::Color(255,10,10)},
    {"Route",sf::Color(250,250,100)},
    {"Text",sf::Color(135,250,250)}
};


RouteVisualizer::RouteVisualizer(CoordinateGraph & cg, VertexT & st, VertexT & end,unsigned int modeWidth,unsigned int modeHeight) 
: coorG(cg),start(st),destination(end),windowWidth(modeWidth),
windowHeight(modeHeight)
{
    mainWindow.clear(colormap["LightWhite"]);
    vectInfo.resize(cg.numVertices());
    double xMax = cg.getMaxXcoord();
    double xMin = cg.getMinXcoord();
    double yMax = cg.getMaxYcoord();
    double yMin = cg.getMinYcoord();
    double xDiff = xMax - xMin;
    double yDiff = yMax - yMin;
    // a_x = (1 - 2 * k)/(xMax - xMin) * windowWidth
    // b_x = ((xMax + xMin) * k * windowWidth - xMin * windowWidth) / (xMax - xMin)
    // s.t a_x * xMin + b = k * windowWidth
    // a_x * xMax + b = (1 - k) * windowWidth, where k is the SIDE_FACTOR
    
    a_x = (1 - 2 * SIDE_FACTOR)/(xMax - xMin) * windowWidth;
    b_x = ((xMax + xMin) * SIDE_FACTOR - xMin)* windowWidth / (xMax - xMin);

    a_y = (1 - 2 * SIDE_FACTOR)/(yMax - yMin) * windowWidth;
    b_y = ((yMax + yMin) * SIDE_FACTOR - yMin)* windowWidth / (yMax - yMin);
    InitializeVectEI(cg);
    // rectShape = shapeSize(len,hei);
    // charsize = (size_t)MIN(rectShape.first,rectShape.second);
}
RouteVisualizer::RouteVisualizer(CoordinateGraph & cg,unsigned int modeWidth,unsigned int modeHeight)
: coorG(cg),windowWidth(modeWidth),windowHeight(modeHeight)
{
    mainWindow.clear(colormap["LightWhite"]);
    vectInfo.resize(cg.numVertices());
    double xMax = cg.getMaxXcoord();
    double xMin = cg.getMinXcoord();
    double yMax = cg.getMaxYcoord();
    double yMin = cg.getMinYcoord();
    double xDiff = xMax - xMin;
    double yDiff = yMax - yMin;
    // a_x = (1 - 2 * k)/(xMax - xMin) * windowWidth
    // b_x = ((xMax + xMin) * k * windowWidth - xMin * windowWidth) / (xMax - xMin)
    // s.t a_x * xMin + b = k * windowWidth
    // a_x * xMax + b = (1 - k) * windowWidth, where k is the SIDE_FACTOR
    
    a_x = (1 - 2 * SIDE_FACTOR)/(xMax - xMin) * windowWidth;
    b_x = ((xMax + xMin) * SIDE_FACTOR - xMin)* windowWidth / (xMax - xMin);

    a_y = (1 - 2 * SIDE_FACTOR)/(yMax - yMin) * windowWidth;
    b_y = ((yMax + yMin) * SIDE_FACTOR - yMin)* windowWidth / (yMax - yMin);
    InitializeVectEI(cg);
}
void RouteVisualizer::InitializeVectEI(CoordinateGraph & cg)
{
    
    size_t bound = cg.numVertices();
    v_eI.resize(bound);
    for(size_t i = 0; i < bound; i++)
    {
        DistanceGraph::NeighborT neighbors = cg.getNeighbors(i);
        size_t vecSize = neighbors.size();
        for(size_t index = 0; index < vecSize; index++)
        {
            edgeInfo edI(neighbors[index].second,EdgeStatus::UnknownEdge);
            sourceEdgeInfo sourcInfo(neighbors[index].first,edI);
            v_eI[i].push_back(sourcInfo);
        }
    }
}
void RouteVisualizer::setStartEnd(VertexT & st, VertexT & end)
{

}
// set the vectInfo
void RouteVisualizer::setVecInfo()
{

}

// set the vectInfo for the current node
void RouteVisualizer::setVertexInfo(VertexT,VertexStatus,CostT g,CostT h)
{

}
// set the cost g h
void RouteVisualizer::setVertexGH(VertexT,CostT g, CostT h)
{

}

// Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
void RouteVisualizer::markVertex(VertexT vertex, VertexStatus status)
{
    
}

// Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
void RouteVisualizer::markEdge(EdgeT e, EdgeStatus status)
{

}

// Aktualisiere jegliche Daten eines Knotens.
void RouteVisualizer::updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status)
{

}

// Zeichne den aktuellen Zustand des Graphen.
void RouteVisualizer::draw()
{

}
// draw the protetype of the maze
void RouteVisualizer::draw_raw()
{

}