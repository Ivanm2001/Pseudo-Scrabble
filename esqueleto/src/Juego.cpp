#include "Juego.h"

Juego::Juego(Nat k, Variante v, Repositorio r) : _tableroHistorico(v.tamanioTablero(), vector<Nat>(v.tamanioTablero())) , _historial(k), _cantPorLetra(k, vector<Nat>(TAMANIO_ALFABETO, 0)) , _puntaje(k), _variante(v), _cantJugadores(k), _rondaGlobal(1), _turno(0), _tablero(
        Tablero(v.tamanioTablero())), _repositorio(r), _repositorioCopia(r) {
        for(Nat i=0; i < v.tamanioTablero(); i++){
            for(Nat j=0; j<v.tamanioTablero(); j++){
                _tableroHistorico[i][j] = 0;
            }
        }

        for(Nat i=0; i<k; i++){
            _puntaje[i] = 0;
            queue<Ocurrencia> q;
            _historial[i] = q;
        }

        for(Nat i=0; i<k; i++){
            vector<Letra> aux = proximas(_repositorio, v.cantFichas());
            agregarLetras(_cantPorLetra[i], aux);
        }

}

void Juego::ubicar(Ocurrencia o){
    quitarFichas(o);
    _tablero.ponerLetras(o);
    llenarHistorial(o, _rondaGlobal);
    vector<Letra> aux = proximas(_repositorio, o.size());
    agregarLetras(_cantPorLetra[_turno], aux);
    _turno = (_turno + 1) % _cantJugadores;
    _rondaGlobal = _rondaGlobal + 1;
}


void Juego::llenarHistorial(Ocurrencia o, Nat R){
    _historial[_turno].push(o);
    for (tuple<Nat, Nat, Letra> i: o){
        _tableroHistorico[get<0>(i)][get<1>(i)] = R;
    }
}

void Juego::quitarFichas(Ocurrencia o){
    for(tuple<Nat, Nat, Letra> i:o){
        _cantPorLetra[_turno][ord(get<2>(i))] = _cantPorLetra[_turno][ord(get<2>(i))] - 1;
    }
}

Variante& Juego::variante() {
    return _variante;
}

Nat Juego::cantJugadores() {
    return _cantJugadores;
}

Nat Juego::turno() {
    return _turno;
}

Repositorio& Juego::repositorio() {
    return _repositorio;
}

Repositorio& Juego::repositorioCopia() {
    return _repositorioCopia;
}

Tablero& Juego::tablero() {
    return _tablero;
}

Nat Juego::puntaje(Nat k) {
    while(!_historial[k].empty()) {
        Ocurrencia prox = _historial[k].front();
        _historial[k].pop();
        if (!prox.empty()) {
            vector<tuple<Nat, Nat, Letra>> palabra = set2secu(prox);
            if (palabra.size() == 1) {
                _puntaje[k] = _puntaje[k] + 2 * (_variante.puntajeLetra(get<2>(palabra[0])));
            } else {
                if (esHorizontal(palabra)) {
                    for (Nat i = 0; i < palabra.size(); i++) {
                        _puntaje[k] = _puntaje[k] + sumarVertical(palabra[i]);
                        if (i == palabra.size() - 1) {
                            _puntaje[k] = _puntaje[k] + sumarHorizontal(palabra[i]);
                        }
                    }
                } else {
                    for (Nat i = 0; i < palabra.size(); i++) {
                        _puntaje[k] = _puntaje[k] + sumarHorizontal(palabra[i]);
                        if (i == palabra.size() - 1) {
                            _puntaje[k] = _puntaje[k] + sumarVertical(palabra[i]);
                        }
                    }
                }
            }
        }
    }
    return _puntaje[k];
}


