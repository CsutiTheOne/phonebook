#include <iostream>
#include <fstream>
#include <cstddef>

#include "memtrace.h"
#include "telefonkonyv.h"


//konstruktor
Telefonkonyv::Telefonkonyv(const String& fn, const String *msz, std::ostream *o, std::istream *i) {
    nextId = 1;
    fajlnev = fn;
    menuSzovegek = msz;
    os = o;
    is = i;
}

//a megjegyzett kimenetre irja azt a menüt, amelyikbe épp beléptünk
//egyebkent ez a fuggveny hivja meg a menupontok vegrahajtasaert felelos fuggvenyeket is
//magyarul nem csak kiirjuk a menut, az a fuggveny a UI
void Telefonkonyv::menuKiir() {
    *os << std::endl;
    switch(menuState.length()) {
        case FOMENU:
        *os << "___Fomenu:___" << std::endl;
            *os << menuSzovegek[FOMENU];
            break;
        case ALMENU:
            //itt már válaszottunk valamit a fomenubol
            switch(menuState[FOMENU]) {
                case UJ:
                    //Uj hozzaadasat valasztottuk
                    menuState.pop(); //mivel ehez nem tartozik almenu
                    hozzaad(beker(false));
                    menuKiir();
                    break;
                case MODOSITAS:
                    menuState.pop();
                    *os << "Szemely modositasa" << std::endl;
                    modosit();
                    menuKiir();
                    break;
                case TORLES:
                    menuState.pop();
                    *os << "Szemely torlese" << std::endl;
                    torol();
                    menuKiir();
                    break;
                case MENTES:
                    menuState.pop();
                    mentes();
                    menuKiir();
                    *os << "Fajlba mentes!" << std::endl;
                    break;
                case BETOLTES:
                    menuState.pop();
                    betolt();
                    menuKiir();
                    *os << "Fajlbol betoltes!" << std::endl;
                    break;
                default:
                    *os << ((menuState[FOMENU] == LISTAZAS) ? "Minek a sorrendjeben?" : "Mi alapjan?") << std::endl;
                    *os << menuSzovegek[ALMENU];
                    break;
            }
            break;
        case NOVCSOK:
            *os << "Milyen sorban? " << std::endl;
            *os << menuSzovegek[NOVCSOK];
            break;
        case EREDMENY:
            bool asc = menuState[NOVCSOK] == 1;
            switch(menuState[ALMENU]) {
                case SORSZAM:
                    tkonyv.sort(idSzerint<Ember>(), asc);
                    break;
                case NEV:
                    tkonyv.sort(nevSzerint<Ember>(), asc);
                    break;
                case BECENEV:
                    tkonyv.sort(becenevSzerint<Ember>(), asc);
                    break;
                case CIM:
                    tkonyv.sort(cimSzerint<Ember>(), asc);
                    break;
                case MUNKAHELYI:
                    tkonyv.sort(munkahelyiSzerint<Ember>(), asc);
                    break;
                case PRIVAT:
                    tkonyv.sort(privatSzerint<Ember>(), asc);
                    break;
            }
            if(menuState[FOMENU] == LISTAZAS) {
                listaz(tkonyv);
            }
            if(menuState[FOMENU] == KERESES) {
                kereses();
            }
            menuState.erease();
            menuKiir();
            break;
    }
    *os << std::endl;
}

//a kilistázott menupontok alapján bekérünk egy számot
//a tárolt bemenetről, így tud választani a felhaszánló
//visszatérési értéke:
//hamis amikor kilépünk a telefonkönyvből
//mindn más esetben igaz
bool Telefonkonyv::ask() {
    //bekerunk egy számot
    //fontos, hogy csak olyan szamot, amihez van menupont
    int val;
    do {
        *os << "Valasztott menupont: ";
        *is >> val;
        if(val < KILEPES || val > BETOLTES) {
            menuKiir();
            *os << "Hibas menupont!" << std::endl;
        }
    } while(val < KILEPES || val > BETOLTES);
    //ezen a ponton val értéke biztosan egy menüpont
    //0-ás választásnál
    if(val == 0) {
        if(menuState.length() == 0) {
            //ha a fomenuben vagyunk -> kilepunk
            return false;
        } else {
            //amugy csak vissza lepunk
            menuState.pop();
        }
    }
    //minden más választásnál
    else {
        menuState.add(val);
    }
    return true;
}

