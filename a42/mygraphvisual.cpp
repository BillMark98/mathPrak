#include "mygraphvisual.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
// using DistanceGraph::NeighborT;

// for outputting coordinates
ostream & outCoord(ostream & os,double x1,double x2);
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
    {"UnknwonEdge",sf::Color(40,40,40)},
    {"Visited",sf::Color(150,150,10)},
    {"EdgeActive",sf::Color(150,10,10)},
    {"Optimal",sf::Color(80,50,200)},
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
                mainWindow.draw(rect);
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
    {"UnknownVertex",sf::Color(50,50,50)},
    {"InQueue",sf::Color(100,100,150)},
    {"Done",sf::Color(10,100,10)},
    {"VertexActive",sf::Color(255,10,10)},
    {"UnknwonEdge",sf::Color(40,40,40)},
    {"Visited",sf::Color(150,150,10)},
    {"EdgeActive",sf::Color(150,10,10)},
    {"Optimal",sf::Color(80,50,200)},
    {"Route",sf::Color(250,250,100)},
    {"EdgeRoute",sf::Color(230,250,30)},
    {"Arrow",sf::Color(100,150,80)},
    {"Text",sf::Color(135,250,250)}
};


RouteVisualizer::RouteVisualizer(CoordinateGraph & cg, VertexT & st, VertexT & end,unsigned int modeWidth,unsigned int modeHeight) 
: coorG(cg),mainWindow(sf::VideoMode(modeWidth,modeHeight),"mywindow"),start(st),destination(end),windowWidth(modeWidth),
windowHeight(modeHeight),predecessors(coorG.numVertices(),NOTVISITED)
{
    mainWindow.clear(colormap["LightWhite"]);
    vectInfo.resize(cg.numVertices());
    AffineIntialize();
    InitializeVectEI();
    CircInitialize();
    // rectShape = shapeSize(len,hei);
    // charsize = (size_t)MIN(rectShape.first,rectShape.second);
}
RouteVisualizer::RouteVisualizer(CoordinateGraph & cg,unsigned int modeWidth,unsigned int modeHeight)
: coorG(cg),mainWindow(sf::VideoMode(modeWidth,modeHeight),"mywindow"),windowWidth(modeWidth),windowHeight(modeHeight),predecessors(coorG.numVertices(),NOTVISITED)
{
    mainWindow.clear(colormap["LightWhite"]);
    vectInfo.resize(cg.numVertices());
    AffineIntialize();
    InitializeVectEI();
    CircInitialize();
}
void RouteVisualizer::AffineIntialize()
{
    double xMax = coorG.getMaxXcoord();
    double xMin = coorG.getMinXcoord();
    double yMax = coorG.getMaxYcoord();
    double yMin = coorG.getMinYcoord();
    cout << "xMax : " << xMax << "\txMin: " << xMin << endl;
    cout << "yMax : " << yMax << "\tyMin: " << yMin << endl;
    double xDiff = xMax - xMin;
    double yDiff = yMax - yMin;
    // a_x = (1 - 2 * k)/(xMax - xMin) * windowWidth
    // b_x = ((xMax + xMin) * k * windowWidth - xMin * windowWidth) / (xMax - xMin)
    // s.t a_x * xMin + b = k * windowWidth
    // a_x * xMax + b = (1 - k) * windowWidth, where k is the SIDE_FACTOR
    
    a_x = (1.0 - 2.0 * SIDE_FACTOR)/xDiff * (double)windowWidth;
    b_x = ((xMax + xMin) * SIDE_FACTOR - xMin)* (double)windowWidth / xDiff;
    cout << "a_x : " << a_x << "\tb_x: " << b_x << endl;
    a_y = (1.0 - 2.0 * SIDE_FACTOR)/yDiff * (double)windowHeight;
    b_y = ((yMax + yMin) * SIDE_FACTOR - yMin)* (double)windowHeight / yDiff;
    cout << "a_y : " << a_y << "\tb_y: " << b_y << endl;
}
void RouteVisualizer::CircInitialize()
{
    // the circles cannot overlap with each other
    // here the worst case the whole vertices are arranged in one row(column)
    float s1 = (float)(1 - 2 * SIDE_FACTOR)* windowWidth/coorG.numVertices();
    float s2 = (float)(1 - 2 * SIDE_FACTOR)* windowHeight/coorG.numVertices();
    float bound1 = MIN(s1,s2) / 2.0;

    // the radius must not exceed the edge of the window
    float upx = (float)SIDE_FACTOR * windowWidth;
    float upy = (float)SIDE_FACTOR * windowHeight;
    float bound2 = MIN(upx,upy)/2.0;
    circShape = MIN(bound1,bound2);
    cout << "The circShape is: " << circShape << endl;
    // set the charsize
    charsize = circShape * TEXT_SCALE;
    cout << "The char size is " << charsize << endl;
    if(charsize < 1)
    {
        cout << "the calculated charsize is too small, set to 2\n";
        charsize = 2;
    }
}
void RouteVisualizer::InitializeVectEI()
{
    
    size_t bound = coorG.numVertices();
    if(!v_eI.empty())
    {
        v_eI.clear();
    }
    v_eI.resize(bound);
    for(size_t i = 0; i < bound; i++)
    {
        DistanceGraph::NeighborT neighbors = coorG.getNeighbors(i);
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
    vectInfo.clear();
    vectInfo.resize(coorG.numVertices());
    InitializeVectEI();
    start = st;
    destination = end;
    predecessors.clear();
    predecessors.resize(coorG.numVertices(),NOTVISITED);
}

sf::Vector2f RouteVisualizer::getPosition(const VertexT & v) const
{
    coordinate vCoord = coorG.getCoordinate(v);
    float xAxis = a_x * vCoord.first + b_x;
    float yAxis = a_y * vCoord.second + b_y;
    cout << "The vertex: " << v << endl;
    cout << "the graph position: (" << vCoord.first << " , " << vCoord.second << " )\n"; 
    cout << "The visual position: (" << xAxis << " , " << yAxis << " )\n";
    return sf::Vector2f(xAxis,yAxis);    
}

coordinate RouteVisualizer::getGraphVisPosition(const VertexT &v) const
{
    coordinate vCoord = coorG.getCoordinate(v);
    double xAxis = a_x * vCoord.first + b_x;
    double yAxis = a_y * vCoord.second + b_y;
    cout << "In getGvP, The vertex: " << v << endl;
    cout << "the graph position: (" << vCoord.first << " , " << vCoord.second << " )\n"; 
    cout << "The visual position: (" << xAxis << " , " << yAxis << " )\n";
    return coordinate(xAxis,yAxis);
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

bool RouteVisualizer::IsEdge(const VertexT & from, const VertexT & to) const
{
    // Assume the vertices are valid
    vectVertexEdgeInfo::const_iterator iter;
    for(iter = v_eI[from].begin(); iter != v_eI[from].end(); iter++)
    {
        if((*iter).first == to)
        {
            return true;
        }
    }
    return false;
}
bool RouteVisualizer::IsEdgeOnPath(const VertexT & from, const VertexT & to) const
{
    return ((predecessors[from] == ON_PATH) && (predecessors[to] == ON_PATH));
}

// Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
void RouteVisualizer::markVertex(VertexT vertex, VertexStatus status)
{
    vectInfo[vertex].first = status;
}

// Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
void RouteVisualizer::markEdge(EdgeT e, EdgeStatus status)
{
    VertexT from = e.first;
    VertexT to = e.second;
    vectVertexEdgeInfo::iterator iter;
    for(iter = v_eI[from].begin(); iter != v_eI[from].end(); iter++)
    {
        if((*iter).first == to)
        {
            break;
        }
    }
    if(iter == v_eI[from].end())
    {
        cout << "No such given edge! " << endl;
        exit(NO_EDGE);
    }
    else
    {
        (*iter).second.second = status;
    }

}

// Aktualisiere jegliche Daten eines Knotens.
void RouteVisualizer::updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status)
{
    // Assume the vertex is valid
    vectInfo[vertex].first = status;
    vectInfo[vertex].second.first = cost;
    vectInfo[vertex].second.second = estimate;
    predecessors[vertex] = parent;
    draw();
}

void RouteVisualizer::drawEdge(VertexT from, VertexT to,CostT cost,EdgeStatus eds)
{
    // // Assume there is an edge from vertex from to vertex to
    // coordinate fromCoord = getGraphVisPosition(from);
    // coordinate toCoord = getGraphVisPosition(to);
    // double fromX = fromCoord.first;
    // double fromY = fromCoord.second;
    // double toX = toCoord.first;
    // double toY = toCoord.second;
    // double distance = sqrt(pow((fromX - toX),2) + pow((fromY - toY),2));
    // double lambda = circShape / distance;

    // cout << "The from graph visual coord: (" << fromX << " , " << fromY << ")\n";
    // cout << "The to graph visual coord: (" << toX << " , " << toY << ")\n";
    // double Bx,By,Ex,Ey;
    // double e45fx = sqrt(2)/2 * (((toX - fromX) - (toY - fromY))/distance);
    // double e45fy = sqrt(2)/2 * (((toX - fromX) + (toY - fromY))/distance);
    // cout << "e45f:";
    // outCoord(cout,e45fx,e45fy)<< endl;
    // // unit vector 90 degree rotated from e_l
    // double eVx = (toY - fromY) / distance;
    // double eVy = (fromX - toX) / distance;
    // cout << "eV:";
    // outCoord(cout,eVx,eVy)<< endl;
    
    // double e45tx = e45fy;
    // double e45ty = -e45fx;
    // cout << "e45t:";
    // outCoord(cout,e45tx,e45ty) << endl;
    // if(IsEdge(to,from))
    // {
    //     // bidirectional
    //     Bx = fromX + circShape * e45fx;
    //     By = fromY + circShape * e45fy;
    //     Ex = toX + circShape * e45tx;
    //     Ey = toY + circShape * e45ty;
    // }
    // else
    // {
    //     Bx = fromX * (1 - lambda) + toX * lambda;
    //     By = fromY * (1 - lambda) + toY * lambda;
    //     Ex = fromX * lambda + toX * (1 - lambda);
    //     Ey = fromY * lambda + toY * (1 - lambda);

    // }

    // cout << "the B coord: ";
    // outCoord(cout,Bx,By) << endl;
    // cout << "the E coord: ";
    // outCoord(cout,Ex,Ey) << endl;

    // string typeName;
    // // if(eds == EdgeStatus::UnknownEdge)
    // // {
        
    // // }
    // switch (eds)
    // {
    // case EdgeStatus::UnknownEdge :
    // {
    //     typeName = "UnknwonEdge";
    //     break;
    // }
    // case EdgeStatus::Visited :
    // {
    //     typeName = "Visited";
    //     break;
    // }
    // case EdgeStatus::Active :
    // {
    //     typeName = "EdgeActive";
    //     break;
    // }
    // case EdgeStatus::Optimal:
    // {
    //     typeName = "Optimal";
    //     break;
    // }
    // default:
    // {
    //     cout << "no other edge status.\n";
    //     exit(WRONT_EDGE_STATUS);
    //     break;
    // }
        
    // }
    // sf::Vertex line[] =
    // {
    //     sf::Vertex(sf::Vector2f(Bx,By),colormap[typeName]),
    //     sf::Vertex(sf::Vector2f(Ex,Ey),colormap[typeName])
    // };

    // mainWindow.draw(line, 2, sf::Lines);
    // double arrLx = Ex + sqrt(2)/2 * circShape * ARROW_SCALE * e45tx;
    // double arrLy = Ey + sqrt(2)/2 * circShape * ARROW_SCALE * e45ty;

    // double arrRx = Ex - sqrt(2)/2 * circShape * ARROW_SCALE * e45fx;
    // double arrRy = Ey - sqrt(2)/2 * circShape * ARROW_SCALE * e45fy;

    // // draw the arrow
    // sf::ConvexShape polygon;
    // polygon.setPointCount(3);
    // polygon.setPoint(0, sf::Vector2f(arrLx, arrLy));
    // polygon.setPoint(1, sf::Vector2f(Ex, Ey));
    // polygon.setPoint(2, sf::Vector2f(arrRx, arrRy));
    // polygon.setFillColor(colormap["Arrow"]);
    // mainWindow.draw(polygon);
    // cout << "polygon \n";
    // cout << "arrL coord: ";
    // outCoord(cout,arrLx,arrLy)<< endl;
    // cout << "arrR coord: ";
    // outCoord(cout,arrRx,arrRy) << endl;

    // // draw the text
    // sf::Font font;
    // if(!font.loadFromFile("font/BebasNeue-Regular.ttf"))
    // {
    //     cout << "the file could not found\n";
    //     exit(FONT_OPEN_ERROR);
    // }
    // sf::Text text;
    // // have to set Font or else the text wont be displayed
    // text.setFont(font);
    // text.setCharacterSize(charsize);
    // // std::string edgecost = std::to_string(cost).substr(0, std::to_string(cost).find(".") + PRECISION + 1);
    // // but all the graph have weights of integer
    // // so could use
    // string edgecost = std::to_string((size_t)cost);
    // text.setString(edgecost);
    // size_t len = edgecost.length();
    // text.setOrigin(sf::Vector2f(charsize * len / 2,charsize / 2));
    // double Tx = 1/2 * (fromX + toX) + circShape * TEXT_TO_LINE_SCALE * eVx;
    // double Ty = 1/2 * (fromY + toY) + circShape * TEXT_TO_LINE_SCALE * eVy;
    // text.setPosition(Tx,Ty);
    // cout << "Text Origin: ";
    // outCoord(cout,charsize * len / 2,charsize / 2);
    // cout << "Text Position: ";
    // outCoord(cout,Tx,Ty) << endl;
    // mainWindow.draw(text);


    // Assume there is an edge from vertex from to vertex to
    // Beware that the coordinate is like interchange the role of x and y
    // and rotate -90 degree 
    // so calculate in that manner
    // while during plotting, remember to enter the correct value of 
    // x and y value (interchange the two)
    coordinate fromCoord = getGraphVisPosition(from);
    coordinate toCoord = getGraphVisPosition(to);
    double fromY = fromCoord.first;
    double fromX = fromCoord.second;
    double toY = toCoord.first;
    double toX = toCoord.second;
    double distance = sqrt(pow((fromX - toX),2) + pow((fromY - toY),2));
    double lambda = circShape / distance;

    cout << "The from graph visual coord: (" << fromX << " , " << fromY << ")\n";
    cout << "The to graph visual coord: (" << toX << " , " << toY << ")\n";
    double Bx,By,Ex,Ey;
    double e45fx = sqrt(2)/2 * (((toX - fromX) - (toY - fromY))/distance);
    double e45fy = sqrt(2)/2 * (((toX - fromX) + (toY - fromY))/distance);
    cout << "e45f:";
    outCoord(cout,e45fy,e45fx)<< endl;
    // unit vector 90 degree rotated from e_l
    double eVx = (fromY - toY ) / distance;
    double eVy = (toX - fromX) / distance;
    cout << "eV:";
    outCoord(cout,eVy,eVx)<< endl;
    
    double e45tx = -e45fy;
    double e45ty = e45fx;
    cout << "e45t:";
    // outCoord(cout,e45tx,e45ty) << endl;
    outCoord(cout,e45ty,e45tx) << endl;
    if(IsEdge(to,from))
    {
        // bidirectional
        Bx = fromX + circShape * e45fx;
        By = fromY + circShape * e45fy;
        Ex = toX + circShape * e45tx;
        Ey = toY + circShape * e45ty;
    }
    else
    {
        Bx = fromX * (1 - lambda) + toX * lambda;
        By = fromY * (1 - lambda) + toY * lambda;
        Ex = fromX * lambda + toX * (1 - lambda);
        Ey = fromY * lambda + toY * (1 - lambda);

    }

    cout << "the B coord: ";
    outCoord(cout,By,Bx) << endl;
    cout << "the E coord: ";
    outCoord(cout,Ey,Ex) << endl;

    string typeName;
    // if(eds == EdgeStatus::UnknownEdge)
    // {
        
    // }
    if(IsEdgeOnPath(from,to))
    {
        typeName = "EdgeRoute";
    }
    else
    {

        switch (eds)
        {
        case EdgeStatus::UnknownEdge :
        {
            typeName = "UnknwonEdge";
            break;
        }
        case EdgeStatus::Visited :
        {
            typeName = "Visited";
            break;
        }
        case EdgeStatus::Active :
        {
            typeName = "EdgeActive";
            break;
        }
        case EdgeStatus::Optimal:
        {
            typeName = "Optimal";
            break;
        }
        default:
        {
            cout << "no other edge status.\n";
            exit(WRONT_EDGE_STATUS);
            break;
        }
        }
    }
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(By,Bx),colormap[typeName]),
        sf::Vertex(sf::Vector2f(Ey,Ex),colormap[typeName])
    };

    mainWindow.draw(line, 2, sf::Lines);
    double arrLx = Ex + sqrt(2)/2 * circShape * ARROW_SCALE * e45tx;
    double arrLy = Ey + sqrt(2)/2 * circShape * ARROW_SCALE * e45ty;

    double arrRx = Ex - sqrt(2)/2 * circShape * ARROW_SCALE * e45fx;
    double arrRy = Ey - sqrt(2)/2 * circShape * ARROW_SCALE * e45fy;

    // draw the arrow
    sf::ConvexShape polygon;
    polygon.setPointCount(3);
    polygon.setPoint(0, sf::Vector2f(arrLy, arrLx));
    polygon.setPoint(1, sf::Vector2f(Ey, Ex));
    polygon.setPoint(2, sf::Vector2f(arrRy, arrRx));
    polygon.setFillColor(colormap["Arrow"]);
    mainWindow.draw(polygon);
    cout << "polygon \n";
    cout << "arrL coord: ";
    // outCoord(cout,arrLx,arrLy)<< endl;
    outCoord(cout,arrLy,arrLx)<< endl;
    cout << "arrR coord: ";
    // outCoord(cout,arrRx,arrRy) << endl;
    outCoord(cout,arrRy,arrRx) << endl;
    // draw the text
    sf::Font font;
    if(!font.loadFromFile("font/BebasNeue-Regular.ttf"))
    {
        cout << "the file could not found\n";
        exit(FONT_OPEN_ERROR);
    }
    sf::Text text;
    // have to set Font or else the text wont be displayed
    text.setFont(font);
    text.setCharacterSize(charsize);
    // std::string edgecost = std::to_string(cost).substr(0, std::to_string(cost).find(".") + PRECISION + 1);
    // but all the graph have weights of integer
    // so could use
    string edgecost = std::to_string((size_t)cost);
    cout << "Edgecost : " << edgecost << endl;
    text.setString(edgecost);
    // have to set color or else won't display
    text.setFillColor(colormap["Text"]);
    size_t len = edgecost.length();


    // the coordinate below is the coordination scheme in the mainWindow 
    //  --------->  x axis
    //  |
    //  |
    //  |
    //  v  y axis
    float originX = charsize * len / 2.0;
    float originY = charsize / 2.0;
    text.setOrigin(sf::Vector2f(originX,originY));
    
    cout << "Calculate Tx: \n";
    cout << "fromX + toX = " << fromX << " + " << toX << endl;
    cout << "1/2 ans = " << 1.0/2.0 * (fromX + toX) << endl;
    cout << "circShape * TEXT_TO_LINE_SCALE * eVx = " << circShape * TEXT_TO_LINE_SCALE * eVx << endl;
    // have to 1.0/2.0  if 1/2 will get 0
    double Tx = 1.0/2.0* (fromX + toX) + circShape * TEXT_TO_LINE_SCALE * eVx;
    double Ty = 1.0/2.0 * (fromY + toY) + circShape * TEXT_TO_LINE_SCALE * eVy;
    text.setPosition(Ty,Tx);
    cout << "Text Origin: ";
    outCoord(cout,originX,originY) << endl;
    cout << "Text Position: ";
    // outCoord(cout,Tx,Ty) << endl;
    outCoord(cout,Ty,Tx) << endl;
    mainWindow.draw(text);

}
void RouteVisualizer::drawVertex(VertexT v,VertexStatus vSt,CostTgh cGH)
{
    sf::CircleShape vcirc(circShape);
    string typeName;
    cout << "In drawVertex, The vertex : " << v << endl;
    if(v == start)
    {
        typeName = "Start";
    }
    else if(v == destination)
    {
        typeName = "Destination";
    }
    else if(predecessors[v] == ON_PATH)
    {
        typeName = "Route";
    }
    else
    {
        switch(vSt)
        {
            case VertexStatus::UnknownVertex :
            {
                typeName = "UnknwonVertex";
                break;
            }
            case VertexStatus::InQueue :
            {
                typeName = "InQueue";
                break;
            }
            case VertexStatus::Active :
            {
                typeName = "VertexActive";
                break;
            }
            case VertexStatus::Done :
            {
                typeName = "Done";
                break;
            }
            default :
            {
                cout << "VertexStatus error, no such type\n";
                exit(WRONG_VERTEX_STATUS);
                break;
            }
        }
    }
    vcirc.setFillColor(colormap[typeName]);
    vcirc.setOrigin(vcirc.getRadius(),vcirc.getRadius());
    vcirc.setPosition(getPosition(v));
    mainWindow.draw(vcirc);
    if(v == start)
    {
        // don't need to draw the text
        return;
    }
    // drawing the text
    coordinate vCoord = getGraphVisPosition(v);

    double vX = vCoord.second;
    double vY = vCoord.first;
    srand (time(NULL));
    int deg = rand() % 1000;
    double theta = deg / 180.0 * pi;
    cout << "the degree: " << deg << endl;
    cout << "the theta: " << theta << endl;
    double lRx = circShape * cos(theta) * VERTEX_TEXT_TO_RADIUS_SCALE;
    double lRy = circShape * sin(theta) * VERTEX_TEXT_TO_RADIUS_SCALE;
    cout << "the lRx: " << lRx << endl;
    cout << "the lRy: " << lRy << endl;

    double textx = vX + lRx;
    double texty = vY + lRy;
    cout << "text position: ";
    outCoord(cout,texty,textx) << endl;
    sf::Font font;
    if(!font.loadFromFile("font/BebasNeue-Regular.ttf"))
    {
        cout << "the file could not found\n";
        exit(FONT_OPEN_ERROR);
    }
    sf::Text text;
    // have to set Font or else the text wont be displayed
    text.setFont(font);
    text.setCharacterSize(charsize);
    // std::string edgecost = std::to_string(cost).substr(0, std::to_string(cost).find(".") + PRECISION + 1);
    // but all the graph have weights of integer
    // so could use
    CostT g = cGH.first;
    CostT h = cGH.second;

    string gcost =  std::to_string(g).substr(0, std::to_string(g).find(".") + PRECISION + 1);
    string hcost =  std::to_string(h).substr(0, std::to_string(h).find(".") + PRECISION + 1);
    string edgecost = "(" + gcost + ","+hcost +")";
    cout << "Edgecost : " << edgecost << endl;
    
    text.setString(edgecost);
    // have to set color or else won't display
    text.setFillColor(colormap["Text"]);
    size_t len = edgecost.length();
    cout << "Edgecost length: " << len << endl;
    // the coordinate below is the coordination scheme in the mainWindow 
    //  --------->  x axis
    //  |
    //  |
    //  |
    //  v  y axis
    float originX = charsize * len / 2.0;
    float originY = charsize / 2.0;
    text.setOrigin(sf::Vector2f(originX,originY));
    cout << "Text gh origin: " ;
    outCoord(cout,originX,originY) << endl;
    text.setPosition(texty,textx);
    cout << "Text gh position: " ;
    outCoord(cout,texty,textx) << endl;
    mainWindow.draw(text);
}
// Zeichne den aktuellen Zustand des Graphen.
void RouteVisualizer::draw()
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
        size_t UpperBound = coorG.numVertices();
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
    }    size_t bound = coorG.numVertices();
    for(VertexT v = 0; v < bound; v++)
    {
        drawVertex(v,vectInfo[v].first,vectInfo[v].second);
        vectVertexEdgeInfo::const_iterator iter;
        for(iter = v_eI[v].begin(); iter != v_eI[v].end(); iter++)
        {
            drawEdge(v,(*iter).first,(*iter).second.first,(*iter).second.second);
        }
    }
     if(!PathFound)
    {
        mainWindow.display();
        sf::sleep(sf::seconds(0.5));
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
// draw the protetype of the maze
void RouteVisualizer::draw_raw()
{
    size_t bound = coorG.numVertices();
    for(VertexT v = 0; v < bound; v++)
    {
        // sf::CircleShape vcirc(circShape);
        // vcirc.setFillColor(colormap["UnknownVertex"]);
        // vcirc.setOrigin(vcirc.getRadius(),vcirc.getRadius());
        // vcirc.setPosition(getPosition(v));
        
        // mainWindow.draw(vcirc);
        drawVertex(v,VertexStatus::InQueue,CostTgh(10,2.56));
        vectVertexEdgeInfo::const_iterator iter;
        for(iter = v_eI[v].begin(); iter != v_eI[v].end(); iter++)
        {
            drawEdge(v,(*iter).first,(*iter).second.first);
        }
    }
    mainWindow.display();
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
ostream & outCoord(ostream & os,double x1,double x2)
{
    cout << "(" << x1 << ","<< x2 << ")";
    return os;
}