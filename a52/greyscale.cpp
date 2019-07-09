#include "greyscale.h"

// help function for the Convolve()
int MaskCoord2Vec(int i, int j,int size);
int MaskSize;
vec_Codes vec_C;

// help function for the min heap building in the BuildTree
struct CompareMyTree
{
    bool operator()(MyTree * lTree,  MyTree * rTree) const
    {
        if(lTree -> GetFrequency() > rTree -> GetFrequency())
        {
            return true;
        }
        else if(lTree -> GetFrequency() == rTree -> GetFrequency())
        {
            if(lTree -> GetGreyValue() > rTree -> GetGreyValue())
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }

    // bool operator()(const MyTree & lTree, const MyTree & rTree) const
    // {
    //     if(lTree.GetFrequency() > rTree.GetFrequency())
    //     {
    //         return true;
    //     }
    //     else if(lTree.GetFrequency() == rTree.GetFrequency())
    //     {
    //         if(lTree.GetGreyValue() > rTree.GetGreyValue())
    //         {
    //             return true;
    //         }
    //     }
    //     else
    //     {
    //         return false;
    //     }
    // }

    // bool operator()(const MyTree & lTree, const MyTree & rTree) const
    // {
    //     cout << "The left freq: " << lTree.GetFrequency() << endl;
    //     cout << "The left grey va: " << lTree.GetGreyValue() << endl;
    //     cout << "The right freq: " << rTree.GetFrequency() << endl;
    //     cout << "The right grey va: " << rTree.GetGreyValue() << endl;
    //     if(lTree.GetFrequency() < rTree.GetFrequency())
    //     {
    //         cout << "True returned\n";
    //         return true;
    //     }
    //     else if(lTree.GetFrequency() == rTree.GetFrequency())
    //     {
    //         if(lTree.GetGreyValue() < rTree.GetGreyValue())
    //         {
    //             cout << "True returned\n";
    //             return true;
    //         }
    //     }
    //     else
    //     {
    //         cout << "False returned\n";
    //         return false;
    //     }
    // }
};
// bool sortCompareTree(const MyTree & lTree, const MyTree & rTree)

void SwapTree(MyTree & lTree, MyTree & rTree)
{
    MyTree temp = lTree;
    lTree = rTree;
    rTree = temp;
}

int GreyScale::Format = 0;
GreyScale::GreyScale(int w, int h,string name):
width(w),height(h),magicNumber(name)
{
    if(width * height)
    {
        // pixels = new float[width * height];
        pixels.resize(width * height);
    }
    // SetFormat(form);
}
GreyScale::GreyScale(const GreyScale & gs)
{
    width = gs.width;
    height = gs.height;
    magicNumber = gs.magicNumber;
    // if(pixels != nullptr)
    // {
    //     delete [] pixels;
    // }
    // pixels.clear();
    // pixels = new float [width * height];
    int sizeBild = width * height;
    pixels.clear();
    pixels.resize(width * height);
    for(int i = 0; i < sizeBild; i++)
    {
        pixels[i] = gs.pixels[i];
    }
}
GreyScale::~GreyScale()
{
    // if(pixels != nullptr)
    // {
    //     delete [] pixels;
    // }
}

void GreyScale::Resize(int w, int h)
{
    // if(pixels != nullptr)
    // {
    //     delete [] pixels;
    // }
    pixels.clear();
    width = w;
    height = h;
    if(width * height > 0)
    {
        // pixels = new float[width * height];
        pixels.resize(width * height);
    }
}

void GreyScale::SetFormat(int form)
{
    Format = form;
    // cout << "After setting format, the format is: "<< Format << endl;
    // cout << "The GreyScale::Format is " << GreyScale::Format << endl;
    // switch(Format)
    // {
    //     case 0:
    //     {
    //         magicNumber = "P2";
    //         break;
    //     }
    //     case 1:
    //     {
    //         magicNumber = "P5";
    //         break;
    //     }
    //     case 2:
    //     {
    //         magicNumber = "MHa";
    //         break;
    //     }
    //     case 3:
    //     {
    //         magicNumber = "MHb";
    //         break;
    //     }
    //     default:
    //     {
    //         cout << "Inappropriate format value.\n";
    //         exit(WRONG_FORMAT);
    //     }

    // }

}

void GreyScale::MagicSetFormat()
{
    if(magicNumber == "P2")
    {
        Format = 0;
    }
    else if(magicNumber == "P5")
    {
        Format = 1;
    }
    else if(magicNumber == "MHa")
    {
        Format = 2;
    }
    else if(magicNumber == "MHb")
    {
        Format = 3;
    }
    else
    {
        cout << "The magic number is invalid\n";
        exit(WRONG_FORMAT);
    }
    // cout << "After magicSetFormat the GreyScale::Format is: " << GreyScale::Format << endl;
}

float GreyScale::operator()(int i, int j) const
{
    // the point outside the picture return the neighouring valid point
    if(i < 0)
    {
        if(j < 0)
        {
            // the left up side of the (0,0)
            return pixels[0];
        }
        else if(j < height)
        {
            return pixels[XYCoord2Vec(0,j)];
        }
        else
        {
            // the left down side of (0,height-1)
            return pixels[XYCoord2Vec(0,height-1)];
        }
    }
    else if( i >= width)
    {
        if(j < 0)
        {
            return pixels[width - 1];
        }
        else if(j < height)
        {
            return pixels[XYCoord2Vec(width-1,j)];
        }
        else
        {
            return pixels[width * height - 1];
        }
    }
    else if(j < 0)
    {
        return pixels[i];
    }
    else if(j > height -1)
    {
        return pixels[XYCoord2Vec(i,height - 1)];
    }
    else
    {
        // valid point
        return pixels[XYCoord2Vec(i,j)];
    }
    
    
    int index = XYCoord2Vec(i,j);
    return pixels[index];
}
int GreyScale::XYCoord2Vec(int i, int j) const
{
    // assume that the coord is valid
    return j * width + i;
}
XYCoord GreyScale::VecCoord2XY(int index) const
{
    // assume that the index is valid
    int i = index % width;
    int j = index / width;
    return XYCoord(i,j);
}
GreyScale & GreyScale::operator=(const GreyScale & gs)
{
    if(this == &gs)
    {
        return *this;
    }
    // delete [] pixels; // safe to delete nullptr so dont check that
    pixels.clear();
    width = gs.width;
    height = gs.height;
    // pixels = new float[width * height];
    pixels.resize(width * height);
    int sizeBild = width * height;
    for(int i = 0; i < sizeBild; i++)
    {
        pixels[i] = gs.pixels[i];
    }
    return *this;
}
GreyScale & GreyScale::operator+=(const GreyScale & gs)
{
    if(width != gs.width || height != gs.height)
    {
        cout << "size mismatch! can't perform += operation\n";
        exit(SIZE_MISMATCH);
    }
    int sizeBild = width * height;
    for(int index = 0; index < sizeBild; index++)
    {
        pixels[index] += gs.pixels[index];
    }
    return *this;
}
GreyScale & GreyScale::operator-=(const GreyScale & gs)
{
    if(width != gs.width || height != gs.height)
    {
        cout << "size mismatch! can't perform -= operation\n";
        exit(SIZE_MISMATCH);
    }
    int sizeBild = width * height;
    for(int index = 0; index < sizeBild; index++)
    {
        pixels[index] -= gs.pixels[index];
    }
    return *this;
}
GreyScale GreyScale::Binarize(float c) const
{
    GreyScale gs(width,height);
    int sizeBild = width * height;
    if(!sizeBild)
    {
        return gs;
    }
    for(int index = 0; index < sizeBild; index++)
    {
        if(pixels[index] < c)
        {
            gs.pixels[index] = 0;
        }
        else
        {
            gs.pixels[index] = 1;
        }
    }
    return gs;
}
GreyScale GreyScale::Blur() const
{
    // GreyScale gs(width,height);
    // int sizeBild = width * height;
    // if(!sizeBild)
    // {
    //     return gs;
    // }
    // for(int index = 0; index < sizeBild; index++)
    // {
    //     double sum = 0;
    //     XYCoord xyco = VecCoord2XY(index);
    //     int x_val = xyco.first;
    //     int y_val = xyco.second;
    //     for(int i = -1; i <= 1; i++)
    //     {
    //         for(int j = -1; j <= 1; j++)
    //         {
    //             // here simply calculate the imagined 9 values for the margin, additional 0 added
    //             sum += pixels[XYCoord2Vec(x_val + i,y_val + j)];
    //         }
    //     }
    //     gs.pixels[index] = sum / 9.0;
    // }
    // return gs;
    int size = 3;
    const float mask[size * size] = 
    {
        0, 0.2,0,0.2,0.2,0.2,0,0.2,0
    };
    return Convolve(mask,size);
}
GreyScale GreyScale::Clamp() const
{
    GreyScale gs(width,height);
    int sizeBild = width * height;
    if(!sizeBild)
    {
        return gs;
    }
    for(int index = 0; index < sizeBild; index++)
    {
        if(pixels[index] < 0)
        {
            gs.pixels[index] = 0;
        }
        else if(pixels[index] > 1)
        {
            gs.pixels[index] = 1;
        }
        else
        {
            gs.pixels[index] = pixels[index];
        }
        
    }
    return gs;
}
GreyScale GreyScale::Contrast() const
{
    // min of the value transformed to 0 maximum to 1
    int sBild = width * height;
    float max = pixels[0];
    float min = pixels[0];
    for(int index = 0; index < sBild; index++)
    {
        float temp = pixels[index];
        if(temp < min)
        {
            min = temp;
        }
        else if(temp > max)
        {
            max = temp;
        }
    }
    float a = 1/(max - min);
    float b = -min/(max - min);
    return LinTrans(a,b);
}
GreyScale GreyScale::Convolve(const float mask[],int size) const
{
    GreyScale gs(width,height);
    int sizeBild = width * height;
    if(!sizeBild)
    {
        return gs;
    }
    if(size % 2)
    {
        // the zero point is in the well defined middle
        int bound = size / 2; // the maximum offset from the center
        // MaskSize = size;
        for(int index = 0; index < sizeBild; index++)
        {
            float sum = 0;
            XYCoord xyco = VecCoord2XY(index);
            int x_val = xyco.first;
            int y_val = xyco.second;
            int z = 0,s = 0;
            for(int i = -bound; i <= bound; i++)
            {
                z = 0;
                for(int j = -bound; j <= bound; j++)
                {
                    // int maskIndex = MaskCoord2Vec(i,j,size);

                    // sum += (*this)(x_val + i,y_val + j) * mask[maskIndex];
                    sum += (*this)(x_val + i,y_val + j) * mask[z * size + s];
                    z++;
                }
                s++;
                
            }
            gs.pixels[index] = sum;
        }
        return gs;
    }
    else
    {
        cout << "The size of the mask is even, center not well defined\n";
        exit(EVEN_SIZE_MASK);
    }
    

}
GreyScale GreyScale::Kirsch() const
{
    int size = 3;
    const float mask[size * size] = 
    {
        1,3,3,-1,0,1,-3,-3,-1
    };
    return Convolve(mask,size);
}
GreyScale GreyScale::Laplace() const
{
    int size = 3;
    const float mask[size * size] = 
    {
        0,-1,0,-1,4,-1,0,-1,0
    };
    return Convolve(mask,size);
}
GreyScale GreyScale::LinTrans(float a, float b) const
{
    GreyScale gs(width,height);
    int sizeBild = width * height;
    if(!sizeBild)
    {
        return gs;
    }
    for(int index = 0; index < sizeBild; index++)
    {
        gs.pixels[index] = a * pixels[index] + b;
    }
    return gs;
}
GreyScale GreyScale::Invert() const
{
    return LinTrans(-1,1);
}
GreyScale GreyScale::Median() const
{
    GreyScale gs(width,height);
    int sizeBild = width * height;
    if(!sizeBild)
    {
        return gs;
    }
    for(int index = 0; index < sizeBild; index++)
    {
        gs.pixels[index] = pixelMedian(index);
    }
    return gs;
}
GreyScale GreyScale::Sobel() const
{
    GreyScale gs(width,height);
    int sizeBild = width * height;
    if(!sizeBild)
    {
        return gs;
    }
    float DX[9] =
    {
        -1,0,1,-2,0,2,-1,0,1
    };
    float DY[9] =
    {
        1,2,1,0,0,0,-1,-2,-1
    };
    int bound = 1;
    int size = 3;
    for(int index = 0; index < sizeBild; index++)
    {
        float sum1 = 0,sum2 = 0;
        XYCoord xyco = VecCoord2XY(index);
        int x_val = xyco.first;
        int y_val = xyco.second;
        for(int i = -bound; i <= bound; i++)
        {
            for(int j = -bound; j <= bound; j++)
            {
                int maskIndex = MaskCoord2Vec(i,j,size);
                sum1 += (*this)(x_val + i,y_val + j) * DX[maskIndex];
                sum2 += (*this)(x_val + i,y_val + j) * DY[maskIndex];
            }
        }
        gs.pixels[index] = sqrt(pow(sum1,2) + pow(sum2,2));
    }
    return gs;
}
float GreyScale::pixelMedian(int index) const
{
    vector<float> neighbours;
    XYCoord xycod = VecCoord2XY(index);
    int xcord = xycod.first;
    int ycord = xycod.second;
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            neighbours.push_back((*this)(xcord + i,ycord + j));
        }
    }
    sort(neighbours.begin(),neighbours.end());
    return neighbours[4];
}


