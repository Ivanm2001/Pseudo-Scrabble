
#ifndef TP_JUEGODEPALABRAS_SERVIDOR_H
#define TP_JUEGODEPALABRAS_SERVIDOR_H
#include "Juego.h"
#include "Notificacion.h"

class Servidor {
public:
    Servidor(Nat k, Variante v, Repositorio r);
    Nat cantEsperados();
    Nat cantConectados();
    tuple<Variante, Repositorio>& configuracion();
    Juego& juego();

    // @sdandois. este tuvieron que cambiar.
    IdCliente conectarCliente();
    list<Notificacion> consultar(IdCliente cid);
    void recibirMensaje(IdCliente cid, Ocurrencia o);
    bool empezo();

private:
    Nat _cantEsperados;
    Nat _cantConectados;
    Juego _juego;

    // @sdandois: para que no fuera tan engorroso, podian usar struct para las tuplas.

    vector<tuple<queue<tuple<Notificacion, Nat>>, list<tuple<Notificacion, Nat>>::iterator>> _notifPorCliente;
    tuple<Variante, Repositorio> _configuracion;
    list<tuple<Notificacion, Nat>> _notifGLobal;
    Nat _contador;

    void desencolarN(queue<tuple<Notificacion, Nat>> &r, Nat f);
    list<tuple<Notificacion, Nat>> cola2list(queue<tuple<Notificacion, Nat>> c);
    list<Notificacion> armarNotificaciones(IdCliente cid);
};

#endif //TP_JUEGODEPALABRAS_SERVIDOR_H
