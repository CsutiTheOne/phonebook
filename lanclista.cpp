#include <iostream>

#include "memtrace.h"
#include "lanclista.h"
#include "ember.h"

//__itt kesobb felhasznalt fuggvenyek__

//kis es nagybetuket ignoralo sorrendmegallapito fuggveny
//igaz ha aS hatrebb van, hamis ha abc rendben vannak
bool abcbe(const String& aS, const String& bS, bool eq) {
    const char *a = aS.c_str(), *b = bS.c_str();
    if((aS.size() == 0 || bS.size() == 0) && aS.size() != bS.size() ) return false;
    size_t i = 0, len = (aS.size() > bS.size()) ? bS.size() : aS.size();
    int sumA = 0, sumB = 0;
    while(sumA == sumB && i < len){
        //kis es nagybetu kozott nem teszunk kolonbseget
        sumA += (a[i] <= 'Z' && a[i] >= 'A') ? a[i] + 32 : a[i];
        sumB += (b[i] <= 'Z' && b[i] >= 'A') ? b[i] + 32 : b[i];
        ++i;
    }
    return eq ? sumA == sumB : sumA >= sumB;
}

//függvény, ami odafigyel azon esetekre
//hogy ha egy listához listaelemet fűzünk, és
//a fűzött listaelem, esetleg korábban is 1 listában volt
//ne tegyük tönkre a korábbi listát
template <class T>
void rescueListaElem(ListaElem<T> *LE) {
    if(LE->kov != NULL) LE->kov->elo = LE->elo;
    if(LE->elo != NULL) LE->elo->kov = LE->kov;
}



//___Lancolt Lista___

//konstruktor
template <class T>
LancLista<T>::LancLista() {
    len = 0;
    elso = NULL;
    utolso = NULL;
}

template <class T>
LancLista<T>::~LancLista() {
    erease();
}

template <class T>
void LancLista<T>::erease() {
    ListaElem<T> *i = elso;
    while(i != NULL) {
        ListaElem<T> *kov = i->kov;
        delete i;
        i = kov;
    }
    elso = NULL;
    utolso = NULL;
    len = 0;
}

//Hozzáfűz a láncolt lista végéhez
template <class T>
void LancLista<T>::add(const T& d) {
    ListaElem<T> *uj = new ListaElem<T>();
    uj->adat = d;
    add(uj);

}
template <class T>
void LancLista<T>::add(ListaElem<T> *LE) {
    rescueListaElem(LE);
    LE->elo = utolso;
    LE->kov = NULL;
    if(!len) {
        elso = LE;
    } else {
        utolso->kov = LE;
    }
    utolso = LE;
    len++;
}

//A listában meghatározott indexű helyre fűz
template <class T>
void LancLista<T>::insertAt(int idx, const T& d) {
    ListaElem<T> *uj = new ListaElem<T>();
    uj->adat = d;
    insertAt(idx, uj);
}
template <class T>
void LancLista<T>::insertAt(int idx, ListaElem<T> *LE) {
    rescueListaElem(LE);
    if(idx == 0) pushFront(LE);
    else if(idx == (int)len-1) add(LE);
    else {
        //keressük me mi van most az adott indexen
        ListaElem<T> *crnt = this->get(idx);
        //tegyük be az ő helyére az újat
        LE->elo = crnt->elo;
        crnt->elo->kov = LE;
        crnt->elo = LE;
        LE->kov = crnt;
        len++;
    }
}

//a lista elejére teszünk
template <class T>
void LancLista<T>::pushFront(const T& d) {
    ListaElem<T> *uj = new ListaElem<T>();
    uj->adat = d;
    pushFront(uj);
}
template <class T>
void LancLista<T>::pushFront(ListaElem<T> *LE) {
    rescueListaElem(LE);
    if(!len) add(LE);
    else {
        LE->elo = NULL;
        LE->kov = elso;
        elso->elo = LE;
        elso = LE;
        len++;
    }
}

//adott indexnél eltávolitás
template <class T>
void LancLista<T>::removeAt(int idx) {
    if(idx == 0) popFront();
    else if(idx == (int)len-1) pop();
    else {
        ListaElem<T> *crnt = get(idx);
        rescueListaElem(crnt);
        len--;
        delete crnt;
    }
}

//legutolsó elem törlése
template <class T>
void LancLista<T>::pop() {
    //removeAt elven is tudunk utolso és elso elemet törölni
    //de ezzel hatekonyabb nagy listak eseten
    if(utolso != NULL){
        ListaElem<T> *tmp = utolso;
        len--;
        if(utolso->elo != NULL) {
            utolso->elo->kov = NULL;
            utolso = utolso->elo;
        } else {
            utolso = NULL;
            elso = NULL;
        }
        delete tmp;
    }
}
//legelso elem törlése
template <class T>
void LancLista<T>::popFront() {
    //removeAt elven is tudunk utolso és elso elemet törölni
    //de ezzel hatekonyabb nagy listak eseten
    if(elso != NULL){
        ListaElem<T> *tmp = elso;
        len--;
        if(elso->kov != NULL) {
            elso->kov->elo = NULL;
            elso = elso->kov;
        } else {
            utolso = NULL;
            elso = NULL;
        }
        delete tmp;
    }
}

//2 listaelem megcserélése
//technikailag akár 2 külön listában is lehet a 2 elem
//de nekünk csak a buborékrendezésnél lesz erre szükségünk
template <class T>
void LancLista<T>::swap(ListaElem<T> *A, ListaElem<T> *B) {
    //igazabol csak az adatot kell megcserelni
    T tmp = A->adat;
    A->adat = B->adat;
    B->adat = tmp;
}

//elemek sorbarendezése
//predikátumot használva
//nyilvan csak egy egyszeru kis buborékrendezéssel
template <class T>
void LancLista<T>::sort(const predikatum<T> &pred, bool asc) {
    if(len <= 1) return;
    ListaElem<T> *i, *j;
    bool swapped;
    for(i = utolso; i->elo != NULL; i = i->elo) {
        swapped = false;
        for(j = elso; j != i; j = j->kov) {
            bool predVal = pred(j->adat, j->kov->adat);
            if((predVal && asc) || (!predVal && !asc)) {
                swap(j, j->kov);
                swapped = true;
            }
        }
        if(!swapped) break;
    }
}

//get függvény
//idx-edik elem címének visszaadása
//pointeres esetekben kényelmesebb lehet a módosítás
template <class T>
ListaElem<T>* LancLista<T>::get(int idx) {
    if((size_t)idx >= len || (size_t)idx < 0) throw std::out_of_range("Tulindexeles");
    ListaElem<T> *mozg = elso;
    for(int i = 0; i < idx; i++) {
        mozg = mozg->kov;
    }
    return mozg;
}
//idx-edik elem adatának címének visszaadása
template <class T>
T& LancLista<T>::getData(int idx) {
    T& ret = get(idx)->adat;
    return ret;
}

template <class T>
T& LancLista<T>::operator[](int idx) {
    return getData(idx);
}


//explicit példányosítás
// template class predikatum<int>;
// template struct predikatum<Ember>;
// template struct predikatum<String>;
template struct ListaElem<int>;
template struct ListaElem<Ember>;
template class LancLista<int>;
template class LancLista<Ember>;
