#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <algorithm>
#include <queue>
#include <map>
#include <cmath>
#include <string>
#include <functional>
#include "unit.h"



using namespace std;
using std::pair;

typedef unsigned int freQuency;
typedef unsigned int greyValue;
// typedef unsigned int codes;
typedef string codes;
typedef unsigned char Color;


typedef pair<int,int> XYCoord;
typedef vector<float> vPixels;
typedef pair<unsigned int,unsigned int> pa_colorFreq;
typedef map<greyValue,unsigned int> map_colorFreq;
typedef map<freQuency,greyValue> map_freqColor;
// typedef map<unsigned int, unsigned int> map_colorCoding;
// typedef map<unsigned int, unsigned int> map_codingColor;
typedef map<greyValue, string> map_colorCoding;
typedef map<string, greyValue> map_codingColor;
typedef vector<pa_colorFreq> vector_colFreq;
typedef vector<greyValue> vector_greyValue;

// typedef vector<unsigned int> vec_Codes;
typedef string vec_Codes;

#define SIZE_MISMATCH 22
#define EVEN_SIZE_MASK 23
#define UNEXPECTED_CHAR 24
#define IDENTICAL_TREE 25
#define WRONG_FORMAT 26
#define MAP_COL_FREQ_EMPtY 27
#define UNBALANCED_BRANCH 28
// when the code to be converted to byte is not 8 byte long
#define DECODE_LEN_MISMATCH 29
// when read in huffman code and cannot decode fully before we reach EOF
#define DECODE_ERROR 30
// #define OUTDEBUG 1

// for testing and debug, if set all class member is public
#define TEST 1
class MyTree
{
    public:
        freQuency Frequency;
        greyValue GreyValue;
        MyTree * LeftTree;
        MyTree * RightTree;
        // for the test whether the subtree is all deleted
        int TreeCount;
    public:
        MyTree();
        MyTree(freQuency freq, greyValue grey);
        MyTree(const MyTree & tr);
        // I think no need to write an explicit copy constructor
        MyTree & operator=(const MyTree & tr);
        bool IsLeaf() const;
        void SetLeft(MyTree & lTree);
        void SetRight(MyTree & rTree);
        // bool operator< (const MyTree & rTree) const;
        // bool operator==
        // friend MyTree TreeMerge(const MyTree & lTree, const MyTree & rTree);

        // Get Functions
        freQuency GetFrequency() const { return Frequency;}
        greyValue GetGreyValue() const { return GreyValue;}
        MyTree * GetLeft() const { return LeftTree;}
        MyTree * GetRight() const { return RightTree;}

        friend bool operator< (const MyTree & lTree,const MyTree & rTree);
        friend bool operator> (const MyTree & lTree, const MyTree & rTree);
        friend MyTree TreeMerge(MyTree & lTree, MyTree & rTree);
        friend ostream & operator<<(ostream & os,const MyTree & tr);
        // friend void deleteTree(MyTree * tr);
};
void deleteTree(MyTree * tr);

typedef deque<MyTree> vector_myTree;
class GreyScale
{

#ifndef TEST
    private:
#else
    public:
#endif
        int     width,height;
        // float   *pixels;
        vPixels pixels;
        // save the original grey value (a value from 0-255)
        vector_greyValue vec_gV;
        // save the transformed greyvalue
        vector_greyValue vtrans_gV;

        string  magicNumber;
        // Format 0 --- PGM 
        //        1 --- PGM-RAW
        //        2 --- MHa
        //        3 --- MHb
        static int Format;
        // save the frequencies of the color ( color is in unsigned int)
        map_colorFreq mapColFreq;
        // map_freqColor mapFreqCol;
        MyTree TrColFreq;
        map_colorCoding mpColCd;
        map_codingColor mpCdCol;
        
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

        // for the huffman
        void HuffmanCoding();
        void BuildTree();
        void BuildMap(MyTree & myT);
        
        // for the compression using transformation
        // transform the greyValue matrix saving the value in vtrans_gV
        void GreyTransform();
        // Inverse transfrom the matrix to the original
        void InverseGreyTransform();
        // calculate the new diff value given the position i,j
        greyValue DiffNeighbor(int i, int j);
        // calculate the new sum value given the position i,j
        greyValue SumNeighbor(int i, int j);
        // write the Huffman coded pixel value into the file
        friend ostream & WriteHuffCode(ostream & os,const GreyScale & gs);
        // read in the Huffman coded pixel value into GreyScale
        friend istream & ReadHuffCode(istream & is, GreyScale & gs);
        // IO function
        friend istream & operator>>(istream & is, GreyScale & gs);
        friend ostream & operator<<(ostream & os, const GreyScale & gs);

        // output the frequency in 32 Bit number
        // friend ostream & outFreq32Bit(ostream & os, const freQuency & freq);
        ostream & outFreq32Bit(ostream & os, const freQuency & freq) const;
};

// help functions
// convert the vec_Codes vec to a unsigned int
// for example vec = {0,1,0,0,1} convert it to 9
codes Vect2Codes(vec_Codes & veC);
// convert the code(string) to unsigned char
byte Codes2Byte(codes & str_code);
// convert the byte to a code, each bit becomes one byte
codes Byte2Codes(const byte & theByte);

// get the next byte from the istream
istream &  GetNextByte(istream & is, byte & theByte);