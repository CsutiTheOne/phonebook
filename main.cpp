#include <iostream>

#include "memtrace.h"
#include "string.h"
#include "ember.h"
#include "lanclista.h"
#include "telefonkonyv.h"
#include "gtest_lite.h"

using namespace std;
int main(int argc, char const *argv[]) {

    //FUNKCIONALIS RESZ

    //Technikailag ez maga a telefonkönyv program,
    //olyan formában, ahogy azt egy felhasználo kezelné
    //jporta-n ezt vezerli a minta bemenet
    Telefonkonyv minden = Telefonkonyv();
    minden.betolt();
    do {
        minden.menuKiir();
    } while(minden.ask());
    minden.mentes();
    std::cout << "Viszlat!" << std::endl;


    //SIMA TESZT RESZ

    GTINIT(std::cin);       // Csak C(J)PORTA működéséhez kell

    //String tesztjei
    //Bar a stringet egy korabbi laborbol nyultam
    TEST(Test0, string_teszt) {
        //ures
        EXPECT_EQ((size_t)0, String().size()) << "Ures stringnek van hossz" << endl;

        //karakter
        String fromc = String('b');
        EXPECT_EQ('b', fromc[0]) << "Karaktebol string hibas" << endl;

        //osszefuzes
        fromc = fromc + "r";
        EXPECT_TRUE(fromc == String("br")) << "osszefuzes hiba" << endl;

        //cstring es indexeles
        String bruh;
        EXPECT_NO_THROW(bruh = String("Bruh")) << "masolas, es cstring hiba" << endl;
        EXPECT_EQ('B', bruh[0]) << "indexeles hiba" << endl;


        //String uns. intbol intbol
        String szam = String((unsigned int)500);
        EXPECT_EQ((size_t)3, szam.size()) << "Hibas szamkezeles" << endl;
        EXPECT_EQ('5', szam[0]) << "Hibas szamkezeles" << endl;


    } ENDM

    //Ember osztály tesztjei
    TEST(Test1, ember_getter) {
        //Ember anonim = Ember(1); //parameter nelkul hivhato
        Ember peti = Ember(1, "Peter", "Petike", "Valami cim", 123456789, 987654321);

        //getterek
        //EXPECT_EQ(0, anonim.getId()) << "Nem 0 id" << endl;
        EXPECT_EQ((unsigned int)1, peti.getId()) << "Nem jo id" << endl;
        EXPECT_EQ((String)"Peter", peti.getNev()) << "Nevet nem adja vissza" << endl;
        EXPECT_EQ((String)"Petike", peti.getBecenev()) << "Becnevet nem adja vissza" << endl;
        EXPECT_EQ((String)"Valami cim", peti.getCim()) << "Cimet nem adja vissza" << endl;
        EXPECT_EQ((unsigned int)123456789, peti.getMunkahelyi()) << "Munkahelyit nem adja vissza" << endl;
        EXPECT_EQ((unsigned int)987654321, peti.getPrivat()) << "Privatot nem adja vissza" << endl;
        //meg nezzuk meg az operatorokat is
        Ember peti2 = peti;
        bool embik = peti == peti2;
        EXPECT_TRUE(embik) << "Egyenloseg vizsgalat" << endl;


    } ENDM

    TEST(Test2, ember_setter) {
        Ember bela = Ember(2, "Bela", "Bela vayok", "Gyar", 56789, 98765);
        Ember senki;

        EXPECT_EQ((unsigned int)0, senki.getId()) << "Param nelkul hivhato hibas" << endl;


        bela.setNev("Bela Vagyok");
        EXPECT_EQ((String)"Bela Vagyok", bela.getNev()) << "Nevet nem seteli" << endl;
        bela.setBecenev("Belaman");
        EXPECT_EQ((String)"Belaman", bela.getBecenev()) << "Bnevet nem seteli" << endl;
        bela.setCim("Gombgyar");
        EXPECT_EQ((String)"Gombgyar", bela.getCim()) << "Cimet nem seteli" << endl;
        bela.setMunkahelyi(98765);
        EXPECT_EQ((unsigned int)98765, bela.getMunkahelyi())  << "Munkahelyit nem seteli" << endl;
        bela.setPrivat(56789);
        EXPECT_EQ((unsigned int)56789, bela.getPrivat()) << "Privatot nem seteli" << endl;
    } ENDM

    //Láncolt lista tesztjei
    TEST(Test3, lancoltlista_intel) {
        LancLista<int> list;
        list.add(10);
        list.add(20);
        list.add(30);
        list.add(40);
        list.add(50);
        list.add(60);
        list.add(70);
        list.add(80);

        //elso elem viszaadasa
        EXPECT_EQ((int)10, list.getElso()->adat) << "nemjo az elso elem" << endl;

        //hossz, ertek visszakeres
        EXPECT_EQ((size_t)8, list.length()) << "Lista hossza nem stimmel" << endl;
        EXPECT_EQ((int)10, list.getData(0)) << "Get es index nem ok" << endl;
        EXPECT_EQ((int)20, list.get(1)->adat) << "Get es index nem ok" << endl;
        EXPECT_EQ((int)30, list[2]) << "Get es index nem ok" << endl;

        //modositas, beszuras
        list[2] = 69;
        EXPECT_EQ((int)69, list[2]) << "Nice" << endl;
        list.pushFront(5);
        EXPECT_EQ((int)5, list[0]) << "Elejere rakas" << endl;
        list.insertAt(2, 15);
        EXPECT_EQ((int)15, list[2]) << "beszuras" << endl;

        //torles indexnel
        list.removeAt(2);
        EXPECT_EQ((int)20, list[2]) << "torles" << endl;

        //elso hatso kivetele
        list.pop();
        EXPECT_EQ((int)70, list.getUtolso()->adat) << "POP rossz" << endl;
        list.popFront();
        EXPECT_EQ((int)10, list[0]) << "POPFRONT rossz" << endl;

        //LISTA RENDEZES
        list.sort(nagyobbMint<int>(), false); //csokkenobe kene rakja
        EXPECT_EQ((int)70, list[0]) << "Rendezes nem jo" << endl;
        list.sort(nagyobbMint<int>(), true); //novekvobe kene rakja
        EXPECT_EQ((int)10, list[0]) << "Rendezes nem jo" << endl;

        //csere (bar ha ez rossz, akkor a rendezes is)
        list.swap(list.getElso(), list.getUtolso());
        EXPECT_EQ((int)70, list[0]) << "Rendezes nem jo" << endl;

    } ENDM

    TEST(Test4, predikatumok) {
        //predikatumok amikkel al ista rendezheto
        //ezek java igazabol embereket hasonlit ossze
        //Itt minden esetben az elso ember lesz elorebb:
        Ember egyik = Ember(1, "A Egyik Embi", "A eki", "A Kozeli cim", 1, 1);
        Ember masik = Ember(2, "B Masik Embi", "B moki", "B Taboli cim", 2, 2);

        EXPECT_FALSE(munkahelyiSzerint<Ember>()(egyik, masik));
        EXPECT_FALSE(privatSzerint<Ember>()(egyik, masik));
        EXPECT_FALSE(becenevSzerint<Ember>()(egyik, masik));
        EXPECT_FALSE(cimSzerint<Ember>()(egyik, masik));

    } ENDM

    //maga telefonkonyv tesztjei
    TEST(Test5, telefonkonyv_mentes-betoltes) {
        //felesleges a parameter, de igy a parameteres konstruktor hivodik
        Telefonkonyv tk = Telefonkonyv(String("szemlyek.txt"), szovgk, &std::cout, &std::cin); //itt direkt hibas a fajlnev
        tk = Telefonkonyv(String("szemelyek.txt"), szovgk, &std::cout, &std::cin);


        EXPECT_EQ((size_t)0, tk.getMenuState().length()) << "Menu alapbol kap valamit!" << std::endl;
        EXPECT_EQ((size_t)0, tk.getTkonyv().length()) << "Adatok nem urasak alapbol!" << std::endl;

        //betoltes ellenorzese
        tk.betolt();
        EXPECT_FALSE((size_t)0 == tk.getTkonyv().length()) << "Beolvasas utan ures!" << std::endl;
        tk.getTkonyv().sort(idSzerint<Ember>(), true); //nev szerint novekvobe teszi
        if(tk.getTkonyv().getElso() != NULL)
            EXPECT_EQ(String("Becenev"), tk.getTkonyv().getElso()->adat.getBecenev()) << "Elso ember elter" << endl;

        //hozzaadas ellenorzese
        tk.hozzaad(Ember(tk.getNextId(), "Utolso Tesztember", "Becenevke", "Cimszoveg", 0, 0));
        EXPECT_EQ(String("Becenevke"), tk.getTkonyv().getUtolso()->adat.getBecenev()) << "Hozzaadott ember elter" << endl;

    } ENDM

    return 0;
}
