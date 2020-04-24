#include "Protocolo.h"

Protocolo* Protocolo::obj=0;

Protocolo::Protocolo(){
    cerrar = false;
}

void Protocolo::createInstance (){
    if (obj==0)
        obj = new Protocolo();
}

Protocolo* Protocolo::getInstance (){
    createInstance();
    return obj;
}
