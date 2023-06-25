#ifndef TP_JUEGODEPALABRAS_CONJTRIE_H
#define TP_JUEGODEPALABRAS_CONJTRIE_H

#include "Tipos.h"
#include "Letra.h"

using namespace std;

class ConjTrie {
public:
    ConjTrie();

    ConjTrie(const ConjTrie& aCopiar);

    ConjTrie& operator=(const ConjTrie& d);

    ~ConjTrie();

    void agregar(const vector<Letra> &elem);

    bool pertenece(const vector<Letra> &elem) const;

    // @sdandois: altura es nuevo. NO estaba en TP1.
    const Nat altura();

private:
    struct Nodo {
        vector<Nodo*> siguientes;
        bool pertenece;
        Nodo() : siguientes(TAMANIO_ALFABETO, nullptr), pertenece(false) {};
    };

    Nodo* raiz;
    Nat _altura;

    void agregarAux(const vector<Letra> &elem, Nodo *N, Nat n);

    bool perteneceAux(const vector<Letra> &elem, Nodo *N, Nat n) const;

    void destructor(Nodo* N);

    void copiar(Nodo* N, Nodo* aCopiar);
};

#endif //TP_JUEGODEPALABRAS_CONJTRIE_H
