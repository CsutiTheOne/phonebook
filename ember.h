#ifndef EMBER_H
#define EMBER_H

#include "string.h"

//Ember osztály
//Hogy 1 ember érvényes tagja legyen a telefonkönyvnek,
//  minimum névre (és id-re) szükség van
//Ezt az osztályt a Telefonkönyv osztály fogja tartalmazni
//  így az id-t tőle kapja, a többit a felhasználótól
//Az Ember változói privátak, de függvényeken keresztül módosíthatóak,
//  kivéve az id-t, az nem változhat
class Ember {
    unsigned int id, munkahelyi, privat;
    String nev;
    String becenev;
    String cim;
public:
    //Paraméter nélkül is hívható
    //"minimalista" Konstruktor
    //Erre nem kéne hogy szükség legyen
    Ember(unsigned int i = 0, const String& nv = ""): id(i), munkahelyi(0), privat(0), nev(nv), becenev(""), cim("") {}

    //Konstruktor, minden érték megadható a paraméterben
    //1 füst alatt megvan mind
    Ember(
        //unsigned int i = 0, const String& nv = "", const String& cm = "", unsigned int munk = 0, unsigned int priv = 0
        unsigned int i, const String& nv, const String& becen, const String& cm, unsigned int munk, unsigned int priv
    );

    //Másoló konstruktor
    Ember(const Ember&);


    //Getterek a legyegyszerűbben
    inline unsigned int getId() const { return id; }
    inline const String& getNev() const { return nev; }
    inline const String& getBecenev() const { return becenev; }
    inline const String& getCim() const { return cim; }
    inline unsigned int getMunkahelyi() const { return munkahelyi; }
    inline unsigned int getPrivat() const { return privat; }

    // //Setterek
    //inline void setId(unsigned int i) { id = i; } //nem kéne hogy lehessen szerkeszteni
    inline void setNev(const String& n) { nev = n; }
    inline void setBecenev(const String& b) { becenev = b; }
    inline void setCim(const String& c) { cim = c; }
    inline void setPrivat(unsigned int p) { privat = p; }
    inline void setMunkahelyi(unsigned int m) { munkahelyi = m; }

    //Egyenlőséget vizsgáló operátor
    bool operator==(const Ember& rhs) const;

    //Értékadó operátor
    void operator=(const Ember& rhs);


};


#endif
