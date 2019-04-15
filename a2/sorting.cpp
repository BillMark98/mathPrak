#include "unit.h"

using std::cout;
using std::cin;
using std::endl;

void bubble_sort(unsigned int *& feld, size_t len);
void selection_sort(unsigned int *& feld, size_t len);
void insertion_sort(unsigned int *& feld, size_t len);
void quick_sort(unsigned int * feld, size_t len);
// can't use unsigned int * & feld, because the 
// function call quick_sort(feld+i+1,len-i-1) is not valid
// since feld + i + 1 can't be a reference, which must be a lvalue
void heap_sort(unsigned int * feld, size_t len);
void sink(unsigned int * feld, size_t k, size_t len);
void merge_sort(unsigned int * feld, size_t len);
unsigned int * merge(unsigned int * feld1, size_t n1, unsigned int * feld2, size_t n2);
int main()
{
    size_t len = 10;
    unsigned int * feld = new unsigned int[len];
    int beispiel = 2;
    start(beispiel,len,feld);
    

    delete [] feld;
}
void bubble_sort(unsigned int *& feld, size_t len)
{
    int i,j;
    int length = (int) len;
    for(i = 0; i < len-1; i++)
    {
        for(j = len-1; j > i; j--)
        {
            if(feld[j-1] > feld[j])
            {
                tausche(feld,j-1,j);
            }
        }
    }
}


void selection_sort(unsigned int *& feld, size_t len)
{
    size_t i,j;
    for(i = 0; i < len-1; i++)
    {
        size_t min = i;
        for(j = i+1; j < len; j++)
        {
            if(feld[j] < feld[min])
            {
                min = j;
            }
        }
        tausche(feld,i,min);
    }
}
void insertion_sort(unsigned int *& feld, size_t len)
{
    size_t i,j;
    for(i = 1; i < len; i++)
    {
        int v = feld[i];
        for(j = i-1;j >= 0; j--)
        {
            if(v < feld[j])
            {
                feld[j+1] = feld[j];
            }
            else
            {
                break;
            }
            
        }
        feld[j+1] = v;
    }
}
void quick_sort(unsigned int * feld, size_t len)
{
    if(len <= 1)
        return;
    
    unsigned int pivot = feld[len-1];// pivot as the last element
    size_t i = 0;
    size_t j = len-2;
    while(i < j)
    {
        if( i <len && feld[i] <= pivot ) i++;
        if(j >= 0 && feld[j] >= pivot) j--;
        if(i >= j) break;
        else
        {
            tausche(feld,i,j);
        }
    }
    tausche(feld,i,len-1); // pivot now at index i

    quick_sort(feld,i);
    quick_sort(feld+i+1,len-i-1);
    
}
void heap_sort(unsigned int * feld, size_t len)
{
    size_t i;
    for(i = len/2; i>=0; i--)
    {
        sink(feld,i,len);
    }
    for(i = len-1; i >=0; i--)
    {
        tausche(feld,0,i);
        sink(feld,0,len-1);
    }
}
void sink(unsigned int * feld, size_t k, size_t len)
{
    size_t son;
    while(1)
    {
        if(2*k+1 >= len) return;
        if(2*k+2 >= len) son = 2*k+1;
        else
        {
            if(feld[2*k+1] < feld[2*k+2])
            {
                son = 2*k+2;
            }
            else
            {
                son = 2*k + 1;
            }
            
        }
        if(feld[son] > feld[k])
        {
            tausche(feld,k,son);
        }
        else
        {
            break;
        }
    }
    
}
void merge_sort(unsigned int * feld, size_t len)
{
    if(len <= 1)
        return;
    size_t i = 0;
    size_t j = len-1;
    size_t middle = (i+j)/2;
    merge_sort(feld,middle+1);
    merge_sort(feld + middle + 1, len - middle - 1);
    unsigned int * newfeld = merge(feld,middle+1,feld + middle + 1,len - middle - 1);
    for(i = 0; i < len; i++)
    {
        feld[i] = newfeld[i];
    }
    delete [] newfeld;
}
unsigned int * merge(unsigned int * sfeld1, size_t n1, unsigned int * sfeld2, size_t n2)
{
    unsigned int * newfeld = new unsigned int[n1 + n2];
    size_t i = 0, j = 0, k = 0;
    while( i < n1 || j < n2)
    {
        if(i >= n1)
        {
            newfeld[k] = sfeld2[j];
            j++;
            k++;
            continue;
        }
        if(j >= n2)
        {
            newfeld[k] = sfeld1[i];
            i++;
            k++;
            continue;
        }
        if(sfeld1[i] < sfeld2[j] && i < n1)
        {
            newfeld[k] = sfeld1[i];
            i++;
        }
        else if(sfeld2[j] < sfeld1[i] && j < n2)
        {
            newfeld[k] = sfeld2[j];
            j++;
        }
        k++;

    }
    return newfeld;
}