int MaskCoord2Vec(int i, int j,int size)
{
    if(size % 2)
    {
        int bound = size / 2;
        // the index of the center
        int centerIndex = bound * size + bound;
        int index = centerIndex + j * size + i;
        return index;
    }
    else
    {
        cout << "The size of the mask is even, center not well defined\n";
        exit(EVEN_SIZE_MASK);
    }
    
}

istream & operator>>(istream & is, GreyScale & gs)
{
    // char ch = is.peek();
    // while(ch != EOF)
    // {
    //   if()
    // }
    getline(is,gs.magicNumber);
    gs.MagicSetFormat();
    is >> ws;
    char ch = is.peek();
    int width,height,colorstep;
    if(ch == '#')
    {
        while((ch = is.get()) != '\n')
            continue;
    }
    else if(!isdigit(ch))
    {
        cout << "Unexpected character in the input\n";
        cout << "The char is " << ch << endl;
        exit(UNEXPECTED_CHAR);
    }
    is >> width >> height;
    gs.Resize(width,height);
    is >> colorstep;
    // colorstep; // e.g read in 255 then there are 256 steps of colors
    int sizeBild = width * height;
    // float colors;
    vec_C.clear();
    vec_C.resize(sizeBild);
    switch(GreyScale::Format)
    {
        case 0:
        {
            greyValue colors;
            map_colorFreq::iterator it;
            // cout << "the case 0\n";
            for( int index = 0; index < sizeBild; index++)
            {
                
                is >> colors;
                vec_C[index] = colors;
                // cout << "index : " << index << "\t color: " << colors << endl;
                gs.pixels[index] = ((float)colors)/colorstep;
                it = gs.mapColFreq.find(colors);
                if(it != gs.mapColFreq.end())
                {
                    // cout << "color already exist, freqency: " << (it -> second) << endl;
                    (it -> second)++;
                }
                else
                {
                    // a new color
                    // cout << "color first time exist\n";
                    gs.mapColFreq[colors] = 1;
                }
            }
            break;
        }
        case 1:
        {
            byte p5_char;
            map_colorFreq::iterator it;
            for( int index = 0; index < sizeBild; index++)
            {
                is >> p5_char;
                gs.pixels[index] = ((float)p5_char)/colorstep;
                greyValue col = (greyValue) p5_char;

                vec_C[index] = col;

                it = gs.mapColFreq.find(col);
                if(it != gs.mapColFreq.end())
                {
                    (it -> second)++;
                }
                else
                {
                    // a new color
                    gs.mapColFreq[col] = 1;
                }
            }
            break;
        }
    }
    
    is >> ch; // should read in EOF
    return is;
}
ostream & operator<<(ostream & os, const GreyScale & gs)
{
    int sBild = gs.width * gs.height;
    if(!sBild)
    {
        cout << "The picture has no image cause it has dimension less than 2\n";
        return os;
    }
    int count = 0;
    
    
    // cout << "*************************************\n";
    // cout << "First the integer version (rounded to 255)\n";
    // int nextline = (gs.width < 10) ? gs.width : 10;
    int nextline = gs.width;
    GreyScale neu = gs.Clamp();
    // cout << "The Format number is: " << GreyScale::Format<< endl;
    // cout << "Using the local format: " << gs.Format << endl;
    switch(GreyScale::Format)
    {
        case 0:
        {
            os << gs.magicNumber << endl;
            os << gs.width << '\t' << gs.height << endl;
            os << 255<< endl;
            // cout << "In the case 0\n";
            for(int index = 0; index < sBild; index++)
            {
                // colorstep is assumed to be 255
                short d = 255;
                os.width(4);
                float res = std::rint(neu.pixels[index] * d);
                os << (short) res << '\t';
                if(count % nextline == nextline - 1)
                {
                    os << endl;
                }
                count++;
            }
            break;
        }
        case 1:
        {
            os << gs.magicNumber << endl;
            os << gs.width << '\t' << gs.height << endl;
            os << 255<< endl;
            // cout << "In the case 1\n";
            for(int index = 0; index < sBild; index++)
            {
                short d = 255;
                byte y = std::rint(neu.pixels[index] * d);
                os << y;
            }
            break;
        }
        case 2:
        {
            os << gs.magicNumber;
            unsigned int theWidth = gs.width;
            unsigned int theHeight = gs.height;
            byte byteWidthHigh = theWidth >> 8;
            byte byteWidthLow = theWidth % 256;

            byte byteHeightHigh = theHeight >> 8;
            byte byteHeightLow = theHeight % 256;
            os << byteWidthHigh << byteWidthLow << byteHeightHigh << byteHeightLow;
            // now the histogramm
            for(int index = 0; index < sBild; index++)
            {
                
            }
        }
        default:
        {
            cout << "No such format\n";
            exit(WRONG_FORMAT);
        }
    }
    
    

    // cout << "\n Next the original grey scale\n";
    // cout << "*************************************\n";
    // cout << "First using the vector index\n";
    // count = 0;
    // for(int index = 0; index < sBild; index++)
    // {
    //     os.width(4);
    //     os << gs.pixels[index] << '\t';
    //     if(count % nextline == nextline - 1)
    //     {
    //         os << endl;
    //     }
    //     count++;
    // }
    // cout << "*************************************\n";
    // cout << "Next using the Pic index\n";
    // count = 0;
    // for(int i = 0; i < gs.width; i++)
    // {
    //     for(int j = 0; j < gs.height; j++)
    //     {
    //         os.width(4);
    //         os << gs(i,j)<< '\t';
    //         if(count % nextline == nextline - 1)
    //         {
    //             os << endl;
    //         }
    //         count++;
    //     }
        
    // }
    return os;
}

