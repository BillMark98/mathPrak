#include "greyscale.h"

// help function for the Convolve()
int MaskCoord2Vec(int i, int j,int size);
int MaskSize;
GreyScale::GreyScale(int w, int h):
width(w),height(h)
{
    if(width * height)
    {
        pixels = new float[width * height];
    }
}
GreyScale::GreyScale(const GreyScale & gs)
{
    width = gs.width;
    height = gs.height;
    if(pixels != nullptr)
    {
        delete [] pixels;
    }
    pixels = new float [width * height];
    int sizeBild = width * height;
    for(int i = 0; i < sizeBild; i++)
    {
        pixels[i] = gs.pixels[i];
    }
}
GreyScale::~GreyScale()
{
    if(pixels != nullptr)
    {
        delete [] pixels;
    }
}

void GreyScale::Resize(int w, int h)
{
    if(pixels != nullptr)
    {
        delete [] pixels;
    }
    width = w;
    height = h;
    if(width * height)
    {
        pixels = new float[width * height];
    }
}

float GreyScale::operator()(int i, int j) const
{
    // the point outside the picture return 0
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
    else
    {
        return pixels[XYCoord2Vec(i,height - 1)];
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
    delete [] pixels; // safe to delete nullptr so dont check that

    width = gs.width;
    height = gs.height;
    pixels = new float[width * height];
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
            for(int i = -bound; i <= bound; i++)
            {
                for(int j = -bound; j <= bound; j++)
                {
                    int maskIndex = MaskCoord2Vec(i,j,size);
                    sum += (*this)(x_val + i,y_val + j) * mask[maskIndex];
                }
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
            neighbours.push_back((*this)(xcord,ycord));
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
    float colors;
    for( int index = 0; index < sizeBild; index++)
    {
        is >> colors;
        gs.pixels[index] = ((float)colors)/colorstep;
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
    for(int index = 0; index < sBild; index++)
    {
        // colorstep is assumed to be 255
        short d = 255;
        os.width(4);
        float res = gs.pixels[index] * d;
        os << (short) res << '\t';
        if(count % gs.width == gs.width - 1)
        {
            os << endl;
        }
    }
    // cout << "\n Next the original grey scale\n";
    // cout << "*************************************\n";
    // for(int index = 0; index < sBild; index++)
    // {
    //     os.width(4);
    //     os << gs.pixels[index] << '\t';
    //     if(count % gs.width == gs.width - 1)
    //     {
    //         os << endl;
    //     }
    // }
    return os;
}