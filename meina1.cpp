/***********************************************************
*  Name       : meina1.cpp                                 *
*  Verwendung : Loesung der ersten Praktikumsaufgabe,      *
*               Nullstellen eines quadratischen Polynoms   *
*  Autor      :                                            *
*  Datum      :                                            *
*  Sprache    : C++                                        *
***********************************************************/

// Einbinden der Praktikums-Umgebung. Falls Sie die Ein-/Ausgabe zuerst
// nicht ueber die Praktikumsumgebung machen wollen, sollten Sie auch noch
// #include <iostream> einbinden.

#include "unit.h"
#define EPSILON 1e-9

// ===== Hauptprogramm =====
using std::cout;
using std::cin;
using std::endl;


int main()
{
  // Hier kommt nun Ihr Programm; viel Erfolg!
  cout << "Anzahl von Polynomen sind: " << AnzahlBeispiele << endl;
  cout << "Bitte die Nummer eingeben: ";
  int Bsp;
  cin >> Bsp;
  double a,b,c;
  Start(Bsp,a,b,c);
  int Anzahl;
  bool IstKomplex;
  double x1,x2;
  if(abs(a) > EPSILON)
  {
    Anzahl = 2;
    double p,q;
    p = b/a;
    q = c/a;
    if(abs(p) < EPSILON) // p = 0 x = sqrt(-q)
    {
      if(abs(q) < EPSILON) // x1 = x2 = 0
      {
         IstKomplex = false;
         x1 = 0;
         x2 = 0;
         Ergebnis(2,IstKomplex,x1,x2);
      }
      else if(q < 0)
      {
          IstKomplex = false;
          x1 = sqrt(-q);
          x2 = -sqrt(-q);
          Ergebnis(2,IstKomplex,x1,x2);
      }
      else
      {
          IstKomplex = true;
          // To do complex value in double
          Ergebnis(2,IstKomplex,0,sqrt(q));

      }
      
    }
    else if(p > 0)
    {
      double threshold = sqrt(DBL_MAX - abs(q));
      if(p/2 < threshold)
      {
        double delta = pow(p/2,2) - q;
        if(abs(delta) < EPSILON) // zwei identische NS
        {
            IstKomplex = false;
            x1 = -p/2;
            x2 = -p/2;
            Ergebnis(2,IstKomplex,x1,x2);
        }
        else if(delta > 0)
        {
            IstKomplex = false;
            x2 = -p/2 - sqrt(delta);
            x1 = q/x2;
            Ergebnis(2,IstKomplex,x1,x2);

        }
        else // delta < 0
        {
          // To do using double to display complex value
          IstKomplex = true;
          double real = -p/2;
          double img = sqrt(-delta);
          Ergebnis(2,IstKomplex,real,img);
        }
      }
      else // p/2 >= threshold
      {
        double delta = 1/4 - (q/p)/p; // cant use pow(p,2) overflow
        if(abs(delta) < EPSILON) // zwei identische NS
        {
            IstKomplex = false;
            x1 = -p/2;
            x2 = -p/2;
            Ergebnis(2,IstKomplex,x1,x2);
        }
        else if(delta > 0)
        {
            IstKomplex = false;
            x2 = -p/2 - abs(p)* sqrt(delta);
            x1 = q/x2;
            Ergebnis(2,IstKomplex,x1,x2);
        }
        else // delta < 0
        {
          // To do using double to display complex value
          IstKomplex = true;
          double real = -p/2;
          double img = abs(p) * sqrt(-delta);
          Ergebnis(2,IstKomplex,real,img);
        }
      }
      

    }
    else // p < 0
    {
      double threshold = sqrt(DBL_MAX - abs(q));
      if(abs(p)/2 < threshold)
      {
        double delta = pow(p/2,2) - q;
        if(abs(delta) < EPSILON) // zwei identische NS
        {
            IstKomplex = false;
            x1 = -p/2;
            x2 = -p/2;
            Ergebnis(2,IstKomplex,x1,x2);
        }
        else if(delta > 0)
        {
            IstKomplex = false;
            x1 = -p/2 + sqrt(delta);
            x2 = q/x1;
            Ergebnis(2,IstKomplex,x1,x2);
        }
        else
        {
          // To do using double to display complex value
          IstKomplex = true;
          double real = -p/2;
          double img = sqrt(-delta);
          Ergebnis(2,IstKomplex,real,img);
        }
      }
      else // abs(p)/2 >= threshold 
      {
        double delta = 1/4 - (q/p)/p; // cant use pow(p,2) overflow
        if(abs(delta) < EPSILON) // zwei identische NS
        {
            IstKomplex = false;
            x1 = -p/2;
            x2 = -p/2;
            Ergebnis(2,IstKomplex,x1,x2);
        } 
        else if(delta > 0)
        {
            IstKomplex = false;
            x1 = -p/2 + abs(p)* sqrt(delta);
            x2 = q/x1;
            Ergebnis(2,IstKomplex,x1,x2);
        }
        else // delta < 0
        {
          // To do using double to display complex value
          IstKomplex = true;
          double real = -p/2;
          double img = abs(p) * sqrt(-delta);
          Ergebnis(2,IstKomplex,real,img);
        }
      }
    }
  }
  else // a = 0
  {
    if( abs(b ) > EPSILON)
    {
      Anzahl = 1;
      x1 = -c/b;
      Ergebnis(1,false,x1);
    }
    else if(abs(c) > EPSILON)
    {
      Anzahl = 0;
      Ergebnis(0);
    }
    else
    {
      Anzahl = Unendlich;
      Ergebnis(Unendlich);
    }
    
  }
  
  return 0;
}
