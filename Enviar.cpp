//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Enviar.h"

Enviar::Enviar(){
    caracter = ' ';
    cont = 0;
    colorEnvio=3+0*16;
    tEnvio=Trama();
    recibo=recibo->getInstance();
    fEnvio=fEnvio->getInstance();
    pEnvio=pEnvio->getInstance();
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
        case 63: //F4
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
                    pEnvio->iniciarProtMaestro(PuertoCOM, Pantalla);
                    llamaOperacion(PuertoCOM, Pantalla);
                    correcto = true;
                    break;
                case '2': //elección de esclavo
                    EnviarCaracter(PuertoCOM, 'M'); //se envia a la otra estacion el rol de maestro
                    pEnvio->iniciarProtEsclavo(PuertoCOM, Pantalla);
                    llamaOperacion(PuertoCOM, Pantalla);
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
        tEnvio.setAll(22, 'T', control, '0', 0, " ", 0);
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
        tEnvio.setAll(22, 'T', 2, '0', strlen(cadenaEnvio), cadenaEnvio, 0);
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
    if(!esProt){
        if(tEnvio.getControl()!=2){ //si es trama de control
            fEnvio->escribirCadena("Se ha enviado una trama de tipo ");
            tEnvio.imprimirTipoTrama();
        }
        //Recibir para no excluirlo en el envío
        char carR = RecibirCaracter(PuertoCOM);
        recibo->recibir(carR, PuertoCOM, Pantalla);
        SetConsoleTextAttribute(Pantalla, colorEnvio);
    }
    else{
        ///imprimir los datos de la trama por pantalla
    }
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
            tEnvio.setAll(22, 'T', 2, '0', strlen(texto), texto, 0);
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
            tEnvio.setAll(22, 'T', 2, '0', strlen(texto), texto, 0);
            tEnvio.setBCE(tEnvio.calcularBce());
            enviarTrama(PuertoCOM, Pantalla);
            cont=cont+tEnvio.getLong();
        }
        fEnt.close();
        EnviarCaracter(PuertoCOM, '}'); //caracter que indica que se ha enviado el fichero completo
        sprintf(texto, "%d", cont);
        tEnvio.setAll(22, 'T', 2, '0', strlen(texto), texto, 0);
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

///********************************************************************************************************************************
///********************************************************************************************************************************
///***************************************    PROTOCOLO MAESTRO/ESCLAVO    ********************************************************
///********************************************************************************************************************************
///********************************************************************************************************************************

void Enviar::llamaOperacion(HANDLE &PuertoCOM, HANDLE &Pantalla){
    if (pEnvio->getTipoOper()=='T')
        sondeo(PuertoCOM, Pantalla);
    else
        seleccion(PuertoCOM, Pantalla);
}

