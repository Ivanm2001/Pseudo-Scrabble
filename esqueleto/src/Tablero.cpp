#include "Tablero.h"
#include "Tipos.h"
#include "Letra.h"


Tablero::Tablero(Nat tamanio) : _tab(tamanio, vector<Nodo*>(tamanio, nullptr)), _tamanio(tamanio) {

}

void Tablero::ponerLetra(Nat i, Nat j, Letra &l) {
    _tab[i][j] = new Nodo(l);
}

Nat Tablero::tamanio() const {
    return _tamanio;
}

bool Tablero::hayLetra(Nat i, Nat j) const {
    return _tab[i][j] != nullptr;
}

Letra Tablero::dameLetra(Nat i, Nat j) const {
        return _tab[i][j]-> valor;
}

bool Tablero::enTablero(Nat i, Nat j) const {
    bool res = true;
  if ( j >= _tamanio ) {
        res = false;
    } else if (i >= _tamanio) {
        res = false;
    }
    return res;

    // @sdandois: Se puede hacer en una linea:
    // return j <_tamanio && i < _tamanio
}

void Tablero::ponerLetras(Ocurrencia o) {
    for(tuple<Nat, Nat, Letra> n : o) {
        ponerLetra(get<0>(n), get<1>(n), get<2>(n));
    }
}

void Tablero::quitarLetras(const Ocurrencia o) {
    for(tuple<Nat,Nat,Letra> t : o) {
        delete _tab[get<0>(t)][get<1>(t)];
        _tab[get<0>(t)][get<1>(t)] = nullptr;
    }
}


Tablero::~Tablero() {
    for(int i = 0; i < _tamanio; i++) {
        for(int j = 0; j < _tamanio;j++) {
           delete _tab[i][j];
        }
    }
}
