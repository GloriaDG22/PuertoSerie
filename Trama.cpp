#include "Trama.h"

Trama::Trama(){
    sincr=' ';
    dir=' ';
    control=' ';
    numTrama=' ';
}

void Trama::setSincr (unsigned char Csincr){
    sincr=Csincr;
}

void Trama::setDir (unsigned char Cdir){
    dir=Cdir;
}

void Trama::setControl (unsigned char Ccontrol){
    control=Ccontrol;
}

void Trama::setNumTrama (unsigned char CnumTrama){
    numTrama=CnumTrama;
}

unsigned char Trama::getSincr(){
    return sincr;
}

unsigned char Trama::getDir(){
    return dir;
}

unsigned char Trama::getControl (){
    return control;
}

unsigned char Trama::getNumTrama(){
    return numTrama;
}

void Trama::imprimirTipoTrama (){
    printf ("Se ha recibido una trama de tipo ");
    switch (control){
    case 05:
        printf ("ENQ \n");
        break;
    case 04:
        printf ("EOT \n");
        break;
    case 06:
        printf ("ACK \n");
        break;
    case 21:
        printf ("NACK \n");
        break;
    default:
        printf("Ha habido un problema \n");
        break;
    }
}