//a megjegyzett kimenetre irja azt a menüt, amelyikbe épp beléptünk
void Telefonkonyv::listaz(const LancLista<Ember>& lista) const {
    *os << std::endl;
    *os << "__#___" << std::setfill('_') << std::setw(30) << "Nev__" << std::setw(30) << "Becenev__" << std::setw(30) << "Cim__" << std::setw(15) << "Munkahelyi__" << std::setw(15) << "Privat_" << std::setfill(' ') << std::endl;
    for(ListaElem<Ember> *mzg = lista.getElso(); mzg != NULL; mzg = mzg->kov) {
        *os
            << std::setw(5) << mzg->adat.getId()
            << std::setw(30) << mzg->adat.getNev()
            << std::setw(30) << mzg->adat.getBecenev()
            << std::setw(30) << mzg->adat.getCim()
            << std::setw(15) << mzg->adat.getMunkahelyi()
            << std::setw(15) << mzg->adat.getPrivat()
        << std::endl;
    }
    *os << std::setfill('_') << std::setw(125) << "__" << std::endl;
}

//A tárolt bemenetről bekéri egy új ember adatait
Ember Telefonkonyv::beker(bool modhoz = false) const {
    String nev, becenev, cim;
    unsigned int munk = 0, priv = 0;
    *os << "Kerem adja meg a szemely " << ((modhoz) ? "uj " : "") << "adatait!" << std::endl;
    char c;
    is->get();
    do {
        *os << "Nev: ";
        c = is->get();
        while(c != '\n') {
            nev = nev + c;
            c = is->get();
        }
        if(nev == "") *os << "Nevet kotelezo megadni!" << std::endl;
    } while(nev == "" && !modhoz);
    *os << "Becenev: ";
    c = is->get();
    while(c != '\n') {
        becenev = becenev + c;
        c = is->get();
    }
    *os << "Cim: ";
    c = is->get();
    while(c != '\n') {
        cim = cim + c;
        c = is->get();
    }
    *os << "Munkahelyi szam: ";
    if((c = is->get()) != '\n') {
        is->unget();
        *is >> munk;
        is->get();
    };
    *os << "Privat szam: ";
    if((c = is->get()) != '\n') {
        //is->unget();
        *is >> priv;
    }
    *os << std::endl;

    return Ember(nextId, nev, becenev, cim, munk, priv);
    //tkonyv.add();
    //++nextId;
}
//egy embert hozzáad a telefonkonyvhoz
void Telefonkonyv::hozzaad(const Ember& e) {
    tkonyv.add(e);
    nextId++;
}
//ID alapjan megtalalt szemelyt modositja a
//parameterkent kapott Embernek megfeleloen
//csak azt irja felul, ami kulonbozik
void Telefonkonyv::modosit() {
    unsigned int id = 0;
    *os << "Kerem adja meg a modositando szemely sorszamat!" << std::endl;
    *os << "Sorszam: ";
    *is >> id;
    tkonyv.sort(idSzerint<Ember>(), true);
    bool talalat = false;
    ListaElem<Ember> *i;
    for(i = tkonyv.getElso(); i != NULL; i = i->kov) {
        if(i->adat.getId() == id) {
            talalat = true;
            break;
        }
    }
    if(talalat) {
        *os << std::endl << i->adat.getNev().c_str() << " modositasa!" << std::endl;
        *os << std::endl << "Amit nem kivan modosiatani, csak hagyja uresen!" << std::endl;
        Ember uj = beker(true);
        //a kulonbozo adatok modositasa
        if(!(uj.getNev() == "")) i->adat.setNev(uj.getNev());
        if(!(uj.getBecenev() == "")) i->adat.setBecenev(uj.getBecenev());
        if(!(uj.getCim() == "")) i->adat.setCim(uj.getCim());
        if(uj.getMunkahelyi() != 0) i->adat.setMunkahelyi(uj.getMunkahelyi());
        if(uj.getPrivat() != 0) i->adat.setPrivat(uj.getPrivat());

    } else *os << "Nincs szemely ilyen sorszam-al!" << std::endl;

}

