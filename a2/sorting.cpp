#include "unit.h"
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::ofstream;
using std::ostream;

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
void display(unsigned int * feld, size_t n1, ostream & out);
typedef void (*sort_func)(unsigned int * feld, size_t len);

ostream& outFile = cout;

int main()
{
    // sort_func sort_f;
    // cout << "welcome, please enter size of the array\n";
    // size_t len;
    // outFile.open("result.txt");
    // if(!outFile.is_open())
    // {
    //     cout << "File error\n";
    //     exit(1);
    // }
    // while(cin >> len)
    // {
        
    //     cout << "Beispiel number: ";
    //     int beispiel;
    //     if(!(cin >> beispiel))
    //         break;
    //     cout << "choose sorting algo:\n";
    //     cout << "1 bubble   2 selection    3 insertion\n";
    //     cout << "4 quicksort        5 quicksort modified\n";
    //     cout << "6 heapsort         7 mergesort\n";
    //     int num;
    //     if(!(cin >> num))
    //         break;
    //     while(num > 7 || num < 1)
    //     {
    //         while(cin.get() != '\n')
    //             continue;
    //         cout << "fault number please choose again.\n";
    //         cout << "1 bubble   2 selection    3 insertion\n";
    //         cout << "4 quicksort        5 quicksort modified\n";
    //         cout << "6 heapsort         7 mergesort\n";
    //         cin >> num;
    //     }
    //     switch(num)
    //     {
    //         case 1 :    sort_f = bubble_sort;
    //                     break;
    //         case 2 :    sort_f = selection_sort;
    //                     break;
    //         case 3 :    sort_f = insertion_sort;
    //                     break;
    //         case 4 :    sort_f = quick_sort;
    //                     break;
    //         case 5 :    sort_f = quick_sort_mid3;
    //                     break;
    //         case 6 :    sort_f = heap_sort;
    //                     break;
    //         case 7 :    sort_f = merge_sort;
    //                     break;
    //         default:    cout << "wrong number\n";
    //                     continue;
    //     }     
    //     unsigned int * feld = new unsigned int[len];
    //     start(beispiel,len,feld);
    //     cout << "Before sorting\n";
    //     display(feld,len,outFile);
    //     cout << "After sorting\n";
    //     sort_f(feld,len);
    //     display(feld,len,outFile);
    //     ergebnis(feld);
    //     delete [] feld;
    //     cout << "please enter array lenth:";
    // }
    
    // outFile.close();

    size_t len = 200;
    unsigned int * feld = new unsigned int[len];
    int beispiel = 3;
    start(beispiel,len,feld);
    cout << "Before sorting\n";
    display(feld,len);
    
    quick_sort_mid3(feld,len);
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
void display(unsigned int * feld, size_t n1, ostream & out)
{
    size_t i;
    for(i = 0; i < n1; i++)
    {
        out.width(6);
        out << feld[i];
        if(i % 10 == 9)
            out << endl;
    }
    out << endl;
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
void quick_sort_mid3(unsigned int * feld, size_t len)
{
    if(len <= 1)
        return;
    size_t first = 0, middle = len/2, end = len - 1;

    // debug
    outFile << "New array to be considered:\n";
    display(feld,len,outFile);

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
    // debug
    outFile << "In the quick funcition,After partition the array:\n";
    display(feld,len,outFile);
    outFile << "The index: " << index << endl;

    quick_sort_mid3(feld,index);
    quick_sort_mid3(feld+index+1,len-index-1);
    
}

size_t partition1(unsigned int * feld, size_t len) // pivot element at last
{
    if(len <= 1)
        return 0;

    // debug
    outFile << "partition 1 called\n";
    outFile << "array considerd\n";
    display(feld,len,outFile);


    // cout << "partition 1 called\n";
    // cout << "The array\n";
    // display(feld,len);


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
        // debug
        outFile << "after two whiles i: " << i << "\tj: " << j << endl;
        if(i >= j) break;
        else
        {
            tausche(feld,i,j);
        }
        // debug
        outFile << "after one complete while loop the array\n";
        display(feld,len,outFile);

        
        // cout << "current i: " << i << "\t j: " << j << endl;
        // cout << "After one loop, the changed array\n";
        // display(feld,len);

    }
    // debug
    outFile << "The final i value is: " << i << endl;
    tausche(feld, i, len - 1);


    // tausche(feld,i,len-1);
    // cout << "Final array\n";
    // display(feld,len);

    return i;
}
size_t partition2(unsigned int * feld, size_t len) // pivot element at first
{
    if(len <= 1)
        return 0;

    // debug
    outFile << "partition 2 called\n";
    outFile << "array considerd\n";
    display(feld,len,outFile);

    // cout << "partition 2 called\n";
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
        // debug
        outFile << "after two whiles i: " << i << "\tj: " << j << endl;
        if( i >= j) break;
        else
        {
            tausche(feld,i,j);
        }
        // debug
        outFile << "after one complete while loop the array\n";
        display(feld,len,outFile);
        
    }

    // debug
    outFile << "The final i value is: " << i << endl;
    outFile << "The final j value is: " << j << endl;

    tausche(feld,j,0);  // different to the case partition1 we exchange j with pivot
    // because the feld[j] <= pivot and we will move the feld[j] forward
    // if we change feld[i] with pivot then the larger element will be moved
    // to the front of the array and contradict to the property that
    // all left hand side of the pivot is smaller or equal to itself
    
    // cout << "After partition 2 the returned j: " << j;
    // cout << "The array\n"; 
    // display(feld,len);

    return j;
}
size_t partition3(unsigned int * feld, size_t len, size_t index) // pivot element at middle
{
    if(len <= 1)
        return 0;
    
    // debug
    outFile << "partition 3 called\n";
    outFile << "array considerd\n";
    display(feld,len,outFile);
    outFile << "The index: " << index  << " the value: " << feld[index]<< endl;


    // cout << "partition 3 called.\n";
    // cout << "index: " << index << "\t the value: " << feld[index]<< endl;
    // cout << "array considered: " << endl;
    // display(feld,len);

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
        // debug
        outFile<< "after two whiles i: " << i << "\tj: " << j << endl;
        if(i >= j) break;
        else
        {
            tausche(feld,i,j);
        }
        // debug
        outFile << "after one complete while loop the array\n";
        display(feld,len,outFile);
    }
    // debug
    outFile << "The final i value is: " << i << endl;
    outFile << "The final j value is: " << j << endl;


        // cout << "after one loop, i: " << i << "\tj: " << j << endl;
        // cout << "The arrray changed.\n";
        // display(feld,len);

    if(i <= index)
    {
        if(i < index) // else i == index need not to swap
            tausche(feld, i, index);

        // debug use
        outFile << "swaping i and index\n";
        display(feld,len,outFile);


        // cout << "the i swapped with index, the i: " << i<< endl;
        // cout << "The array is:\n";
        // display(feld,len);
        return i;
    }
    else if(j >= index)
    {
        if(j != index)
            tausche(feld,j,index);

        // debug use
        outFile << "swaping j and index\n";
        display(feld,len,outFile);


        // cout << "The j swapped with index, the j: " << j << endl;
        // cout << "The array is : \n";
        // display(feld,len);


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
        
        // debug use
        outFile << "index at the right position!\n";
        display(feld,len,outFile);

        // cout << "No need to change index.\n";
        // cout << "the array: \n";
        // display(feld,len);

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
    // outFile << "In merge sort: the len " << len << endl;
    // outFile << " The whole array\n";
    // display(feld,len,outFile);

    merge_sort(feld,middle);
    // for debug usage

    // outFile << "after sorting the first half\n";
    // display(feld,middle,outFile);

    merge_sort(feld + middle, len - middle);
    // for debug usage

    // outFile << "after sorting the rest\n";
    // display(feld+middle,len-middle,outFile);

    unsigned int * newfeld = merge(feld,middle,feld + middle,len - middle);
    for(i = 0; i < len; i++)
    {
        feld[i] = newfeld[i];
    }
    // for debug usage
    // outFile << "After merging\n";
    // display(feld,len,outFile);

    delete [] newfeld;
}
unsigned int * merge(unsigned int * sfeld1, size_t n1, unsigned int * sfeld2, size_t n2)
{
    outFile << "the merge function successfully called\n";
    outFile << "the array 1:\n";
    display(sfeld1,n1,outFile);
    outFile << "the array 2:\n";
    display(sfeld2,n2,outFile);
    outFile << "In the merge function\n";
    outFile << "The size n1: " << n1 << "\t n2: " << n2 << endl;
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
        // outFile << "After the while loop, i: " << i << "\t j: " << j << endl;
        // display(newfeld,k-1,outFile);

    }
    return newfeld;
}

