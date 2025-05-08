#include "memtrace.h"
#include "ember.h"

//konstruktor
Ember::Ember(unsigned int i = 0, const String& nv = "", const String& becen = "", const String& cm = "", unsigned int munk = 0, unsigned int priv = 0) {
    id = i;
    nev = nv;
    becenev = becen;
    cim = cm;
    munkahelyi = munk;
    privat = priv;
}


//Másoló konstruktor
Ember::Ember(const Ember& e) {
    id = e.getId();
    munkahelyi = e.getMunkahelyi();
    privat = e.getPrivat();
    nev = e.getNev();
    becenev = e.getBecenev();
    cim = e.getCim();
}

//Egyenlőséget vizsgáló operátor
bool Ember::operator==(const Ember& rhs) const {
    return (
        id == rhs.id &&
        nev == rhs.nev &&
        becenev == rhs.becenev &&
        cim == rhs.cim &&
        munkahelyi == rhs.munkahelyi &&
        privat == rhs.privat
    );
}

void Ember::operator=(const Ember& rhs) {
    id = rhs.id;
    munkahelyi = rhs.munkahelyi;
    privat = rhs.privat;
    nev = rhs.nev;
    becenev = rhs.becenev;
    cim = rhs.cim;
}