void GreyScale::HuffmanCoding()
{
    BuildTree();


    // cout << "After the tree building the TrColFreq\n";
    // cout << TrColFreq << endl;


    // BuildMap(TrColFreq);
    cout << "*******************************\n";
    cout << "The coding is:\n";
    map_colorCoding::const_iterator iter;
    cout << "   color      |   code " << endl;
    for(iter = mpColCd.begin(); iter != mpColCd.end(); iter++)
    {
        cout.width(8);
        cout << iter -> first << "\t" <<  iter -> second << endl;
    }
    cout << "*******************************\n";
    cout << "The inverse is:\n";
    map_codingColor::const_iterator iter2;
    cout << "   code       |  color " << endl;
    for(iter2 = mpCdCol.begin(); iter2 != mpCdCol.end(); iter2++)
    {
        cout.width(8);
        cout << iter2 -> first << "\t" << iter2 -> second << endl;
    }
    cout << "The huff completed\n";
}


void GreyScale::BuildTree()
{
    if(mapColFreq.empty())
    {
        cout << "The mapColFreq is empty! cant build the tree\n";
        exit(MAP_COL_FREQ_EMPtY);
    }
    
    
    priority_queue<MyTree *, vector<MyTree*>, CompareMyTree> minHeap;
    vector_myTree v_myT;
    map_colorFreq::iterator iter;

    cout << "In the Build Tree, the mapColFreq:\n";
    cout << "color  |  freq\n";
    for(iter = mapColFreq.begin();iter != mapColFreq.end(); iter++)
    {
        cout.width(6);
        cout << iter -> first << "\t" <<  iter -> second << endl;
    }
    // build first the container for the sorted array, each element
    // is a color-frequency pair
    for(iter = mapColFreq.begin();iter != mapColFreq.end(); iter++)
    {
        greyValue col = iter -> first;
        freQuency freq = iter -> second;
        // MyTree pcF(freq,col);
        // v_myT.push_back(pcF);
        minHeap.push(new MyTree(freq,col));
    }

    // build the min heap, that is the root is the smallest element
    // the next small element is one of the two sons of the root so v_myT[1] or v_myT[2]

    // sorting first instead of heap making
    // sort(v_myT.begin(),v_myT.end(),myCompare);

    // TrColFreq = v_myT[0];  //*(v_myT.begin());
    // v_myT.erase(v_myT.begin());
    // make_heap(v_myT.begin(),v_myT.end(),CompareMyTree());
    // MyTree TempTree;


    // vector_myTree::const_iterator outMyTree;
    // cout << "After heap making\n";
    // for(outMyTree = v_myT.begin(); outMyTree != v_myT.end(); outMyTree++)
    // {
    //     cout << (outMyTree -> GetFrequency()) << "\t" << (outMyTree -> GetGreyValue()) << endl;

    // }
    // MyTree minTre(v_myT[0]);


    MyTree *lTree, *rTree, *topTree;
    while(minHeap.size() != 1)
    {
    //     minTre = v_myT[0];
    //     cout << "The min Tre is \n";
    //     cout << minTre.GetFrequency() << "\t" << minTre.GetGreyValue() << endl;
    //     cout << "The begin is \n";
    //     cout << v_myT.begin() -> GetFrequency() << "\t" << v_myT.begin() -> GetGreyValue() << endl;
    //     v_myT.erase(v_myT.begin());

    //     cout << "After erasing\n";
    //     for(outMyTree = v_myT.begin(); outMyTree != v_myT.end(); outMyTree++)
    //     {
    //         cout << (outMyTree -> GetFrequency()) << "\t" << (outMyTree -> GetGreyValue()) << endl;

    //     }

    //     if(!v_myT.empty())
    //     {
    //         if(v_myT.size() >= 2)
    //         {
    //             cout << "The size of v_myT >=2 \n";
    //             MyTree min2Tre;
    //             cout << "The v_myT[0] is \n";
    //             cout << v_myT[0] << endl;
    //             cout << "The v_myT[1] is \n";
    //             cout << v_myT[1] << endl;
    //             if(v_myT[0] > v_myT[1])
    //             {
    //                 // swap the two elements so that v_myT[0] is the second smallest
    //                 SwapTree(v_myT[0],v_myT[1]);
    //             }
    //             else if(! (v_myT[0] < v_myT[1]))
    //             {
    //                 cout << "The two trees are idendical impossible\n";
    //                 exit(IDENTICAL_TREE);
    //             }
    //             min2Tre = v_myT[0];
    //             cout << "The min2Tre is : " << endl;
    //             cout << min2Tre << endl;

    //             cout << "The TrcolFreq before merge\n";
    //             cout << TrColFreq << endl;

    //             TrColFreq = TreeMerge(minTre,min2Tre);

    //             cout << "The trcolfreq after merging \n";
    //             cout << TrColFreq << endl;

    //             v_myT.erase(v_myT.begin());
    //             v_myT.push_back(TrColFreq);

    //             // using sort first
    //             sort(v_myT.begin(),v_myT.end(),myCompare);
    //             // make_heap(v_myT.begin(),v_myT.end(),CompareMyTree());
    //         }
    //         else if(v_myT.size() == 1)
    //         {
    //             // just one element left merge it
    //             cout << "The v_myT has only one left\n";
    //             cout << v_myT[0] << endl;
    //             cout << "The TrcolFreq before merge\n";
    //             cout << TrColFreq << endl;

    //             TrColFreq = TreeMerge(minTre,v_myT[0]);


    //             cout << "The Trcolfreq after merging\n";
    //             cout << TrColFreq << endl;
    //             break;
    //         }
            
    //     }
    //     cout << "After heap making\n";
    //     for(outMyTree = v_myT.begin(); outMyTree != v_myT.end(); outMyTree++)
    //     {
    //         cout << (outMyTree -> GetFrequency()) << "\t" << (outMyTree -> GetGreyValue()) << endl;

    //     }
    //     // TrColFreq = minTre;

        lTree = minHeap.top();
        cout << "The top of the min heap is: \n";
        cout << *lTree;
        minHeap.pop();

        rTree = minHeap.top();
        cout << "The second min is :\n";
        cout << *rTree;
        minHeap.pop();
        int freq = lTree -> Frequency + rTree -> Frequency;
        // int grey = ((lTree -> GreyValue) < (rTree -> GreyValue)) ? (lTree -> GreyValue) : (rTree -> GreyValue);
        int grey = lTree -> GreyValue;
        topTree = new MyTree(freq, grey);
        topTree -> LeftTree = lTree;
        topTree -> RightTree = rTree;
        cout << "After merging the top\n";
        cout << *topTree;

        minHeap.push(topTree);
    }


    // initialize the vec_C


    vec_C.clear();
    BuildMap(*minHeap.top());
    deleteTree(minHeap.top());
}