//beker egy keresei kulcsot, majd annak megfeleloen meghivja a listazast
void Telefonkonyv::kereses() {
    String kulcs;
    int mode = menuState[ALMENU];
    *os << "Keresesi kulcs: ";
    *is >> kulcs;
    //actual kereses
    LancLista<Ember> megfelelok;
    for(ListaElem<Ember> *i = tkonyv.getElso(); i != NULL; i = i->kov) {
        String comp;
        switch(mode) {
            case SORSZAM:
                comp = String(i->adat.getId());
                break;
            case NEV:
                comp = String(i->adat.getNev());
                break;
            case BECENEV:
                comp = String(i->adat.getBecenev());
                break;
            case CIM:
                comp = String(i->adat.getCim());
                break;
            case PRIVAT:
                comp = String(i->adat.getPrivat());
                break;
            case MUNKAHELYI:
                comp = String(i->adat.getMunkahelyi());
                break;
        };
        bool res = abcbe(kulcs, comp, true);
        if(res) {
            megfelelok.add(Ember(i->adat));
        }

    }
    listaz(megfelelok);
    //if(megfelelok.length() == 0) *os << "Nincs talalat a megadott keresesi kulcsal!" << std::endl;
}

//szemely ID-nek bekerese utan az ad id-jut toroljuk
void Telefonkonyv::torol() {
    *os << "Torolni kivant szemely id-je:";
    unsigned int id, ind = 0;
    *is >> id;
    tkonyv.sort(idSzerint<Ember>(), true);
    bool talalat = false;
    ListaElem<Ember> *i;
    for(i = tkonyv.getElso(); i != NULL; i = i->kov) {
        if(i->adat.getId() == id) {
            talalat = true;
            break;
        }
        ind++;
    }
    if(talalat) {
        *os << i->adat.getNev().c_str() << " sikeresen torolve!" << std::endl;
        tkonyv.removeAt(ind);
    } else *os << "Nincs szemely ilyen id-vel!" << std::endl;
}

//az osztályban tárolt fájlnévre mentjük a tárolt adatainkat
void Telefonkonyv::mentes() {
    //fajlba id szerint novkevo sorban irunk
    tkonyv.sort(idSzerint<Ember>(), true);

    std::ofstream kimenet(fajlnev.c_str());
    for(ListaElem<Ember>* i = tkonyv.getElso(); i != NULL; i = i->kov) {
        kimenet
            << i->adat.getId() << ';'
            << i->adat.getNev() << ';'
            << i->adat.getBecenev() << ';'
            << i->adat.getCim() << ';'
            << i->adat.getMunkahelyi() << ';'
            << i->adat.getPrivat() << ';'
            << std::endl;
    }
    kimenet.close();

}

//a tárolt fájlnevű fájlból betöltjük az adatokat, és a nekünk megfelelő módon eltároljuk
//itt hibakezelés fontos lesz!
void Telefonkonyv::betolt() {
    //betoltes reseteli a tarolt adatokat
    tkonyv.erease();
    //kell figyelni:
    //-az adatok helyességére: név, cim, munka, privat (plusz adat eldobható)
    //-több id ne forduljon elő
    std::ifstream bemenet(fajlnev.c_str());
    //akkor olvasunk, ha van fájl
    if(bemenet.is_open()) {
        unsigned int id;
        char c;
        try {
            while(bemenet >> id >> c && c == ';') {
                if(id >= nextId) nextId = id + 1;
                String nev, becenev, cim;
                unsigned int munkahelyi, privat, scdb = 0;
                char ic = bemenet.get();
                while(ic != ';') {
                    nev = nev + ic;
                    ic = bemenet.get();
                }
                if(ic == ';') scdb++;
                ic = bemenet.get();
                while(ic != ';') {
                    becenev = becenev + ic;
                    ic = bemenet.get();
                }
                if(ic == ';') scdb++;
                ic = bemenet.get();
                while(ic != ';') {
                    cim = cim + ic;
                    ic = bemenet.get();
                }
                if(ic == ';') scdb++;
                bemenet >> munkahelyi >> c;
                if(c == ';') scdb++;
                bemenet >> privat >> c;
                if(c != ';' || scdb < 4) throw("Hibas sor!");
                tkonyv.add(Ember(id, nev, becenev, cim, munkahelyi, privat));
            }
        } catch(const char*) {
            //hibas a fajl
            tkonyv.erease();
            nextId = 1;
        }

        bemenet.close();
    }
}
