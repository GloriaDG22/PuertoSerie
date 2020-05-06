//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Protocolo.h"

Protocolo* Protocolo::obj=0;

Protocolo::Protocolo(){
    cerrar = false;
    ficheroProt = NULL;
    estacionQueInicia = false;
    esProt = false;
    finTransferencia=false;
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

void Protocolo::abrirFichero(){
    if(esMaestro)
        ficheroProt = fopen(PROTMAESTRO, "wt");
    else
        ficheroProt = fopen(PROTESCLAVO, "wt");
    if(ficheroProt==NULL)
        escribirCadena("No se ha podido abrir el fichero de escritura\n");
}

void Protocolo::cerrarFichero(){
    if(ficheroProt!=NULL)
        fclose(ficheroProt);
}

void Protocolo::escribirCadena(string cadena){
    printf("\n%s", cadena.c_str());
    if (ficheroProt!=NULL)
        fprintf(ficheroProt, "\n%s", cadena.c_str());
}

void Protocolo::escribirEntero(int numero){
    printf(" %d", numero);
    if (ficheroProt!=NULL)
        fprintf(ficheroProt, " %d", numero);
}

void Protocolo::escribirCaracter (char car){
    printf(" %c", car);
     if (ficheroProt!=NULL)
        fprintf(ficheroProt, " %c", car);
}

void Protocolo::iniciarProtMaestro(HANDLE &PuertoCOM, HANDLE &Pantalla){
    esMaestro = true;
    abrirFichero();
    if (estacionQueInicia)
        fprintf(ficheroProt, "Seleccione maestro o esclavo: \n 1.Maestro\n 2.Esclavo\n");
    escribirCadena("Has seleccionado MAESTRO, seleccione la operacion a realizar: \n 1.Seleccion\n 2.Sondeo\n");
    char sel = getch();
    bool correcto = false;
    while (!correcto){///seleccion
        switch(sel){
        case '1':
            escribirCadena("Ha elegido seleccion \n");
            tipoOper='R';
            EnviarCaracter(PuertoCOM, tipoOper);
            correcto=true;
            break;
        case '2':///sondeo
            escribirCadena("Ha elegido sondeo \n");
            tipoOper='T';
            EnviarCaracter(PuertoCOM, tipoOper);
            correcto=true;
            break;
        default:
            escribirCadena("Ha introducido un valor erroneo, intentelo de nuevo \n");
            break;
        }
    }
    setEstacionQueEnvia();
}

void Protocolo::iniciarProtEsclavo(HANDLE &PuertoCOM, HANDLE &Pantalla){
    esMaestro = false;
    abrirFichero();
    if (estacionQueInicia)
        fprintf(ficheroProt, "Seleccione maestro o esclavo: \n 1.Maestro\n 2.Esclavo\n");
    escribirCadena("Has seleccionado ESCLAVO\n");
    char carR;
    while (!carR)
        carR = RecibirCaracter(PuertoCOM);
    tipoOper=carR; ///Guarda el tipo de operacion que haya elegido
    setEstacionQueEnvia();
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
    return numTrama;
}

bool Protocolo::getFinTransferencia(){
    return finTransferencia;
}

string Protocolo::getTipoTrama(unsigned char control){
    string tipoTrama=" ";
    switch (control){
    case 05:
        tipoTrama = "ENQ";
        break;
    case 04:
        tipoTrama = "EOT";
        break;
    case 06:
        tipoTrama = "ACK";
        break;
    case 21:
        tipoTrama = "NACK";
        break;
    case 02:
        tipoTrama = "STX";
        break;
    }
    return tipoTrama;
}

int Protocolo::getColor(){
    return color;
}

bool Protocolo::getEsProt (){
    return esProt;
}

bool Protocolo::getEstacionQueEnvia(){
    return estacionQueEnvia;
}

void Protocolo::setCerrar(bool _cerrar){
    cerrar = _cerrar;
}

void Protocolo::setTCorrecta(bool _tCorrecta){
    tCorrecta = _tCorrecta;
}

void Protocolo::setEstacionQueInicia(bool _estacion){
    estacionQueInicia = _estacion;
}

void Protocolo::setEsProt(bool _esProt){
    esProt = _esProt;
}

void Protocolo::setColor(int _color){
    color = _color;
}

void Protocolo::setFinTransferencia(bool finT){
    finTransferencia = finT;
}

void Protocolo::setEstacionQueEnvia(){
    if((esMaestro && tipoOper == 'R') || (!esMaestro && tipoOper == 'T'))
        estacionQueEnvia=true;
    else
        estacionQueEnvia=false;
}

void Protocolo::imprimirTrama(unsigned char control, unsigned char bce, unsigned char dir, unsigned char nTrama, string aux){
    string tOper, numT;
    tOper=string(1, (char)dir);
    numT=string(1, (char)nTrama);
    string cad = aux + " " + tOper + " " + getTipoTrama(control) + " " + numT ;
    escribirCadena(cad);
    if (control==2){
       escribirEntero((int)bce);
    }
}


