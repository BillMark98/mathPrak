#include "vektor.h"
#include "sparse_matrix.h"
#include "unit.h"
using std::cin;
using std::cout;
using std::endl;

int gsv(Sparse_Matrix &A, const Vektor &b, Vektor &x0, const int k_max, double &eps);
int cg(const Sparse_Matrix &A, const Vektor &b, Vektor &x0, const int k_max, double &eps);
int main()
{
    // Sparse_Matrix test_mat(4,3);
    // Sparse_Matrix test2(4,3);
    // // test_mat.put(1,3,2);
    // // test_mat.put(3,1,5);
    // // test_mat.put(2,3,10);
    // // test_mat.put(2,2,7);
    // // test_mat.put(4,3,17);
    // // test2.put(1,3,5);
    // // test2.put(3,1,10);
    // // test2.put(1,1,8);
    // // test2.put(4,1,5);

    // hashfunktion myhash;
    // key pos1(0,0);
    // key pos2(3,1);
    // cout << "(0,0) will be hashed to " << myhash(pos1) << endl;
    // cout << "(3,1) will be hashed to " << myhash(pos2) << endl;
    // cout << "Value at 0,0" << endl;
    // test_mat.put(0,0,1);
    // cout << test_mat(0,0) << endl;

    // cout << "Matrix1 out\n";
    // cout << test_mat << endl;
    // cout << "Mtrix2 out\n";
    // cout << test2 << endl;

    // cout << "Matrix addition\n";
    // test_mat += test2;

    // cout << test_mat << endl;
    // hashmap mymatrix;

    // mymatrix[key(3,1)] = 4;
    // mymatrix[key(3,2)] = 1;
    // mymatrix.erase(key(3,1));
    // mymatrix.erase(key(3,1));
    // cout << mymatrix[key(3,1)] << endl;
    // cout << mymatrix[key(3,2)] << endl;
    // cout << mymatrix[key(2,2)] << endl;

    Sparse_Matrix A;
    Vektor b;
    Vektor x0;
    double eps;
    int Bsp = 2;
    int maxiter;
    // int Bsp, Sparse_Matrix &A, Vektor &x0, Vektor &b, double &tol, int &maxiter
    Start(Bsp, A, x0, b, eps, maxiter);
    try
    {
        int result = gsv(A, b, x0, maxiter, eps);
        Ergebnis(x0, result, 0);
    }
    catch (...)
    {
        cout << "exception called\n";
        b.~Vektor();
        x0.~Vektor();
    }
    // void Ergebnis ( Vektor &x, int Iterationen, int Methode );

    return 0;
}

int gsv(Sparse_Matrix &A, const Vektor &b, Vektor &x0, const int k_max, double &eps)
{
    if (A.Zeilen() != A.Spalten() || A.Spalten() != b.Dimen())
    {
        cout << "Size mismatch in gsv, can't do the calculation\n";
        return -1;
    }
    // A = AL + D + AR
    size_t Asize = A.Zeilen();
    Vektor d(Asize);
    // cout << "The size :" << Asize << endl;
    // cout << "testing next" << endl;
    for (size_t i = 0; i < Asize; i++)
    {
        // cout << "creating d" << endl;
        d(i) = A(i, i);
        if (d(i) <= EPSILON)
        {
            cout << "small value on the diagonal at " << i << endl;
        }
    }
    // Sparse_Matrix AL(Asize,Asize);
    // Sparse_Matrix AR(Asize,Asize);
    // for(size_t i = 1; i < Asize; i++) // i begins at the second row
    // {
    //     cout << "creating AL" << endl;
    //     for(size_t j = 0; j < i; j++) // j < i
    //     {
    //         double value = A(i,j);
    //         AL.put(i,j,value);
    //     }
    // }
    // for(size_t i = 0; i < Asize; i++)
    // {
    //     cout << "creating AR" << endl;
    //     for(size_t j = i+1; j < Asize; j++)  // j > i
    //     {
    //         cout << "j value " << j << endl;
    //         double value = A(i,j);
    //         AR.put(i,j,value);

    //     }
    //     cout << "i value " << i << endl;
    // }
    // cout << "after the AR" << endl;
    // Sparse_Matrix ALR  = AL + AR;
    for (size_t i = 0; i < Asize; i++)
    {
        // cout << "creating d" << endl;
        A.put(i, i, 0);
    }
    // cout << "the matrix all created" << endl;
    int count = 0;
    Vektor diff = A * x0 - b;
    while (count <= k_max && diff.Norm2() > eps)
    {
        Vektor x_temp = (b - A * x0) / d;
        x0 = x_temp;
        diff = A * x0 - b;
        diff += d % x0; // since A is modified as AL + AR , the diagonal has to be multiplied componentwise
        // with x0 to get the correct diff
        count++;
    }

    for (size_t i = 0; i < Asize; i++)
    {
        // cout << "creating d" << endl;
        A.put(i, i, d(i));
    }

    if (count <= k_max)
    {
        return count;
    }
    else
    {
        return 0;
    }
}

int cg(const Sparse_Matrix &A, const Vektor &b, Vektor &x0, const int k_max, double &eps)
{
    if (A.Zeilen() != A.Spalten() || A.Spalten() != b.Dimen())
    {
        cout << "Size mismatch in cg, can't do the calculation\n";
        return -1;
    }
    size_t Asize = A.Zeilen();
    // cout << "The matrix A: " << endl;
    // cout << A;
    // cout << "The size: " << endl;
    // cout << Asize << endl;
    // cout << "The vector b: " << endl;
    // cout << b;
    // cout << "x0: " << x0 << endl;
    // cout << "\nk_max: " << k_max << endl;
    // cout << "eps: " << eps << endl;
    // test symmetric

    for (size_t i = 0; i < Asize; i++)
    {
        for (size_t j = i + 1; j < Asize; j++)
        {
            if (A(i, j) != A(i, j))
            {
                cout << "matrix not symmetric!\n";
                break;
            }
        }
    }
    Vektor r = b - A * x0;
    Vektor d(r);
    int count = 0;
    while (count <= k_max && r.Norm2() > eps)
    {
        // cout << "***********************" << endl;
        // cout << "In the " << count << " loop" << endl;
        Vektor Ad = A * d;
        // cout << "Ad: \n"
        //      << Ad << endl;
        double rnorm2_square = r.Norm2() * r.Norm2();
        double alpha = rnorm2_square / (Ad * d);
        // cout << "rnorm2_square: " << rnorm2_square << "\talpha: " << alpha << endl;

        x0 +=  alpha * d;
        // cout << "x_temp: \n"
        //      << x0<< endl;
        Vektor r_next = r - alpha * Ad;
        // cout << "r_next:\n " << r_next << endl;
        double r_next_norm2_square = r_next.Norm2() * r_next.Norm2();
        double beta = r_next_norm2_square / rnorm2_square;
        // cout << "r_next_norm2_square: " << r_next_norm2_square << "\tbeta: " << beta << endl;
        d *= beta;
        d += r_next;
        // cout << "d_temp = \n"
        //      << d << endl;
        r = r_next;
        count++;
    }
    // cout << "x0: " << endl;
    // cout << x0;
    // cout << "count: " << count << endl;
    // cout << "The difference: Ax - b\n"
    //      << A * x0 - b << endl;
    if (count <= k_max)
    {
        return count;
    }
    else
    {
        return 0;
    }
}
