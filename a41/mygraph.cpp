#include "mygraph.h"

typedef DistanceGraph::NeighborT neighT;
typedef DistanceGraph::LocalEdgeT pa_verCost;
// help function for the maze debug
void outCellType(ostream & os,CellType & ct)
{
    if(ct == CellType::Wall)
    {
        os << "Type is Wall\n";
    }
    else if(ct == CellType::Ground)
    {
        os << "Type is Ground\n";
    }
    else
    {
        os << "Other type\n";
    }
}
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
CostT DistSphereGraph::estimatedCost(VertexT from, VertexT to) const
{
    if(from < 0 || from > vertexCount || to < 0 || to > vertexCount)
    {
        cout << "Wrong index of vertex in CoGraph:estiCos\n";
        exit(WRONG_VERTEX_INDEX);
    }
    double fromx = deg2rad(vcMap.at(from).first); // can't use [] operator here because expected is a const pair
    // no operator "[]" matches these operands -- operand types are: const VertexCoord [ VertexT ]
    double fromy = deg2rad(vcMap.at(from).second);
    double tox = deg2rad(vcMap.at(to).first);
    double toy = deg2rad(vcMap.at(to).second);
    // x dist squared
    CostT dist1 = pow(cos(fromx)*cos(fromy)-cos(tox)*cos(toy),2);
    // y dist squared
    CostT dist2 = pow(cos(fromx)*sin(fromy)-cos(tox)*sin(toy),2);
    // z dist squared
    CostT dist3 = pow(sin(fromx) - sin(tox),2);
    CostT distance = radius * sqrt(dist1 + dist2 + dist3);
    return distance;
}
CostT TimeCoordGraph::estimatedCost(VertexT from, VertexT to) const
{
    if(from < 0 || from > vertexCount || to < 0 || to > vertexCount)
    {
        cout << "Wrong index of vertex in CoGraph:estiCos\n";
        exit(WRONG_VERTEX_INDEX);
    }
    double fromx = deg2rad(vcMap.at(from).first); // can't use [] operator here because expected is a const pair
    // no operator "[]" matches these operands -- operand types are: const VertexCoord [ VertexT ]
    double fromy = deg2rad(vcMap.at(from).second);
    double tox = deg2rad(vcMap.at(to).first);
    double toy = deg2rad(vcMap.at(to).second);
    // x dist squared
    CostT dist1 = pow(cos(fromx)*cos(fromy)-cos(tox)*cos(toy),2);
    // y dist squared
    CostT dist2 = pow(cos(fromx)*sin(fromy)-cos(tox)*sin(toy),2);
    // z dist squared
    CostT dist3 = pow(sin(fromx) - sin(tox),2);
    CostT distance = radius * sqrt(dist1 + dist2 + dist3);
    CostT time = distance/vehicleSpeed * Hour2Min;
    return time;
}


// MazeGraph

