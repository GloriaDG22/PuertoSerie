#include "Trama.h"

Trama::Trama(){
    sincr=22;
    dir='T';
    control=0;
    numTrama='0';
    longit=0;
    Datos[0]='\0';
    BCE=0;
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

void Trama::setLong (unsigned char Clongit){
    longit=Clongit;
}

void Trama::setBCE (unsigned char Cbce){
    BCE=Cbce;
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

unsigned char Trama::getLong(){
    return longit;
}

char* Trama::getDatos(){
    return Datos;
}

void Trama::setDatos (char cadena[]){
   strcpy(Datos, cadena);
}

unsigned char Trama::getBCE(){
    return BCE;
}

void Trama::imprimirTipoTrama (){
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

// hay que probar el bce con vocales acentuadas
// y hay que mostrar el bce tanto al enviarlo como al recibirlo
unsigned char Trama::calcularBce (){
    int i=1;
    unsigned char total=Datos[0];
    while (Datos[i]!='\0'){
        total=total^Datos[i];
        i++;
    }
    if (total==0||total==255)
        total=1;
    return total;
}

void Trama::addChar (char carR, int i){
    Datos[i]=carR;
}

void Trama::mostrarTrama(){
    printf(Datos);
}
