/*************************************************
*  Name       : sparse_matrix.h                  *
*  Verwendung : Header zu sparser Matrixklasse   *
*  Autoren    : V. Reichelt, T. Barsch, K. Urban *
*               IGPM RWTH Aachen                 *
*  Anpassung:   K. Brix, F. Gruber,              *
*  Anpassung:  H. Sass, P. Brandner, S. Groß     *
*  Datum      : Nov '96 - Apr '19                *
*************************************************/

#ifndef _SPARSE_MATRIX_H             // Sparse_Matrix.h nicht doppelt benutzen
#define _SPARSE_MATRIX_H

#include <iostream>
#include <unordered_map>

class Vektor;

typedef  std::pair<size_t, size_t> key; // Datentyp des Schlüssels

struct hashfunktion {             // Hashfunktion für Schlüsseldatentyp
    std::size_t operator() (key wert) const {
	std::hash<size_t> hashFunktion;
	return hashFunktion(wert.first) + hashFunktion(wert.second);
}};

typedef std::unordered_map<key, double, hashfunktion> hashmap; // Hashmap


class Sparse_Matrix
{
  private:
    size_t  Zeil, Spalt;             // Matrixdimension
    hashmap Mat;                     // Hasmap fuer Matrixelemente

    static void MatFehler (const char str[]);  // Fehlermeldung ausgeben

  public:
    explicit Sparse_Matrix (size_t ze=1, size_t sp=1);  // Konstruktor
    Sparse_Matrix  (const Sparse_Matrix&);              // Kopierkonstruktor

    void    put(size_t i, size_t j, double x);   // Matrixelement schreiben
    double  operator () (size_t, size_t) const;  // Matrixelement lesen
    double  get(size_t i, size_t j) const;

    Sparse_Matrix& operator =  (const Sparse_Matrix&);   // Zuweisung
    Sparse_Matrix& operator += (const Sparse_Matrix&);   // Zuweisungen mit arithm. Operation
    Sparse_Matrix& operator -= (const Sparse_Matrix&);
    Sparse_Matrix& operator *= (const Sparse_Matrix&);
    Sparse_Matrix& operator *= (double);
    Sparse_Matrix& operator /= (double);

    Sparse_Matrix& ReDim   (size_t, size_t);    // neue Dimensionen festlegen
    size_t  Zeilen  () const { return Zeil; }   // Zeilen
    size_t  Spalten () const { return Spalt; }  // Spalten

  friend Sparse_Matrix   operator +  (const Sparse_Matrix&, const Sparse_Matrix&); // Addition
  friend Sparse_Matrix   operator -  (const Sparse_Matrix&, const Sparse_Matrix&); // Subtraktion
  friend Sparse_Matrix   operator -  (const Sparse_Matrix&); // Vorzeichen

  friend Sparse_Matrix   operator *  (double, const Sparse_Matrix&); // Vielfache
  friend Sparse_Matrix   operator *  (const Sparse_Matrix&, double);
  friend Sparse_Matrix   operator /  (const Sparse_Matrix&, double);

  friend bool     operator == (const Sparse_Matrix&, const Sparse_Matrix&); // Vergleich
  friend bool     operator != (const Sparse_Matrix&, const Sparse_Matrix&);

  friend std::istream& operator >> (std::istream&, Sparse_Matrix&);            // Eingabe
  friend std::ostream& operator << (std::ostream&, const Sparse_Matrix&);      // Ausgabe

  friend Vektor   operator *  (const Sparse_Matrix&, const Vektor&); // Sparse_Matrix-Vektor-
  friend Vektor   operator *  (const Vektor&, const Sparse_Matrix&); // Multiplikation
};

#endif
