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
    if(fSal!=NULL){
        fclose(fSal);
        escribir = false;
    }
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

void Funciones::imprimirTrama(HANDLE &Pantalla, Trama aux, int campoProt, unsigned char RE, int cont){ ///RE: R si es Recibo o E si es Envio
    int color;
    switch(campoProt){
    case 1:
        color=colorEstablecimiento;
        break;
    case 2:
        color=colorCabecera;
        break;
    case 3:
        color=colorCuerpo;
        break;
    case 4:
        color=colorTamanio;
        break;
    case 5:
        color=colorCierre;
        break;
    }
    /*string tipoTrama;
    switch(aux.getControl()){
    case 02:
        tipoTrama="STX";
        break;
    case 04:
        tipoTrama="EOT";
        break;
    case 05:
        tipoTrama="ENQ";
        break;
    case 06:
        tipoTrama="ACK";
        break;
    case 21:
        tipoTrama="NACK";
        break;
    }
    SetConsoleTextAttribute(Pantalla, color);
    printf("\n %c %c %s %d", RE, aux.getDir(), tipoTrama, cont);
    if(aux.getControl()==2)
        printf("%d", (int)aux.getBCE());*/
}
