/*************************************************
*  Name       : vektor.h                  *
*  Verwendung : Header zu Vektorklasse   *
*  Autoren    : V. Reichelt, T. Barsch, K. Urban *
*               IGPM RWTH Aachen                 *
*  Anpassung:   K. Brix, F. Gruber,              *
*  Anpassung:  H. Sass, P. Brandner, S. Groß     *
*  Datum      : Nov '96 - Apr '19                *
*************************************************/

#ifndef _VEKTOR_H                         // vektor.h nicht doppelt benutzen
#define _VEKTOR_H

#include <iostream>
#include <cfloat>
class Sparse_Matrix;                      // fuer friend Matrix * Vektor etc.
#define EPSILON DBL_MIN

class Vektor
{
  private:
    double*  Vek;                         // Zeiger auf Feld fuer Vektorelemente
    size_t   _dimen;                      // Vektorlaenge

    static void VekFehler (const char str[]);   // Fehlermeldung ausgeben

  public:
    explicit Vektor (size_t i=1);         // Konstruktor mit Laengenangabe
    ~Vektor ();                           // Destruktor
    Vektor  (const Vektor&);              // Kopierkonstruktor

    double& operator () (size_t);         // Standard Zugriff auf Vektorelement
    double  operator () (size_t) const;   // Zugriff auf const Vektor

    Vektor& operator =  (const Vektor&);  // Zuweisung
    Vektor& operator += (const Vektor&);  // Zuweisungen mit arithm. Operation
    Vektor& operator -= (const Vektor&);
    Vektor& operator *= (double);
    Vektor& operator /= (double);

    Vektor& ReDim   (size_t);                    // neue Laenge festlegen
    size_t  Dimen   () const { return _dimen; }  // Laenge
    size_t  Laenge  () const { return _dimen; }  // Laenge
    double  Norm2   () const;                    // Euklidische Norm des Vektors
    double  NormMax () const;                    // Maximum-Norm des Vektors

    friend Vektor   operator +  (const Vektor&, const Vektor&); // Addition
    friend Vektor   operator -  (const Vektor&, const Vektor&); // Subtraktion
    friend Vektor   operator -  (const Vektor&);                // Vorzeichen

    friend double   operator *  (const Vektor&, const Vektor&); // Skalarprodukt
    friend Vektor   operator *  (double, const Vektor&);        // Vielfache
    friend Vektor   operator *  (const Vektor&, double);
    friend Vektor   operator /  (const Vektor&, double);
    friend Vektor   operator /  (const Vektor&, const Vektor&);
    friend Vektor   operator %  (const Vektor&, const Vektor&);

    friend bool     operator == (const Vektor&, const Vektor&); // Vergleich
    friend bool     operator != (const Vektor&, const Vektor&);

    friend std::istream& operator >> (std::istream&, Vektor&);       // Eingabe
    friend std::ostream& operator << (std::ostream&, const Vektor&); // Ausgabe

    friend Vektor   operator *  (const Sparse_Matrix&, const Vektor&); // Matrix-Vektor-
    friend Vektor   operator *  (const Vektor&, const Sparse_Matrix&); // Multiplikation
};

#endif
