//============================================================================
// ------------ Gloria M�ndez S�nchez y Gloria D�az Gonz�lez------------------
//============================================================================

#include "Enviar.h"

Enviar::Enviar(){
    caracter = ' ';
    cont = 0;
    colorEnvio=3+0*16;
    tEnvio=Trama();
    recibo=recibo->getInstance();
    fEnvio=fEnvio->getInstance();
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
            fEnvio->escribirCaracter('\n');
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
        case 63:
            fEnvio->abrirFlujo();
            fEnvio->setEscribir(true);
            break;
    }
}

void Enviar::enviarCadena (char carE, HANDLE &Pantalla){
    SetConsoleTextAttribute(Pantalla, colorEnvio);
    switch (carE){
    case 8: //Borrar
        if(cadena[cont-1]!='\n'){
            fEnvio->escribirCaracter(carE);
            fEnvio->escribirCaracter(' ');
            fEnvio->escribirCaracter(carE);
            cont--;
        }
        break;
    case 13: //Enter
        if(cont < MAX){
            fEnvio->escribirCaracter('\n');
            cadena[cont] = '\n';
            cont++;
        }
        break;
    default: //Cualquier otra tecla
        if(cont < MAX){
            fEnvio->escribirCaracter(carE);
            cadena[cont]=carE;
            cont++;
        }
        break;
    }
}

void Enviar::crearTramaControl(HANDLE &PuertoCOM, HANDLE &Pantalla){
    fEnvio->escribirCadena("\nElija el tipo de trama que quiere enviar:\n 1: Trama ENQ \n 2: Trama EOT \n 3: Trama ACK \n 4: Trama NACK \n");
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
                fEnvio->escribirCadena("Se ha cancelado el envio de la trama \n");
                valorCorrecto = true;
                envio = false;
                break;
            default:
                fEnvio->escribirCadena("Ha introducido un valor erroneo, intentelo de nuevo \n");
                break;
        }
    } //Creamos con los valores por defecto y solo a�adimos el car�cter de control
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
    dividirCadena(cont, numTramas); //calculamos en n�mero de tramas que tenemos que enviar
    for (int i=0; i<numTramas; i++){
        offset = 254 * (i);
        copiarCadena (cadena, cadenaEnvio, offset, 254);
        tEnvio=Trama(22, 'T', 2, '0', strlen(cadenaEnvio), cadenaEnvio, 0);
        tEnvio.setBCE(tEnvio.calcularBce());
        enviarTrama(PuertoCOM, Pantalla);
    }
    fEnvio->escribirCadena("MENSAJE ENVIADO.\n");
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
        fEnvio->escribirCadena("Se ha enviado una trama de tipo ");
        tEnvio.imprimirTipoTrama();
    }
    //Recibir para no excluirlo en el env�o
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
    bool teclaESC = false;
    ifstream fEnt;
    char autores[50], texto[255];
    fEnt.open(FICHERO);
    if(!fEnvio->comprobarESC(teclaESC) && fEnt.is_open()){
        EnviarCaracter(PuertoCOM, '{'); //caracter que indica que se va a enviar un fichero
        for(int i=0;i<LINCABECERA && !fEnvio->comprobarESC(teclaESC);i++){
            fEnt.getline(texto, 50);
            if (i==0)
                strcpy(autores, texto);
            tEnvio=Trama(22, 'T', 2, '0', strlen(texto), texto, 0);
            tEnvio.setBCE(tEnvio.calcularBce());
            enviarTrama(PuertoCOM, Pantalla);
        }
        if(!teclaESC){
            string cad = "Enviando fichero por " + (string)autores + ". \n";
            fEnvio->escribirCadena(cad);
        }
        while(!fEnt.eof() && !fEnvio->comprobarESC(teclaESC)){
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
        if (!teclaESC)
            fEnvio->escribirCadena("Fichero enviado. \n");
        else
            fEnvio->escribirCadena("Se ha cancelado el envio del fichero.\n");
    }
    else{
        if(!teclaESC)
            fEnvio->escribirCadena("ERROR: el fichero no existe.\n");
        else
            fEnvio->escribirCadena("Se ha cancelado el envio del fichero.\n");
    }
}


