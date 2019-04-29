#include "student.h"
using std::endl;
// Eingabeoperator ">>"
std::istream& operator>> ( std::istream& s, Student& a)
{
    s >> a.Vorname >> a.Nachname >> a.MatNr >> a.Note;
    return s;
}

// Ausgabeoperator "<<"
std::ostream& operator<< ( std::ostream& s, const Student& a)
{
    // hier den Ausgabeoperator definierenn
    s << "Name: ";
    s << a.Vorname << "\t" << a.Nachname << endl;
    s << "Note: "<< a.Note << endl;
    s << "Matr: " << a.MatNr<<endl;
    return s;
}

// Vergleichsoperator "<"
bool operator< ( const Student& s1, const Student& s2)
{
    int result = s1.Nachname.compare(s2.Nachname);
    if(result)
    {
        if(result < 0)
        {
            return true;
        }
        else // result > 0
        {
            return false;
        }
        
    }
    else
    {
        result = s1.Vorname.compare(s2.Vorname);
        if(result < 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
}

// Vergleichsoperatoren "==" bzw. "!="
bool operator== ( const Student& s1, const Student& s2)
{
    if(s1.Nachname.compare(s2.Nachname))
    {
        return false;
    }
    if(s1.Vorname.compare(s2.Vorname))
    {
        return false;
    }
    return true;
}

bool operator<=(const Student& s1, const Student& s2)
{
    return (s1 < s2) || (s1 == s2);
}
bool operator>=(const Student& s1, const Student& s2)
{
    return (s1 > s2) || (s1 == s2);
}
bool operator>(const Student& s1, const Student& s2)
{
    int result = s1.Nachname.compare(s2.Nachname);
    if(result)
    {
        if(result > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        result = s1.Vorname.compare(s2.Vorname);
        if(result > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
        
    }
    
}


bool operator!= ( const Student& s1, const Student& s2)
{
    if(s1.Nachname.compare(s2.Nachname))
    {
        return true;
    }
    if(s1.Vorname.compare(s2.Vorname))
    {
        return true;
    }
    return false;
}


