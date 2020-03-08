//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Enviar.h"

Enviar::Enviar(){
    caracter = ' ';
    cont = 0;
}

Enviar::~Enviar(){
}

int Enviar::getCont(){
    return cont;
}

void Enviar::setCont(int valor){
    cont=valor;
}

void Enviar::addChar(char carE){
    cadena[cont]=carE;
    cont++;
}

void Enviar::comprobarTeclaFuncion(char carE, HANDLE PuertoCOM){
    switch (carE){
        case 59: //F1
            printf("\n");
            addChar('\n');
            addChar('\0');
            crearTramaDatos(PuertoCOM);
            cont=0;
            break;
        case 60: //F2
            crearTramaControl(PuertoCOM);
    }
}

void Enviar::enviarCaracter(HANDLE PuertoCOM, char carE){
    printf("%c", carE);   //Envío
    EnviarCaracter(PuertoCOM,caracter);
}

void Enviar::enviarCadena (char carE){
    switch (carE){
    case 8: //Borrar
        if(cadena[cont-1]!='\n'){
            printf("%c", carE);
            printf(" ");
            printf("%c", carE);
            cont--;
        }
        break;
    case 13: //Enter
        if(cont < MAX){
            printf("\n");
            cadena[cont] = '\n';
            cont++;
        }
        break;
    default: //Cualquier otra tecla
        if(cont < MAX){
            printf("%c", carE);
            cadena[cont]=carE;
            cont++;
        }
        break;
    }
}

void Enviar::crearTramaControl(HANDLE PuertoCOM){
    printf ("\nElija el tipo de trama que quiere enviar \n 1: Trama ENQ \n 2: Trama EOT \n 3: Trama ACK \n 4: Trama NACK \n");
    int opcionT;
    unsigned char control;
    bool valorCorrecto=false;
    bool envio=true;
    while (!valorCorrecto){
        opcionT = getch();
        switch (opcionT){
            case '1': //Tipo ENQ
                control = 05;
                valorCorrecto = true;
                break;
            case '2': //Tipo EOT
                control = 04;
                valorCorrecto = true;
                break;
            case '3': //Tipo ACK
                control = 06;
                valorCorrecto = true;
                break;
            case '4': //Tipo NACK
                control = 21;
                valorCorrecto = true;
                break;
            case 27: //escape
                printf ("Se ha cancelado el envio de la trama \n");
                valorCorrecto = true;
                envio = false;
                break;
            default:
                 printf("Ha introducido un valor erroneo, intentelo de nuevo \n");
                break;
        }
    } //Creamos con los valores por defecto y solo añadimos el carácter de control
    if (envio==true){
        //los ultimos 3 campos se inicializan vacios porque es una trama de control
        Trama t=Trama(22, 'T', control, '0', 0, " ", 0);
        enviarTrama(t, PuertoCOM);
    }
}

void Enviar::crearTramaDatos(HANDLE PuertoCOM){
    Trama t, aux;
    int offset;
    char cadenaEnvio [255], carR;
    int numTramas, campoT=1;
    dividirCadena(cont, numTramas); //calculamos en número de tramas que tenemos que enviar
    for (int i=0; i<numTramas; i++){
        offset = 254 * (i);
        copiarCadena (cadena, cadenaEnvio, offset, 254);
        t=Trama(22, 'T', 2, '0', strlen(cadenaEnvio), cadenaEnvio, 0);
        t.setBCE(t.calcularBce());
        enviarTrama(t, PuertoCOM);
        //Recibir para no excluirlo en el envío
        carR = RecibirCaracter(PuertoCOM);
        recibo.recibir(carR, campoT, aux, PuertoCOM);
    }
    printf("MENSAJE ENVIADO.\n");
}

void Enviar::enviarTrama(Trama t, HANDLE PuertoCOM){
    EnviarCaracter(PuertoCOM, t.getSincr());
    EnviarCaracter(PuertoCOM, t.getDir());
    EnviarCaracter(PuertoCOM, t.getControl());
    EnviarCaracter(PuertoCOM, t.getNumTrama());
    if (t.getControl()==2){ //si es trama de datos
        EnviarCaracter(PuertoCOM, t.getLong());
        EnviarCadena(PuertoCOM, t.getDatos(), t.getLong());
        EnviarCaracter(PuertoCOM, t.getBCE());
    }
    else{ //si es trama de control
        printf ("Se ha enviado una trama de tipo ");
        t.imprimirTipoTrama();
    }
}

void Enviar::dividirCadena(int cont, int &numTramas){
    if (cont%254==0)
        numTramas=cont/254;
    else
        numTramas=(cont/254)+1;
}

void Enviar::copiarCadena (const char* cadenaFuente, char* cadenaDestino, int offset, int longit){
    int i = 0;
    char actual;
    do{
        actual = cadenaFuente [i+offset];
        cadenaDestino[i] = actual;
        i++;
    } while ((actual!='\0')&&(i<longit));
    cadenaDestino[i]='\0';
}
