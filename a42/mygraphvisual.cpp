#include "mygraphvisual.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
mapRGB MazeVisualizer::colormap = 
{
    {"LightBlack",sf::Color(150,150,150)},
    {"Black",sf::Color(0,0,0)},
    {"White",sf::Color::White},
    {"Start",sf::Color::Yellow},
    {"Destination",sf::Color(240,190,45)},
    {"InQueue",sf::Color(100,100,150)},
    {"Done",sf::Color(10,100,10)},
    {"Active",sf::Color(255,10,10)},
    {"Text",sf::Color(135,250,250)}
};
MazeVisualizer::MazeVisualizer(MazeGraph & mz, VertexT & st, VertexT & end,unsigned int modeWidth,unsigned int modeHeight)
: mainWindow(sf::VideoMode(modeWidth,modeHeight),"mywindow"),mzGraph(mz),start(st),destination(end),windowWidth(modeWidth),windowHeight(modeHeight)
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
: mainWindow(sf::VideoMode(modeWidth,modeHeight),"mywindow"),mzGraph(mz),windowWidth(modeWidth),windowHeight(modeHeight)
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
    // vectInfo.clear();
    // vectInfo.resize(mzGraph.numVertices());
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
    draw();
}

// // Zeichne den aktuellen Zustand des Graphen.
void MazeVisualizer::draw()
{
    sf::Font font;
    if(!font.loadFromFile("font/BebasNeue-Regular.ttf"))
    {
        cout << "the file could not found\n";
        exit(FONT_OPEN_ERROR);
    }
    
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
                text.setFont(font);
                size_t charsi = charsize/2;
                cout << "charsize is: " << charsi << endl;
                if(charsize == 0)
                {
                    cout << "The char size is too small\n";
                    cout << "Using size = 1\n";
                    charsize = 1;
                }
                
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
                cout << "charsize is: " << charsi << endl;
                if(charsize == 0)
                {
                    cout << "The char size is too small\n";
                    cout << "Using size = 1\n";
                    charsize = 1;
                }
                text.setCharacterSize(charsi);
                text.setString("Goal");
                text.setPosition(sf::Vector2f(widthCoor,heightCoor));
                text.setFillColor(colormap["Text"]);
                mainWindow.draw(text);
            }
            else 
            {

                sf::RectangleShape rect(sf::Vector2f(rectShape.first,rectShape.second));
                if(vectInfo[currentNode].first == VertexStatus::InQueue)
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
                cout << "charsize is: " << charsize << endl;
                if(charsize == 0)
                {
                    cout << "The char size is too small\n";
                    cout << "Using size = 1\n";
                    charsize = 1;
                }
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
    mainWindow.display();
    sf::sleep(sf::seconds(0.1));
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