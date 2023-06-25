#include "ConjTrie.h"
#include "Letra.h"

ConjTrie::ConjTrie() : raiz(new Nodo()), _altura(0) {

}

ConjTrie::ConjTrie(const ConjTrie& aCopiar) : ConjTrie() {
    *this = aCopiar;
}


ConjTrie& ConjTrie::operator=(const ConjTrie& t) {
    destructor(raiz);
    _altura = t._altura;
    raiz = new Nodo();
    copiar(raiz, t.raiz);

    return *this;
}

const Nat ConjTrie::altura() {
    return _altura;
}

void ConjTrie::agregar(const vector<Letra> &elem) {
    if(_altura < elem.size()) {
        _altura = elem.size();
    }
    agregarAux(elem, raiz, 0);
}

bool ConjTrie::pertenece(const vector<Letra> &elem) const {
    return perteneceAux(elem, raiz, 0);
}

void ConjTrie::agregarAux(const vector<Letra> &elem, Nodo* N, Nat n) {
    if(n == elem.size()) {
        N->pertenece = true;
    } else if(N->siguientes[ord(elem[n])] == nullptr) {
        N->siguientes[ord(elem[n])] = new Nodo();
        agregarAux(elem, N->siguientes[ord(elem[n])],n+1);
    } else {
        agregarAux(elem, N->siguientes[ord(elem[n])],n+1);
    }
}

bool ConjTrie::perteneceAux(const vector<Letra> &elem, Nodo* N, Nat n) const {
    if (n == elem.size()) {
        return N->pertenece;
    } else {
        if(N->siguientes[ord(elem[n])] == nullptr) {
            return false;
        } else {
            return perteneceAux(elem, N->siguientes[ord(elem[n])], n + 1);
        }
    }
}


ConjTrie::~ConjTrie() {
    destructor(raiz);
    // @sdandois. La siguiente linea no es necesaria.
    _altura = 0;
}


void ConjTrie::destructor(Nodo* N) {
    if (N != nullptr){
        for (int i = 0; i < TAMANIO_ALFABETO; i++) {
            if (N->siguientes[i] != nullptr) {
                destructor(N->siguientes[i]);
                N->siguientes[i] = nullptr;
            }
        }
        delete N;
    }
}

void ConjTrie::copiar(ConjTrie::Nodo* N, ConjTrie::Nodo* aCopiar) {
    if (aCopiar->pertenece) {
        N->pertenece = true;
    }
    for (int i = 0; i < TAMANIO_ALFABETO; i++) {
        if (aCopiar->siguientes[i] != nullptr) {
            N->siguientes[i] = new Nodo();
            copiar(N->siguientes[i], aCopiar->siguientes[i]);
        }
    }
}