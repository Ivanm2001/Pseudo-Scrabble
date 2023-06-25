#include "gtest-1.8.1/gtest.h"
#include "../src/Fachada_Juego.h"

#include "utils/PalabrasComunes.h"

class JuegosTests : public testing::Test{
protected:
    Nat cantJugadores = 3;
    Nat tamanoTab = 10;
    Nat cantFichas = 2;

    map<Letra, Nat> puntajes;
    set<vector<Letra>> palabrasLegitimas;
    Repositorio repo;

    //Agregados nuestros

    map<Letra, Nat> puntajes_nuestro;
    Repositorio repo_nuestro;
    Nat cantFichas_nuestro = 5;

    void SetUp()
    {
        puntajes = {{'a', 1}, {'q' , 0} ,{'x', 20}, {'z', 20}};

        palabrasLegitimas = set<vector<Letra>>(begin(palabrasComunes), end(palabrasComunes));
        repo = {'a', 'a', 'f', 'e', 't', 'g', 'w', 'r', 'e'};

        //Agregados nuestros
        puntajes_nuestro = {{'t', 3}, {'o', 10}, {'u', 2}, {'a', 5}, {'c',11}, {'m',1}};
        repo_nuestro = {'t','d','o','o','s','m','u','c','o','o','m','t','o','o','r','a','v','h','r','u'};
    }
};

TEST_F(JuegosTests, juego_recien_empieza){
    Fachada_Variante v(tamanoTab, cantFichas, puntajes, palabrasLegitimas);

    Fachada_Juego juego(2, v, repo);

    EXPECT_EQ(0, juego.turno());
    EXPECT_EQ(0, juego.puntaje(0));
    EXPECT_EQ(0, juego.puntaje(1));
};

TEST_F(JuegosTests, crear_juego_tiene_tablero_vacio) {
    Fachada_Variante v(tamanoTab, cantFichas, puntajes, palabrasLegitimas);

    Fachada_Juego juego(2, v, repo);

    EXPECT_EQ(juego.variante().tamanoTablero(), tamanoTab);

    for (Nat i = 0; i < tamanoTab; i++){
        for (Nat j = 0; j < tamanoTab; j++){
            EXPECT_FALSE(juego.hayLetra(i, j));
        }
    }
};

TEST_F(JuegosTests, ubicar_vacio_no_da_puntos){
    Fachada_Variante v(tamanoTab, cantFichas, puntajes, palabrasLegitimas);

    Fachada_Juego juego(2, v, repo);

    juego.ubicar({});

    EXPECT_EQ(1, juego.turno());
    EXPECT_EQ(0, juego.puntaje(0));
    EXPECT_EQ(0, juego.puntaje(1));
};


TEST_F(JuegosTests, ubicar_a_da_dos_puntos){
    Fachada_Variante v(tamanoTab, cantFichas, puntajes, palabrasLegitimas);

    Fachada_Juego juego(2, v, repo);

    juego.ubicar({ {0, 0, 'a'}});

    EXPECT_TRUE(juego.hayLetra(0,0));
    EXPECT_EQ(juego.letra(0, 0), 'a');
    EXPECT_EQ(1, juego.turno());
    EXPECT_EQ(2, juego.puntaje(0));
    EXPECT_EQ(0, juego.puntaje(1));
};

TEST_F(JuegosTests, jugada_discontigua_ilegitima){
    Fachada_Variante v(tamanoTab, cantFichas, puntajes, palabrasLegitimas);

    Fachada_Juego juego(2, v, repo);

    EXPECT_FALSE(juego.jugadaValida({{0, 0, 'a'}, {0, 2, 'a'}}));
};

TEST_F(JuegosTests, jugada_letra_sola_valida){
    Fachada_Variante v(tamanoTab, cantFichas, puntajes, palabrasLegitimas);

    Fachada_Juego juego(2, v, repo);

    EXPECT_TRUE(juego.jugadaValida({{0, 0, 'a'}}));
};

