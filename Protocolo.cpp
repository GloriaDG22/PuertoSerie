//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

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

void Protocolo::reiniciarNumTrama(){
    numTrama='0';
}

void Protocolo::cambiarNumTrama(){
    if(numTrama=='0')
        numTrama='1';
    else
        numTrama='0';
}

void Protocolo::iniciarProtMaestro(HANDLE &PuertoCOM, HANDLE &Pantalla){
    printf("Has seleccionado MAESTRO, seleccione la operacion a realizar: \n 1.Seleccion\n 2.Sondeo\n");
    char sel = getch();
    bool correcto = false;
    esMaestro = true;
    while (!correcto){///seleccion
        switch(sel){
        case '1':
            printf("Ha elegido seleccion \n");
            tipoOper='R';
            EnviarCaracter(PuertoCOM, tipoOper);
            correcto=true;
            break;
        case '2':///sondeo
            printf("Ha elegido sondeo \n");
            tipoOper='T';
            EnviarCaracter(PuertoCOM, tipoOper);
            correcto=true;
            break;
        default:
            printf("Ha introducido un valor erroneo, intentelo de nuevo \n");
            break;
        }
    }
}

void Protocolo::iniciarProtEsclavo(HANDLE &PuertoCOM, HANDLE &Pantalla){
    printf("Has seleccionado ESCLAVO\n");
    esMaestro = false;
    char carR = RecibirCaracter(PuertoCOM);
    while (!carR){
        carR = RecibirCaracter(PuertoCOM);
    }
    tipoOper=carR; ///Guarda el tipo de operacion que haya ele
}

unsigned char Protocolo::getTipoOper(){
    return tipoOper;
}

bool Protocolo::getEsMaestro(){
    return esMaestro;
}

bool Protocolo::getCerrar(){
    return cerrar;
}

bool Protocolo::getTCorrecta(){
    return tCorrecta;
}

unsigned char Protocolo::getNumTrama(){
    return tipoOper;
}

void Protocolo::setCerrar(bool _cerrar){
    cerrar = _cerrar;
}

void Protocolo::setTCorrecta(bool _tCorrecta){
    tCorrecta = _tCorrecta;
}

