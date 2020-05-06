//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Enviar.h"

Enviar::Enviar(){
    caracter = ' ';
    cont = 0;
    colorEnvio=3+0*16;
    errorProt=false;
    contError=0;
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
    bool correcto;
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
            fEnvio->escribirCadena("Se ha pulsado la tecla F5\n");
            break;
        case 64: //F6
            fEnvio->cerrarFlujo(); //cerramos el fichero log de F5
            pEnvio->setEsProt(true);
            pEnvio->setCerrar(false);
            errorFichero = false;
            correcto=false;
            pEnvio->setFinTransferencia(false);
            SetConsoleTextAttribute(Pantalla, 5);
            printf("Seleccione maestro o esclavo: \n 1.Maestro\n 2.Esclavo\n");
            pEnvio->setEstacionQueInicia(true);
            while (!correcto){
                char seleccion = getch();
                switch(seleccion){
                case '1': //eleccion de maestro
                    EnviarCaracter(PuertoCOM, 'E'); //se envia a la otra estacion el rol de esclavo
                    SetConsoleTextAttribute(Pantalla, 14);
                    pEnvio->iniciarProtMaestro(PuertoCOM, Pantalla);
                    llamaOperacion(PuertoCOM, Pantalla);
                    correcto = true;
                    break;
                case '2': //elección de esclavo
                    EnviarCaracter(PuertoCOM, 'M'); //se envia a la otra estacion el rol de maestro
                    SetConsoleTextAttribute(Pantalla, 14);
                    pEnvio->iniciarProtEsclavo(PuertoCOM, Pantalla);
                    llamaOperacion(PuertoCOM, Pantalla);
                    correcto = true;
                    break;
                case 27: //escape
                    pEnvio->escribirCadena("Se ha cancelado el protocolo \n");
                    correcto = true;
                    break;
                default:
                    pEnvio->escribirCadena("Ha introducido un valor erroneo, intentelo de nuevo \n");
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
    if(!pEnvio->getEsProt()){
        if(tEnvio.getControl()!=2){ //si es trama de control
            fEnvio->escribirCadena("Se ha enviado una trama de tipo ");
            tEnvio.imprimirTipoTrama();
        }
        //Recibir para no excluirlo en el envío
        char carR = RecibirCaracter(PuertoCOM);
        recibo->recibir(carR, PuertoCOM, Pantalla);
        SetConsoleTextAttribute(Pantalla, colorEnvio);
    }
}


void Enviar::enviarFichero(HANDLE &PuertoCOM, HANDLE &Pantalla){
    int cont=0;
    bool teclaESC = false;
    ifstream fEnt;
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
            if(!fEnt.eof()){
                texto[fEnt.gcount()]='\0';
                tEnvio.setAll(22, 'T', 2, '0', strlen(texto), texto, 0);
                tEnvio.setBCE(tEnvio.calcularBce());
                enviarTrama(PuertoCOM, Pantalla);
                cont=cont+tEnvio.getLong();
            }
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
    if(pEnvio->getEsMaestro()){
        pEnvio->reiniciarNumTrama();
        ///empieza fase establecimiento
        pEnvio->setColor(CESTABLECIMIENTO);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        faseEstablecimiento(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase transferencia
        pEnvio->setColor(CTRANSFERENCIA1);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        faseTransferenciaRecibo(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase cierre
        pEnvio->setColor(CCIERRE);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        aceptarCierreComunicacion(PuertoCOM, Pantalla); ///pregunta por pantalla si se desea cerrar o no la comunicacion
    }
    else{
        pEnvio->reiniciarNumTrama();
        ///empieza fase establecimiento
        pEnvio->setColor(CESTABLECIMIENTO);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        faseEstablecimiento(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase transferencia
        pEnvio->setColor(CTRANSFERENCIA1);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        faseTransferenciaEnvio(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase cierre
        pEnvio->setColor(CCIERRE);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        while (!pEnvio->getCerrar()){
            faseCierre(PuertoCOM, Pantalla);
        }
    }
    pEnvio->escribirCadena("FIN PROTOCOLO\n\n");
    pEnvio->cerrarFichero();
    pEnvio->setEsProt(false);
}

void Enviar::seleccion(HANDLE &PuertoCOM, HANDLE &Pantalla){
    if(pEnvio->getEsMaestro()){
        pEnvio->reiniciarNumTrama();
        ///empieza fase establecimiento
        pEnvio->setColor(CESTABLECIMIENTO);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        faseEstablecimiento(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase transferencia
        pEnvio->setColor(CTRANSFERENCIA1);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        faseTransferenciaEnvio(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase cierre
        pEnvio->setColor(CCIERRE);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        faseCierre(PuertoCOM, Pantalla); ///solicita cierre de la comunicacion y recibe la respuesta
    }
    else{
        pEnvio->reiniciarNumTrama();
        ///empieza fase establecimiento
        pEnvio->setColor(CESTABLECIMIENTO);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        faseEstablecimiento(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase transferencia
        pEnvio->setColor(CTRANSFERENCIA1);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        faseTransferenciaRecibo(PuertoCOM, Pantalla);
        pEnvio->reiniciarNumTrama();
        ///empieza fase cierre
        pEnvio->setColor(CCIERRE);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        if (!errorFichero)
            esperarRespuesta(04, PuertoCOM, Pantalla); ///esperar trama EOT
        enviarTramaAceptacion(PuertoCOM, Pantalla); ///envia una trama ACK para cerrar la comunicacion
        imprimirTrama();
    }
    pEnvio->escribirCadena("FIN PROTOCOLO\n\n");
    pEnvio->cerrarFichero();
    pEnvio->setEsProt(false);
}

void Enviar::faseEstablecimiento(HANDLE &PuertoCOM, HANDLE &Pantalla){
    if(pEnvio->getEsMaestro()){
        crearTramaProt(05, PuertoCOM, Pantalla); ///enviamos trama ENQ
        imprimirTrama();
        pEnvio->cambiarNumTrama();
        esperarTramaAceptacion(PuertoCOM, Pantalla);
    }
    else{
        ///poner color establecimiento: azul
        esperarRespuesta(05, PuertoCOM, Pantalla); ///Esperamos una trama ENQ
        enviarTramaAceptacion(PuertoCOM, Pantalla);
        imprimirTrama();
        pEnvio->cambiarNumTrama();
    }
}

void Enviar::faseTransferenciaEnvio(HANDLE &PuertoCOM, HANDLE &Pantalla){
    ifstream fEnt;
    int linFichero=1, i=0, cont=0;
    fEnt.open(FICHEROLECTURA);
    bool teclaESC = false;
    EnviarCaracter(PuertoCOM, '{'); //caracter que indica que se empieza a enviar el fichero
    if(fEnt.is_open()){
        while(!pEnvio->getFinTransferencia()&&!fEnvio->comprobarESC(teclaESC)){
        trocearFicheroProt(fEnt, i, linFichero, cont, PuertoCOM, Pantalla);
            imprimirTrama();
            pEnvio->cambiarNumTrama();
            esperarTramaAceptacion(PuertoCOM, Pantalla);
            if(i==LINCABECERA){
                string cad = "\nEnviando fichero por " + (string)autores + ".\n";
                pEnvio->escribirCadena(cad);
                i++;
            }
        }
        pEnvio->setFinTransferencia(true);
        if (!teclaESC)
            fEnvio->escribirCadena("\nFichero enviado.");
        else
            fEnvio->escribirCadena("\nSe ha cancelado el envio del fichero.\n");
        fEnt.close();
    }
    else{
        pEnvio->setFinTransferencia(true);
        if(!teclaESC)
            fEnvio->escribirCadena("\nERROR: el fichero no existe.\n");
        else
            fEnvio->escribirCadena("\nSe ha cancelado el envio del fichero.\n");
    }
}

void Enviar::faseTransferenciaRecibo(HANDLE &PuertoCOM, HANDLE &Pantalla){
    int cont=0;
    while(!pEnvio->getFinTransferencia()){
        if(cont>=LINCABECERA)
            pEnvio->setColor(CTRANSFERENCIA2);
        recibirTramaDatos(PuertoCOM, Pantalla);
        if (!errorFichero){
            enviarTramaAceptacion(PuertoCOM, Pantalla);
            imprimirTrama();
            pEnvio->cambiarNumTrama();
            cont++;
        }
    }
}

void Enviar::faseCierre(HANDLE &PuertoCOM, HANDLE &Pantalla){
    crearTramaProt(04, PuertoCOM, Pantalla);///enviar trama EOT
    imprimirTrama();
    pEnvio->cambiarNumTrama();
    esperarRespuesta(06, PuertoCOM, Pantalla);
    pEnvio->setCerrar(pEnvio->getTCorrecta());
}

void Enviar::aceptarCierreComunicacion(HANDLE &PuertoCOM, HANDLE &Pantalla){
    while (!pEnvio->getCerrar()){
        if (!errorFichero)
            esperarRespuesta(04, PuertoCOM, Pantalla); ///esperar trama EOT
        else
            errorFichero = false;
        pEnvio->escribirCadena("\nEl esclavo ha solicitado cerrar la comunicacion, quiere cerrarla?\n 1. Si\n 2. No");
        char carR = getch();
        bool correcto=false;
        while(!correcto){
            if(carR=='1'){
                enviarTramaAceptacion(PuertoCOM, Pantalla);
                imprimirTrama();
                correcto = true;
                pEnvio->setCerrar(true);
            }
            else{
                if (carR=='2'){
                    pEnvio->setTCorrecta(false);
                    enviarTramaAceptacion(PuertoCOM, Pantalla);
                    imprimirTrama();
                    correcto=true;
                }
                else
                    pEnvio->escribirCadena("\nIntroduzca un valor correcto\n");
            }
        }
        pEnvio->cambiarNumTrama();
    }
}

void Enviar::crearTramaProt(unsigned char control, HANDLE &PuertoCOM, HANDLE &Pantalla){
    tEnvio.setAll(22, pEnvio->getTipoOper(), control, pEnvio->getNumTrama(), 0, "", 0);
    enviarTrama(PuertoCOM, Pantalla);
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
    else if (vControl==4){
        errorFichero = true;
        pEnvio->setFinTransferencia(true);
        pEnvio->setTCorrecta(true);
    }
}

void Enviar::trocearFicheroProt(ifstream &fEnt, int &i, int &linFichero, int &cont, HANDLE &PuertoCOM, HANDLE &Pantalla){ ///la i es un iterador
     if (kbhit()){
        char car = getch();
        if(car == '\0'){
            car = getch();
            if(car == 65 && pEnvio->getEstacionQueEnvia()){
                printf("\nSe ha pulsado f7");
                errorProt = true;
                contError++;
            }
        }
     }
    switch (linFichero){
    case 1:
        if(i<LINCABECERA){
            fEnt.getline(texto, 50);
            if (i==0)
                strcpy(autores, texto);
            i++;
        }
        if(i==LINCABECERA)
            linFichero++;
        break;
    case 2:
        pEnvio->setColor(CTRANSFERENCIA2);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        if(!fEnt.eof()){
            fEnt.read(texto, 254);
            if(!fEnt.eof()){
                texto[fEnt.gcount()]='\0';
                cont=cont+strlen(texto);
            }
            else
                linFichero++;
        }
        else
            linFichero++;
        break;
    case 3:
        pEnvio->setColor(CTRANSFERENCIA3);
        SetConsoleTextAttribute(Pantalla, pEnvio->getColor());
        EnviarCaracter(PuertoCOM, '}'); //caracter que indica que se ha enviado el fichero completo
        pEnvio->setFinTransferencia(true);
        sprintf(texto, "%d", cont);

        break;
    }
    tEnvio.setAll(22, pEnvio->getTipoOper(), 2, pEnvio->getNumTrama(), strlen(texto), texto, 0);
    tEnvio.setBCE(tEnvio.calcularBce());
    if(errorProt){
        printf("Se ha enviado un error");
        texto[0]='ç';
        contError--;
        if (contError==0)
            errorProt = false;
    }
    enviarTrama(PuertoCOM, Pantalla);

}

void Enviar::imprimirTrama(){
    unsigned char control, bce;
    if((control=tEnvio.getControl())!=2)
        bce = 0;
    else
        bce=tEnvio.getBCE();
    pEnvio->imprimirTrama(control, bce, tEnvio.getDir(), tEnvio.getNumTrama(), "E");
}