bool Juego::JugadaValida(const Ocurrencia o) {
    vector<tuple<Nat, Nat, Letra>> v = set2secu(o);
    if (o.size() == 0) { //es vacia
        return true;
    }
    if(o.size() > _variante.Lmax()){
        return false;
    }
    if (esDiscontigua(v)) {
        return false;
    }
    if((!esHorizontal(v) && !esVertical(v))){
        return false;
    }
    if ((!tieneLetras(v))){
        return false;
    }
    if((!estanEnTablero(v))){
        return false;
    } else {
        _tablero.ponerLetras(o); //ponemos la ocurrencia en tablero para corroborar que sea valida
        list<Letra> L; //inicializo lista vacia de letras
        tuple<Nat, Nat, Letra> inicio = v[0]; //pongo en inicio la primer letra de la ocurrencia y su posicion
        L.push_back(get<2>(inicio)); //pusheo en L la letra sola de inicio
        if (esHorizontal(v)){ // es horizontal
            int continua = 1;
            for (Nat j = get<0>(inicio) + 1; j < _variante.tamanioTablero() && continua == 1; j++) {
                if (_tablero.hayLetra(j, get<1>(inicio))) {
                    L.push_back(_tablero.dameLetra(j, get<1>(inicio)));
                } else {
                    continua = 0;
                }
            }
            continua = 1;
            for (Nat j = get<0>(inicio) - 1; j >= 0 && continua == 1 && j < _tablero.tamanio(); j--) {
                if (_tablero.hayLetra(j, get<1>(inicio))) {
                    L.push_front(_tablero.dameLetra(j, get<1>(inicio)));
                } else {
                    continua = 0;
                }
            }
            vector<Letra> palabra = list2arrayLetra(L); //inicializamos vector de palabras con las letras de la ocurrencia
            if (_variante.palabraLegitima(palabra)){
                list<Letra> vacia;
                int sigue = 1;
                for (Nat i = 0; i < v.size(); i++){
                    list<Letra> LV = vacia;
                    LV.push_back(get<2>(v[i]));

                    for (Nat j = get<1>(v[i]) + 1; j < _variante.tamanioTablero() && sigue == 1; j++) {
                        if (_tablero.hayLetra(get<0>(v[i]), j)) {
                            LV.push_back(_tablero.dameLetra(get<0>(v[i]), j));
                        } else {
                            sigue = 0;
                        }
                    }
                    sigue = 1;
                    for (Nat j = get<1>(v[i]) - 1; j >= 0  && sigue == 1 && j < _tablero.tamanio() ; j--) {
                        if (_tablero.hayLetra(get<0>(v[i]), j)) {
                            LV.push_front(_tablero.dameLetra(get<0>(v[i]), j));
                        } else {
                            sigue = 0;
                        }
                    }
                    vector<Letra> palabra2 = list2arrayLetra(LV);
                    if (!_variante.palabraLegitima(palabra2)) {
                        _tablero.quitarLetras(o);
                        return false;
                    }
                }
            } else {
                _tablero.quitarLetras(o);
                return false;
            }
        } else { //es vertical
            int continua = 1;
            for (Nat j = get<1>(inicio) + 1; j < _variante.tamanioTablero() && continua == 1; j++) {
                if (_tablero.hayLetra(get<0>(inicio), j)) {
                    L.push_back(_tablero.dameLetra(get<0>(inicio), j));
                } else {
                    continua = 0;
                }
            }
            continua = 1;
            for (Nat j = get<1>(inicio) - 1; j >= 0 && continua == 1 && j < _tablero.tamanio(); j--) {
                if (_tablero.hayLetra(get<0>(inicio), j)) {
                    L.push_front(_tablero.dameLetra(get<0>(inicio), j));
                } else {
                    continua = 0;
                }
            }
            vector<Letra> palabra = list2arrayLetra(L);
            if (_variante.palabraLegitima(palabra)){
                list<Letra> vacia;
                int sigue = 1;
                for (Nat i = 0; i < v.size(); i++){
                    list<Letra> LH;
                    LH.push_back(get<2>(v[i]));
                    for (Nat j = get<0>(v[i]) + 1; j < _variante.tamanioTablero() && sigue == 1; j++) {
                        if (_tablero.hayLetra(j, get<1>(v[i]))) {
                            LH.push_back(_tablero.dameLetra(j, get<1>(v[i])));
                        } else {
                            sigue = 0;
                        }
                    }
                    sigue = 1;
                    for (Nat j = get<0>(v[i]) - 1; j >= 0 && sigue == 1 && j < _tablero.tamanio(); j--) {
                        if (_tablero.hayLetra(j, get<1>(v[i]))) {
                            LH.push_front(_tablero.dameLetra(j, get<1>(v[i])));
                        } else {
                            sigue = 0;
                        }
                    }
                    vector<Letra> palabra2 = list2arrayLetra(LH);
                    if (!_variante.palabraLegitima(palabra2)) {
                        _tablero.quitarLetras(o);
                        return false;
                    }
                }
            } else{
                _tablero.quitarLetras(o);
                return false;
            }
        }
        _tablero.quitarLetras(o);
    }
    return true;
}

