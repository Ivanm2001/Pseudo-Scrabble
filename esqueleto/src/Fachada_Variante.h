#ifndef FACHADA_VARIANTE_H
#define FACHADA_VARIANTE_H

#include "Tipos.h"
#include "Variante.h"

class Fachada_Variante {
public:
    /**
     * Construye una Fachada_Variante a partir de los parametros de las variantes
     */
    Fachada_Variante(
            Nat tamanoTab,
            Nat cantFichas,
            const map<Letra, Nat>& puntajes,
            const set<vector<Letra>>& palabrasLegitimas
    ) : _variante(tamanoTab,cantFichas,puntajes,palabrasLegitimas) {

    };

    /**
     * Retorna el tamano del tablero de la variante
     */
    Nat tamanoTablero() const{
        return _variante.tamanioTablero();
    };

    /**
     * Retorna la cantidad de fichas que cada jugador deberia tener en su mano.
     */
    Nat fichas() const {
        return _variante.cantFichas();
    };

    /**
     * Retorna el puntaje de la letra l
     */
    Nat puntajeLetra(Letra l) const {
        return _variante.puntajeLetra(l);
    };

    /**
     * Indica si una palabra es legitima o no
     */
    bool palabraLegitima(const Palabra& palabra) const {
        return _variante.palabraLegitima(palabra);
    };

    Variante dameVariante() const{
        return _variante;
    };

    Fachada_Variante(Variante &v) : _variante(v) {

    }
    const Nat Lmax(){
        _variante.Lmax();
    };

private:
    Variante _variante;
};

#endif //FACHADA_VARIANTE_H
