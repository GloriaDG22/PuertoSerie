//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Funciones.h"

Funciones* Funciones::obj=0;

Funciones::Funciones(){
    fSal = NULL;
    escribir = false;
}

void Funciones::createInstance (){
    if (obj==0)
        obj = new Funciones();
}

Funciones* Funciones::getInstance (){
    createInstance();
    return obj;
}

void Funciones::abrirFlujo(){
    fSal = fopen(NOMFICHEROF5, "wt");
}

void Funciones::cerrarFlujo(){
    fclose(fSal);
}

void Funciones::setEscribir(bool _escribir){
    escribir=_escribir;
}

void Funciones::escribirCadena(string cadena){
    printf("%s", cadena.c_str());
    if (escribir && fSal!=NULL)
        fprintf(fSal, "%s", cadena.c_str());
}

void Funciones::escribirCaracter(char caracter){
    printf("%c", caracter);
    if (escribir && fSal!=NULL)
        fprintf(fSal, "%c", caracter);
}

bool Funciones::comprobarESC(bool &teclaESC){
    if(kbhit()){
        char carR = getch();
        if(carR == 27)
            teclaESC = true;
    }
    return teclaESC;
}