void MazeGraph::setNeighbors()
{
    if(width < 1 || height < 1)
    {
        cout << "The maze is of invalid size\n";
        exit(INVALID_SIZE);
    }
    if(v_vC.empty())
    {
        cout << "The v_vC vector empty cant set neighbors\n";
        exit(INITIALIZING_ERROR);
    }
    for(VertexT v = 0; v < vertexCount; v++)
    {
        // the v is a Wall
        // no neighbors continue
        if(v_vC[v] == CellType::Wall)
        {
            continue;
        }
        neighT& vec = neighbour_vector[v];
        pa_verCost mzcod = Vertex2mzCoord(v);
        size_t w = mzcod.first;
        size_t h = mzcod.second;
        int index[3] = {-1,1};
        for(size_t i = 0; i < 2; i++)
        {
            // according to aufgabestellung
            // there are maximal 4 neighbours
            // the 1 ~ 4 are neighbours of 0
            //   *  1  *
            //   2  0  3
            //   *  4  *
            // be sure that setNeighbors will be called only once
            // because the following push back mechanism doesn't exam
            // whether the same node will be added twice 
            if(ismzCodValid(w+index[i],h))
            {
                if(at(w+index[i],h) == CellType::Ground)
                {
                    vec.push_back(pa_verCost(mzCoord2VertexT(w+index[i],h),1));
                }
            }
            if(ismzCodValid(w,h+index[i]))
            {
                if(at(w,h+index[i]) == CellType::Ground)
                {
                    vec.push_back(pa_verCost(mzCoord2VertexT(w,h+index[i]),1));
                }
            }
        }
    }
    
}
bool MazeGraph::isBorder(VertexT v) const
{
    if(!isValid(v))
    {
        cout << "wrong index of vertex\n";
        exit(WRONG_VERTEX_INDEX);
    }
    // if v at left border || v at right border || v at upper border || v at down border
    if((v % width == 0) || (v % width == (width - 1)) ||(v < width) || (v > width * (height-1)))
    {
        return true;
    }
    return false;
}
bool MazeGraph::isLeftBorder(VertexT v) const
{
    // can actually eliminate this test isBorder
    // if one can be sure that this function is called
    // if isBorder being true is confirmed
    if(!isBorder(v))
    {
        cout << "Not at border\n";
        exit(WRONG_POSITION);
    }
    return (v % width == 0);

}
bool MazeGraph::isUpBorder(VertexT v) const
{
    // can actually eliminate this test isBorder
    // if one can be sure that this function is called
    // if isBorder being true is confirmed
    if(!isBorder(v))
    {
        cout << "Not at border\n";
        exit(WRONG_POSITION);
    }
    return (v < width);
}
bool MazeGraph::isRightBorder(VertexT v) const
{
    // can actually eliminate this test isBorder
    // if one can be sure that this function is called
    // if isBorder being true is confirmed
    if(!isBorder(v))
    {
        cout << "Not at border\n";
        exit(WRONG_POSITION);
    }
    return (v % width == width - 1);
}
bool MazeGraph::isCorner(VertexT v) const
{
    if(!isValid(v))
    {
        cout << "wrong index of vertex\n";
        exit(WRONG_VERTEX_INDEX);
    }
    // if v at upper left corner || v at upper right corner || v at down left corner || v at down right corner
    if((v == 0) || (v == (width - 1)) || (v == width * (height-1)) || (v == width * (height - 1)))
    {
        return true;
    }
    return false;
}
bool MazeGraph::isUpCorner(VertexT v) const
{
    if(!isCorner(v))
    {
        cout << "Not at the corner\n";
        exit(WRONG_POSITION);
    }
    return (v < width);
}
mzCoord MazeGraph::Vertex2mzCoord(VertexT v) const
{
    if(!isValid(v))
    {
        cout << "The VertexT v is " << v << endl;
        cout << "in Vertex2mzCoord Invalid index, cant convert to mzCoord\n";
        exit(WRONG_VERTEX_INDEX);
    }
    // the index is analog to in matlab
    // i.e index begins at 1, so we dont have to worry
    // issue like for a size_t 0 - 1 is very large
    size_t h = v / width + 1;
    size_t w = v % width + 1;
    return mzCoord(w,h);
}
    // mzCoord in primitive version, i.e two size_t