void Enviar::sondeo(HANDLE &PuertoCOM, HANDLE &Pantalla){
    printf("Estas en la operacion de sondeo y eres");
    if(pEnvio->getEsMaestro()){
        printf(" maestro\n");
        pEnvio->reiniciarNumTrama();
        ///empieza fase establecimiento
        faseEstablecimiento(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase transferencia
        faseTransferenciaRecibo(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase cierre
        aceptarCierreComunicacion(PuertoCOM, Pantalla); ///pregunta por pantalla si se desea cerrar o no la comunicacion
    }
    else{
        printf(" esclavo\n");
        pEnvio->reiniciarNumTrama();
        ///empieza fase establecimiento
        faseEstablecimiento(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase transferencia
        faseTransferenciaEnvio(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase cierre
        while (!pEnvio->getCerrar()){
            faseCierre(PuertoCOM, Pantalla);
        }
    }
}

void Enviar::seleccion(HANDLE &PuertoCOM, HANDLE &Pantalla){
    printf("Estas en la operacion de seleccion y eres");
    if(pEnvio->getEsMaestro()){
        printf(" maestro\n");
        pEnvio->reiniciarNumTrama();
        ///empieza fase establecimiento
        faseEstablecimiento(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase transferencia
        faseTransferenciaEnvio(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase cierre
        faseCierre(PuertoCOM, Pantalla); ///solicita cierre de la comunicacion y recibe la respuesta
    }
    else{
        printf(" esclavo\n");
        pEnvio->reiniciarNumTrama();
        ///empieza fase establecimiento
        faseEstablecimiento(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase transferencia
        faseTransferenciaRecibo(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase cierre
        esperarRespuesta(04, PuertoCOM, Pantalla); ///esperar trama EOT
        imprimirTrama(Pantalla);
        enviarTramaAceptacion(PuertoCOM, Pantalla); ///envia una trama ACK para cerrar la comunicacion
        imprimirTrama(Pantalla);
    }
}

void Enviar::faseEstablecimiento(HANDLE &PuertoCOM, HANDLE &Pantalla){
    if(pEnvio->getEsMaestro()){
        ///poner color establecimiento: azul
        crearTramaProt(05, PuertoCOM, Pantalla); ///enviamos trama ENQ
        imprimirTrama(Pantalla);
        pEnvio->cambiarNumTrama();
        esperarTramaAceptacion(PuertoCOM, Pantalla);
        imprimirTrama(Pantalla);
    }
    else{
        ///poner color establecimiento: azul
        esperarRespuesta(05, PuertoCOM, Pantalla); ///Esperamos una trama ENQ
        imprimirTrama(Pantalla);
        enviarTramaAceptacion(PuertoCOM, Pantalla);
        imprimirTrama(Pantalla);
        pEnvio->cambiarNumTrama();
    }
}

void Enviar::faseTransferenciaEnvio(HANDLE &PuertoCOM, HANDLE &Pantalla){
//    while(!finFichero){
        ///troceamos fichero: metodo en enviar que cree una trama con los parametros
        ///dados y la envie e imprima
        imprimirTrama(Pantalla);
        pEnvio->cambiarNumTrama();
        esperarTramaAceptacion(PuertoCOM, Pantalla);
        imprimirTrama(Pantalla);
//    }
}

void Enviar::faseTransferenciaRecibo(HANDLE &PuertoCOM, HANDLE &Pantalla){
    while(true){
        recibirTramaDatos(PuertoCOM, Pantalla);
        imprimirTrama(Pantalla);
        enviarTramaAceptacion(PuertoCOM, Pantalla);
        imprimirTrama(Pantalla);
        pEnvio->cambiarNumTrama();
    }
}

void Enviar::faseCierre(HANDLE &PuertoCOM, HANDLE &Pantalla){
    crearTramaProt(04, PuertoCOM, Pantalla);///enviar trama EOT
    imprimirTrama(Pantalla);
    pEnvio->cambiarNumTrama();
    esperarRespuesta(06, PuertoCOM, Pantalla);
    pEnvio->setCerrar(pEnvio->getTCorrecta());
    imprimirTrama(Pantalla);
}

void Enviar::aceptarCierreComunicacion(HANDLE &PuertoCOM, HANDLE &Pantalla){
    while (!pEnvio->getCerrar()){
        esperarRespuesta(04, PuertoCOM, Pantalla); ///esperar trama EOT
        imprimirTrama(Pantalla);
        printf("El esclavo ha solicitado cerrar la comunicacion, ¿quiere cerrarla?\n 1. Si\n 2. No\n");
        char carR = getch();
        bool correcto=false;
        while(!correcto){
            if(carR=='1'){
                enviarTramaAceptacion(PuertoCOM, Pantalla);
                imprimirTrama(Pantalla);
                correcto = true;
                pEnvio->setCerrar(true);
            }
            else{
                if (carR=='2'){
                    pEnvio->setTCorrecta(false);
                    enviarTramaAceptacion(PuertoCOM, Pantalla);
                    imprimirTrama(Pantalla);
                    correcto=true;
                }
                else
                    printf("Introduzca un valor correcto\n");
            }
        }
        pEnvio->cambiarNumTrama();
    }
}

void Enviar::crearTramaProt(unsigned char control, HANDLE &PuertoCOM, HANDLE &Pantalla){
    tEnvio.setAll(22, pEnvio->getTipoOper(), control, pEnvio->getNumTrama(), 0, "", 0);
    enviarTrama(PuertoCOM, Pantalla); ///cambiar la exclusion
}

void Enviar::esperarRespuesta (unsigned char control, HANDLE &PuertoCOM, HANDLE &Pantalla){
    unsigned char vControl;
    pEnvio->setTCorrecta(false);
    char carR;
    do {
        carR = RecibirCaracter(PuertoCOM);
    } while((vControl = recibo->recibir(carR, PuertoCOM, Pantalla))== 0);
    if(vControl==control)
        pEnvio->setTCorrecta(true);
}

void Enviar::enviarTramaAceptacion(HANDLE &PuertoCOM, HANDLE &Pantalla){
    int vControl;
    if(pEnvio->getTCorrecta())
        vControl=06;
    else
        vControl=21;
    tEnvio.setAll(22, pEnvio->getTipoOper(), vControl, pEnvio->getNumTrama(), 0, "", 0);
    enviarTrama(PuertoCOM, Pantalla); ///cambiar la exclusion
}

void Enviar::esperarTramaAceptacion(HANDLE &PuertoCOM, HANDLE &Pantalla){
    unsigned char vControl;
    pEnvio->setTCorrecta(false);
    char carR;
    do{
        carR = RecibirCaracter(PuertoCOM);
    } while((vControl = recibo->recibir(carR, PuertoCOM, Pantalla))==0); ///espera nack o ack
    if (vControl == 06 || vControl == 21)
        pEnvio->setTCorrecta(true);
}

void Enviar::recibirTramaDatos(HANDLE &PuertoCOM, HANDLE &Pantalla){
    unsigned char vControl;
    pEnvio->setTCorrecta(false);
    char carR;
    do {
        carR = RecibirCaracter(PuertoCOM);
    } while((vControl = recibo->recibir(carR, PuertoCOM, Pantalla)) == 0);
    if(vControl==2)
        pEnvio->setTCorrecta(true);
}
