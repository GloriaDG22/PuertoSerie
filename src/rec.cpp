#include "rec.h"

rec* rec::obj=0;

rec::rec (){
    esFichero=false;
    finFichero=false;
}

void rec::createInstance (){
    if (obj==0)
        obj = new rec();
}

rec* rec::getInstance (){
    createInstance();
    return obj;
}

int rec::calcular (){
    return 5*10;
}
