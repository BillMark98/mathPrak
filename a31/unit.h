/*************************************************
*  Name       : unit.h                           *
*  Verwendung : Header zu unit.cpp               *
*  Autoren    : V. Reichelt, T. Barsch, K. Urban *
*               IGPM RWTH Aachen                 *
*  Anpassung:   K. Brix, F. Gruber,              *
*  Anpassung:  H. Sass, P. Brandner, S. Groß     *
*  Datum      : Nov '96 - Apr '19                *
*************************************************/

#include <cmath>
#include <cstdlib>
#include <iostream>
#include "sparse_matrix.h"
#include "vektor.h"
#include <vector>

extern const int AnzahlBeispiele;
/**
   Eingabe: int Bsp, Matrix A, Vektro x0, Vektor b, double tol, int maxiter.
   Ausgabe: Matrix A, Vektor x0, Vektor b, double tol, int maxiter.
 */
void Start ( int Bsp, Sparse_Matrix &A, Vektor &x0, Vektor &b, double &tol, int &maxiter);

/**
 Methode: 0 -> GSV, 1-> CG
 */
void Ergebnis ( Vektor &x, int Iterationen, int Methode );