Nat Juego::sumarHorizontal(tuple<Nat, Nat, Letra> t) {
    list<tuple<Nat, Nat, Letra>> L;
    L.push_back(t);

    if(get<0>(t) == 0) {
        Nat Sig = get<0>(t) + 1;
        while(Sig < _variante.tamanioTablero() && (_tableroHistorico[get<0>(t)][get<1>(t)] >= _tableroHistorico[Sig][get<1>(t)]) && _tableroHistorico[Sig][get<1>(t)] > 0) {
            L.push_back(make_tuple(Sig,get<1>(t),_tablero.dameLetra(Sig, get<1>(t) )));
            Sig = Sig + 1;
        }
    } else if(get<0>(t) == _tablero.tamanio() - 1) {
        Nat Ant = get<0>(t) - 1;
        while(0 <= Ant && Ant < _tablero.tamanio() && (_tableroHistorico[get<0>(t)][get<1>(t)] >= _tableroHistorico[Ant][get<1>(t)])&& _tableroHistorico[Ant][get<1>(t)]>0) {
            L.push_front(make_tuple(Ant,get<1>(t),_tablero.dameLetra(Ant, get<1>(t) )));
            Ant = Ant - 1;
        }
    } else {
        Nat Sig = get<0>(t) + 1;
        Nat Ant = get<0>(t) - 1;
        while(Sig < _variante.tamanioTablero() && (_tableroHistorico[get<0>(t)][get<1>(t)] >= _tableroHistorico[Sig][get<1>(t)]) &&_tableroHistorico[Sig][get<1>(t)]>0) {
            L.push_back(make_tuple(Sig,get<1>(t),_tablero.dameLetra(Sig,get<1>(t))));
            Sig = Sig + 1;
        }
        while(0 <= Ant && Ant < _tablero.tamanio() && (_tableroHistorico[get<0>(t)][get<1>(t)] >= _tableroHistorico[Ant][get<1>(t)])&&_tableroHistorico[Ant][get<1>(t)]>0) {
            L.push_front(make_tuple(Ant,get<1>(t),_tablero.dameLetra(Ant,get<1>(t))));
            Ant = Ant - 1;
        }
    }
    Nat Suma = 0;
    auto itG = L.begin();
    while(itG != L.end()) {
        Suma = Suma + _variante.puntajeLetra(get<2>(*itG));
        ++itG;
    }
    return Suma;
}

Nat Juego::sumarVertical(tuple<Nat, Nat, Letra> t) {
    list<tuple<Nat, Nat, Letra>> L;
    L.push_back(t);
    if(get<1>(t) == 0) {
        Nat Sig = get<1>(t) + 1;
        while(Sig < _variante.tamanioTablero() && (_tableroHistorico[get<0>(t)][get<1>(t)] >= _tableroHistorico[get<0>(t)][Sig]) && 0<_tableroHistorico[get<0>(t)][Sig]) {
            L.push_back(make_tuple(get<0>(t),Sig,_tablero.dameLetra(get<0>(t), Sig)));
            Sig = Sig + 1;
        }
    } else if(get<1>(t) == _tablero.tamanio() - 1) {
        Nat Ant = get<1>(t) - 1;
        while(0 <= Ant && Ant < _tablero.tamanio() && (_tableroHistorico[get<0>(t)][get<1>(t)] >= _tableroHistorico[get<0>(t)][Ant])&& 0<_tableroHistorico[get<0>(t)][Ant]) {
            L.push_front(make_tuple(get<0>(t),Ant,_tablero.dameLetra(get<0>(t), Ant)));
            Ant = Ant - 1;
        }
    } else {
        Nat Sig = get<1>(t) + 1;
        while(Sig < _variante.tamanioTablero() && (_tableroHistorico[get<0>(t)][get<1>(t)] >= _tableroHistorico[get<0>(t)][Sig])&& 0<_tableroHistorico[get<0>(t)][Sig]) {
            L.push_back(make_tuple(get<0>(t),Sig,_tablero.dameLetra(get<0>(t), Sig)));
            Sig = Sig + 1;
        }
        Nat Ant = get<1>(t) - 1;
        while(0 <= Ant && Ant < _tablero.tamanio() && (_tableroHistorico[get<0>(t)][get<1>(t)] >= _tableroHistorico[get<0>(t)][Ant])&& 0<_tableroHistorico[get<0>(t)][Ant]) {
            L.push_front(make_tuple(get<0>(t),Ant,_tablero.dameLetra(get<0>(t), Ant)));
            Ant = Ant - 1;
        }
    }
    Nat Suma = 0;
    auto itG = L.begin();
    while(itG != L.end()) {
        Suma = Suma + _variante.puntajeLetra(get<2>(*itG));
        ++itG;
    }
    return Suma;
}

vector<Letra> Juego::proximas(Repositorio &r, Nat f) {
    vector<Letra> prox;
    for(Nat i = 0;i < f;i++) {
        prox.push_back(r.front());
        r.pop_front();
    }
    return prox;
}

