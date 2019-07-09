/********************************************************************
*  Name       : mapraview.cpp                                       *
*  Verwendung : Benutzerschnittstelle zu einer Bildklasse (MAPRA),  *
*               Bildbearbeitung: PGM-Format,                        *
*               Punktoperationen und lokale Filter                  *
*  Autor      : Sven Gross, Volker Reichelt, IGPM RWTH Aachen       *
*  Datum      : Jun '00                                             *
********************************************************************/


#include <fstream>
#include "greyscale.h"


using namespace std;

// globale Konstanten, die die Darstellung des Bildes kontrollieren

int  ZoomFac=1;  // Vergroesserungsfaktor
bool Show=true;  // Bild anzeigen oder nicht

#define SET_FORMAT 1
#define DEBUG 1
// Die Funktion "Display" dient als Verbindung zwischen der "Greyscale"-Klasse
// und der Funktion "ShowImage" aus "unit7". Hier wird der Datentyp "float"
// in den Datentyp "byte" (inclusive Clamping) umgewandelt. Je nach dem Wert
// von "ZoomFac" werden aus einem "float"-Pixel ein oder mehrere "byte"-Pixel.
template<class T>
void MyShow(T a){cout.width(4); cout << (short)a;}
void Display ( const GreyScale &Pic, int Id, const char Text[] )
{
  if (!Show) return;                           // Bild nicht anzeigen

  std::vector<byte>  Pixel(Pic.GetWidth()*Pic.GetHeight()*ZoomFac*ZoomFac);

  for ( int j=0; j<Pic.GetHeight(); j++ )      // Fuer alle Bildpunkte ...
    for ( int i=0; i<Pic.GetWidth(); i++ )
    {
      // cout << "\nThe i: " << i << " the j : " << j << "\tPic(i,j)= " << Pic(i,j) << endl;
      float gr=std::rint(Pic(i,j)*255);        // [0,1] auf [0,255] skalieren
      // cout << "The gr is " << gr << "\t the byte(gr) " << (short)byte(gr) << endl;
      if (gr>255) gr=255; else if (gr<0) gr=0; // clampen
      if (ZoomFac==1)                          // behandle den Fall ZoomFac==1
        Pixel[i+j*Pic.GetWidth()]=byte(gr);    // der Effizienz wegen einzeln
      else
        for ( int k=0; k<ZoomFac; k++ )
          for ( int l=0; l<ZoomFac; l++ )
            Pixel[(i*ZoomFac+k)+(j*ZoomFac+l)*Pic.GetWidth()*ZoomFac]=byte(gr);
    }
  // cout << "The Pixel array: \n";



  //   int sBild = Pic.GetWidth() * Pic.GetHeight() * ZoomFac*ZoomFac;
  //   int count = 0;
  //   // cout << "*************************************\n";
  //   // cout << "First the integer version (rounded to 255)\n";
  //   int nextline = (Pic.GetWidth() < 10) ? Pic.GetWidth() : 10;
  //   for(int index = 0; index < sBild ; index++)
  //   {
  //       // colorstep is assumed to be 255
  //       cout << "\nThe index : " << index << endl;
  //       short d = 255;
  //       cout.width(4);
  //       float res = Pixel[index];
  //       cout << (short) res << '\t';
  //       if(count % nextline == nextline - 1)
  //       {
  //           cout << endl;
  //       }
  //       count++;
  //       cout << "The count is " << count << endl;
  //       // assume the Zoom Factor is 1
  //       cout << "The i, j : " << Pic.VecCoord2XY(index).first << ',' << Pic.VecCoord2XY(index).second << endl;
  //   }
  // cout << "All values diaplayed\n";
  // cout << endl;
  ShowImage(Pic.GetWidth()*ZoomFac, Pic.GetHeight()*ZoomFac,
            Pixel.data(), Id, Text);
}