void GreyScale::BuildMap(const MyTree & myT)
{
    // if(TrColFreq.isempty())
    // {

    // }
    cout << "The tree being tested\n";
    cout << myT << endl;
    cout << "The vec_C content: \n";
    cout << vec_C << endl;
    MyTree * lTree = myT.GetLeft();
    MyTree * rTree = myT.GetRight();
    if(lTree == nullptr)
    {
        if(rTree != nullptr)
        {
            // actually mustn't be nullptr
            // since one branch will be assigned 1 and one 0
            cout << "lTree is nullptr while right isn't\n";
            exit(UNBALANCED_BRANCH);
        }
        // is a leaf
        if(!vec_C.empty())
        {
            cout << "at leaf, the color is: " << myT.GetGreyValue() << "\t freq: " << myT.GetFrequency() << endl;
            codes theCode = Vect2Codes(vec_C);
            cout << "The code is: " << theCode << endl;
            mpColCd[myT.GetGreyValue()] = theCode;
            mpCdCol[theCode] = myT.GetGreyValue();
            vec_C.pop_back();
            return;
        }
        else
        {
            // the only case is that there is only one color
            mpColCd[myT.GetGreyValue()] = "0";
            mpCdCol["0"] = myT.GetGreyValue();
            return;
        }
        
    }
    else
    {
        if(rTree == nullptr)
        {
            // the analog case
            // unbalanced tree impossible
            cout << "lTree is not nullptr while right is\n";
            exit(UNBALANCED_BRANCH);
        }
        
    }
    vec_C.push_back('0');
    BuildMap(*lTree);
    vec_C.push_back('1');
    BuildMap(*rTree);
    vec_C.pop_back();
    
}

