#include "greyscale.h"

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
    if(i < 0 || j < 0 || i >= width || j >= width)
    {
        return 0;
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