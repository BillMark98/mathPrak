#include "unit.h"
#include "student.h"
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::ifstream;
using std::ostream;
using std::string;

template<typename ElemT>
void einlesen(ifstream & ifs, vector<ElemT>& feld);

template<typename ElemT>
void ausgeben(ostream & os, const vector<ElemT>& feld);



bool operator< (const string& s1, const string & s2);
bool operator<= (const string& s1, const string & s2);
bool operator> (const string& s1, const string & s2);
bool operator>= (const string& s1, const string & s2);
bool operator==(const string& s1, const string& s2);
bool operator!= (const string& s1, const string& s2);

// sorting algo
template<typename T>
void tausche(vector<T> & feld, size_t i, size_t j);

template <typename T>
void bubble_sort(vector<T>& v);

template <typename T>
void selection_sort(vector<T>& v);

template <typename T>
void merge_sort(vector<T>&v);

template <typename T>
void merge(vector<T>& left, vector<T>& sfeld2,vector<T>& feld);
void eatline(void)
{
    while(cin.get() != '\n')
        continue;
}
int main()
{
    ifstream inFile;
    inFile.open("doubles.txt");
    if(!inFile.is_open())
    {
        cout << "File not opened successfully\n";
        exit(2);
    }
    vector<double> dArray;
    einlesen(inFile,dArray);
    cout << "Data read\n";
    ausgeben(cout,dArray);
    inFile.close();


    inFile.open("strings.txt");
    if(!inFile.is_open())
    {
        cout << "File not opened successfully\n";
        exit(2);
    }
    vector<string> sArray;
    einlesen(inFile,sArray);
    cout << "strings read\n";
    ausgeben(cout,sArray);
    inFile.close();


    inFile.open("studenten.txt");
    if(!inFile.is_open())
    {
        cout << "File not opened successfully\n";
        exit(2);
    }
    vector<Student> STArray;
    einlesen(inFile,STArray);
    cout << "strings read\n";
    ausgeben(cout,STArray);
    inFile.close();

    char ch;
    cout << "Please choose sorting algo for sortin doubles\n";
    cin >> ch;
    switch(ch)
    {
        case 'B':   bubble_sort(dArray);
                    break;
        case 'A':   selection_sort(dArray);
                    break;
        case 'M':   merge_sort(dArray);
                    break;
        default:    cout << "No sorting algo for this \n";
                    break;
    }
    eatline();
    cout << "\n\n*****sorting******* \n\n";
    cout << "After sorting\n";
    ausgeben(cout,dArray);
    ergebnis(dArray);


    cout << "Please choose sorting algo for sortin strings\n";
    cin >> ch;
    switch(ch)
    {
        case 'B':   bubble_sort(sArray);
                    break;
        case 'A':   selection_sort(sArray);
                    break;
        case 'M':   merge_sort(sArray);
                    break;
        default:    cout << "No sorting algo for this \n";
                    break;
    }
    eatline();
    cout << "\n\n*****sorting******* \n\n";
    cout << "After sorting\n";
    ausgeben(cout,sArray);
    ergebnis(sArray);

    cout << "Please choose sorting algo for sorting Students\n";
    cin >> ch;
    switch(ch)
    {
        case 'B':   bubble_sort(STArray);
                    break;
        case 'A':   selection_sort(STArray);
                    break;
        case 'M':   merge_sort(STArray);
                    break;
        default:    cout << "No sorting algo for this \n";
                    break;
    }
    eatline();
    cout << "\n\n*****sorting******* \n\n";
    cout << "After sorting\n";
    ausgeben(cout,STArray);
    ergebnis(STArray);

    
    return 0;
}


template<typename ElemT>
void einlesen(ifstream & ifs, vector<ElemT>& feld)
{
    ElemT temp;
    ifs >> temp;
    while(ifs.good())
    {
        feld.push_back(temp);
        ifs >> temp;
    }
    if(ifs.eof())
    {
        cout << "End of file reached.\n";
    }
    else if(ifs.fail())
    {
        cout << "Input terminated by data mismatch\n";
    }
    else
    {
        cout << "Input terminated by unknown reasons.\n";
    }
    
}

template<typename ElemT>
void ausgeben(ostream & os, const vector<ElemT>& feld)
{
    // ElemT temp;
    typename vector<ElemT>::const_iterator iter; // have to add typename at the beginning
    // or else error: need 'typename' before 'std::vector<ElemT>::iterator' because 'std::vector<ElemT>' is a dependent scope
    // int count = 0;

    // error: no match for 'operator=' (operand types are 'std::vector<double>::iterator {aka __gnu_cxx::__normal_iterator<double*, std::vector<double> >}' and 'std::vector<double>::const_iterator {aka __gnu_cxx::__normal_iterator<const double*, std::vector<double> >}')
    // for(iter = feld.begin(); iter != feld.end(); iter++)
    for(iter = feld.begin(); iter != feld.end(); iter++)
    {
        os << *iter << endl;
    }
    os << endl;
}
bool operator< (const string& s1, const string & s2)
{
    return (s1.compare(s2) < 0);
}
bool operator<= (const string& s1, const string & s2)
{
    return (s1 < s2) || (s1 == s2);
}
bool operator> (const string& s1, const string & s2)
{
    return (s1.compare(s2) > 0);
}
bool operator>= (const string& s1, const string & s2)
{
    return (s1 > s2) || s1 == s2;
}
bool operator==(const string& s1, const string& s2)
{
    return (s1.compare(s2) == 0);
}
bool operator!= (const string& s1, const string& s2)
{
    return (s1.compare(s2)) ;
}

template<typename T>
void tausche(vector<T> & feld, size_t i, size_t j)
{
    T temp;
    temp = feld[i];
    feld[i] = feld[j];
    feld[j] = temp;
}

template <typename T>
void bubble_sort(vector<T>& feld)
{
    size_t i,j;
    size_t len = feld.size();
    if(!len) // vector of length 0
    {
        return;
    }
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

template <typename T>
void selection_sort(vector<T>& feld)
{
    size_t i,j;
    size_t len = feld.size();
    if(!len) // len == 0
    {
        return;
    }
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

template <typename T>
void merge_sort(vector<T>& feld)
{
    size_t len = feld.size();
    if(len <= 1)
        return;
    //size_t j = len-1;
    size_t middle = len/2;
    vector<T> left(feld.begin(),feld.begin()+middle);
    vector<T> right(feld.begin()+middle, feld.end());
    merge_sort(left);
    merge_sort(right);
    merge(left,right,feld);
}

template <typename T>
void merge(vector<T>& left, vector<T>& right, vector<T>& feld)
{
    size_t n1 = left.size();
    size_t n2 = right.size();
    size_t i = 0, j = 0, k = 0;
    
    while( i < n1 || j < n2)
    {
        if(i >= n1)
        {
            feld[k] = right[j];
            j++;
            k++;
            continue;
        }
        if(j >= n2)
        {
            feld[k] = left[i];
            i++;
            k++;
            continue;
        }
        if(i < n1 && left[i] <= right[j])  // using <= in case  the two elements are equal if strict less than
                                            // since the else if also excludes the case of equality, no new element will be 
                                            // copied and the loop just continues forever since k++ performs unlimited times
        {
            feld[k] = left[i];
            i++;
        }
        else if(j < n2 && right[j] <= left[i]) // it should be the same reason as above use 
                                                // less than or equal
        {
            feld[k] = right[j];
            j++;
        }
        k++;
    }
}