// Das Hauptprogramm besteht im wesentlichen aus einer Schleife, die auf
// die Benutzereingaben wartet und die Kommandos (in der switch-Anweisung)
// interpretiert (z.B. die Filter aus der Greyscale-Klasse aufruft oder die
// globalen Variablen "Show" und "ZoomFac" setzt und das Ergebnis anzeigt).

int main()
{
  GreyScale pic, newpic;
  char      name[200], c='h';

#ifndef DEBUG

#ifdef SET_FORMAT
  int       frmt;
#endif

  do                // Main loop
  {
    switch (c)
    {
      case 'h':     // Help
#ifdef SET_FORMAT
        cout << "(h)elp (r)ead (w)rite (f)ormat (u)ndo (q)uit (d)isplay (1) (2) (3) (4)\n"
#else
        cout << "(h)elp (r)ead (w)rite (u)ndo (q)uit (d)isplay on/off (1) (2) (3) (4)\n"
#endif
             << "(+) (-) (<) (>) (b)inarize (B)lur (c)lamp (C)ontrast (i)nvert\n"
             << "(k)irsch (K)irsch+ (l)aplace (L)aplace+ (m)edian (s)obel (S)obel+" << endl;
        break;

      case 'r':     // Read
        cout << "Read image... Enter filename: ";
        cin >> name;
        {
          ifstream file(name);
          if (file.rdstate())
            cerr << "Error: Couldn't open '" << name << "'!\n";
          else
          {
            file >> newpic;
            Display(newpic,0,name);
            pic.Resize(0,0);
            Display(pic,1,"");
          }
        }
        break;

      case 'w':     // Write
        cout << "Write image... Enter filename: ";
        cin >> name;
        {
          ofstream file(name);
          if (file.rdstate())
            cerr << "Error: Couldn't open '" << name << "'!\n";
          else
            file << newpic;
        }
        break;

#ifdef SET_FORMAT
      case 'f':     // Output-Format
        cout << "Output format... (0) PGM-Ascii (1) PGM-Raw (2) Huffman (3) Huffman-Dist\n"
             << "Enter format: ";
        cin >> frmt;
        GreyScale::SetFormat(frmt);
        break;
#endif

      case 'u':     // Undo
        cout << "Undo...\n";
        newpic=pic;
        Display(newpic,1,"Undo");
        break;

      case 'd':     // Display on/off
        Show=!Show;
        cout << "Display " << (Show?"on":"off") << " ...\n";
        Display(newpic,1,"Display");
        break;

      case '1':     // Zoom
      case '2':
      case '3':
      case '4':
        ZoomFac=c-48;
        cout << "Set zoom factor " << ZoomFac << "...\n";
        Display(newpic,1,"Zoom");
        break;

      case '+':     // Lighter
        cout << "Lighter...\n";
        pic=newpic;
        newpic=pic.LinTrans(1,0.1);
        Display(newpic,1,"Lighter");
        break;

      case '-':     // Darker
        cout << "Darker...\n";
        pic=newpic;
        newpic=pic.LinTrans(1,-0.1);
        Display(newpic,1,"Darker");
        break;

      case '<':     // Increase Contrast
        cout << "Increase Contrast...\n";
        pic=newpic;
        newpic=pic.LinTrans(1.1,-0.05);
        Display(newpic,1,"Increase Contrast");
        break;

      case '>':     // Decrease Contrast
        cout << "Decrease Contrast...\n";
        pic=newpic;
        newpic=pic.LinTrans(0.9,0.05);
        Display(newpic,1,"Decrease Contrast");
        break;

      case 'b':     // Binarize
        cout << "Binarize...\n";
        pic=newpic;
        newpic=pic.Binarize(0.5);
        Display(newpic,1,"Binarize");
        break;

      case 'B':     // Blur
        cout << "Blur...\n";
        pic=newpic;
        newpic=pic.Blur();
        Display(newpic,1,"Blur");
        break;

      case 'c':     // Clamp
        cout << "Clamp...\n";
        pic=newpic;
        newpic=pic.Clamp();
        Display(newpic,1,"Clamp");
        break;

      case 'C':     // Contrast
        cout << "Maximize Contrast...\n";
        pic=newpic;
        newpic=pic.Contrast();
        Display(newpic,1,"Maximize Contrast");
        break;

      case 'i':     // Invert
        cout << "Invert...\n";
        pic=newpic;
        newpic=pic.LinTrans(-1,1);
        Display(newpic,1,"Invert");
        break;

      case 'k':     // Kirsch
        cout << "Kirsch filter...\n";
        pic=newpic;
        newpic=pic.Kirsch();
        Display(newpic,1,"Kirsch");
        break;

      case 'K':     // Kirsch+
        cout << "Add Kirsch filter...\n";
        pic=newpic;
        newpic=pic.Kirsch().LinTrans(0.1,0);
        newpic+=pic;
        Display(newpic,1,"Add Kirsch");
        break;

      case 'l':     // Laplace
        cout << "Laplace filter...\n";
        pic=newpic;
        newpic=pic.Laplace();
        Display(newpic,1,"Laplace");
        break;

      case 'L':     // Laplace+
        cout << "Add Laplace filter...\n";
        pic=newpic;
        newpic=pic.Laplace().LinTrans(0.1,0);
        newpic+=pic;
        Display(newpic,1,"Add Laplace");
        break;

      case 'm':     // Median
        cout << "Median filter...\n";
        pic=newpic;
        newpic=pic.Median();
        Display(newpic,1,"Median");
        break;

      case 's':     // Sobel
        cout << "Sobel filter...\n";
        pic=newpic;
        newpic=pic.Sobel();
        Display(newpic,1,"Sobel");
        break;

      case 'S':     // Sobel+
        cout << "Add Sobel filter...\n";
        pic=newpic;
        newpic=pic.Sobel().LinTrans(0.1,0);
        newpic+=pic;
        Display(newpic,1,"Add Sobel");
        break;

      default:      // Error
        cout << "Warning: Ignored unknown command '" << c << "'\n";
        break;
    }

    cout << "Enter Command (h for help): ";
    cin  >> c;

  } while (c!='q'); // 

#endif

  // test function for io
  // ifstream inFile;
  // inFile.open("bilder/dom.pgm");
  // if(!inFile.is_open())
  // {
  //   cout << "cant open the file\n";
  //   exit(1);
  // }
  // inFile >> pic;
  // cout << "The file read in, the pixel picture is\n";
  // cout << pic << endl;
  // newpic=pic.Median();
  // ofstream outFile("result3.pgm");
  // outFile << newpic;
  // Display(pic,0,"TexT");
  // std::vector<byte>  Pixel(1);
  // cout << "\n\n 0 vector created\n";
  //           Display(newpic,1,"");

  


  // test function for huffman coding
  ifstream inFile;
  inFile.open("bilder/bauteil.pgm");
  if(!inFile.is_open())
  {
    cout << "Can't open the file\n";
    exit(1);
  }
  inFile >> pic;
  pic.HuffmanCoding();
  cout << "Testing the string lenth\n";

  cout << "pic.mpColCd[117]: " << pic.mpColCd[117] << "  has length " << pic.mpColCd[117].size() << endl;

  cout << "Test the substring \n";
  codes theC = pic.mpColCd[133];
  int pos = 3;

  cout << "The code is " << theC ;
  theC = theC.substr((size_t)pos,theC.size());
  cout << "  begis at 3 is " << theC << endl;

  cout << "Testing the for loop, whether continue will miss the ++ \n";
  for(int i = 0; i < 10; i++)
  {
    cout << i << endl;
    if(i == 3)
    {
      cout << "In the if branch i will be 7\n";
      i = 7;
      continue;
    }
  }
  return 0;
}
