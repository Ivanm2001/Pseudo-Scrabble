#include <queue>
#include "ConjTrie.h"
#include "Tablero.h"
#include "Tipos.h"
#include "Variante.h"
#include "Letra.h"

#ifndef TP_JUEGODEPALABRAS_JUEGO_H
#define TP_JUEGODEPALABRAS_JUEGO_H


class Juego {
public:

    Juego(Nat k, Variante v, Repositorio r);

    Variante &variante();

    Nat cantJugadores();

    Repositorio &repositorio();

    Repositorio &repositorioCopia();

    Tablero &tablero();

    void ubicar(Ocurrencia o);

    Nat puntaje(Nat i);

    bool JugadaValida(const Ocurrencia o);

    vector<Nat> &fichasPorJugador(Nat k);

    Nat cantidadPorLetra(Letra l, Nat k);

    Letra contenidoEnCoordenada(Nat i, Nat j);

    Nat turno();

    bool hayLetra(Nat x, Nat y);


private:
    Tablero _tablero;
    Repositorio _repositorio;
    // sdandois: repositorioCopia no estaba en la estructura del TP1.
    Repositorio _repositorioCopia;
    Variante _variante;
    Nat _cantJugadores;
    Nat _turno;
    vector<vector<Nat>> _cantPorLetra;
    vector<Nat> _puntaje;
    vector<queue<Ocurrencia>> _historial;
    Nat _rondaGlobal;
    vector<vector<Nat>> _tableroHistorico;

    vector<Letra> proximas(Repositorio &r, Nat f);

    void agregarLetras(vector<Nat> &fs, vector<Letra> ls);

    void desencolarN(Repositorio r, Nat f);

    void llenarHistorial(Ocurrencia o, Nat R);

    void quitarFichas(Ocurrencia o);

    Nat sumarHorizontal(tuple<Nat, Nat, Letra> t);

    Nat sumarVertical(tuple<Nat, Nat, Letra> t);

    vector<Letra> list2arrayLetra(list<Letra> L);

    bool esHorizontal(const vector<tuple<Nat, Nat, Letra>> o);

    bool esVertical(const vector<tuple<Nat, Nat, Letra>> o);

    void desencolar(Repositorio &r, Nat f);

    bool tieneLetras(vector<tuple<Nat, Nat, Letra>> v);

    bool estanEnTablero(vector<tuple<Nat, Nat, Letra>> v);

    vector<tuple<Nat, Nat, Letra>> set2secu(Ocurrencia o);

    Nat cantidadApariciones(Letra i, vector<tuple<Nat, Nat, Letra>> v);

    bool esDiscontigua(vector<tuple<Nat, Nat, Letra>> v);

    void sortHorizontal(vector<tuple<Nat, Nat, Letra>> v);

    void sortVertical(vector<tuple<Nat, Nat, Letra>> v);

#endif //TP_JUEGODEPALABRAS_JUEGO_H
};