ostream & WriteHuffCode(ostream & os,const GreyScale & gs)
{
    int sizeBild = gs.width * gs.height;
    // indicates how many bits have to be 'borrowed' from the next
    // code to be a multiple of 8 bits
    int overflow = 0;
    for(int index = 0; index < sizeBild ; index++)
    {
        greyValue greyV = gs.vec_gV[index];
        codes theCode = gs.mpColCd.at(greyV);
        if(overflow)
        {
            theCode = theCode.substr((size_t)overflow,theCode.size());
        }
        int len = theCode.size();
        if(len == 0)
        {
            continue;
        }
        int segment = len / 8;
        overflow = len % 8;
        codes temp;
        for(int i = 0; i < segment; i++)
        {
            // temp is the byte wise slice of the code
            temp = theCode.substr(i*8,8);
            byte toWrite = Codes2Byte(temp);
            os << toWrite;
        }
        if(overflow)
        {
            // the index need to be added, until we can fill the gap
            int up = 1;
            // the gap to be filled
            int toBeFillled = 8-overflow;
            // the rest term
            codes restTerm = theCode.substr(segment * 8,overflow);
            codes merged = restTerm;

            codes glued;
            // indicating whether already written to the file
            bool written = false;
            // glue the next code if it exists
            while(index + up < sizeBild)
            {
                codes nextCode = gs.mpColCd.at(gs.vec_gV[index + up]);
                int nexLen = nextCode.size();
                if(nexLen >= toBeFillled)
                {
                    glued = nextCode.substr(0,toBeFillled);
                    overflow = nexLen - toBeFillled;
                    merged = restTerm + glued;
                    byte toWrite = Codes2Byte(merged);
                    os << toWrite;
                    index += up - 1;
                    written = true;
                    break;
                }
                else
                {
                    toBeFillled -= nexLen;
                    restTerm += nextCode;
                    up++;
                }
            }
            if(written)
            {
                continue;
            }
            else
            {
                // have to pad with 0
                for(int i = 0; i < toBeFillled; i++)
                {
                    restTerm.push_back('0');
                }
                byte toWrite = Codes2Byte(restTerm);
                os << toWrite;
                index += up - 1;
            }
            
        }
    }
    return os;
}


