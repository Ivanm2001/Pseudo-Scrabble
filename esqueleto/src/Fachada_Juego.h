#ifndef FACHADA_JUEGO_H
#define FACHADA_JUEGO_H

#include "Tipos.h"
#include "Fachada_Variante.h"
#include "Juego.h"


class Fachada_Juego {
public:
    /**
     * Construye un juego a partir de la cantidad de jugadores k, la variante v y el repositorio r
     *
     * Complejidad: O(tamanoTab**2 + ALPHABET_SIZE*cantJugadores + cantFichas*cantJugadores)
     */
    Fachada_Juego(Nat k, Fachada_Variante v, const Repositorio& r) : _juego(k,v.dameVariante(), r), _facha(v)
            {
    };

    /**
     * Ubica una Ocurrencia o en el juego
     *
     * Complejidad: O(m)
     *   donde m es el numero de fichas que se ubican
     */
    void ubicar(const Ocurrencia& o){
        _juego.ubicar(o);
    };

    /**
     * Retorna el id del cliente al cual le toca jugar
     *
     * Complejidad: O(1)
     */
    IdCliente turno(){
        return _juego.turno();
    };

    /**
     * Retorna informacion sobre la variante del juego
     *
     * Complejidad: O(1)
     */
    const Fachada_Variante& variante(){
        Variante v = _juego.variante();
        _facha = Fachada_Variante(v);

        return _facha;
    };

    /**
     * Determina si una jugada es valida o no
     *
     * Complejidad: O(Lmax ** 2)
     */
    bool jugadaValida(const Ocurrencia& o){
        return _juego.JugadaValida(o);
    };

    /**
     * Retorna true sii la coordenada se encuentra ocupada.
     *
     * Complejidad: O(1)
     */
    bool hayLetra(Nat x, Nat y){
        return _juego.hayLetra(x,y);
    };

    /**
     * Obtiene el contenido del tablero en una coordenada.
     *
     * Complejidad: O(1)
     */
    Letra letra(Nat i, Nat j) {
        return _juego.contenidoEnCoordenada(i,j);
    };

    /**
     * Retorna el puntaje del jugador id
     *
     * Complejidad: O(1 + m*Lmax)
     *   donde m es la cantidad de fichas que ubico el jugador desde la ultima vez que se preguntó por su puntaje.
     */
    Nat puntaje(IdCliente id) {
        return _juego.puntaje(id);
    };


    /**
     * Dado un jugador id y una letra l, retorna cuantas fichas tiene el jugador de dicha letra.
     *
     * Complejidad: O(1)
     */
    Nat cantFicha(IdCliente id, Letra l) {
        return _juego.cantidadPorLetra(l,id);
    };

private:
    Juego _juego;
    Fachada_Variante _facha;
};

#endif // FACHADA_JUEGO_H
