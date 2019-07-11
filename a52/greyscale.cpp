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

    // bool operator()(MyTree * lTree,  MyTree * rTree) const
    // {
    //     if(lTree -> GetFrequency() < rTree -> GetFrequency())
    //     {
    //         return true;
    //     }
    //     else if(lTree -> GetFrequency() == rTree -> GetFrequency())
    //     {
    //         if(lTree -> GetGreyValue() < rTree -> GetGreyValue())
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
#ifdef OUTDEBUG
    // cout << "*******************************\n";
    // cout << "in the reading The inverse is:\n";
    // map_codingColor::const_iterator iter2;
    // cout << "   code       |  color " << endl;
    // for(iter2 = gs.mpCdCol.begin(); iter2 != gs.mpCdCol.end(); iter2++)
    // {
    //     cout.width(8);
    //     cout << iter2 -> first << "\t" << iter2 -> second << endl;
    // }

#endif
    gs.magicNumber.clear();
    byte toRead;
    toRead = is.peek();
    // cout << "The toRead is \n" << toRead <<" int: " << (unsigned short) toRead <<endl;
    if(toRead == (byte)('P'))
    {
        // the first two cases just read in the whole line
        // cout << "to Read is P\n";
        getline(is,gs.magicNumber);
    }
    else if(toRead == 'M')
    {
        // cout << "to Read is M\n";
        // read in M
        is >> toRead;  
        gs.magicNumber.push_back(toRead);
        // read in H
        is >> toRead;
        gs.magicNumber.push_back(toRead);
        // read in a/b
        is >> toRead;
        gs.magicNumber.push_back(toRead);
    }
    // cout << "The magic number is " << gs.magicNumber<< endl;
    gs.MagicSetFormat();
#ifdef TEST
    cout << "The format is: " << GreyScale::Format << endl;