void Juego::agregarLetras(vector<Nat> &fs, vector<Letra> ls) {
    for(Nat i = 0;i < ls.size();i++) {
        fs[ord(ls[i])] = fs[ord(ls[i])] + 1;
    }
}

void Juego::desencolarN(Repositorio r, Nat f) {
    for(Nat i = 0;i<f;i++) {
        r.pop_front();
    }
}

bool Juego::tieneLetras(vector<tuple<Nat, Nat, Letra>> v) {
    bool res = true;
    for (int i = 0;i < v.size();i++) {
        if (_cantPorLetra[_turno][ord(get<2>(v[i]))] < cantidadApariciones(get<2>(v[i]), v)) {
            res = false;
        }
    }
    return res;
}

Nat Juego::cantidadApariciones(Letra i, vector<tuple<Nat, Nat, Letra>> v){
    Nat contador = 0;
    for(Nat j=0; j<v.size(); j++){
        if(i== get<2>(v[j])){
            contador = contador + 1;
        }
    }
    return contador;
}

bool Juego::estanEnTablero(vector<tuple<Nat, Nat, Letra>> v) {
    for (int i = 0;i < v.size();i++) {
        if (!_tablero.enTablero(get<0>(v[i]), get<1>(v[i]))) {
            return false;
        } else if (_tablero.hayLetra(get<0>(v[i]), get<1>(v[i]))) {
            return false;
        }
    }
    return true;
}

bool Juego::hayLetra(Nat x, Nat y) {
    return _tablero.hayLetra(x,y);
}

vector<Letra> Juego::list2arrayLetra(list<Letra> L) {
    vector<Letra> v;
    auto it = L.begin();
    Nat i = 0;
    while(it != L.end()) {
        v.push_back(*it);
        ++it;
    }
    return v;
}


bool Juego::esHorizontal(const vector<tuple<Nat,Nat,Letra>> o) {
    Nat referencia = get<1>(o[0]);
    for(Nat i = 0; i < o.size();i++) {
        if(referencia != get<1>(o[i])) {
            return false;
        }
    }
    return true;
}

bool Juego::esVertical(const vector<tuple<Nat,Nat,Letra>> o) {
    Nat referencia = get<0>(o[0]);
    for(Nat i = 0; i < o.size();i++) {
        if(referencia != get<0>(o[i])) {
            return false;
        }
    }
    return true;
}

void Juego::desencolar(Repositorio &r, Nat f) {
    for(Nat j=0; j<_cantJugadores; j++) {
        for (Nat i = 0; i < f; i++) {
            r.pop_front();
        }
    }
}


vector<Nat>& Juego::fichasPorJugador(Nat k) {
    return _cantPorLetra[k];
}

Nat Juego::cantidadPorLetra(Letra l, Nat k) {
    return _cantPorLetra[k][ord(l)];
}

Letra Juego::contenidoEnCoordenada(Nat i, Nat j) {
    return _tablero.dameLetra(i,j);
}

vector<tuple<Nat, Nat, Letra>> Juego::set2secu(Ocurrencia o){
    vector<tuple<Nat, Nat, Letra>> v;
    for (tuple<Nat, Nat, Letra> t: o) {
        v.push_back(t);
    }
    return v;
}

bool Juego::esDiscontigua(vector<tuple<Nat, Nat, Letra>> v) {
    if (esHorizontal(v)) {
        sortHorizontal(v);
        for (int i = 0; i < v.size() - 1; i ++) {
            if (get<0>(v[i]) != get<0>(v[i + 1]) - 1) {
                return true;
            }
        }
    } else if (esVertical(v)) {
        sortVertical(v);
        for (int i = 0; i < v.size() - 1; i ++) {
            if (get<1>(v[i]) != get<1>(v[i + 1]) - 1) {
                return true;
            }
        }
    }
    return false;
}

void Juego::sortHorizontal(vector<tuple<Nat, Nat, Letra>> v) {
    for(int i = 0; i < v.size() - 1; i++){
        int min = i;
        for (int j = i + 1; j < v.size(); j++){
            if (get<0>(v[j]) < get<0>(v[min])){
                min = j;
            }
        }
        swap(v[i], v[min]);
    }
}

void Juego::sortVertical(vector<tuple<Nat, Nat, Letra>> v) {
    for(int i = 0; i < v.size() - 1; i++){
        int min = i;
        for (int j = i + 1; j < v.size(); j++){
            if (get<1>(v[j]) < get<1>(v[min])){
                min = j;
            }
        }
        swap(v[i], v[min]);
    }
}