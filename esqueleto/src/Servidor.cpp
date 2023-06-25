#include "Servidor.h"

Servidor::Servidor(Nat k, Variante v, Repositorio r) : _cantEsperados(k), _cantConectados(0), _juego(Juego(k, v, r)), _notifPorCliente(k), _configuracion(make_tuple(v, r)), _notifGLobal(), _contador(0){
    for(Nat i = 0;i < _notifPorCliente.size();i++) {
        get<1>(_notifPorCliente[i]) = _notifGLobal.begin();
    }
}


Nat Servidor::cantEsperados() {
    return _cantEsperados;
}


Nat Servidor::cantConectados() {
    return _cantConectados;
}


tuple<Variante, Repositorio>& Servidor::configuracion() {
    return _configuracion;
}


Juego& Servidor::juego() {
    return _juego;
}


IdCliente Servidor::conectarCliente() {
    Notificacion cid = Notificacion::nuevaIdCliente(_cantConectados);
    get<0>(_notifPorCliente[_cantConectados]).push(make_tuple(cid, _contador));
    _contador = _contador + 1;
    _cantConectados = _cantConectados + 1;
    if (_cantConectados == _cantEsperados) {
        _notifGLobal.push_back(make_tuple(Notificacion::nuevaEmpezar(_juego.variante().tamanioTablero()), _contador));
        _contador = _contador + 1;
        _notifGLobal.push_back(make_tuple(Notificacion::nuevaTurnoDe(0), _contador));
        _contador = _contador + 1;
        for(Nat i = 0; i < _cantEsperados; i++) {
            multiset<Letra> s;
            for(Nat j = 0;j < _juego.variante().cantFichas(); j++) {
                s.insert(_juego.repositorioCopia().front());
                _juego.repositorioCopia().pop_front();
            }
            Notificacion notif = Notificacion::nuevaReponer(s);
            get<0>(_notifPorCliente[i]).push(make_tuple(notif,_contador));
            _contador = _contador + 1;
            get<1>(_notifPorCliente[i]) = _notifGLobal.begin();
        }
    }
    return _cantConectados - 1;
}

list<Notificacion> Servidor::consultar(IdCliente cid) {
        auto itG = _notifGLobal.begin();

        // @sdandois. Este while que tienen a continuacion es trampa.
        // Estan haciendo O(notifGlobal.size() ^ 2) operaciones 

        // El segundo componente de notifCliente deberia ser un iterador sobre la lista global. 
        // El iterador sobre notif cliente no necesitan guardarlo porque siempre arrancan desde el
        // principio.
        while(distance(itG,_notifGLobal.end())>1){
            ++itG;
        }
        list<Notificacion> notifs;
        if(get<1>(_notifPorCliente[cid])==_notifGLobal.begin()){
            notifs = armarNotificaciones(cid);
        } else {
            ++get<1>(_notifPorCliente[cid]);
            notifs = armarNotificaciones(cid);
        }
        get<1>(_notifPorCliente[cid]) = itG;
        return notifs;
}

list<Notificacion> Servidor::armarNotificaciones(IdCliente cid) {
    list<tuple<Notificacion, Nat>>  listaPrivada = cola2list(get<0>(_notifPorCliente[cid]));
    desencolarN(get<0>(_notifPorCliente[cid]), get<0>(_notifPorCliente[cid]).size());
    auto itP = listaPrivada.begin();
    list<Notificacion> notifs;
    while(get<1>(_notifPorCliente[cid]) != _notifGLobal.end() && itP != listaPrivada.end()) {
        if(get<1>(*itP) < get<1>(*get<1>(_notifPorCliente[cid]))) {
            notifs.push_back(get<0>(*itP));
            ++itP;
        } else {
            notifs.push_back(get<0>(*get<1>(_notifPorCliente[cid])));
            ++get<1>(_notifPorCliente[cid]);
        }
    }
    if(get<1>(_notifPorCliente[cid]) == _notifGLobal.end()) {
        while(itP != listaPrivada.end()) {
            notifs.push_back(get<0>(*itP));
            ++itP;
        }
    } else if (itP == listaPrivada.end()) {
        while (get<1>(_notifPorCliente[cid]) != _notifGLobal.end()) {
            notifs.push_back(get<0>(*get<1>(_notifPorCliente[cid])));
            ++get<1>(_notifPorCliente[cid]);
        }
    }
    return notifs;
}


bool Servidor::empezo() {
    return _cantConectados == _cantEsperados;
}

void Servidor::recibirMensaje(IdCliente cid, Ocurrencia o) {
    if(empezo() && _juego.turno() == cid && _juego.JugadaValida(o)) {
        Nat preUbicar = _juego.puntaje(cid);
        _notifGLobal.push_back(make_tuple(Notificacion::nuevaUbicar(cid, o), _contador));
        _contador = _contador +1;
        _juego.ubicar(o);
        _notifGLobal.push_back(make_tuple(Notificacion::nuevaSumaPuntos(cid, _juego.puntaje(cid) - preUbicar), _contador));
        _contador = _contador + 1;
        multiset<Letra> proximas;
        for(int i = 0; i < o.size(); i++){
            proximas.insert(_juego.repositorioCopia().front());
            _juego.repositorioCopia().pop_front();
        }
        get<0>(_notifPorCliente[cid]).push(make_tuple(Notificacion::nuevaReponer(proximas), _contador));
        _contador = _contador + 1;
        Notificacion id = Notificacion::nuevaTurnoDe(_juego.turno());
        _notifGLobal.push_back(make_tuple(id, _contador));
        _contador = _contador + 1;
    } else {
        get<0>(_notifPorCliente[cid]).push(make_tuple(Notificacion::nuevaMal(), _contador));
        _contador = _contador + 1;
    }
}

void Servidor::desencolarN(queue<tuple<Notificacion, Nat>> &r, Nat f) {
    for(Nat i = 0;i<f;i++) {
        r.pop();
    }
}

list<tuple<Notificacion, Nat>> Servidor::cola2list(queue<tuple<Notificacion, Nat>> c) {
    list<tuple<Notificacion, Nat>> l;
    while(c.size() > 0) {
        l.push_back(c.front());
        c.pop();
    }
    return l;
}