codes Vect2Codes(vec_Codes & veC)
{
    int vecSize = veC.size();
    // codes mycode = 0;
    vec_Codes::const_iterator iter;
    cout << "The veC content\n";
    for(iter = veC.begin(); iter != veC.end(); iter++)
    {
        cout << *iter << '\t';
    }
    cout << endl;
    // for(int index = 0; index < vecSize; index++)
    // {
    //     mycode << 1;
    //     mycode += veC[index];
    // }
    codes mycode = veC;
    return mycode;
}
byte Codes2Byte(codes & str_code)
{
    if(str_code.size() != 8)
    {
        cout << "The str_code size is not 8\n";
        exit(DECODE_LEN_MISMATCH);

    }
    byte result = 0;
    int len = str_code.size();
    for(int i = 0; i < len; i++)
    {
        result << 1;
        result += str_code[i] - '0';
    }
    return result;
}

 MyTree::MyTree():
 Frequency(0),GreyValue(0),LeftTree(nullptr),RightTree(nullptr)
 {
     TreeCount = 1;
 }
MyTree::MyTree(freQuency freq, greyValue grey):
LeftTree(nullptr),RightTree(nullptr)
{
    Frequency = freq;
    GreyValue = grey;
    TreeCount = 1;
}
MyTree::MyTree(const MyTree & tr)
{
    Frequency = tr.Frequency;
    GreyValue = tr.GreyValue;
    LeftTree = tr.LeftTree;
    RightTree = tr.RightTree;
    TreeCount = tr.TreeCount;
}
        // I think no need to write an explicit copy constructor
