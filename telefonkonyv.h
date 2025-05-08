#ifndef TELEFONKONYV_H
#define TELEFONKONYV_H

#include <iostream>
#include "string.h"
#include "ember.h"
#include "lanclista.h"

/*
MENÜ NAGYJÁBÓL:
->___Főmenü___
0. KILEPES
1. LISTÁZÁS
    ->Milyen sorrendben?
    0. vissza
    1. Sorszam
    2. Nev
    3. Cim
    4. Munkahelyi szam
    5. Privat szám
2. UJ
3. KERESES
    ->Mi alapján keresel?
    0. vissza
    1. Sorszam
    2. Nev
    3. Cim
    4. Munkahelyi szam
    5. Privat szám
        ->Keresesi kulcs?
4. MODOSITAS
    ->Adja meg a modosítando elem sorszamat
        ->___Modosítas___
        ->(amit üresen hagy, nem modosul)
5. TORLES
6. MENTES
7. BETOLTES
*/
enum MENUK{FOMENU = 0, ALMENU, NOVCSOK, EREDMENY};
enum FOMENU{KILEPES = 0, LISTAZAS, UJ, KERESES, MODOSITAS, TORLES, MENTES, BETOLTES };
enum SORREND{VISSZA = 0, SORSZAM, NEV, BECENEV, CIM, MUNKAHELYI, PRIVAT};
enum NOVEKVO{NOVEKVO = 1, CSOKKENO};


//const String FomenuSz = "0. Kilepes \n1. Listazas \n2. Uj \n3. Keresses \n4. Modositas \n5. Torles \n6. Mentes \n7. Betoltes \n"; //FOMENU SZINT
//const String AlmenuSz = "0. Vissza \n1. Sorszam \n2. Nev \n3. Cim \n 4. Munkahelyi szam \n5. Privat szam \n"; //Almenu szint
const String szovgk[] = {
    "0. Kilepes \n1. Listazas \n2. Uj \n3. Keresses \n4. Modositas \n5. Torles \n6. Mentes \n7. Betoltes \n",
    "0. Vissza \n1. Sorszam \n2. Nev \n3. Becenev \n4. Cim \n5. Munkahelyi szam \n6. Privat szam \n",
    "0. Vissza \n1. Novekvo \n2. Csokkeno \n"
};


class Telefonkonyv {
    LancLista<Ember> tkonyv;
    LancLista<int> menuState;
    const String *menuSzovegek;
    String fajlnev;
    unsigned int nextId;

    std::ostream *os;
    std::istream *is;
public:
    //parameter nelkuli konstruktor
    Telefonkonyv() : menuSzovegek(szovgk), fajlnev("szemelyek.txt"),  nextId(1), os(&std::cout), is(&std::cin) {}

    //konstruktor
    //bár ez is kap alapértelmezett értékeket
    Telefonkonyv(const String& fn, const String *msz, std::ostream *o, std::istream *i);

    //a megjegyzett kimenetre irja azt a menüt, amelyikbe épp beléptünk
    void menuKiir();

    //a kilistázott menupontok alapján bekérünk egy számot
    //a tárolt bemenetről, így tud választani a felhaszánló
    //visszatérési értéke:
    //hamis amikor kilépünk a telefonkönyvből
    //mindn más esetben igaz
    bool ask();

    //kilistázzuk a tárolt személyeket
    //elotte erdemes rendezni a listát
    void listaz(const LancLista<Ember>&) const;

    //az osztálynyak megadott bementről bekérünk
    //személyadatokat amiket a telefonkönyvünkben eltárolunk
    //a parameter azt adja meg, hogy modositashoz futtatjuk-e
    //modositaskor nem kotelezo megadni a nevet
    Ember beker(bool modhoz) const;
    //egy embert hozzáad a telefonkonyvhoz
    void hozzaad(const Ember& e);
    //ID alapjan megtalalt szemelyt modositja a
    //parameterkent kapott Embernek megfeleloen
    //csak azt irja felul, ami kulonbozik
    void modosit();

    //beker egy keresei kulcsot, majd annak megfeleloen meghivja a listazast
    void kereses();

    //szemely ID-nek bekerese utan az ad id-jut toroljuk
    void torol();

    //az osztályban tárolt fájlnévre mentjük a tárolt adatainkat
    void mentes();

    //a tárolt fájlnevű fájlból betöltjük az adatokat, és a nekünk megfelelő módon eltároljuk
    //itt hibakezelés fontos lesz!
    void betolt();

    //ezek csak a tesztelesnel relevans fuggvenyek
    //hogy ne csak kiiraskor ferjunk hozza a telefonkonyv adataihoz
    inline unsigned int& getNextId() { return nextId; }
    inline LancLista<Ember>& getTkonyv() { return tkonyv; }
    inline LancLista<int>& getMenuState() { return menuState; }
};





#endif
