//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Funciones.h"

Funciones* Funciones::obj=0;

Funciones::Funciones(){
    fSal = NULL;
    escribir = false;
    tipoTecla = 3;
}

void Funciones::createInstance (){
    if (obj==0)
        obj = new Funciones();
}

Funciones* Funciones::getInstance (){
    createInstance();
    return obj;
}

void Funciones::dividirCadena(int cont, int &numTramas){
    if (cont%254==0)
        numTramas=cont/254;
    else
        numTramas=(cont/254)+1;
}

void Funciones::copiarCadena (const char* cadenaFuente, char* cadenaDestino, int offset, int longit){
    int i = 0;
    char actual;
    do{
        actual = cadenaFuente [i+offset];
        cadenaDestino[i] = actual;
        i++;
    } while ((actual!='\0')&&(i<longit));
    cadenaDestino[i]='\0';
}

void Funciones::abrirFlujo(){
    fSal = fopen(NOMFICHEROF5, "wt");
}

void Funciones::cerrarFlujo(){
    if(fSal!=NULL)
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

void Funciones::escribirCadenaSoloEnFichero(string cadena){
    if (escribir && fSal!=NULL)
        fprintf(fSal, "%s", cadena.c_str());
}

void Funciones::escribirCaracter(char caracter){
    printf("%c", caracter);
    if (escribir && fSal!=NULL)
        fprintf(fSal, "%c", caracter);
}

bool Funciones::comprobarTecla(){
    bool hayTecla = false;
    if(kbhit()){
        char carR = getch();
        switch (carR){
        case 27:
            tipoTecla = 1;
            hayTecla = true;
            break;
        case '\0':
            carR = getch();
            if(carR == 65){
                tipoTecla = 2;
                hayTecla = true;
            }
            break;
        }
    }
    return hayTecla;
}

int Funciones::getTecla(){
    return tipoTecla;
}

void Funciones::setTecla(int tecla){
    tipoTecla = tecla;
}
