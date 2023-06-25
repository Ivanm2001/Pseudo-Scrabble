#include "Variante.h"
#include "Letra.h"

Variante::Variante(const Nat n, const Nat f, map<Letra, Nat> d, set<vector<Letra>> c) : _tamanio(n), _fichasIniciales(f), _puntajeLetra(TAMANIO_ALFABETO, 1), _palabrasLegitimas(ConjTrie()) {
    for(pair<const Letra, unsigned int> l : d) {
        _puntajeLetra[ord(l.first)] = d[l.first];
    }

    for(vector<Letra> palabra : c) {
        _palabrasLegitimas.agregar(palabra);
    }
}

const Nat Variante::tamanioTablero() const {
    return _tamanio;
}
const Nat Variante::Lmax() {
    return _palabrasLegitimas.altura();
}

const Nat Variante::cantFichas() const {
    return _fichasIniciales;
}

const Nat Variante::puntajeLetra(Letra l) const {
    if(ord(l) < TAMANIO_ALFABETO) {
        return _puntajeLetra[ord(l)];
    } else {
        return 1;
    }
}

bool Variante::palabraLegitima(const vector<Letra> palabra) const{
    return _palabrasLegitimas.pertenece(palabra);
}