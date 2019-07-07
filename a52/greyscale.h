#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <string>
#include "unit.h"

using namespace std;
using std::pair;

typedef pair<int,int> XYCoord;
typedef vector<float> vPixels;
typedef pair<unsigned int,int> pa_colorFreq;
typedef map<unsigned int,int> map_colorFreq;

#define SIZE_MISMATCH 22
#define EVEN_SIZE_MASK 23
#define UNEXPECTED_CHAR 24
#define IDENTICAL_TREE 25
#define WRONG_FORMAT 26
class GreyScale
{
    private:
        int     width,height;
        // float   *pixels;
        vPixels pixels;
        string  magicNumber;
        // Format 0 --- PGM 
        //        1 --- PGM-RAW
        //        2 --- MHa
        //        3 --- MHb
        static int Format;
        // save the frequencies of the color ( color is in unsigned int)
        map_colorFreq mapColFreq;
    public:
        GreyScale(int w = 0, int h = 0,string name = "P2");
        GreyScale(const GreyScale &);
        ~GreyScale();

        void Resize(int w, int h);
        int GetWidth() const {return width;}
        int GetHeight() const { return height;}
        static void SetFormat(int);
        // set the format via the magic number
        void MagicSetFormat();

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

class MyTree
{
    private:
        int Frequency;
        int GreyValue;
        MyTree * LeftTree;
        MyTree * RightTree;
    public:
        MyTree();
        MyTree(int freq, int grey);
        void SetLeft(MyTree & lTree);
        void SetRight(MyTree & rTree);
        // bool operator< (const MyTree & rTree) const;
        // bool operator==
        // friend MyTree TreeMerge(const MyTree & lTree, const MyTree & rTree);


        friend bool operator< (const MyTree & lTree,const MyTree & rTree);
        friend bool operator> (const MyTree & lTree, const MyTree & rTree);
        friend MyTree TreeMerge(MyTree & lTree, MyTree & rTree);
};