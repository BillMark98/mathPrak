#include "greyscale.h"

// help function for the Convolve()
int MaskCoord2Vec(int i, int j,int size);
int MaskSize;

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
    
    switch(GreyScale::Format)
    {
        case 0:
        {
            unsigned int colors;
            map_colorFreq::iterator it;
            for( int index = 0; index < sizeBild; index++)
            {
                is >> colors;
                gs.pixels[index] = ((float)colors)/colorstep;
                it = gs.mapColFreq.find(colors);
                if(it != gs.mapColFreq.end())
                {
                    (it -> second)++;
                }
                else
                {
                    // a new color
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
                unsigned int col = (unsigned int) p5_char;
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
    os << gs.magicNumber << endl;
    os << gs.width << '\t' << gs.height << endl;
    os << 255<< endl;
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
            // cout << "In the case 1\n";
            for(int index = 0; index < sBild; index++)
            {
                short d = 255;
                byte y = std::rint(neu.pixels[index] * d);
                os << y;
            }
            break;
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
 MyTree::MyTree():
 Frequency(0),GreyValue(0),LeftTree(nullptr),RightTree(nullptr)
 {

 }
MyTree::MyTree(int freq, int grey):
LeftTree(nullptr),RightTree(nullptr)
{
    Frequency = freq;
    GreyValue = grey;
}
void MyTree::SetLeft(MyTree & lTree)
{
    LeftTree = &lTree;
}
void MyTree::SetRight(MyTree & rTree)
{
    RightTree = &rTree;
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
    return mergeTree;
}
        