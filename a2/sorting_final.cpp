#include "unit.h"
using std::cout;
using std::cin;
using std::endl;

void bubble_sort(unsigned int * feld, size_t len);
void selection_sort(unsigned int * feld, size_t len);
void insertion_sort(unsigned int * feld, size_t len);
void quick_sort(unsigned int * feld, size_t len);
size_t partition1(unsigned int * feld, size_t len); // pivot element at last
size_t partition2(unsigned int * feld, size_t len); // pivot element at first
size_t partition3(unsigned int * feld, size_t len, size_t index); // pivot element at middle
// can't use unsigned int * & feld, because the 
// function call quick_sort(feld+i+1,len-i-1) is not valid
// since feld + i + 1 can't be a reference, which must be a lvalue
void quick_sort_mid3(unsigned int * feld, size_t len);
void heap_sort(unsigned int * feld, size_t len);
void sink(unsigned int * feld, size_t k, size_t len);
void merge_sort(unsigned int * feld, size_t len);
unsigned int * merge(unsigned int * feld1, size_t n1, unsigned int * feld2, size_t n2);
void display(unsigned int * feld, size_t n1);

int main()
{
    size_t len = 200;
    unsigned int * feld = new unsigned int[len];
    int beispiel = 2;
    start(beispiel,len,feld);
    cout << "Before sorting\n";
    display(feld,len);
    
    merge_sort(feld,len);
    cout << "After sorting\n";
    display(feld,len);
    ergebnis(feld);


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

void bubble_sort(unsigned int * feld, size_t len)
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


void selection_sort(unsigned int * feld, size_t len)
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
void insertion_sort(unsigned int * feld, size_t len)
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
void quick_sort_mid3(unsigned int * feld, size_t len)
{
    if(len <= 1)
        return;
    size_t first = 0, middle = len/2, end = len - 1;


    size_t min = 0;
    size_t index;
    if(feld[first] <= feld[end])
    {
        if(feld[first] >= feld[middle])
        {
            min = first;
            index = partition2(feld,len);

        }
        else if(feld[middle] <= feld[end])
        {
            min = middle;
            index = partition3(feld,len,min);
        }
        else
        {
            min = end;
            index = partition1(feld,len);
        }
    }
    else if(feld[middle] <= feld[end])
    {
        min = end;
        index = partition1(feld,len);
    }
    else
    {
        min = middle;
        index = partition3(feld,len,min);
    }

    quick_sort_mid3(feld,index);
    quick_sort_mid3(feld+index+1,len-index-1);
    
}

size_t partition1(unsigned int * feld, size_t len) // pivot element at last
{
    if(len <= 1)
        return 0;

    unsigned int pivot = feld[len-1];// pivot as the last element
    size_t i = 0;
    size_t j = len-2;
    // instead of while(i < j) if len = 2 i = j = 0 directly quit the loop
            // but actually need modify i example 1 64 i must be 1 instead of 0
            // else would change the position after the while loop tausche(feld,i,len-1)
    while(1)
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
    tausche(feld,i,len-1);
    return i;
}
size_t partition2(unsigned int * feld, size_t len) // pivot element at first
{
    if(len <= 1)
        return 0;

    size_t i = 1;
    size_t j = len - 1;

    unsigned int pivot = feld[0];
    while(1)
    {
        while(i < len && feld[i] <= pivot)
        {
            if(i == len - 1) break; // in case i at the end of the array and will still add one will come
            // the out of the array and tausche(feld,i,index) will swap the element which is not in the array 
            // to the array
            i++;
        }
        while(j >= 1 && feld[j] >= pivot)
        {
            j--;
        }
        if( i >= j) break;
        else
        {
            tausche(feld,i,j);
        }
    }
    tausche(feld,j,0);  // different to the case partition1 we exchange j with pivot
    // because the feld[j] <= pivot and we will move the feld[j] forward
    // if we change feld[i] with pivot then the larger element will be moved
    // to the front of the array and contradict to the property that
    // all left hand side of the pivot is smaller or equal to itself
    
    return j;
}
size_t partition3(unsigned int * feld, size_t len, size_t index) // pivot element at middle
{
    if(len <= 1)
        return 0;


    unsigned int pivot = feld[index];
    size_t i = 0;
    size_t j = len - 1;
    while(1)
    {
        while(i < len && feld[i] <= pivot)
        {
            if(i == len - 1) break; // in case i at the end of the array and will still add one will come
            // the out of the array and tausche(feld,i,index) will swap the element which is not in the array 
            // to the array
            i++;
        }
        while(j >= 0 && feld[j] >= pivot)
        {
            if(j == 0) break;
            j--;
        }
        if(i >= j) break;
        else
        {
            tausche(feld,i,j);
        }
    }
    if(i <= index)
    {
        if(i < index) // else i == index need not to swap
            tausche(feld, i, index);
        return i;
    }
    else if(j >= index)
    {
        if(j != index)
            tausche(feld,j,index);
        return j;
    }
    else
    {
        // in this case pivot is at the right place
        // since i > index means that all elements at the left hand side
        // of the index has value smaller or equal to itself
        // j < index means that all the elements at the right hand side
        // of index is larger orr equal to itself need not to swap
        // this case is possible for example [16 17 18] pivot is 17 
        // in the end, i = 2, j = 0 so need not to swap
        
        return index;
    }
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
    }
    else
    {
        // len == 1 or 0
        sink(feld,0,len);
    }
    
    for(i = len-1; i >=0; i--)
    {
        tausche(feld,0,i);
        sink(feld,0,i); // not sink(feld,0,len-1) or the heap building will continue forever
        
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
            k = son; // no forgetting this statement, else running continuously
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

    merge_sort(feld,middle);
    merge_sort(feld + middle, len - middle);
    unsigned int * newfeld = merge(feld,middle,feld + middle,len - middle);
    for(i = 0; i < len; i++)
    {
        feld[i] = newfeld[i];
    }
    delete [] newfeld;
}
unsigned int * merge(unsigned int * sfeld1, size_t n1, unsigned int * sfeld2, size_t n2)
{
    size_t sum = n1 + n2;
    unsigned int * newfeld = new unsigned int[sum];
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
        else if(j < n2 && sfeld2[j] <= sfeld1[i]) // it should be the same reason as above use 
                                                // less than or equal
        {
            newfeld[k] = sfeld2[j];
            j++;
        }
        k++;
    }
    return newfeld;
}