VertexT MazeGraph::mzCoord2VertexT(size_t w, size_t h) const
{
    if(!ismzCodValid(w,h))
    {
        cout << "Invalid mzCoord, cant convert to vertex\n";
        exit(WRONG_VERTEX_INDEX);
    }
    // cout << "The w: " << w << "  the h: " << h << endl;
    // cout << "The width of the graph:\n";
    // cout << "width: " << width << cout << " height : " << height << endl;
    // VertexT v = (h - 1) * width + w - 1;
    // cout << "the v is " << v;
    // one error:  should be (h - 1) * width + width - 1
    return (h - 1) * width + w - 1;
}
bool MazeGraph::ismzCodValid(size_t w, size_t h) const
{
    if(w < 1 || w > width || h < 1 || h > height)
    {
        return false;
    }
    return true;
}
const CellType& MazeGraph::at(size_t w, size_t h) const
{
    VertexT v = mzCoord2VertexT(w,h);
    return v_vC[v];
}
CellType& MazeGraph::operator()(size_t w, size_t h)
{
    VertexT v = mzCoord2VertexT(w,h);
    return v_vC[v];
}
const DistanceGraph::NeighborT& MazeGraph::getNeighbors( VertexT v) const
{
    if(!isValid(v))
    {
        cout << "Invalid vertex index\n";
        exit(WRONG_VERTEX_INDEX);
    }
    
    return neighbour_vector[v];

    // if(width >= 3 && height >= 3)
    // {
    //     int situation;
    //     // the following is for width >=3, height >= 3
    //     // the situation is defined as followed
    //     // 1 2 2 2 2 3
    //     // 4 5 5 5 5 6
    //     // 4 5 5 5 5 6
    //     // 7 8 8 8 8 9

    //     if(isBorder(v))
    //     {
    //         if(isLeftBorder(v))
    //         {
    //             // if v at the upper left corner
    //             if(v == 0)
    //             {
    //                 situation = 1;
    //             }
    //             // if v at the down left corner
    //             else if(v == width * (height - 1))
    //             {
    //                 situation = 7;
    //             }
    //             else
    //             {
    //                 situation = 4;
    //             }
    //         }
    //         else if(isUpBorder(v))
    //         {
    //             // if v at the upper right corner
    //             if(v == width - 1)
    //             {
    //                 situation = 3;
    //             }
    //             else
    //             {
    //                 situation = 2;
    //             }
    //         }
    //         else if(isRightBorder(v))
    //         {
    //             if(v == width * height - 1)
    //             {
    //                 situation = 9;
    //             }
    //             else
    //             {
    //                 situation = 6;
    //             }
    //         }
    //         else
    //         {
    //             situation = 8;
    //         }
    //     }
    //     else
    //     {
    //         situation = 5;
    //     }
    //     switch (situation)
    //     {
    //         // since width >= 3 && height >= 3 
    //         // the following e.g v+1, v+width is always valid index
    //     case 1:
    //         if(v_vC[v + 1] == CellType::Ground)
    //         {
    //             vec.push_back(pa_verCost(v+1,1));
    //         }
    //         if(v_vC[v + width] == CellType::Ground)
    //         {
    //             vec.push_back(pa_verCost(v+width,1));
    //         }
    //         break;
    //     case 2:
    //         if(v_vC[v - 1] == CellType::Ground)
    //         {
    //             vec.push_back(pa_verCost(v-1,1));
    //         }
    //         if(v_vC[v + 1] == CellType::Ground)
    //         {
    //             vec.push_back(pa_verCost(v+1,1));
    //         }
    //         if(v_vC[v+width] == CellType::Ground)
    //         {
    //             vec.
    //         }
    //     default:
    //         break;
    //     }
    // }
}
CostT MazeGraph::estimatedCost( VertexT from, VertexT to) const
{
    // if one of the vertices is a wall there is no path to it
    if(v_vC[from] == CellType::Wall || v_vC[to] == CellType::Wall)
    {
        return infty;
    }
    mzCoord mzC1 = Vertex2mzCoord(from);
    mzCoord mzC2 = Vertex2mzCoord(to);


    double cost_hori = abs(mzC1.first - mzC2.first);
    double cost_verti = abs(int(mzC1.second - mzC2.second));
    return cost_hori + cost_verti;
}
CostT MazeGraph::cost( VertexT from, VertexT to) const
{
    if(!isValid(from) || !isValid(to))
    {
        cout << "Invalid vertice in mzGraph::cost\n";
        exit(WRONG_VERTEX_INDEX);
    }
    DistanceGraph::NeighborT::const_iterator iter;
    for(iter = neighbour_vector[from].begin(); iter != neighbour_vector[from].end(); iter++)
    {
        if(iter -> first == to)
        {
            return iter -> second;
        }
    }
    return infty;
}
istream & operator>>(istream & is, MazeGraph & mz)
{
    is >> mz.height;
    is >> mz.width;
    mz.vertexCount = mz.width * mz.height;
    // cout << "The height: " << mz.height << " the width: " << mz.width << endl;
    mz.neighbour_vector.resize(mz.vertexCount);
    // cout << "neibor vectors 's size after resize is: " << mz.neighbour_vector.size();
    mz.v_vC.resize(mz.vertexCount);
    // cout << "v_vC 's size after resize is: " << mz.v_vC.size();
    char ch;
    size_t height = mz.height;
    size_t width = mz.width;
    // initializing the v_vC, setting the celltype according 
    // to the file to be read
    for(size_t h = 1; h <= height; h++)
    {
        for(size_t w = 1; w <= width; w++)
        {
            is >> ch;
            switch(ch)
            {
                case '#':
                    mz(w,h) = CellType::Wall;
                    // cout << " \n# read set to wall\n";
                    // cout << "The mz(" << w << ", " << h << "): " ;
                    // outCellType(cout,mz(w,h));
                    // cout << "The vertexT v: " << mz.mzCoord2VertexT(w,h)<< endl;
                    // cout << "The corresponding v_vC[v]: ";
                    // outCellType(cout,mz.v_vC[mz.mzCoord2VertexT(w,h)]);
                    break;
                case '.':
                    mz(w,h) = CellType::Ground;
                    // cout << " \n. read set to ground\n";
                    // cout << "The mz(" << w << ", " << h << "): " ;
                    // outCellType(cout,mz(w,h));
                    // cout << "The vertexT v: " << mz.mzCoord2VertexT(w,h)<< endl;
                    // cout << "The corresponding v_vC[v]: ";
                    // outCellType(cout,mz.v_vC[mz.mzCoord2VertexT(w,h)]);
                    break;
                default:
                    cout << "types other than # and . read\n";
                    exit(READ_ERROR);
                    break;
            }
        }
    }
    // cout << "\nAfter reading in\n";
    // cout << "The data of v_vC\n";
    // v_Cell::const_iterator iter;
    // int count = 1;
    // for(iter = mz.v_vC.begin(); iter != mz.v_vC.end();iter++)
    // {
    //     cout.width(4);
    //     if((*iter) == CellType::Wall)
    //     {
    //         cout << '#';
    //     }
    //     else if((*iter) == CellType::Ground)
    //     {
    //         cout << '.';
    //     }
    //     if(count % width == 0)
    //     {
    //         cout << endl;
    //     }
    //     count++;
    // }
    mz.setNeighbors();
    // cout << "\nAfter setting Neighbors\n";
    // // v_Cell::const_iterator iter;
    // count = 1;
    // for(iter = mz.v_vC.begin(); iter != mz.v_vC.end();iter++)
    // {
    //     cout.width(4);
    //     if((*iter) == CellType::Wall)
    //     {
    //         cout << '#';
    //     }
    //     else if((*iter) == CellType::Ground)
    //     {
    //         cout << '.';
    //     }
    //     if(count % width == 0)
    //     {
    //         cout << endl;
    //     }
    //     count++;
    // }
    return is;
}