#endif
    int width,height,colorstep;


    // is >> ws;
    // char ch = is.peek();
    
    // if(ch == '#')
    // {
    //     while((ch = is.get()) != '\n')
    //         continue;
    // }
    // else if(!isdigit(ch))
    // {
    //     cout << "Unexpected character in the input\n";
    //     cout << "The char is " << ch << endl;
    //     exit(UNEXPECTED_CHAR);
    // }


    // is >> width >> height;
    // gs.Resize(width,height);
    // is >> colorstep;
    // // colorstep; // e.g read in 255 then there are 256 steps of colors
    // int sizeBild = width * height;
    // // float colors;
    // gs.vec_gV.clear();
    // gs.vec_gV.resize(sizeBild);

    switch(GreyScale::Format)
    {
        case 0:
        {
            greyValue colors;

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

            map_colorFreq::iterator it;


            is >> width >> height;
            is >> colorstep;
            int sizeBild = width * height;
            gs.Resize(width,height);
            gs.vec_gV.clear();
            gs.vec_gV.resize(sizeBild);

            // cout << "the case 0\n";
            for( int index = 0; index < sizeBild; index++)
            {
                
                is >> colors;
                gs.vec_gV[index] = colors;
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
            is >> ch; // should read in EOF

            // huffman code build mpColCd, mpCdCol
            gs.HuffmanCoding(gs.mapColFreq,gs.mpColCd,gs.mpCdCol);
            gs.SetmpTransColFreq();
            gs.HuffmanCoding(gs.mpTransColFreq,gs.mpTransColCd,gs.mpTransCdCol);
            break;
        }
        case 1:
        {
            greyValue colors;
            
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
            is >> colorstep;
            int sizeBild = width * height;
            gs.Resize(width,height);
            gs.vec_gV.clear();
            gs.vec_gV.resize(sizeBild);
            byte p5_char;
            map_colorFreq::iterator it;
            for( int index = 0; index < sizeBild; index++)
            {
                is >> p5_char;
                gs.pixels[index] = ((float)p5_char)/colorstep;
                greyValue col = (greyValue) p5_char;

                gs.vec_gV[index] = col;

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
            is >> ch; // should read in EOF

            // huffman code build mpColCd, mpCdCol
            gs.HuffmanCoding(gs.mapColFreq,gs.mpColCd,gs.mpCdCol);
            gs.SetmpTransColFreq();
            gs.HuffmanCoding(gs.mpTransColFreq,gs.mpTransColCd,gs.mpTransCdCol);
            break;
        }
        case 2:
        {
            // cout << "In the case"
            byte Highbits;
            byte Lowbits;
            // read in width

            // is >> Highbits >> Lowbits;
            GetNextByte(is,Highbits);
            GetNextByte(is,Lowbits);
            int width = (Highbits << 8) + Lowbits;
#ifdef OUTDEBUG
            cout << "the width bytes read: " << (unsigned short) Highbits << " , "<<(unsigned short) Lowbits << endl;
#endif 
            // read in height
            // is >> Highbits >> Lowbits;

            GetNextByte(is,Highbits);
            GetNextByte(is,Lowbits);
            int height = (Highbits << 8) + Lowbits;
#ifdef OUTDEBUG
            cout << "the height bytes read: " << (unsigned short) Highbits << " , "<<(unsigned short) Lowbits << endl;
#endif 
            int sizeBild = width * height;
#ifdef OUTDEBUG
            cout << "width: " << width << "  height: " << height << endl;
#endif     
            gs.Resize(width,height);
            gs.vec_gV.clear();
            gs.vec_gV.resize(sizeBild);

            byte b1,b2,b3,b4;
            // read in the histogram
            for(greyValue grev = 0; grev < 256; grev++)
            {
                // is >> b1 >> b2 >> b3 >> b4;
                // is will jump 0x0a and 0x0d
                // char tempChar;
                // is.get(tempChar);
                // b1 = (byte) tempChar;
                // is.get(tempChar);
                // b2 = (byte) tempChar;
                // is.get(tempChar);
                // b3 = (byte) tempChar;
                // is.get(tempChar);
                // b4 = (byte) tempChar;
                GetNextByte(is,b1);
                GetNextByte(is,b2);
                GetNextByte(is,b3);
                GetNextByte(is,b4);

                freQuency freq = (b1 << (24)) + (b2 << 16) + (b3 << 8) + b4;
#ifdef OUTDEBUG
                cout << "The grey value " << grev << " The freq is " << freq;
                cout <<  "  the bs are " << (unsigned short) b1 << '\t' <<(unsigned short) b2 << "\t" 
                    << (unsigned short)b3 << "\t" << (unsigned short) b4 << endl;
#endif                    
                if(freq)
                {
                    gs.mapColFreq[grev] = freq;
                }
            }
            // build the huffman code
            gs.HuffmanCoding(gs.mapColFreq,gs.mpColCd,gs.mpCdCol);
            

            ReadHuffCode(is,gs);

            // must after the ReadHuffCode since before that the vec_gV is not initialized
            gs.SetmpTransColFreq();
            gs.HuffmanCoding(gs.mpTransColFreq,gs.mpTransColCd,gs.mpTransCdCol);
            break;
        }
        case 3:
        {
            // cout << "In the case"
            byte Highbits;
            byte Lowbits;
            // read in width

            // is >> Highbits >> Lowbits;
            GetNextByte(is,Highbits);
            GetNextByte(is,Lowbits);
            int width = (Highbits << 8) + Lowbits;
#ifdef OUTDEBUG
            cout << "the width bytes read: " << (unsigned short) Highbits << " , "<<(unsigned short) Lowbits << endl;
#endif 
            // read in height
            // is >> Highbits >> Lowbits;

            GetNextByte(is,Highbits);
            GetNextByte(is,Lowbits);
            int height = (Highbits << 8) + Lowbits;
#ifdef OUTDEBUG
            cout << "the height bytes read: " << (unsigned short) Highbits << " , "<<(unsigned short) Lowbits << endl;
#endif 
            int sizeBild = width * height;
#ifdef OUTDEBUG
            cout << "width: " << width << "  height: " << height << endl;
#endif     
            gs.Resize(width,height);
            gs.vtrans_gV.clear();
            gs.vtrans_gV.resize(sizeBild);

            byte b1,b2,b3,b4;
            // read in the histogram
            for(greyValue grev = 0; grev < 256; grev++)
            {
                // is >> b1 >> b2 >> b3 >> b4;
                // is will jump 0x0a and 0x0d
                // char tempChar;
                // is.get(tempChar);
                // b1 = (byte) tempChar;
                // is.get(tempChar);
                // b2 = (byte) tempChar;
                // is.get(tempChar);
                // b3 = (byte) tempChar;
                // is.get(tempChar);
                // b4 = (byte) tempChar;
                GetNextByte(is,b1);
                GetNextByte(is,b2);
                GetNextByte(is,b3);
                GetNextByte(is,b4);

                freQuency freq = (b1 << (24)) + (b2 << 16) + (b3 << 8) + b4;
#ifdef OUTDEBUG
                cout << "The grey value " << grev << " The freq is " << freq;
                cout <<  "  the bs are " << (unsigned short) b1 << '\t' <<(unsigned short) b2 << "\t" 
                    << (unsigned short)b3 << "\t" << (unsigned short) b4 << endl;
#endif                    
                if(freq)
                {
                    gs.mpTransColFreq[grev] = freq;
                }
            }
            // build the huffman code
            gs.HuffmanCoding(gs.mpTransColFreq,gs.mpTransColCd,gs.mpTransCdCol);
            

            ReadHuffCode(is,gs,3);

            // must after the ReadHuffCode since before that the vec_gV is not initialized
            gs.SetmapColFreq();
            gs.HuffmanCoding(gs.mapColFreq,gs.mpColCd,gs.mpCdCol);
            break;
        }
    }
    
    
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

    // for example roentgen.pgm is 16
    // int nextline = gs.width;
    int nextline = 16;
    GreyScale neu = gs.Clamp();
    // cout << "The Format number is: " << GreyScale::Format<< endl;
    // cout << "Using the local format: " << gs.Format << endl;
    // cout << "The format is now: " << GreyScale::Format << endl;


    switch(GreyScale::Format)
    {
        case 0:
        {
            os << "P2" << endl;
            os << gs.width << ' ' << gs.height << endl;
            os << 255<< endl;
            // cout << "In the case 0\n";
            for(int index = 0; index < sBild; index++)
            {
                // colorstep is assumed to be 255
                short d = 255;
                
                float res = std::rint(neu.pixels[index] * d);
                os << ' ';
                os << std::setw(3) << (short) res ;
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
            // os << gs.magicNumber << endl;
            os << "P5" << endl;
            os << gs.width << ' ' << gs.height << endl;
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
            // os << gs.magicNumber;
            os << "MHa";
            unsigned int theWidth = gs.width;
            unsigned int theHeight = gs.height;
            byte byteWidthHigh = theWidth >> 8;
            byte byteWidthLow = theWidth % 256;

            byte byteHeightHigh = theHeight >> 8;
            byte byteHeightLow = theHeight % 256;
            os << byteWidthHigh << byteWidthLow << byteHeightHigh << byteHeightLow;
            // now the histogramm
            // cout << "The size of vec_gV is " << gs.vec_gV.size();
            // os << "The histogram\n";
            // for(int index = 0; index < sBild; index++)
            // {
            //     greyValue greV = gs.vec_gV[index];
            //     os << "cl:"<< greV << endl;
            //     map_colorFreq::const_iterator iter = gs.mapColFreq.find(greV);
            //     freQuency freq = (freQuency) 0;
            //     if(iter != gs.mapColFreq.end())
            //     {
            //         freq = (iter -> second);
            //     }
            //     gs.outFreq32Bit(os,freq);
            // }
            
#ifdef OUTDEBUG
            cout << "in the operator << , output the map_colorFreq\n";
#endif                    
            for(greyValue greV = 0; greV < 256; greV++)
            {

                // os << "cl:"<< greV << endl;

                map_colorFreq::const_iterator iter = gs.mapColFreq.find(greV);
                freQuency freq = (freQuency) 0;
                if(iter != gs.mapColFreq.end())
                {
                    freq = (iter -> second);
                }
#ifdef OUTDEBUG                
                cout << "color : " << greV << " frequency is " << freq << endl;
#endif                
                gs.outFreq32Bit(os,freq);
            }
            // now the huffman code
            // os << "The huffman\n";
            WriteHuffCode(os,gs);
            break;
        }
        case 3:
        {
            os << "MHb";
            unsigned int theWidth = gs.width;
            unsigned int theHeight = gs.height;
            byte byteWidthHigh = theWidth >> 8;
            byte byteWidthLow = theWidth % 256;

            byte byteHeightHigh = theHeight >> 8;
            byte byteHeightLow = theHeight % 256;
            os << byteWidthHigh << byteWidthLow << byteHeightHigh << byteHeightLow;
            // now the histogramm
            // cout << "The size of vec_gV is " << gs.vec_gV.size();
            // os << "The histogram\n";
            // for(int index = 0; index < sBild; index++)
            // {
            //     greyValue greV = gs.vec_gV[index];
            //     os << "cl:"<< greV << endl;
            //     map_colorFreq::const_iterator iter = gs.mapColFreq.find(greV);
            //     freQuency freq = (freQuency) 0;
            //     if(iter != gs.mapColFreq.end())
            //     {
            //         freq = (iter -> second);
            //     }
            //     gs.outFreq32Bit(os,freq);
            // }
            
#ifdef OUTDEBUG
            cout << "in the operator << , output the map_colorFreq\n";
#endif                    
            for(greyValue greV = 0; greV < 256; greV++)
            {

                // os << "cl:"<< greV << endl;

                map_colorFreq::const_iterator iter = gs.mpTransColFreq.find(greV);
                freQuency freq = (freQuency) 0;
                if(iter != gs.mpTransColFreq.end())
                {
                    freq = (iter -> second);
                }
#ifdef OUTDEBUG                
                cout << "color : " << greV << " frequency is " << freq << endl;
#endif                
                gs.outFreq32Bit(os,freq);
            }
            // now the huffman code
            // os << "The huffman\n";
            WriteHuffCode(os,gs,3);
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

void GreyScale::HuffmanCoding(map_colorFreq & mpcF, map_colorCoding & mpcolCode,map_codingColor & mpcodCol)
{
    BuildTree(mpcF, mpcolCode,mpcodCol);


    // cout << "After the tree building the TrColFreq\n";
    // cout << TrColFreq << endl;


    // BuildMap(TrColFreq);
#ifdef OUTDEBUG
    cout << "*******************************\n";
    cout << "The coding is:\n";

    map_colorCoding::const_iterator iter;

    cout << "   color      |   code " << endl;

    for(iter = mpcolCode.begin(); iter != mpcolCode.end(); iter++)
    {
        cout.width(8);
        cout << iter -> first << "\t" <<  iter -> second << endl;
    }
    cout << "*******************************\n";
    cout << "The inverse is:\n";
    map_codingColor::const_iterator iter2;
    cout << "   code       |  color " << endl;
    for(iter2 = mpcodCol.begin(); iter2 != mpcodCol.end(); iter2++)
    {
        cout.width(8);
        cout << iter2 -> first << "\t" << iter2 -> second << endl;
    }
    cout << "The huff completed\n";
#endif
}


void GreyScale::BuildTree(map_colorFreq & mpcF, map_colorCoding & mpcolCode,map_codingColor & mpcodCol)
{
    if(mpcF.empty())
    {
        cout << "The mpcF is empty! cant build the tree\n";
        exit(MAP_COL_FREQ_EMPtY);
    }
    
    
    priority_queue<MyTree *, vector<MyTree*>, CompareMyTree> minHeap;
    vector_myTree v_myT;
    map_colorFreq::iterator iter;
#ifdef OUTDEBUG
    cout << "In the Build Tree, the mapColFreq:\n";
    cout << "color  |  freq\n";
    for(iter = mpcF.begin();iter != mpcF.end(); iter++)
    {
        cout.width(6);
        cout << iter -> first << "\t" <<  iter -> second << endl;
    }
#endif
    // build first the container for the sorted array, each element
    // is a color-frequency pair
    for(iter = mpcF.begin();iter != mpcF.end(); iter++)
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

#ifdef OUTDEBUG
        cout << "The top of the min heap is: \n";
        cout << *lTree;
#endif
        minHeap.pop();

        rTree = minHeap.top();
#ifdef OUTDEBUG
        cout << "The second min is :\n";
        cout << *rTree;
#endif

        minHeap.pop();
        int freq = lTree -> Frequency + rTree -> Frequency;
        int grey = ((lTree -> GreyValue) < (rTree -> GreyValue)) ? (lTree -> GreyValue) : (rTree -> GreyValue);
        // int grey = lTree -> GreyValue;
        topTree = new MyTree(freq, grey);
        topTree -> LeftTree = lTree;
        topTree -> RightTree = rTree;

#ifdef OUTDEBUG
        cout << "After merging the top\n";
        cout << *topTree;
#endif
        minHeap.push(topTree);
    }


    // initialize the vec_C
    // don't need to place two such vec_C for the huff code for the format 2 ,3 
    // 1. reason, it would be a bit more troublesome to implement, the BuildTree, Huffman,BuildMap would need
    // new parameter, i.e the vec_C so that it can handle which vec_C to deal with
    // 2. we don't have to, since the only time we access to vec_C is in these three functions,
    // but these three functions have a tight connection. Once we call HuffmanCoding, we will call
    // BuildTree BuildMap and it's the only case we will access the vec_C, which means we can deal with
    // one vec_C, which serves something like a local variable shared between the three functions
    // and it is invisible outside the three functions, so it will not have effect to the outside

    vec_C.clear();
    BuildMap(mpcolCode,mpcodCol,*minHeap.top());
    deleteTree(minHeap.top());
}


void GreyScale::BuildMap(map_colorCoding & mpcolCode,map_codingColor & mpcodCol,MyTree & myT)
{
    // if(TrColFreq.isempty())
    // {

    // }
#ifdef OUTDEBUG
    cout << "The tree being tested\n";
    cout << myT << endl;
    cout << "The vec_C content: \n";
    cout << vec_C << endl;
#endif
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
#ifdef OUTDEBUG
            cout << "at leaf, the color is: " << myT.GetGreyValue() << "\t freq: " << myT.GetFrequency() << endl;
#endif

            codes theCode = Vect2Codes(vec_C);
#ifdef OUTDEBUG
            cout << "The code is: " << theCode << endl;
            cout << "if code == 11 " << (theCode == "11") << endl;
#endif            
            mpcolCode[myT.GetGreyValue()] = theCode;
            mpcodCol[theCode] = myT.GetGreyValue();
#ifdef OUTDEBUG
            // cout << "mpCdCol[theCode] = " << mpCdCol[theCode]<<endl;
            // cout << "mpCdCol[11] = " << mpCdCol["11"] << endl;
#endif
            vec_C.pop_back();
            return;
        }
        else
        {
            // the only case is that there is only one color
            mpcolCode[myT.GetGreyValue()] = "0";
            mpcodCol["0"] = myT.GetGreyValue();
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
    BuildMap(mpcolCode,mpcodCol,*lTree);
    vec_C.push_back('1');
    BuildMap(mpcolCode,mpcodCol,*rTree);
    vec_C.pop_back();
    
}


void GreyScale::GreyTransform()
{
    vtrans_gV.clear();
    vtrans_gV.resize(width * height);
    for(int j = height - 1; j >= 0; j--)
    {
        for(int i = width -1; i >= 0; i--)
        {
            greyValue temp = DiffNeighbor(i,j);
            int index = XYCoord2Vec(i,j);
            vtrans_gV[index] = temp;
        }
    }
}
// should also set the pixels
void GreyScale::InverseGreyTransform()
{
    vec_gV.clear();
    vec_gV.resize(width * height);
    pixels.clear();
    pixels.resize(width * height);
    for(int j = 0; j < height; j++)
    {
        for(int i = 0; i < width; i++)
        {
            greyValue temp = SumNeighbor(i,j);
            int index = XYCoord2Vec(i,j);
            vec_gV[index] = temp;
            pixels[index] = ((float)temp)/255;
        }
    }
}

greyValue GreyScale::DiffNeighbor(int i, int j)
{
    // assume i,j is valid
    // i is width, j is height
    // and assume width >= 2 height >= 2
    if(i == width - 1)
    {
        if(j >= 1)
        {
            int sum = vec_gV[XYCoord2Vec(i,j-1)] + vec_gV[XYCoord2Vec(i-1,j)] 
                        + vec_gV[XYCoord2Vec(i-1,j-1)];
            sum /= 3;
            int diff = (vec_gV[XYCoord2Vec(i,j)] - sum) % 256;
            greyValue result = (greyValue) diff;
            return result;
        }
        else
        {
            int sum = vec_gV[XYCoord2Vec(i-1,j)];
            int diff = (vec_gV[XYCoord2Vec(i,j)] - sum) % 256;
            greyValue result = (greyValue) diff;
            return result;
        }
    }
    if(i == 0)
    {
        if(j >= 1)
        {
            int sum = vec_gV[XYCoord2Vec(i,j-1)] + vec_gV[XYCoord2Vec(i+1,j-1)];
            sum /= 2;
            int diff = (vec_gV[XYCoord2Vec(i,j)] - sum) % 256;
            greyValue result = (greyValue) diff;
            return result;
        }
        else
        {
            // the (0,0) return itself
            return (vec_gV[0]);
        }
    }
    if(j == 0)
    {
        //  1 <= i <= width - 2
            int sum = vec_gV[XYCoord2Vec(i-1,j)];
            int diff = (vec_gV[XYCoord2Vec(i,j)] - sum) % 256;
            greyValue result = (greyValue) diff;
            return result;
    }
    // this case is that the point is in the middle
    int sum = vec_gV[XYCoord2Vec(i,j-1)] + vec_gV[XYCoord2Vec(i+1,j-1)]
                + vec_gV[XYCoord2Vec(i-1,j)] + vec_gV[XYCoord2Vec(i-1,j-1)];
    sum /= 4;
    int diff = (vec_gV[XYCoord2Vec(i,j)] - sum) % 256;
    greyValue result = (greyValue) diff;
    return result;
}


greyValue GreyScale::SumNeighbor(int i, int j)
{
    // assume i,j is valid
    // i is width, j is height
    // and assume width >= 2 height >= 2
    if(i == width - 1)
    {
        if(j >= 1)
        {
            int sum = vec_gV[XYCoord2Vec(i,j-1)] + vec_gV[XYCoord2Vec(i-1,j)] 
                        + vec_gV[XYCoord2Vec(i-1,j-1)];
            sum /= 3;
            int invdiff = (vec_gV[XYCoord2Vec(i,j)] + sum) % 256;
            greyValue result = (greyValue) invdiff;
            return result;
        }
        else
        {
            int sum = vec_gV[XYCoord2Vec(i-1,j)];
            int invdiff = (vec_gV[XYCoord2Vec(i,j)] + sum) % 256;
            greyValue result = (greyValue) invdiff;
            return result;
        }
    }
    if(i == 0)
    {
        if(j >= 1)
        {
            int sum = vec_gV[XYCoord2Vec(i,j-1)] + vec_gV[XYCoord2Vec(i+1,j-1)];
            sum /= 2;
            int invdiff = (vec_gV[XYCoord2Vec(i,j)] + sum) % 256;
            greyValue result = (greyValue) invdiff;
            return result;
        }
        else
        {
            // the (0,0) return itself
            return (vec_gV[0]);
        }
    }
    if(j == 0)
    {
        //  1 <= i <= width - 2
            int sum = vec_gV[XYCoord2Vec(i-1,j)];
            int invdiff = (vec_gV[XYCoord2Vec(i,j)] + sum) % 256;
            greyValue result = (greyValue) invdiff;
            return result;
    }
    // this case is that the point is in the middle
    int sum = vec_gV[XYCoord2Vec(i,j-1)] + vec_gV[XYCoord2Vec(i+1,j-1)]
                + vec_gV[XYCoord2Vec(i-1,j)] + vec_gV[XYCoord2Vec(i-1,j-1)];
    sum /= 4;
    int invdiff = (vec_gV[XYCoord2Vec(i,j)] + sum) % 256;
    greyValue result = (greyValue) invdiff;
    return result;
}

// set the mpTransColFreq
void GreyScale::SetmpTransColFreq()
{
    // assume the 
    GreyTransform();
    map_colorFreq::iterator iter;
    int sizeBild = width * height;
    greyValue grey;
    for(int index = 0; index < sizeBild; index++)
    {
        grey = vtrans_gV[index];
        iter = mpTransColFreq.find(grey);
        if(iter != mpTransColFreq.end())
        {
            (iter -> second)++;
        }
        else
        {
            mpTransColFreq[grey] = 1;
        }
    }
}
// set the mapColFreq
void GreyScale::SetmapColFreq()
{
    InverseGreyTransform();
    map_colorFreq::iterator iter;
    int sizeBild = width * height;
    greyValue grey;
    for(int index = 0; index < sizeBild; index++)
    {
        grey = vec_gV[index];
        iter = mapColFreq.find(grey);
        if(iter != mapColFreq.end())
        {
            (iter -> second)++;
        }
        else
        {
            mapColFreq[grey] = 1;
        }
    }
}


ostream & WriteHuffCode(ostream & os,const GreyScale & gs,int form )
{
    int sizeBild = gs.width * gs.height;
    // indicates how many bits have to be 'borrowed' from the next
    // code to be a multiple of 8 bits
    int overflow = 0;

    if(form == 2)
    {
        for(int index = 0; index < sizeBild ; index++)
        {
            greyValue greyV = gs.vec_gV[index];
            codes theCode = gs.mpColCd.at(greyV);
    #ifdef OUTDEBUG
            cout << "The greyV: " << greyV << "  code: " << theCode << endl;
    #endif
            if(overflow)
            {
                theCode = theCode.substr((size_t)overflow,theCode.size());
    #ifdef OUTDEBUG
            cout << "after modification the code : " << theCode << endl;
    #endif
            }
            int len = theCode.size();
            if(len == 0)
            {
                // have to update the overflow
                overflow = 0;
                continue;
            }
            int segment = len / 8;
            overflow = len % 8;
    #ifdef OUTDEBUG
            cout << "segment: " <<segment << "  overflow: " << overflow << endl;
    #endif    
            codes temp;
            for(int i = 0; i < segment; i++)
            {
                // temp is the byte wise slice of the code
                temp = theCode.substr(i*8,8);
                byte toWrite = Codes2Byte(temp);
    #ifdef OUTDEBUG
                cout << "temp is: " << temp << endl;
                cout << "the byte toWrite is " << (unsigned short) toWrite << endl;
    #endif
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
    #ifdef OUTDEBUG
                cout << "begin while loop\n";
                cout << "the restTerm is : " << restTerm << endl;
    #endif
                while(index + up < sizeBild)
                {
                    codes nextCode = gs.mpColCd.at(gs.vec_gV[index + up]);

    #ifdef OUTDEBUG
                    cout << "the nextCode is : " << nextCode << endl;
    #endif
                    int nexLen = nextCode.size();
                    if(nexLen >= toBeFillled)
                    {
                        glued = nextCode.substr(0,toBeFillled);
                        // this caculates the len remained 
                        // overflow = nexLen - toBeFillled;
                        overflow = toBeFillled;
                        merged = restTerm + glued;

                        byte toWrite = Codes2Byte(merged);
    #ifdef OUTDEBUG
                        cout << "nextCode cann fill the gap.\n";
                        cout << "the glued " << glued << " the merged: " << merged << endl;
                        cout << "the overflow " << overflow << "  the toWrite:  " << (unsigned short) toWrite << endl;
    #endif
                        os << toWrite;
                        index += up - 1;
                        written = true;
                        break;
                    }
                    else
                    {
                        toBeFillled -= nexLen;
                        restTerm += nextCode;

    #ifdef OUTDEBUG
                        cout << " the nextCode fails to fill \n";
                        cout << "the restTerm now is " << restTerm << "  toBeFilled is " << toBeFillled << endl;
    #endif
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
                    // I think could use break here
                }
                
            }
        }
    }
    else if(form == 3)
    {
        for(int index = 0; index < sizeBild ; index++)
    {
        greyValue greyV = gs.vtrans_gV[index];
        codes theCode = gs.mpTransColCd.at(greyV);
#ifdef OUTDEBUG
        cout << "The greyV: " << greyV << "  code: " << theCode << endl;
#endif
        if(overflow)
        {
            theCode = theCode.substr((size_t)overflow,theCode.size());
#ifdef OUTDEBUG
        cout << "after modification the code : " << theCode << endl;
#endif
        }
        int len = theCode.size();
        if(len == 0)
        {
            // have to update the overflow
            overflow = 0;
            continue;
        }
        int segment = len / 8;
        overflow = len % 8;
#ifdef OUTDEBUG
        cout << "segment: " <<segment << "  overflow: " << overflow << endl;
#endif    
        codes temp;
        for(int i = 0; i < segment; i++)
        {
            // temp is the byte wise slice of the code
            temp = theCode.substr(i*8,8);
            byte toWrite = Codes2Byte(temp);
#ifdef OUTDEBUG
            cout << "temp is: " << temp << endl;
            cout << "the byte toWrite is " << (unsigned short) toWrite << endl;
#endif
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
#ifdef OUTDEBUG
            cout << "begin while loop\n";
            cout << "the restTerm is : " << restTerm << endl;
#endif
            while(index + up < sizeBild)
            {
                codes nextCode = gs.mpTransColCd.at(gs.vtrans_gV[index + up]);

#ifdef OUTDEBUG
                cout << "the nextCode is : " << nextCode << endl;
#endif
                int nexLen = nextCode.size();
                if(nexLen >= toBeFillled)
                {
                    glued = nextCode.substr(0,toBeFillled);
                    // this caculates the len remained 
                    // overflow = nexLen - toBeFillled;
                    overflow = toBeFillled;
                    merged = restTerm + glued;

                    byte toWrite = Codes2Byte(merged);
#ifdef OUTDEBUG
                    cout << "nextCode cann fill the gap.\n";
                    cout << "the glued " << glued << " the merged: " << merged << endl;
                    cout << "the overflow " << overflow << "  the toWrite:  " << (unsigned short) toWrite << endl;
#endif
                    os << toWrite;
                    index += up - 1;
                    written = true;
                    break;
                }
                else
                {
                    toBeFillled -= nexLen;
                    restTerm += nextCode;

#ifdef OUTDEBUG
                    cout << " the nextCode fails to fill \n";
                    cout << "the restTerm now is " << restTerm << "  toBeFilled is " << toBeFillled << endl;
#endif
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
                // I think could use break here
            }
            
        }
    }
    }
    
    return os;
}

istream & ReadHuffCode(istream & is, GreyScale & gs,int form)
{
    // idea, read in a bunch of bytes then convert each bit to a byte
    // the slice it to form a string to which a code corresponds 

    if(form == 2)
    {

        
    #ifdef OUTDEBUG
        cout << "The form is 2\n";
        cout << "*******************************\n";
        cout << "in the readhuff code The inverse is:\n";
        map_codingColor::const_iterator iter2;
        cout << "   code       |  color " << endl;
        for(iter2 = gs.mpCdCol.begin(); iter2 != gs.mpCdCol.end(); iter2++)
        {
            cout.width(8);
            cout << iter2 -> first << "\t" << iter2 -> second << endl;
        }

    #endif

        byte readIn;
        codes theCode;
        theCode.clear();
        // indicates the bits borrowed from the next complete byte
        // int overflow = 0;
        // indicates the code left from the previous one
        codes prevLeftCode;
        prevLeftCode.clear();
        // indicates the accumulated code
        codes accumulated;
        accumulated.clear();

        int sizeBild = gs.width * gs.height;
        map_codingColor::iterator iter;
        greyValue grey;
        for(int index = 0; index < sizeBild; index++)
        {
            // first test whether the prevLeftCode includes code
            int prevLClen = prevLeftCode.size();
    #ifdef OUTDEBUG
            if(prevLClen > 0)
            {
                cout << "the prevLeftCode : " << prevLeftCode << endl;
            }
    #endif
            codes prevSubCode;
            prevSubCode.clear();
            // flag indicating whether we have read in a greyvalue
            bool readGrey = false;
            int plen;
            for( plen = 1; plen <= prevLClen; plen++)
            {
                prevSubCode = prevLeftCode.substr(0,plen);
                iter = gs.mpCdCol.find(prevSubCode);
                if(iter != gs.mpCdCol.end())
                {
                    // the subCode is valid code
                    grey = (iter -> second);
    #ifdef OUTDEBUG
                    cout << "The valid preSubCode is " << prevSubCode << endl;
                    cout << "The color is : " << grey << endl;
    #endif
                    
                    gs.vec_gV[index] = grey;
                    gs.pixels[index] = ((float)grey)/255;
                    readGrey = true;
                    break;
                }
            }
            if(readGrey)
            {
                prevLeftCode= prevLeftCode.substr(plen,prevLClen);
                // do I need this one?
                accumulated = prevSubCode;
                continue; 
            }
            // is >> readIn;
            GetNextByte(is,readIn);
            theCode = Byte2Codes(readIn);
    #ifdef OUTDEBUG
            cout << "the code is " << theCode << endl;
    #endif
            if(prevLClen > 0)
            {
                theCode = prevLeftCode + theCode;
            }


            // if(overflow)
            // {
            //     theCode = theCode.substr(overflow,8);
            // }


            codes subCode;
            
            int theCodeBound = theCode.size();
            
            // since the prevLeftCode is examined before and
            // we are sure that the prevLeftCode doesnt include 
            // the code so we begin the substring length of prevLClen + 1
            // note this is also true if prevLeftCode is nullstring
            // in which we case we simply start at the len 1

    #ifdef OUTDEBUG
            cout << "Before theCodeBOund loop the prevLCLen is: " << prevLClen << endl;
    #endif
            for(int j = (prevLClen + 1); j <= theCodeBound; j++)
            {
                subCode = theCode.substr(0,j);

    #ifdef OUTDEBUG
                cout << "In the for loop theCodeBound: subCode\n";
                cout << subCode << endl;

    #endif
                iter = gs.mpCdCol.find(subCode);
            

    #ifdef OUTDEBUG
                // if(subCode == "11")
                // {
                //     cout << " the 11 code is " << gs.mpCdCol["11"] << endl;
                //     cout << "the subCode mpCdCol is " <<  gs.mpCdCol[subCode] << endl;
                // }

    #endif
                if(iter != gs.mpCdCol.end())
                {
                    // the subCode is valid code
    #ifdef OUTDEBUG                
                    cout << "The valid code is: " << subCode << endl;
    #endif                
                    grey = (iter -> second);
                    gs.vec_gV[index] = grey;

    #ifdef OUTDEBUG
                    cout << "The color is: " << grey << endl;
    #endif            
                    gs.pixels[index] = ((float)grey)/255;
                    readGrey = true;
                    prevLeftCode = theCode.substr(j,theCodeBound);
                    break;
                }
            }
            if(readGrey)
            {
                continue;
            }
            else
            {
                accumulated = theCode;
                // codes temp;

                // as long as we can read
                // read as many bits as possible until we find
                // the corresponding code
                while(is.good())
                {   

                    // is >> readIn;
                    GetNextByte(is,readIn);
                    theCode = Byte2Codes(readIn);
                    // if(overflow)
                    // {
                    //     theCode = theCode.substr(overflow,8);
                    // }
                    // codes subCode;
                    theCode = accumulated + theCode;

    #ifdef OUTDEBUG
                    cout << "in the while " << endl;
                    cout << "The theCode " << theCode << endl;
    #endif
                    // the len of the string at which the search
                    // for code begin
                    int downBound = accumulated.size();
                    int tcBound = theCode.size();
                    for(int j = (downBound + 1); j <= tcBound; j++)
                    {
                        subCode = theCode.substr(0,j);
                        iter = gs.mpCdCol.find(subCode);
                        if(iter != gs.mpCdCol.end())
                        {
                            // the subCode is valid code
    #ifdef OUTDEBUG
                            cout << "the valid subcode: " <<endl;
                            cout << subCode << endl;
    #endif
                            grey = (iter -> second);
                            gs.vec_gV[index] = grey;

    #ifdef OUTDEBUG
                            cout << "color is " << grey << endl;
    #endif
                            gs.pixels[index] = ((float)grey)/255;
                            readGrey = true;
                            prevLeftCode = theCode.substr(j,tcBound);

    #ifdef OUTDEBUG
                            cout << "the prevLeftC is " << prevLeftCode << endl;
    #endif
                            break;
                        }
                    }
                    if(readGrey)
                    {
                        break;
                    }
                    else
                    {
                        accumulated = theCode;
                    }
                    
                }
                if(!is.good())
                {
                    cout << "cant decode fully\n";
                    exit(DECODE_ERROR);
                }
            }
        }
    }
    else if(form == 3)
    {
    #ifdef OUTDEBUG
        cout << "The form is 3\n";
        cout << "*******************************\n";
        cout << "in the readhuff code The inverse is:\n";
        map_codingColor::const_iterator iter2;
        cout << "   code       |  color " << endl;
        for(iter2 = gs.mpCdCol.begin(); iter2 != gs.mpCdCol.end(); iter2++)
        {
            cout.width(8);
            cout << iter2 -> first << "\t" << iter2 -> second << endl;
        }

    #endif

        byte readIn;
        codes theCode;
        theCode.clear();
        // indicates the bits borrowed from the next complete byte
        // int overflow = 0;
        // indicates the code left from the previous one
        codes prevLeftCode;
        prevLeftCode.clear();
        // indicates the accumulated code
        codes accumulated;
        accumulated.clear();

        int sizeBild = gs.width * gs.height;
        map_codingColor::iterator iter;
        greyValue grey;
        for(int index = 0; index < sizeBild; index++)
        {
            // first test whether the prevLeftCode includes code
            int prevLClen = prevLeftCode.size();
    #ifdef OUTDEBUG
            if(prevLClen > 0)
            {
                cout << "the prevLeftCode : " << prevLeftCode << endl;
            }
    #endif
            codes prevSubCode;
            prevSubCode.clear();
            // flag indicating whether we have read in a greyvalue
            bool readGrey = false;
            int plen;
            for( plen = 1; plen <= prevLClen; plen++)
            {
                prevSubCode = prevLeftCode.substr(0,plen);
                iter = gs.mpTransCdCol.find(prevSubCode);
                if(iter != gs.mpTransCdCol.end())
                {
                    // the subCode is valid code
                    grey = (iter -> second);
    #ifdef OUTDEBUG
                    cout << "The valid preSubCode is " << prevSubCode << endl;
                    cout << "The color is : " << grey << endl;
    #endif
                    
                    gs.vtrans_gV[index] = grey;
                    // have to transform first to get the value
                    // gs.pixels[index] = ((float)grey)/255;
                    readGrey = true;
                    break;
                }
            }
            if(readGrey)
            {
                prevLeftCode= prevLeftCode.substr(plen,prevLClen);
                // do I need this one?
                accumulated = prevSubCode;
                continue; 
            }
            // is >> readIn;
            GetNextByte(is,readIn);
            theCode = Byte2Codes(readIn);
    #ifdef OUTDEBUG
            cout << "the code is " << theCode << endl;
    #endif
            if(prevLClen > 0)
            {
                theCode = prevLeftCode + theCode;
            }


            // if(overflow)
            // {
            //     theCode = theCode.substr(overflow,8);
            // }


            codes subCode;
            
            int theCodeBound = theCode.size();
            
            // since the prevLeftCode is examined before and
            // we are sure that the prevLeftCode doesnt include 
            // the code so we begin the substring length of prevLClen + 1
            // note this is also true if prevLeftCode is nullstring
            // in which we case we simply start at the len 1

    #ifdef OUTDEBUG
            cout << "Before theCodeBOund loop the prevLCLen is: " << prevLClen << endl;
    #endif
            for(int j = (prevLClen + 1); j <= theCodeBound; j++)
            {
                subCode = theCode.substr(0,j);

    #ifdef OUTDEBUG
                cout << "In the for loop theCodeBound: subCode\n";
                cout << subCode << endl;

    #endif
                iter = gs.mpTransCdCol.find(subCode);
            

    #ifdef OUTDEBUG
                // if(subCode == "11")
                // {
                //     cout << " the 11 code is " << gs.mpCdCol["11"] << endl;
                //     cout << "the subCode mpCdCol is " <<  gs.mpCdCol[subCode] << endl;
                // }

    #endif
                if(iter != gs.mpTransCdCol.end())
                {
                    // the subCode is valid code
    #ifdef OUTDEBUG                
                    cout << "The valid code is: " << subCode << endl;
    #endif                
                    grey = (iter -> second);
                    gs.vtrans_gV[index] = grey;

    #ifdef OUTDEBUG
                    cout << "The color is: " << grey << endl;
    #endif            
                    // gs.pixels[index] = ((float)grey)/255;
                    readGrey = true;
                    prevLeftCode = theCode.substr(j,theCodeBound);
                    break;
                }
            }
            if(readGrey)
            {
                continue;
            }
            else
            {
                accumulated = theCode;
                // codes temp;

                // as long as we can read
                // read as many bits as possible until we find
                // the corresponding code
                while(is.good())
                {   

                    // is >> readIn;
                    GetNextByte(is,readIn);
                    theCode = Byte2Codes(readIn);
                    // if(overflow)
                    // {
                    //     theCode = theCode.substr(overflow,8);
                    // }
                    // codes subCode;
                    theCode = accumulated + theCode;

    #ifdef OUTDEBUG
                    cout << "in the while " << endl;
                    cout << "The theCode " << theCode << endl;
    #endif
                    // the len of the string at which the search
                    // for code begin
                    int downBound = accumulated.size();
                    int tcBound = theCode.size();
                    for(int j = (downBound + 1); j <= tcBound; j++)
                    {
                        subCode = theCode.substr(0,j);
                        iter = gs.mpTransCdCol.find(subCode);
                        if(iter != gs.mpTransCdCol.end())
                        {
                            // the subCode is valid code
    #ifdef OUTDEBUG
                            cout << "the valid subcode: " <<endl;
                            cout << subCode << endl;
    #endif
                            grey = (iter -> second);
                            gs.vtrans_gV[index] = grey;

    #ifdef OUTDEBUG
                            cout << "color is " << grey << endl;
    #endif
                            // gs.pixels[index] = ((float)grey)/255;
                            readGrey = true;
                            prevLeftCode = theCode.substr(j,tcBound);

    #ifdef OUTDEBUG
                            cout << "the prevLeftC is " << prevLeftCode << endl;
    #endif
                            break;
                        }
                    }
                    if(readGrey)
                    {
                        break;
                    }
                    else
                    {
                        accumulated = theCode;
                    }
                    
                }
                if(!is.good())
                {
                    cout << "cant decode fully\n";
                    exit(DECODE_ERROR);
                }
            }
        }
    }

    // eat zero
    char ch;
    is >> ch;
    while((ch = is.get()) != EOF)
    {
        continue;
    }
    return is;  
}

codes Vect2Codes(vec_Codes & veC)
{
    int vecSize = veC.size();
    // codes mycode = 0;
#ifdef OUTDEBUG
    vec_Codes::const_iterator iter;

    cout << "The veC content\n";
    for(iter = veC.begin(); iter != veC.end(); iter++)
    {
        cout << *iter << '\t';
    }
    cout << endl;
#endif
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
        result = result << 1;
        result += str_code[i] - '0';
    }
    return result;
}

codes Byte2Codes(const byte & theByte)
{
    codes theCode;
    byte copyByte = theByte;
    theCode.clear();
    for(int count = 7; count >= 0; count--)
    {
        byte theCharacter = (copyByte >> 7) + '0';
        copyByte = copyByte << 1; 
        theCode.push_back((char)theCharacter);
    }
    return theCode;
}

istream &  GetNextByte(istream & is, byte & theByte)
{
    char tempCh;
    is.get(tempCh);
    theByte = (byte) tempCh;
    return is;
}

ostream & GreyScale::outFreq32Bit(ostream & os, const freQuency & freq) const
{   
    byte toWrite;
#ifdef OUTDEBUG
    cout << "fr:" << freq << endl;
#endif    
    for(int i = 3; i > 0; i--)
    {
        toWrite =  freq >> 8 * i;
#ifdef OUTDEBUG        
        cout << "The toWrite is \n";
        cout << (unsigned short) toWrite << endl;
        
        // if(!toWrite)
        // {
        //     toWrite = 0b00000000;
        // }
        // // os << "toWrite:" << toWrite << endl;
#endif        
        os << toWrite;
    }
    toWrite = (byte) freq % 256;
#ifdef OUTDEBUG
    cout << "The toWrite is \n";
    cout <<  (unsigned short) toWrite << endl;
    // if(!toWrite)
    // {
    //     toWrite = 0b00000000;
    // }
#endif
    os << toWrite;
    // os << "toWrite:" << toWrite << endl;
    return os;
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
        return (*this);
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