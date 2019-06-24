#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
using std::pair;
typedef pair<int,int> XYCoord;

#define SIZE_MISMATCH 22
#define EVEN_SIZE_MASK 23
#define UNEXPECTED_CHAR 24
class GreyScale
{
    private:
        int     width,height;
        float   *pixels;

    public:
        GreyScale(int w = 0, int h = 0);
        GreyScale(const GreyScale &);
        ~GreyScale();

        void Resize(int w, int h);
        int GetWidth() const {return width;}
        int GetHeight() const { return height;}
        // float& operator()(int i, int j);
        float operator()(int i, int j) const;
        // convert the tuple(i,j) to the corresponding index of the array pixels
        // for the tuple (i,j) i stands for the (i+1)-th column j for (j+1)-th row
        // because --------->  width
        //         |
        //         |
        //         |
        //         v  height   
        int XYCoord2Vec(int i, int j) const;
        // convert the index of the pixels array to the tuple (i,j)
        XYCoord VecCoord2XY(int i) const;

        // arithmetic operation
        GreyScale & operator=(const GreyScale & gs);
        GreyScale & operator+=(const GreyScale & gs);
        GreyScale & operator-=(const GreyScale & gs);

        // filter function
        GreyScale Binarize(float c) const;
        GreyScale Blur() const;
        GreyScale Clamp() const;
        GreyScale Contrast() const;
        GreyScale Convolve(const float mask[],int size = 3) const;
        GreyScale Kirsch() const;
        GreyScale Laplace() const;
        GreyScale LinTrans(float a, float b) const;
        GreyScale Invert() const;
        GreyScale Median() const;
        GreyScale Sobel() const;

        // help functions
        float pixelMedian(int index) const;

        // IO function
        friend istream & operator>>(istream & is, GreyScale & gs);
        friend ostream & operator<<(ostream & os, const GreyScale & gs);

};