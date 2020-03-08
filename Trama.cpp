//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Trama.h"

Trama::Trama(){
    sincr=0;
    dir=' ';
    control=0;
    numTrama=' ';
    longit=0;
    Datos[0]='\0';
    BCE=0;
}

Trama::Trama(unsigned char Csincr, unsigned char Cdir, unsigned char Ccontrol, unsigned char CnumTrama,
             unsigned char Clongit, char Cdatos[], unsigned char Cbce){
    sincr=Csincr;
    dir=Cdir;
    control=Ccontrol;
    numTrama=CnumTrama;
    longit=Clongit;
    strcpy(Datos, Cdatos);
    BCE=Cbce;
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

void Trama::setDatos (char cadena[]){
   strcpy(Datos, cadena);
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

void Trama::mostrarTrama(){
    printf(Datos);
}