ostream & operator<<(ostream & os, MazeGraph & mz)
{
    
    if(mz.isEmpty())
    {
        cout << "mz is empty cant output\n";
        exit(GRAPH_EMPTY);
    }
    size_t width = mz.width;
    size_t height = mz.height;
    // os << "first the v_vC\n";
    // v_Cell::const_iterator iter;
    // int count = 1;
    // for(iter = mz.v_vC.begin(); iter != mz.v_vC.end();iter++)
    // {
    //     cout.width(4);
    //     if((*iter) == CellType::Wall)
    //     {
    //         cout << '#';
    //     }
    //     else if((*iter) == CellType::Ground)
    //     {
    //         cout << '.';
    //     }
    //     if(count % width == 0)
    //     {
    //         cout << endl;
    //     }
    //     count++;
    // }

    
    for(size_t h = 1; h <= height; h++)
    {
        if(h == 1)
        {
            for(size_t w = 1; w <= width; w++)
            {
                os.width(4);
                os << w;
            }
            os << "\n--------------------------------------------\n";
        }
        os.width(2);
        os << h;
        for(size_t w = 1; w <= width; w++)
        {
            
            if(w == 1)
            {
                os.width(2);
            }
            else
            {
                os.width(4);
            }
            
            if(mz.at(w,h) == CellType::Wall)
            {
                os << '#';
            }
            else
            {
                os << '.';
            }
        }
        os << endl;
    }
    os << "\nThe neighbors of each vertex\n";
    os.width(10);
    os << "width";
    
    os.width(10);
    os << "height";
    
    
    os.width(20);
    os << "neighbors(w,h)\n";
    for(size_t h = 1; h <= height; h++)
    {
        for(size_t w = 1; w <= width; w++)
        {
            os.width(10);
            os << w;
            os.width(10);
            os << h <<'\t';
            neighT neV = mz.getNeighbors(mz.mzCoord2VertexT(w,h));
            neighT::const_iterator iter;
            os.width(3);
            for(iter = neV.begin(); iter != neV.end();iter++)
            {
                VertexT vert = (*iter).first;
                mzCoord mzC = mz.Vertex2mzCoord(vert);
                os << "("<< mzC.first << "," << mzC.second<<") ";
            }
            os << endl;
        }
    }
    // os << "In ostream << \n";
    // os << "Testing whether we can change the mz's v_vC\n";
    // // mz.v_vC[0] = CellType::Ground;
    // mz.v_vC[1] = CellType::Ground;
    // mz.v_vC[width] = CellType::Ground;
    // mz.v_vC[width+3] = CellType::Ground;
    // mz.setNeighbors();
    // os << "\nAfter modifying by hand\n";
    // for(size_t h = 1; h <= height; h++)
    // {
    //     if(h == 1)
    //     {
    //         for(size_t w = 1; w <= width; w++)
    //         {
    //             os.width(4);
    //             os << w;
    //         }
    //         os << "\n--------------------------------------------\n";
    //     }
    //     os.width(2);
    //     os << h;
    //     for(size_t w = 1; w <= width; w++)
    //     {
            
    //         if(w == 1)
    //         {
    //             os.width(2);
    //         }
    //         else
    //         {
    //             os.width(4);
    //         }
            
    //         if(mz.at(w,h) == CellType::Wall)
    //         {
    //             os << '#';
    //         }
    //         else
    //         {
    //             os << '.';
    //         }
    //     }
    //     os << endl;
    // }
    // for(size_t h = 1; h <= height; h++)
    // {
    //     for(size_t w = 1; w <= width; w++)
    //     {
    //         os.width(10);
    //         os << h;
    //         os.width(10);
    //         os << w <<'\t';
    //         neighT neV = mz.getNeighbors(mz.mzCoord2VertexT(w,h));
    //         neighT::const_iterator iter;
    //         for(iter = neV.begin(); iter != neV.end();iter++)
    //         {
    //             VertexT vert = (*iter).first;
    //             mzCoord mzC = mz.Vertex2mzCoord(vert);
    //             os << "("<< mzC.first << "," << mzC.second<<") ";
    //         }
    //         os << endl;
    //     }
    // }


    return os;
}