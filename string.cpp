/**
 *  Órai STRING osztály
 *  És akkor így nem használok STL tárolót
 *
 * \file string.cpp
 *
 */

#ifdef _MSC_VER
// MSC ne adjon figyelmeztető üzenetet a stringkezelő függvényekre.
  #define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>             // Kiíratáshoz
#include <cstring>              // Stringműveletekhez

#include "memtrace.h"           // a standard headerek után kell lennie
#include "string.h"

using std::cin;
using std::ios_base;

/// Konstruktor: egy char karakterből (createStrFromChar)
String::String(char ch) {
   // Meghatározzuk a hosszát
    len = 1;
    // Lefoglalunk a helyet a hossznak + a lezaró nullának
    pData = new char[len+1];
    // Betesszük a karaktert
    pData[0] = ch;
    pData[1] = '\0';
}

/// Konstruktor egy char szambol
/// @param ch - szam
String::String(unsigned int ch) {
    len = 1;
    int dec = 10;
    while((ch / dec) != 0) {
        len++;
        dec *= 10;
    }
    dec /= 10;
    size_t i = 0;
    if(ch < 0) {
        ch *= -1;
        len++;
        pData = new char[len+1];
        pData[i++] = '-';
    } else pData = new char[len+1];
    for(; i < len; i++) {
        pData[i] = '0' + (ch / dec);
        ch %= dec;
        dec /= 10;
    }
    pData[i] = '\0';
}


// Konstruktor: egy nullával lezárt char sorozatból (createStringFromCharStr)
String::String(const char *p) {
    // Meghatározzuk a hosszát
    len = strlen(p);
    // Helyet foglalunk
    pData = new char[len+1];
    // Bemásoljuk a stringet, ami le van zárva 0-val így használható az strcpy is
    strcpy(pData, p);
}

// Másoló konstruktor
String::String(const String& s1) {
    // Meghatározzuk a hosszát
    len = s1.len;
    // Helyet foglalunk
    pData = new char[len+1];
    // Bemásoljuk a stringet, ami le van zárva 0-val így használható az strcpy is
    strcpy(pData, s1.pData);
}

// operator=
String& String::operator=(const String& rhs_s) {
    if (this != &rhs_s) {
        delete[] pData;
        len = rhs_s.len;
        // Helyet foglalunk
        pData = new char[len+1];
        // Bemásoljuk a stringet, ami le van zárva 0-val így használható az strcpy is
        strcpy(pData, rhs_s.pData);
    }
    return *this;
}

// [] operátorok: egy megadott indexű elem REFERENCIÁJÁVAL térnek vissza.
// indexhiba esetén dobjon egy const char * típusú hibát!
char& String::operator[](unsigned int idx) {
    if (idx >= len) throw "String: indexelesi hiba";
    return pData[idx];
}

const char& String::operator[](unsigned int idx) const {
    if (idx >= len) throw "String: indexelesi hiba";
    return pData[idx];
}

// + operátor, ami két stringet ad össze (concatString)
String String::operator+(const String& rhs_s) const {
    String temp;		// ide kerül az eredmény
    // Meghatározza az új string hosszát
    temp.len = len + rhs_s.len;
    // Felszabadítja a temp adattaerületét
    delete []temp.pData;
    // lefoglalja a memóriát az új stringnek.
    temp.pData = new char[temp.len+1];
    // Az elejére bemásolja az első stringet
    strcpy(temp.pData, pData);
    // Bemásolja a második stringet.
    strcat(temp.pData, rhs_s.pData);

    return temp;		// visszatér az eredménnyel

}
// << operator, ami kiír az ostream-re
std::ostream& operator<<(std::ostream& os, const String& s0) {
    os << s0.c_str();
    return os;
}

// << operátor, ami beolvas az istreamről egy szót
std::istream& operator>>(std::istream& is, String& s0) {
    unsigned char ch;
    s0 = String("");            // üres string, ehhez fűzünk hozzá
	std::ios_base::fmtflags fl = is.flags(); // eltesszük a régi flag-eket
	is.setf(ios_base::skipws);			// az elején eldobjuk a ws-t
    while (is >> ch) {
	    is.unsetf(ios_base::skipws);	// utána pedig már nem
        if (isspace(ch)) {
            is.putback(ch);             // na ezt nem kérjük
            break;
        } else {
            s0 = s0 + ch;               // végére fűzzük a karaktert
        }
    }
	is.setf(fl);						// visszaállítjuk a flag-eket
    return is;
}


//Összehasonlítás
bool String::operator==(const String& rhs) const {
    if(len != rhs.size()) return false;
    size_t i = 0;
    while(i < len && pData[i] == rhs[i]) ++i;
    return i == len;
}