TEST_F(JuegosTests, fichas_se_reparten_correctamente) {
    Fachada_Variante v(tamanoTab, cantFichas, puntajes, palabrasLegitimas);

    Fachada_Juego juego(2, v, repo);

    EXPECT_EQ(juego.cantFicha(0, 'a'), 2);
    EXPECT_EQ(juego.cantFicha(1, 'f'), 1);
    EXPECT_EQ(juego.cantFicha(1, 'e'), 1);

    juego.ubicar({{0, 0, 'a'}});

    EXPECT_EQ(juego.cantFicha(0, 'a'), 1);
    EXPECT_EQ(juego.cantFicha(0, 't'), 1);
    EXPECT_EQ(juego.cantFicha(1, 'f'), 1);
    EXPECT_EQ(juego.cantFicha(1, 'e'), 1);

};

//Test Agregado
TEST_F(JuegosTests, puntaje_funciona_bien){
    Fachada_Variante v(tamanoTab, cantFichas_nuestro, puntajes_nuestro, palabrasLegitimas);

    Fachada_Juego juego(2, v, repo_nuestro);

    EXPECT_EQ(0, juego.turno());

    juego.ubicar({ {2, 2, 't'}, {2, 3, 'o'}, {2, 4, 'd'}, {2, 5,'o'}});

    EXPECT_TRUE(juego.hayLetra(2,2));
    EXPECT_TRUE(juego.hayLetra(2,3));
    EXPECT_TRUE(juego.hayLetra(2,4));
    EXPECT_TRUE(juego.hayLetra(2,5));

    EXPECT_EQ(juego.letra(2, 2), 't');
    EXPECT_EQ(juego.letra(2, 3), 'o');
    EXPECT_EQ(juego.letra(2, 4), 'd');
    EXPECT_EQ(juego.letra(2, 5), 'o');

    EXPECT_EQ(1, juego.turno());

    EXPECT_EQ(48, juego.puntaje(0));
    EXPECT_EQ(0, juego.puntaje(1));
};


//Test Agregado
TEST_F(JuegosTests, pedir_puntaje_despues_de_ubicar_dos_veces){
    Fachada_Variante v(tamanoTab, cantFichas_nuestro, puntajes_nuestro, palabrasLegitimas);

    Fachada_Juego juego(2, v, repo_nuestro);

    EXPECT_EQ(0, juego.turno());

    juego.ubicar({ {2, 2, 't'}, {2, 3, 'o'}, {2, 4, 'd'}, {2, 5,'o'}});

    EXPECT_TRUE(juego.hayLetra(2,2));
    EXPECT_TRUE(juego.hayLetra(2,3));
    EXPECT_TRUE(juego.hayLetra(2,4));
    EXPECT_TRUE(juego.hayLetra(2,5));

    EXPECT_EQ(juego.letra(2, 2), 't');
    EXPECT_EQ(juego.letra(2, 3), 'o');
    EXPECT_EQ(juego.letra(2, 4), 'd');
    EXPECT_EQ(juego.letra(2, 5), 'o');

    EXPECT_EQ(1, juego.turno());

    juego.ubicar({ {1, 3, 'c'}, {3, 3, 'm'}, {4, 3,'o'}});

    EXPECT_TRUE(juego.hayLetra(1,3));
    EXPECT_TRUE(juego.hayLetra(3,3));
    EXPECT_TRUE(juego.hayLetra(4,3));

    EXPECT_EQ(juego.letra(1, 3), 'c');
    EXPECT_EQ(juego.letra(3, 3), 'm');
    EXPECT_EQ(juego.letra(4, 3), 'o');

    EXPECT_EQ(0, juego.turno());

    juego.ubicar({ {3, 5, 't'}, {4, 5, 'r'}, {5, 5,'o'}});

    EXPECT_TRUE(juego.hayLetra(3,5));
    EXPECT_TRUE(juego.hayLetra(4,5));
    EXPECT_TRUE(juego.hayLetra(5,5));

    EXPECT_EQ(juego.letra(3, 5), 't');
    EXPECT_EQ(juego.letra(4, 5), 'r');
    EXPECT_EQ(juego.letra(5, 5), 'o');

    EXPECT_EQ(86, juego.puntaje(0));
    EXPECT_EQ(54, juego.puntaje(1));
};
