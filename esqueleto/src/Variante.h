#include "Tipos.h"
#include "Letra.h"
#include "ConjTrie.h"
#ifndef TP_JUEGODEPALABRAS_VARIANTE_H
#define TP_JUEGODEPALABRAS_VARIANTE_H

class Variante {
public:
    Variante(Nat n, Nat f, map<Letra, Nat> d, set<vector<Letra>> c);
    const Nat tamanioTablero() const;
    const Nat cantFichas() const;
    const Nat puntajeLetra(Letra l) const;
    bool palabraLegitima(const vector<Letra> palabra) const;
    const Nat Lmax();

private:
    Nat _tamanio;
    Nat _fichasIniciales;
    vector<Nat> _puntajeLetra;
    ConjTrie _palabrasLegitimas;

};

#endif //TP_JUEGODEPALABRAS_VARIANTE_H
