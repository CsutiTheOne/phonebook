#ifndef LANCLISTA_H
#define LANCLISTA_H

#include <cstddef>
#include "string.h"

//predikatum osztaly (struktura)
//kesobb, rendezeskor ennek a szakositasaval
//lehet kulonfele sorrendekbe rendezni a listat
template <typename T>
struct predikatum {
    virtual bool operator()(T, T) const = 0;
};

//majd kesobb hasznalt predikatumok a sorbarendeeshez
template <typename T>
struct nagyobbMint : predikatum<T> {
    inline bool operator()(T l, T r) const { return l > r; }
};
//id szerint
template <typename T>
struct idSzerint : predikatum<T> {
    inline bool operator()(T l, T r) const { return l.getId() > r.getId(); }
};
//munkahelyi szerint
template <typename T>
struct munkahelyiSzerint : predikatum<T> {
    inline bool operator()(T l, T r) const { return l.getMunkahelyi() > r.getMunkahelyi(); }
};
//privat szerint
template <typename T>
struct privatSzerint : predikatum<T> {
    inline bool operator()(T l, T r) const { return l.getPrivat() > r.getPrivat(); }
};
//nev szerint
template <typename T>
struct nevSzerint : predikatum<T> {
    inline bool operator()(T l, T r) const { return abcbe(l.getNev(), r.getNev(), false); }
};
//becenev szerint
template <typename T>
struct becenevSzerint : predikatum<T> {
    inline bool operator()(T l, T r) const { return abcbe(l.getBecenev(), r.getBecenev(), false); }
};
//cim szeint
template <typename T>
struct cimSzerint : predikatum<T> {
    inline bool operator()(T l, T r) const { return abcbe(l.getCim(), r.getCim(), false); }
};

template<class T>
struct ListaElem {
    T adat;
    ListaElem *elo;
    ListaElem *kov;
};

//Láncolt lista osztály
//Később majd embereket fogunk benne tárolni, de heterogénre csináljuk meg
template<class T>
class LancLista {
    ListaElem<T> *elso, *utolso;
    size_t len;
public:
    LancLista();

    ~LancLista();

    void erease();

    inline size_t length() const { return len; }
    inline ListaElem<T>* getElso() const { return elso; }
    inline ListaElem<T>* getUtolso() const { return utolso; }

    //Hozzáfűz a láncolt lista végéhez
    void add(const T& d);
    void add(ListaElem<T> *LE);

    //A listában meghatározott indexű helyre fűz
    void insertAt(int idx, const T& d);
    void insertAt(int idx, ListaElem<T> *LE);

    //a lista elejére teszünk
    void pushFront(const T& d);
    void pushFront(ListaElem<T> *LE);

    //adott indexnél eltávolitás
    void removeAt(int idx);

    //legutolsó elem törlése
    void pop();
    //legelso elem törlése
    void popFront();

    void swap(ListaElem<T> *A, ListaElem<T> *B);

    //elemek sorbarendezése
    //predikátumot használva
    //asc paraméter hogy predikátum szerint novekvo/csokkeno sorrend jojjon letre
    void sort(const predikatum<T>& pred, bool asc);

    //idx-edik elem visszaadása
    //specifikálva különböző esetekre
    //a get pointert ad vissza a listaelemre
    //a többiek a tárolt értékre referenciát
    //(értelem szerűen a többieket fogjuk jobban szeretni)
    ListaElem<T>* get(int idx);
    T& getData(int idx);

    T& operator[](int idx);
};



//nehany altalanos fuggveny
template <class T>
void rescueListaElem(ListaElem<T> *LE);

bool abcbe(const String& aS, const String& bS, bool eq);


#endif