MyTree& MyTree::operator=(const MyTree & tr)
{
    if(this == &tr)
    {
        return (*this);
    }
    else
    {
        // MyTree temp(tr.Frequency,tr.GreyValue);
        // temp.LeftTree = tr.LeftTree;
        // temp.RightTree = tr.RightTree;
        // return temp;
        GreyValue = tr.GreyValue;
        Frequency = tr.Frequency;
        LeftTree = tr.LeftTree;
        RightTree = tr.RightTree;
        TreeCount = tr.TreeCount;
    }
    
}

void MyTree::SetLeft(MyTree & lTree)
{
    LeftTree = &lTree;
}
void MyTree::SetRight(MyTree & rTree)
{
    RightTree = &rTree;
}

bool MyTree::IsLeaf() const
{
    return (LeftTree == nullptr) && (RightTree == nullptr);
}

bool operator< (const MyTree & lTree, const MyTree & rTree)
{
    if(lTree.Frequency < rTree.Frequency)
    {
        return true;
    }
    else if(lTree.Frequency == rTree.Frequency)
    {
        if(lTree.GreyValue < rTree.GreyValue)
        {
            return true;
        }
    }
    else
    {
        return false;
    }
    
}

bool operator> (const MyTree & lTree, const MyTree & rTree)
{
    if(lTree.Frequency > rTree.Frequency)
    {
        return true;
    }
    else if(lTree.Frequency == rTree.Frequency)
    {
        if(lTree.GreyValue > rTree.GreyValue)
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

// merge two subtree to form a larger one
// with the smaller tree(smaller frequency or smaller greyvalue)
// on the left, the larger one on the right
// later the left one will be assigned the value 0
// the right 1
MyTree TreeMerge( MyTree & lTree, MyTree & rTree)
{
    int freq = lTree.Frequency + rTree.Frequency;
    int grey = (lTree.GreyValue < rTree.GreyValue) ? lTree.GreyValue : rTree.GreyValue;
    MyTree mergeTree(freq,grey);
    cout << "After initialize in the tree merge, the merge tree is\n";
    cout << mergeTree << endl;
    if(lTree < rTree)
    {
        mergeTree.SetLeft(lTree);
        mergeTree.SetRight(rTree);  
    }
    else if(lTree > rTree)
    {
        mergeTree.SetLeft(rTree);
        mergeTree.SetRight(lTree);  
    }
    else
    {
        // the two trees are identical
        cout << "the two trees to be merged are identical\n";
        exit(IDENTICAL_TREE);
    }
    cout << "Now after pointer setting\n";
    cout << mergeTree << endl;

    return mergeTree;
}

ostream & operator<<(ostream & os,const MyTree & tr)
{
    os << "grey: " << tr.GreyValue << "\tfreq: " << tr.Frequency << endl;
    if(tr.LeftTree == nullptr)
    {
        if(tr.RightTree != nullptr)
        {
            os << "the tree's left null while right not!\n";

        }
        else
        {
            os << "it's a leaf\n";
        }
        
    }
    else if(tr.RightTree == nullptr)
    {
        os << "the tree's right null while left not\n";
    }
    else
    {
        os << "it's a inner node\n";
    }
    return os;
}

void deleteTree(MyTree * tr)
{
    if((tr -> LeftTree == nullptr) && (tr -> RightTree == nullptr))
    {
        delete tr;
        return;
    }
    if((tr -> LeftTree) != nullptr)
    {
        // if(tr -> LeftTree -> IsLeaf())
        // {
        //     // int temp = tr ->
        //     delete (tr -> LeftTree);
        // }
        // else
        // {
        //     deleteTree(tr -> LeftTree);
        // }
        delete(tr -> LeftTree);
    }
    if((tr -> RightTree) != nullptr)
    {
        // if(tr -> RightTree -> IsLeaf())
        // {
        //     delete (tr -> RightTree);
        // }
        // else
        // {
        //     deleteTree(tr -> RightTree);
        // }
        delete(tr -> RightTree);
    }
    delete tr;
    return;
}