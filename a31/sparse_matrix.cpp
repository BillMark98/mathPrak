#include "sparse_matrix.h"
#include "vektor.h"
#include <cmath>
#include <cstdlib>
#include <iomanip>


using std::cout;

Sparse_Matrix::Sparse_Matrix (size_t ze, size_t sp) 
: Zeil(ze),Spalt(sp) // Konstruktor
{

}
Sparse_Matrix::Sparse_Matrix  (const Sparse_Matrix& m1)          // Kopierkonstruktor
{
    Zeil = m1.Zeil;
    Spalt = m1.Spalt;
    Mat = m1.Mat;
}

    void    Sparse_Matrix::put(size_t i, size_t j, double x)   // Matrixelement schreiben
    {
        // cout << "put function called\n";
        if( i < 0 || j < 0 || i >= Zeil || j >= Spalt)  // index begins with 0
        {
            MatFehler("Illegal index to read matrix data in put");
        }
        key pos(i,j);
        hashmap::const_iterator iter = Mat.find(pos);
        if(x != 0)
        {
            Mat[pos] = x;
        }
        else if(iter != Mat.end()) // set 0 so will erase the value
        {
            Mat.erase(iter);
        }
        
    }
    double & Sparse_Matrix::operator() (size_t i,size_t j)
    {
        if( i < 0 || j < 0 || i >= Zeil || j >= Spalt)
        {
            MatFehler("Illegal index to read matrix data in double & op()");
        }
        key pos(i,j);
        return Mat[pos];
    }
    double  Sparse_Matrix::operator () (size_t ze, size_t sp) const
    {
        // cout << "the double () const called\n";
        if( ze < 0 || sp < 0 || ze >= Zeil || sp >= Spalt)
        {
            MatFehler("Illegal index to read matrix data in double op()");
        }
        
        key pos(ze,sp);
        hashmap::const_iterator iter = Mat.find(pos);
        if(iter != Mat.end())
        {
            return iter -> second;
        }
        else
        {
            // cout << "should be zero\n";
            return 0; // no value specified ,default 0
        }
        
    }
    double  Sparse_Matrix::get(size_t ze, size_t sp) const
    {
        // cout << "The get function called\n";
        if( ze < 0 || sp < 0 || ze >= Zeil || sp >= Spalt)
        {
            MatFehler("Illegal index to read matrix data in get()");
        }
        key pos(ze,sp);
        hashmap::const_iterator iter = Mat.find(pos);
        if(iter != Mat.end())
        {
            return iter -> second;
        }
        else
        {
            return 0; // no value specified ,default 0
        }
    }

    Sparse_Matrix& Sparse_Matrix::operator =  (const Sparse_Matrix& m1)   // Zuweisung
    {
        // cout << "The operator=(const SM&) called\n";
        Zeil = m1.Zeil;
        Spalt = m1.Spalt;
        Mat = m1.Mat;
        return *this;
    }
    Sparse_Matrix& Sparse_Matrix::operator += (const Sparse_Matrix& m1)   // Zuweisungen mit arithm. Operation
    {
        // cout << "The operator+=(const SM&) called\n";
        if(m1.Zeil != Zeil || m1.Spalt != Spalt)
        {
            MatFehler("Size mismatch by addition of matrix");
        }
        hashmap::const_iterator iter1;
        hashmap::const_iterator iter2;
        for(size_t i = 0; i < Zeil; i++)
        {
            for(size_t j = 0; j < Spalt; j++)
            {
                key pos(i,j);
                iter1 = Mat.find(pos);
                iter2 = m1.Mat.find(pos);
                if(iter1 != Mat.end())
                {
                    if(iter2 != Mat.end())
                    {
                        double result = (*this)(i,j) + m1(i,j);
                        if(abs(result) > EPSILON)
                        {
                            (*this)(i,j) += m1(i,j);
                        }
                        else
                        {
                            Mat.erase(iter1);
                        }
                        
                    }
                    // otherwise the corresponding place at mat is zero
                    // no need to do anything
                }
                else if(iter2 != Mat.end())
                {
                    // Mat has value 0 at this place

                    put(i,j,m1(i,j));
                }
            }
        }
        return (*this);
    }

    Sparse_Matrix& Sparse_Matrix::operator -= (const Sparse_Matrix& m1)
    {
        // cout << "The operator -= (const SM&) called\n";
        if(m1.Zeil != Zeil || m1.Spalt != Spalt)
        {
            MatFehler("Size mismatch by addition of matrix");
        }
        hashmap::const_iterator iter1;
        hashmap::const_iterator iter2;
        for(size_t i = 0; i < Zeil; i++)
        {
            for(size_t j = 0; j < Spalt; j++)
            {
                key pos(i,j);
                iter1 = Mat.find(pos);
                iter2 = m1.Mat.find(pos);
                if(iter1 != Mat.end())
                {
                    if(iter2 != Mat.end())
                    {
                        double result = (*this)(i,j) - m1(i,j);
                        if(abs(result) > EPSILON)
                        {
                            (*this)(i,j) -= m1(i,j);
                        }
                        else // a zero value no need to save
                        {
                            Mat.erase(iter1);
                        }
                        
                    }
                    // otherwise the corresponding place at mat is zero
                    // no need to do anything
                }
                else if(iter2 != Mat.end())
                {
                    // Mat has value 0 at this place

                    put(i,j,-m1(i,j));
                }
            }
        }
        return (*this);
    }
    Sparse_Matrix& Sparse_Matrix::operator *= (const Sparse_Matrix& m1)
    {
        if(m1.Zeil != Spalt)
        {
            MatFehler("Size mismatch by matrix multiplication");
        }
        hashmap::const_iterator iter1;
        hashmap::const_iterator iter2;
        Sparse_Matrix product(Zeil,m1.Spalt);
        for(size_t i = 0; i < Zeil; i++)
        {
            for(size_t j = 0; j < m1.Spalt; j++)
            {
                double sum = 0;
                for(size_t k = 0; k < Spalt; k++)
                {
                    key pos1(i,k);
                    key pos2(k,j);
                    iter1 = Mat.find(pos1);
                    iter2 = m1.Mat.find(pos2);
                    if(iter1 != Mat.end() && iter2 != Mat.end())
                    {
                       // this is the nontrivial case
                       // a nonnegative term will be added to sum
                       sum += (iter1 -> second) * (iter2 -> second);
                    }
                }
                if(abs(sum) > EPSILON)
                    put(i,j,sum);
            }
        }
        (*this) = product;
        return (*this);
    }
    Sparse_Matrix& Sparse_Matrix::operator *= (double c)
    {
        if(Zeil < 0 || Spalt < 0)
        {
            MatFehler("Multiplication of a null matrix with const");
        }
        hashmap::const_iterator iter;
        if(abs(c) <= EPSILON) // multiply with zero
        {
            Mat.erase(Mat.begin(),Mat.end());
            return (*this);
        }
        for(size_t i = 0; i < Zeil; i++)
        {
            for(size_t j = 0; j < Spalt; j++)
            {
                key pos(i,j);
                iter = Mat.find(pos);
                if(iter != Mat.end())
                {
                    (*this)(i,j) *= c; // c is non-zero
                }
            }
        }
        return (*this);
    }
    Sparse_Matrix& Sparse_Matrix::operator /= (double c)
    {
        if(Zeil < 0 || Spalt < 0)
        {
            MatFehler("Multiplication of a null matrix with const");
        }
        if(abs(c) <= EPSILON)
        {
            MatFehler("Division of zero in matrix division by const");
        }
        hashmap::const_iterator iter;
        for(size_t i = 0; i < Zeil; i++)
        {
            for(size_t j = 0; j < Spalt; j++)
            {
                key pos(i,j);
                iter = Mat.find(pos);
                if(iter != Mat.end())
                {
                    (*this)(i,j) /= c;
                }
            }
        }
        return (*this);
    }

    Sparse_Matrix& Sparse_Matrix::ReDim(size_t ze, size_t sp)    
    {
        if(ze <= 0 || sp <= 0)
        {
            MatFehler("Invalid parameter for size. Row and Column have to be positive");
        }
        // if(ze < Zeil || sp < Spalt)
        // {
        //     cout << "Matrix converted to a smaller matrix, data losed.\n";
        // }
        // if(Zeil > ze)
        // {
        //     for(size_t i = ze + 1; i <= Zeil; i++)
        //     {
        //         for(size_t j = 1; j <= Spalt; j++)
        //         {
        //             Mat.erase(key(i,j));
        //         }
        //     }
        // }
        // if(Spalt > sp)
        // {
        //     for(size_t i = 1; i <= Zeil; i++)
        //     {
        //         for(size_t j = sp + 1; j <= Spalt; j++)
        //         {
        //             Mat.erase(key(i,j));
        //         }
        //     }
        // }
        hashmap::iterator iter;
        Mat.erase(Mat.begin(),Mat.end());
        Zeil = ze;
        Spalt = sp;

        return *this;
    }

  Sparse_Matrix   operator +  (const Sparse_Matrix& m1, const Sparse_Matrix& m2)
  {
      if(m1.Zeil <= 0 || m1.Spalt <= 0 || m2.Zeil <= 0 || m2.Spalt <= 0)
      {
          Sparse_Matrix::MatFehler("At least one matrix is null during friend matrix addition");
      }
      if(m1.Zeil != m2.Zeil || m1.Spalt != m2.Spalt)
      {
          Sparse_Matrix::MatFehler("Size mismatch during friend matrix addition");
      }
      size_t ze = m1.Zeil;
      size_t sp = m1.Spalt;
      Sparse_Matrix sumMatrix(ze,sp);
      hashmap::const_iterator iter1;
      hashmap::const_iterator iter2;
      for(size_t i = 0; i < ze; i++)
      {
          for(size_t j = 0; j< sp; j++)
          {
              iter1 = m1.Mat.find(key(i,j));
              iter2 = m2.Mat.find(key(i,j));
              // since all the values are initialized to zero
              // the following is the only case we have to modify 
              // the sumMatrix at (i,j) , which is there is at least one nonzero term
              // during addition
              if(iter1 != m1.Mat.end() || iter2 != m2.Mat.end())
              {
                  double result = (iter1 -> second) + (iter2 -> second);
                  if(abs(result) > EPSILON) // the result is nonzero
                  {
                      sumMatrix(i,j) = result;
                  }
              }
          }
      }
      return sumMatrix;
  }
    Sparse_Matrix   operator -  (const Sparse_Matrix& m1, const Sparse_Matrix& m2)
  {
        if(m1.Zeil <= 0 || m1.Spalt <= 0 || m2.Zeil <= 0 || m2.Spalt <= 0)
      {
          Sparse_Matrix::MatFehler("At least one matrix is null during friend matrix addition");
      }
      if(m1.Zeil != m2.Zeil || m1.Spalt != m2.Spalt)
      {
          Sparse_Matrix::MatFehler("Size mismatch during friend matrix addition");
      }
      size_t ze = m1.Zeil;
      size_t sp = m1.Spalt;
      Sparse_Matrix subMatrix(ze,sp);
      hashmap::const_iterator iter1;
      hashmap::const_iterator iter2;
      for(size_t i = 0; i < ze; i++)
      {
          for(size_t j = 0; j< sp; j++)
          {
              iter1 = m1.Mat.find(key(i,j));
              iter2 = m2.Mat.find(key(i,j));
              // since all the values are initialized to zero
              // the following is the only case we have to modify 
              // the sumMatrix at (i,j) , which is there is at least one nonzero term
              // during addition
              if(iter1 != m1.Mat.end() || iter2 != m2.Mat.end())
              {
                  double result =  (iter1 -> second) - (iter2 -> second);
                  if(abs(result) > EPSILON) // the result is nonzero
                  {
                      subMatrix(i,j) = result;
                  }
              }
          }
      }
      return subMatrix;
  }
  Sparse_Matrix   operator -  (const Sparse_Matrix& m1)
  {
      if(m1.Zeil <= 0 || m1.Spalt <= 0)
      {
          Sparse_Matrix::MatFehler("trying to negate a null matrix");
      }
      size_t ze = m1.Zeil;
      size_t sp = m1.Spalt;
      Sparse_Matrix negMatrix(ze,sp);
      hashmap::const_iterator iter;
      for(size_t i = 0; i < ze; i++)
      {
          for(size_t j = 0; j < sp; j++)
          {
              iter = m1.Mat.find(key(i,j));
              if(iter != m1.Mat.end())
              {
                  negMatrix(i,j) = -m1(i,j);
              }
          }
      }
      return negMatrix;
  }

  Sparse_Matrix   operator *  (double c, const Sparse_Matrix& m1)
  {
        if(m1.Zeil <= 0 || m1.Spalt <= 0)
        {
            Sparse_Matrix::MatFehler("trying to multiply const with a null matrix");
        }
        size_t ze = m1.Zeil;
        size_t sp = m1.Spalt;
        Sparse_Matrix prodMatrix(ze,sp);
        if(abs(c) <= EPSILON)
        {
            return prodMatrix; // prodMatrix is a null matrix;
        }
        hashmap::const_iterator iter;
        for(size_t i = 0; i < ze; i++)
        {
            for(size_t j = 0; j < sp; j++)
            {
                iter = m1.Mat.find(key(i,j));
                if(iter != m1.Mat.end())
                {
                    prodMatrix(i,j) = m1(i,j) * c;
                }
            }
        }
        return prodMatrix;
      
  }
  Sparse_Matrix   operator *  (const Sparse_Matrix& m1, double c)
  {
      return operator *(c,m1);
  }
    Sparse_Matrix   operator /  (const Sparse_Matrix& m1, double c)
  {
    //   if(abs(c) <= EPSILON)
    //   {
    //      Sparse_Matrix::MatFehler("Matrix divide zero in friend");
    //   }
      return (1/c * m1);
  }

    bool     operator == (const Sparse_Matrix& m1, const Sparse_Matrix& m2)
    {
        if(m1.Zeil != m2.Zeil || m1.Spalt != m2.Spalt)
        {
            return false;
        }
        return m1.Mat == m2.Mat;
    }
    bool     operator != (const Sparse_Matrix& m1, const Sparse_Matrix& m2)
  {
      return !(m1 == m2);
  }

    std::istream& operator >> (std::istream& is, Sparse_Matrix& m1)            // Eingabe
    {
        std::cout << std::setiosflags(std::ios::right);
        for (size_t i = 0; i < m1.Zeil; i++)
        {
            for(size_t j = 0; j < m1.Spalt; j++)
            {
                std::cout << "\n(" << std::setw(4) << i << " , " << j << ") ";
                is >> m1(i,j);
            }
            
        }
        return is;
    }

  std::ostream& operator << (std::ostream& os, const Sparse_Matrix& mat)
  {
    //   hashmap::const_iterator iter;
    // //   wrong version 
    // //   Sparse_Matrix test_mat(4,3);
    // //     test_mat.put(1,3,2);
    // //     test_mat.put(3,1,5);
    // //     test_mat.put(2,3,10);
    // //     test_mat.put(2,2,7); 
    // //     test_mat.put(4,3,17); 
    //     // cout << test_mat;  the output will be 17      10      7       2       5 
    //     // two problems: 1. unordered 
    //   for(iter = mat.Mat.begin(); iter != mat.Mat.end(); iter++)
    //   {
    //       os << iter -> second << "\t";
    //   }
        hashmap::const_iterator iter;
        for(size_t i = 0; i < mat.Zeil; i++)
        {
            for(size_t j = 0; j < mat.Spalt; j++)
            {
                os.width(5);
                // can't do that terminate called after throwing an instance of 'std::out_of_range'
                //  what():  _Map_base::at
                // os << mat(i,j); 
                key pos(i,j);
                iter = mat.Mat.find(pos);
                if(iter != mat.Mat.end())
                {
                    os << iter -> second; 
                }
                else  // element not saved thus is a zero
                {
                    os << 0;
                }
                
            }
            os << std::endl;
        }
      return os;
  }

    Vektor   operator *  (const Sparse_Matrix& m1, const Vektor& v1) // Sparse_Matrix-Vektor-
  {
      if(m1.Spalt != v1.Dimen())
      {
          Sparse_Matrix::MatFehler("unmatched size of matrix and vector");
      }
        size_t ze = m1.Zeil;      
        Vektor vresult(ze);
        hashmap::const_iterator iter;
        for(size_t i = 0; i < ze; i++)
        {
            double sum = 0;
            for(size_t k = 0; k < m1.Spalt; k++)
            {
                iter = m1.Mat.find(key(i,k));
                if(iter != m1.Mat.end())
                {
                    sum += (iter -> second) * v1(k); // vector index begins with 0
                }
            }  
            vresult(i) = sum;
        }
        return vresult;
  }
    Vektor   operator *  (const Vektor& v1, const Sparse_Matrix& m1)
    {
        if(m1.Zeil != v1.Dimen())
        {
            Sparse_Matrix::MatFehler("unmatched size of matrix and vector in transpose mult");
        }
        size_t ze = m1.Zeil;  
        size_t sp = m1.Spalt;    
        Vektor vresult(ze);
        hashmap::const_iterator iter;
        for(size_t i = 0; i < sp; i++)
        {
            double sum = 0;
            for(size_t k = 0; k < ze; k++)
            {
                iter = m1.Mat.find(key(k,i));
                if(iter != m1.Mat.end())
                {
                    sum += (iter -> second) * v1(k); // vector index begins with 0
                }
            } 
            vresult(i) = sum;
        }
        return vresult;
    }

void Sparse_Matrix::MatFehler (const char str[])
{
    std::cerr << "\nMatrixfehler: " << str << '\n' << std::endl;
    std::abort();
}