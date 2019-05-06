#include "vektor.h"
#include "sparse_matrix.h"
using std::cout;
using std::cin;
using std::endl;

int main()
{
    Sparse_Matrix test_mat(4,3);
    Sparse_Matrix test2(4,3);
    test_mat.put(1,3,2);
    test_mat.put(3,1,5);
    test_mat.put(2,3,10);
    test_mat.put(2,2,7);
    test_mat.put(4,3,17);
    test2.put(1,3,5);
    test2.put(3,1,10);
    test2.put(1,1,8);
    test2.put(4,1,5);

    hashfunktion myhash;
    key pos1(1,3);
    key pos2(3,1);
    cout << "(1,3) will be hashed to " << myhash(pos1) << endl;
    cout << "(3,1) will be hashed to " << myhash(pos2) << endl;
    
    cout << "Matrix1 out\n";
    cout << test_mat << endl;
    cout << "Mtrix2 out\n";
    cout << test2 << endl;

    cout << "Matrix addition\n";
    test_mat += test2;

    cout << test_mat << endl;
    hashmap mymatrix;
    
    mymatrix[key(3,1)] = 4;
    mymatrix[key(3,2)] = 1;
    mymatrix.erase(key(3,1));
    mymatrix.erase(key(3,1));
    cout << mymatrix[key(3,1)] << endl;
    cout << mymatrix[key(3,2)] << endl;
    cout << mymatrix[key(2,2)] << endl;
    return 0;
}