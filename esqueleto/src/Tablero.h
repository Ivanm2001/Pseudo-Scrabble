#include "Tipos.h"


#ifndef TP2_ESQUELETO_Y_ENUNCIADO_TABLERO_H
#define TP2_ESQUELETO_Y_ENUNCIADO_TABLERO_H

class Tablero {
public:
    Tablero(Nat tamanio);
    ~Tablero();
   void ponerLetra(Nat i, Nat j, Letra &l);
   Nat tamanio() const;
   bool hayLetra( Nat i, Nat j) const;
   Letra dameLetra( Nat i, Nat j) const;
   bool enTablero( Nat i, Nat j) const;
   // @sdandois: Asumo esto lo quitaron porque se dieron cuenta
   // de que no era necesario
   //set<Ocurrencia> ocurrenciaDePalabras() const;
   void ponerLetras(Ocurrencia o);
   void quitarLetras(const Ocurrencia o);

private:
    struct Nodo{
        Letra valor;
        Nodo(Letra l) : valor(l) {};
    };
    vector<vector<Nodo*>> _tab;
    Nat _tamanio;

};
//Nos dimos cuenta que en la especificacion, tab es arr[arr[Nodo]] y Nodo es simplemente un puntero a letra,
// por lo que decidimos reemplazarlo directamente.
// @sdandois: Acá está como Nodo y no como puntero a letra. Igual, es lo mismo.

#endif //TP2_ESQUELETO_Y_ENUNCIADO_TABLERO_H
