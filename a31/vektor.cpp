/***********************************************
*  Name       : vektor.cpp                     *
*  Verwendung : Vektorklasse                   *
*  Autor      :                                *
*  Datum      :                                *
***********************************************/

#include "vektor.h"
#include <cmath>
#include <cstdlib>
#include <iomanip>


// =======================
//      Konstruktoren
// =======================


// ----- Konstruktor -----
using std::nothrow;
using std::cout;


Vektor::Vektor (size_t l)
{
#ifndef NDEBUG
  if (l<=0)
    VekFehler("Nur Vektoren mit positiver Laenge!");
#endif

  _dimen=l;

  Vek=new (std::nothrow) double[l]; //This constant value is used as an argument for operator new and operator new[] to indicate that these functions shall not throw an exception on failure, but return a null pointer instead.
  if (Vek==NULL)
    VekFehler("Nicht genuegend Speicher!");

  for (size_t i=0; i<l; i++)
    (*this)(i)=0;
}


// ----- Destruktor -----

Vektor::~Vektor () {
  if(Vek != NULL)
  {
    delete [] Vek;
  }
}


// ----- Kopierkonstruktor -----

Vektor::Vektor (const Vektor &x)
{
  _dimen=x._dimen;

  Vek=new (nothrow) double[_dimen];
  if (Vek==NULL)
    VekFehler("Nicht genuegend Speicher!");

  for (size_t i=0; i<_dimen; i++)
    (*this)(i)=x(i);
}


// ===========================================
//      Vektorelement schreiben und lesen
// ===========================================


// ----- Standardzugriff auf Vektorelemente -----

double& Vektor::operator () (size_t i)
{
#ifndef NDEBUG
  if (i>=_dimen)
    VekFehler("Ungueltiger Index!");
#endif

  return Vek[i];
}


// ----- Lesezugriff auf Elemente eines const Vektors -----

double Vektor::operator () (size_t i) const
{
#ifndef NDEBUG
  if (i>=_dimen)
    VekFehler("Ungueltiger Index!");
#endif

  return Vek[i];
}


// =====================
//      Zuweisungen
// =====================


// ----- Zuweisungsoperator "=" ----

Vektor& Vektor::operator = (const Vektor &x)
{
  if (_dimen!=x._dimen) {
    delete []Vek;
    _dimen=x.Dimen();
    Vek=new (nothrow) double[_dimen];
    if (Vek==NULL)
      VekFehler("Nicht genuegend Speicher!");
  }

  for (size_t i=0; i<_dimen; i++)
    (*this)(i)=x(i);

  return *this;
}


// ----- Zuweisungsoperator mit Addition "+=" ----

Vektor& Vektor::operator += (const Vektor &x)
{
  if(_dimen != x._dimen)
  {
    VekFehler("Size mismatch. Can't do addition between two vectors of different match");
  }
  for(size_t i = 0; i < _dimen; i++)
  {
    (*this)(i) += x(i);
  }
  return *this;
}


// ----- Zuweisungsoperator mit Subtraktion "-=" ----

Vektor& Vektor::operator -= (const Vektor &x)
{
  if(_dimen != x._dimen)
  {
    VekFehler("Size mismatch. Can't do subtraction between two vectors of different match");
  }
  for(size_t i = 0; i < _dimen; i++)
  {
    (*this)(i) -= x(i);
  }
  return *this;
}


// ----- Zuweisungsoperator mit Multiplikation "*=" ----

Vektor& Vektor::operator *= (double c)
{
  for(size_t i = 0; i < _dimen; i++)
  {
    (*this)(i) *= c;
  }
  return *this;
}


// ----- Zuweisungsoperator mit Divsion "/=" ----

Vektor& Vektor::operator /= (double c)
{
  if(abs(c) < EPSILON)
  {
    VekFehler("Divion over 0");
  }
  for(size_t i = 0; i < _dimen; i++)
  {
    (*this)(i) /= c;
  }
  return *this;
}


// ==============================
//      Vektorlaenge aendern
// ==============================


// ----- Vektorlaenge aendern -----

Vektor& Vektor::ReDim (size_t l)
{
  if(l < _dimen)
  {
    cout << "reducing size of vectors, choosing the first l elements.\n";
  }
  double * temp = new (nothrow) double[l];
  size_t min = (_dimen < l) ? _dimen : l;
  if(Vek != NULL)
  {
    for(size_t i = 0; i < min; i++)
    {
      temp[i] = Vek[i];
    }
    delete [] Vek;
  }
  Vek = temp;
  _dimen = l;
}


// ======================
//      Vektornormen
// ======================


// ----- Euklidische Norm -----

double Vektor::Norm2 () const
{
  if(Vek == NULL)
  {
    VekFehler("norm2 of no-existing vectors");
  }
  double sum = 0;
  for(size_t i = 0; i < _dimen; i++)
  {
    sum += Vek[i] * Vek[i];
  }
  return sqrt(sum);
}


// ----- Maximum-Norm -----

double Vektor::NormMax () const
{
  if(Vek == NULL)
  {
    VekFehler("norm2 of no-existing vectors");
  }
  double max = 0;
  for(size_t i = 0; i < _dimen; i++)
  {
    double temp = abs(Vek[i]);
    if(max < temp)
    {
      max = temp;
    }
  }
  return max;
}


