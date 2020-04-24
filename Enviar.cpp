//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Enviar.h"

Enviar* Enviar::obj=0;

Enviar::Enviar(){
    caracter = ' ';
    cont = 0;
    colorEnvio = 3+0*16;
    tEnvio = tEnvio->getInstance();
    recibo = recibo->getInstance();
    fEnvio = fEnvio->getInstance();
}

void Enviar::createInstance (){
    if (obj==0)
        obj = new Enviar();
}

Enviar* Enviar::getInstance (){
    createInstance();
    return obj;
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
        case 63: //F5
            fEnvio->abrirFlujo();
            fEnvio->setEscribir(true);
            break;
        case 64: //F6
            fEnvio->cerrarFlujo();
            bool correcto=false;
            printf("Seleccione maestro o esclavo: \n 1.Maestro\n 2.Esclavo\n");
            char seleccion = getch();
            while (!correcto){
                switch(seleccion){
                case '1': //eleccion de maestro
                    EnviarCaracter(PuertoCOM, 'E'); //se envia a la otra estacion el rol de esclavo
                    fEnvio->iniciarProtMaestro(PuertoCOM, Pantalla);
                    correcto = true;
                    break;
                case '2': //elección de esclavo
                    EnviarCaracter(PuertoCOM, 'M'); //se envia a la otra estacion el rol de maestro
                    fEnvio->iniciarProtEsclavo(PuertoCOM, Pantalla);
                    correcto = true;
                    break;
                case 27: //escape
                    printf("Se ha cancelado el protocolo \n");
                    correcto = true;
                    break;
                default:
                    printf("Ha introducido un valor erroneo, intentelo de nuevo \n");
                    break;
                }
            }
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
    } //Creamos con los valores por defecto y solo añadimos el carácter de control
    if (envio==true){
        //los ultimos 3 campos se inicializan vacios porque es una trama de control
        tEnvio->setAll(22, 'T', control, '0', 0, " ", 0);
        enviarTrama(PuertoCOM, Pantalla);
    }
}

void Enviar::crearTramaDatos(HANDLE &PuertoCOM, HANDLE &Pantalla){
    int offset;
    char cadenaEnvio [255];
    int numTramas;
    fEnvio->dividirCadena(cont, numTramas); //calculamos en número de tramas que tenemos que enviar
    for (int i=0; i<numTramas; i++){
        offset = 254 * (i);
        fEnvio->copiarCadena (cadena, cadenaEnvio, offset, 254);
        tEnvio->setAll(22, 'T', 2, '0', strlen(cadenaEnvio), cadenaEnvio, 0);
        tEnvio->setBCE(tEnvio->calcularBce());
        enviarTrama(PuertoCOM, Pantalla);
    }
    fEnvio->escribirCadena("MENSAJE ENVIADO.\n");
}

void Enviar::enviarTrama(HANDLE &PuertoCOM, HANDLE &Pantalla){
    EnviarCaracter(PuertoCOM, tEnvio->getSincr());
    EnviarCaracter(PuertoCOM, tEnvio->getDir());
    EnviarCaracter(PuertoCOM, tEnvio->getControl());
    EnviarCaracter(PuertoCOM, tEnvio->getNumTrama());
    if (tEnvio->getControl()==2){ //si es trama de datos
        EnviarCaracter(PuertoCOM, tEnvio->getLong());
        EnviarCadena(PuertoCOM, tEnvio->getDatos(), tEnvio->getLong());
        EnviarCaracter(PuertoCOM, tEnvio->getBCE());
    }
    else{ //si es trama de control
        fEnvio->escribirCadena("Se ha enviado una trama de tipo ");
        tEnvio->imprimirTipoTrama();
    }
    //Recibir para no excluirlo en el envío
    char carR = RecibirCaracter(PuertoCOM);
    recibo->recibir(carR, PuertoCOM, Pantalla);
    SetConsoleTextAttribute(Pantalla, colorEnvio);
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
            tEnvio->setAll(22, 'T', 2, '0', strlen(texto), texto, 0);
            tEnvio->setBCE(tEnvio->calcularBce());
            enviarTrama(PuertoCOM, Pantalla);
        }
        if(!teclaESC){
            string cad = "Enviando fichero por " + (string)autores + ". \n";
            fEnvio->escribirCadena(cad);
        }
        while(!fEnt.eof() && !fEnvio->comprobarESC(teclaESC)){
            fEnt.read(texto, 254);
            texto[fEnt.gcount()]='\0';
            tEnvio->setAll(22, 'T', 2, '0', strlen(texto), texto, 0);
            tEnvio->setBCE(tEnvio->calcularBce());
            enviarTrama(PuertoCOM, Pantalla);
            cont=cont+tEnvio->getLong();
        }
        fEnt.close();
        EnviarCaracter(PuertoCOM, '}'); //caracter que indica que se ha enviado el fichero completo
        sprintf(texto, "%d", cont);
        tEnvio->setAll(22, 'T', 2, '0', strlen(texto), texto, 0);
        tEnvio->setBCE(tEnvio->calcularBce());
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

void Enviar::enviarTramaProt(HANDLE &PuertoCOM, Trama *t){
    EnviarCaracter(PuertoCOM, t->getSincr());
    EnviarCaracter(PuertoCOM, t->getDir());
    EnviarCaracter(PuertoCOM, t->getControl());
    EnviarCaracter(PuertoCOM, t->getNumTrama());
    if (t->getControl()==2){ //si es trama de datos
        EnviarCaracter(PuertoCOM, t->getLong());
        EnviarCadena(PuertoCOM, t->getDatos(), t->getLong());
        EnviarCaracter(PuertoCOM, t->getBCE());
    }
}
