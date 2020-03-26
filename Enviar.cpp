//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Enviar.h"

Enviar::Enviar(){
    caracter = ' ';
    cont = 0;
    colorEnvio=3+0*16;
    tEnvio=Trama();
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

void Enviar::comprobarTeclaFuncion(char carE, HANDLE &PuertoCOM, HANDLE &Pantalla){
    SetConsoleTextAttribute(Pantalla, colorEnvio);
    switch (carE){
        case 59: //F1
            printf("\n");
            addChar('\n');
            addChar('\0');
            crearTramaDatos(PuertoCOM, Pantalla);
            cont=0;
            break;
        case 60: //F2
            crearTramaControl(PuertoCOM, Pantalla);
            break;
        case 61: //F3
            enviarFichero(PuertoCOM, Pantalla);
            break;
    }
}

void Enviar::enviarCaracter(HANDLE &PuertoCOM, char carE){
    printf("%c", carE);   //Envío
    EnviarCaracter(PuertoCOM,caracter);
}

void Enviar::enviarCadena (char carE, HANDLE &Pantalla){
    SetConsoleTextAttribute(Pantalla, colorEnvio);
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

void Enviar::crearTramaControl(HANDLE &PuertoCOM, HANDLE &Pantalla){
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
        tEnvio=Trama(22, 'T', control, '0', 0, " ", 0);
        enviarTrama(PuertoCOM, Pantalla);
    }
}

void Enviar::crearTramaDatos(HANDLE &PuertoCOM, HANDLE &Pantalla){
    int offset;
    char cadenaEnvio [255];
    int numTramas;
    dividirCadena(cont, numTramas); //calculamos en número de tramas que tenemos que enviar
    for (int i=0; i<numTramas; i++){
        offset = 254 * (i);
        copiarCadena (cadena, cadenaEnvio, offset, 254);
        printf("la cadena a enviar es: %s", cadenaEnvio);
        tEnvio=Trama(22, 'T', 2, '0', strlen(cadenaEnvio), cadenaEnvio, 0);
        tEnvio.setBCE(tEnvio.calcularBce());
        enviarTrama(PuertoCOM, Pantalla);
    }
    printf("MENSAJE ENVIADO.\n");
}

void Enviar::enviarTrama(HANDLE &PuertoCOM, HANDLE &Pantalla){
    EnviarCaracter(PuertoCOM, tEnvio.getSincr());
    EnviarCaracter(PuertoCOM, tEnvio.getDir());
    EnviarCaracter(PuertoCOM, tEnvio.getControl());
    EnviarCaracter(PuertoCOM, tEnvio.getNumTrama());
    if (tEnvio.getControl()==2){ //si es trama de datos
        EnviarCaracter(PuertoCOM, tEnvio.getLong());
        EnviarCadena(PuertoCOM, tEnvio.getDatos(), tEnvio.getLong());
        EnviarCaracter(PuertoCOM, tEnvio.getBCE());
    }
    else{ //si es trama de control
        printf ("Se ha enviado una trama de tipo ");
        tEnvio.imprimirTipoTrama();
    }
    //Recibir para no excluirlo en el envío
    char carR = RecibirCaracter(PuertoCOM);
    recibo->recibir(carR, PuertoCOM, Pantalla);
    SetConsoleTextAttribute(Pantalla, colorEnvio);
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

void Enviar::enviarFichero(HANDLE &PuertoCOM, HANDLE &Pantalla){
    int cont=0;
    ifstream fEnt;
    char autores[50], texto[255];
    fEnt.open(FICHERO);
    if(fEnt.is_open()){
        EnviarCaracter(PuertoCOM, '{'); //caracter que indica que se va a enviar un fichero
        for(int i=0;i<LINCABECERA;i++){
            fEnt.getline(texto, 50);
            if (i==0)
                strcpy(autores, texto);
            tEnvio=Trama(22, 'T', 2, '0', strlen(texto), texto, 0);
            tEnvio.setBCE(tEnvio.calcularBce());
            enviarTrama(PuertoCOM, Pantalla);
        }
        printf("Enviando fichero por %s. \n", autores);
        while(!fEnt.eof()){
            fEnt.read(texto, 254);
            texto[fEnt.gcount()]='\0';
            tEnvio=Trama(22, 'T', 2, '0', strlen(texto), texto, 0);
            tEnvio.setBCE(tEnvio.calcularBce());
            enviarTrama(PuertoCOM, Pantalla);
            cont=cont+tEnvio.getLong();
        }
        fEnt.close();
        EnviarCaracter(PuertoCOM, '}'); //caracter que indica que se ha enviado el fichero completo
        sprintf(texto, "%d", cont);
        tEnvio=Trama(22, 'T', 2, '0', strlen(texto), texto, 0);
        tEnvio.setBCE(tEnvio.calcularBce());
        enviarTrama(PuertoCOM, Pantalla);
        printf("Fichero enviado. \n");
    }
    else
        printf("ERROR: el fichero no existe");
}