// ==================================
//      arithmetische Operatoren
// ==================================


// ----- Addition "+" -----

Vektor operator + (const Vektor &x, const Vektor &y)
{
  if(x._dimen != y._dimen)
  {
    Vektor::VekFehler("Size mismatch of addition");
  }
  double dsize = x._dimen;
  Vektor vect = Vektor(dsize);
  for(size_t i = 0; i < dsize; i++)
  {
    vect(i) = x(i) + y(i);
  }
  return vect;
}


// ----- Subtraktion "-" -----

Vektor operator - (const Vektor &x, const Vektor &y)
{
  if(x._dimen != y._dimen)
  {
    Vektor::VekFehler("Size mismatch of subtraction");
  }
  double dsize = x._dimen;
  Vektor vect = Vektor(dsize);
  for(size_t i = 0; i < dsize; i++)
  {
    vect(i) = x(i) - y(i);
  }
  return vect;
}


// ----- Vorzeichen wechseln "-" -----

Vektor operator - (const Vektor &x)
{
  Vektor vect = Vektor(x._dimen);
  double dsize = x._dimen;
  for(size_t i = 0; i < dsize; i++)
  {
    vect(i) = -x(i);
  }
  return vect;
}


// ----- Skalarprodukt "*" -----

double operator * (const Vektor &x, const Vektor &y)
{
  if(x._dimen != y._dimen)
  {
    Vektor::VekFehler("scalar product mismatch");
  }
  double dsize = x._dimen;
  double sum = 0;
  for(size_t i = 0; i < dsize; i++)
  {
    sum += x(i) * y(i);
  }
  return sum;
}


// ----- Multiplikation Skalar*Vektor "*" -----

Vektor operator * (double c, const Vektor &x)
{
  double dsize = x._dimen;
  Vektor vect = Vektor(dsize);
  for(size_t i = 0; i < dsize; i++)
  {
    vect(i) = c * x(i);
  }
  return vect;
}


// ----- Multiplikation Vektor*Skalar "*" -----

Vektor operator * (const Vektor &x, double c)
{
  double dsize = x._dimen;
  Vektor vect = Vektor(dsize);
  for(size_t i = 0; i < dsize; i++)
  {
    vect(i) = c * x(i);
  }
  return vect;
}


// ----- Division Vektor/Skalar "/" -----

Vektor operator / (const Vektor &x, double c)
{
  if(abs(c) < EPSILON)
  {
    Vektor::VekFehler("Division over 0");
  }
  double dsize = x._dimen;
  Vektor vect = Vektor(dsize);
  for(size_t i = 0; i < dsize; i++)
  {
    vect(i) = x(i) / c;
  }
  return vect;
}

// ----- Division Vektor/Vektor "/"  <-->  D^(-1)*x -----

Vektor operator / (const Vektor &x, const Vektor &d)
{
  if(x._dimen != d._dimen)
  {
    Vektor::VekFehler("size mismatch of vector division");
  }
  double dsize = x._dimen;
  Vektor vect = Vektor(dsize);
  for(size_t i = 0; i < dsize; i++)
  {
    double temp = d(i);
    if( temp < EPSILON)
    {
      Vektor::VekFehler("Division over 0");
    }
    vect(i) = x(i) / temp;
  }
  return vect;
}

// ----- Vektor*.Vektor "%"  <--> komponentenweise Multiplikation -----

Vektor operator % (const Vektor &x, const Vektor &d)
{
  if(x._dimen != d._dimen)
  {
    Vektor::VekFehler("size mismatch of vector multiplication");
  }
  double dsize = x._dimen;
  Vektor vect = Vektor(dsize);
  for(size_t i = 0; i < dsize; i++)
  {
    vect(i) = d(i) * x(i);
  }
  return vect;
}


// ==============================
//      Vergleichsoperatoren
// ==============================


// ----- Test auf Gleichheit "==" -----

bool operator == (const Vektor &x, const Vektor &y)
{
  if (x._dimen!=y._dimen) return false;

  for (size_t i=0; i<x._dimen; i++)
    if (x(i)!=y(i)) return false;

  return true;
}


// ----- Test auf Ungleichheit "!=" -----

bool operator != (const Vektor &x, const Vektor &y)
{
  return !( x == y);
}


// ==========================
//      Ein- und Ausgabe
// ==========================


// ----- Ausgabe "<<" -----

std::ostream& operator << (std::ostream &s, const Vektor &x)
{
  s << std::setiosflags(std::ios::right);
  for (size_t i=0; i<x._dimen; i++)
    s << "\n(" << std::setw(4) << i << ") " << x(i);

  return s << std::endl;
}


// ----- Eingabe ">>" -----

std::istream& operator >> (std::istream &s, Vektor &x)
{
  std::cout << std::setiosflags(std::ios::right);
  for (size_t i=0; i<x._dimen; i++)
  {
    std::cout << "\n(" << std::setw(4) << i << ") ";
    s >> x(i);
  }
  return s;
}


// ==========================
//      Fehlerbehandlung
// ==========================


// ----- Ausgabe der Fehlermeldung "str" -----

// to do: exception handling
void Vektor::VekFehler (const char str[])
{
  std::cerr << "\nVektorfehler: " << str << '\n' << std::endl;
  std::abort();
}
