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
void display(unsigned int * feld, size_t n1);
int main()
{
    size_t len = 3;
    unsigned int * feld = new unsigned int[len];
    int beispiel = 2;
    start(beispiel,len,feld);
    cout << "Before sorting\n";
    display(feld,len);
    cout << "After sorting\n";
    quick_sort(feld,len);
    display(feld,len);


    delete [] feld;
}
void display(unsigned int * feld, size_t n1)
{
    size_t i;
    for(i = 0; i < n1; i++)
    {
        cout.width(6);
        cout << feld[i];
        if(i % 10 == 9)
            cout << endl;
    }
    cout << endl;
}
void bubble_sort(unsigned int *& feld, size_t len)
{
    int i,j;
    int length = (int) len;
    for(i = 0; i < length-1; i++)
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
        unsigned int v = feld[i];
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
    // cout << "New array to be considered:\n";
    // display(feld,len);
    if(len <= 1)
        return;
    
    unsigned int pivot = feld[len-1];// pivot as the last element
    size_t i = 0;
    size_t j = len-2;
    while(1) // instead of while(i < j) if len = 2 i = j = 0 directly quit the loop
            // but actually need modify i example 1 64 i must be 1 instead of 0
            // else would change the position after the while loop tausche(feld,i,len-1)

    {
        while( i < len-1 && feld[i] <= pivot )
        {
            i++;
        }
        while(j >= 0 && feld[j] >= pivot)
        {
            if(j == 0) break; // in case j == 0 then j-- will be very large since j unsigned
                            // which will make the comparison i >= j useless
            j--;
        }
        if(i >= j) break;
        else
        {
            tausche(feld,i,j);
        }
    }
    tausche(feld,i,len-1); // pivot now at index i
    quick_sort(feld,i);
    // if(len - i == 0)
    //     return;
    quick_sort(feld+i+1,len-i-1);
    
}
void heap_sort(unsigned int * feld, size_t len)
{
    size_t i;
    if(len >= 3)
    {
        for(i = len/2 - 1; i>=0; i--)  // len/2-1 in case of unsigned 
        {
            sink(feld,i,len);
            if(i == 0)
            {
                break; // since for i = 0, i-- will be very large due to the fact that
                // i is unsigned
            }
        }
        // for debug usage
        // cout << "the array after primary heap building.\n";
        // display(feld,len); 
    }
    else
    {
        // len == 1 or 0
        sink(feld,0,len);
    }
    
    for(i = len-1; i >=0; i--)
    {
        tausche(feld,0,i);
        // for debug usage
        // cout << "The "<< i << "-th element exchanged" << endl;
        // display(feld,len);
        sink(feld,0,i); // not sink(feld,0,len-1) or the heap building will continue forever
        // for debug usage
        // cout << "sink operation performed: \n";
        // display(feld,len);
        if(i == 0)
        {
            break; // the same as the above loop 
        }
    }
}
void sink(unsigned int * feld, size_t k, size_t len)
{
    // Creating maximum heap, that is the father is no less equal
    // than the son --> the largest element of the array is 
    // at the root of the tree
    size_t son;
    // for debug usage
    // cout << "In the sink function: \n";
    // display(feld,len);
    // cout << "the len: " << len << endl;
    while(1)
    {
        // for debug usage
        // cout << "The k: " << k << endl;
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
            k = son; // no forgetting this statement, else running continuously
            // for debug usage
            // cout << "After exchange of the father and son\n";
            // display(feld,len);
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
    //size_t j = len-1;
    size_t middle = len/2;
    // for debug usage
    // cout << "In merge sort: the len " << len << endl;
    // cout << " The whole array\n";
    // display(feld,len);
    merge_sort(feld,middle);
    // for debug usage
    // cout << "after sorting the first half\n";
    // display(feld,middle);
    merge_sort(feld + middle, len - middle);
    // for debug usage
    // cout << "after sorting the rest\n";
    // display(feld+middle,len-middle);
    unsigned int * newfeld = merge(feld,middle,feld + middle,len - middle);
    for(i = 0; i < len; i++)
    {
        feld[i] = newfeld[i];
    }
    // for debug usage
    // cout << "After merging\n";
    // display(feld,len);
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
        if(i < n1 && sfeld1[i] <= sfeld2[j])  // using <= in case  the two elements are equal if strict less than
                                            // since the else if also excludes the case of equality, no new element will be 
                                            // copied and the loop just continues forever since k++ performs unlimited times
        {
            newfeld[k] = sfeld1[i];
            i++;
        }
        else if(j < n2 && sfeld2[j] < sfeld1[i])
        {
            newfeld[k] = sfeld2[j];
            j++;
        }
        k++;
    }
    return newfeld;
}

