//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Protocolo.h"

Protocolo::Protocolo(){
    cerrar = false;
    envio = envio->getInstance();
    recibo = recibo->getInstance();
    tProt = tProt->getInstance();
    this->PuertoCOM = PuertoCOM;
    this->Pantalla = Pantalla;
}

void Protocolo::faseTransferenciaEnvio(HANDLE &PuertoCOM, HANDLE &Pantalla){
//    while(!finFichero){
        ///troceamos fichero: metodo en enviar que cree una trama con los parametros
        ///dados y la envie e imprima
        cambiarNumTrama();
        esperarTramaAceptacion(PuertoCOM, Pantalla);
        imprimirTrama(Pantalla);
//    }
}

void Protocolo::faseTransferenciaRecibo(HANDLE &PuertoCOM, HANDLE &Pantalla){
    while(true){
        recibirTramaDatos(PuertoCOM, Pantalla);
        imprimirTrama(Pantalla);
        enviarTramaAceptacion(PuertoCOM, Pantalla);
        cambiarNumTrama();
        imprimirTrama(Pantalla);
    }
}

void Protocolo::faseCierre(HANDLE &PuertoCOM, HANDLE &Pantalla){
    enviarTrama(04, PuertoCOM, Pantalla);///enviar trama EOT
    cambiarNumTrama();
    imprimirTrama(Pantalla);
    esperarRespuesta(06, PuertoCOM, Pantalla);
    cerrar = tCorrecta;
    imprimirTrama(Pantalla);
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

void Protocolo::enviarTrama(int control, HANDLE &PuertoCOM, HANDLE &Pantalla){
    tProt->setAll(22, tipoOper, control, numTrama, 0, "", 0);
    envio->enviarTramaProt(PuertoCOM, tProt);
}

void Protocolo::esperarRespuesta(int control, HANDLE &PuertoCOM, HANDLE &Pantalla){
    int vControl;
    tCorrecta = false;
    char carR;
    do {
        carR = RecibirCaracter(PuertoCOM);
    } while((vControl = recibo->recibir(carR, PuertoCOM, Pantalla))!= 0);
    if(vControl==control)
        tCorrecta = true;
}

void Protocolo::enviarTramaAceptacion(HANDLE &PuertoCOM, HANDLE &Pantalla){
    int vControl;
    if(tCorrecta)
        vControl=06;
    else
        vControl=21;
    tProt->setAll(22, tipoOper, vControl, numTrama, 0, "", 0);
    envio->enviarTramaProt(PuertoCOM, tProt);
}

void Protocolo::esperarTramaAceptacion(HANDLE &PuertoCOM, HANDLE &Pantalla){
    int vControl;
    tCorrecta = false;
    char carR;
    do{
        carR = RecibirCaracter(PuertoCOM);
    } while((vControl = recibo->recibir(carR, PuertoCOM, Pantalla))!=0); ///espera nack o ack
    if (vControl == (06||21))
        tCorrecta = true;
}

void Protocolo::recibirTramaDatos(HANDLE &PuertoCOM, HANDLE &Pantalla){

}

Protocolo::~Protocolo(){
    if (ficheroProt!=NULL)
        fclose(ficheroProt);
}

///MÉTODOS DEL MAESTRO

ProtMaestro::ProtMaestro(){
    Protocolo();
}

void ProtMaestro::iniciarProtocolo(HANDLE &PuertoCOM, HANDLE &Pantalla){
    printf("Has seleccionado MAESTRO, seleccione la operacion a realizar: \n 1.Seleccion\n 2.Sondeo\n");
    char sel = getch();
    bool correcto = false;
    while (!correcto){///seleccion
        switch(sel){
        case '1':
            printf("Ha elegido seleccion \n");
            seleccion(PuertoCOM, Pantalla);
            tipoOper='R';
            EnviarCaracter(PuertoCOM, tipoOper);
            correcto=true;
            break;
        case '2':///sondeo
            printf("Ha elegido sondeo \n");
            sondeo(PuertoCOM, Pantalla);
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

void ProtMaestro::seleccion(HANDLE &PuertoCOM, HANDLE &Pantalla){ ///SELECCION MAESTRO
    reiniciarNumTrama();
    ///empieza fase establecimiento
    faseEstablecimiento(PuertoCOM, Pantalla);
    reiniciarNumTrama();
    ///empieza fase transferencia
    faseTransferenciaEnvio(PuertoCOM, Pantalla);
    reiniciarNumTrama();
    ///empieza fase cierre
    faseCierre(PuertoCOM, Pantalla); ///solicita cierre de la comunicacion y recibe la respuesta
}

void ProtMaestro::sondeo(HANDLE &PuertoCOM, HANDLE &Pantalla){ ///SONDEO MAESTRO
    reiniciarNumTrama();
    ///empieza fase establecimiento
    faseEstablecimiento(PuertoCOM, Pantalla);
    reiniciarNumTrama();
    ///empieza fase transferencia
    faseTransferenciaRecibo(PuertoCOM, Pantalla);
    reiniciarNumTrama();
    ///empieza fase cierre
    aceptarCierreComunicacion(PuertoCOM, Pantalla); ///pregunta por pantalla si se desea cerrar o no la comunicacion
}

void ProtMaestro::faseEstablecimiento(HANDLE &PuertoCOM, HANDLE &Pantalla){
    ///poner color establecimiento: azul
    enviarTrama(05, PuertoCOM, Pantalla); ///enviamos trama ENQ
    cambiarNumTrama();
    imprimirTrama(Pantalla);
    esperarTramaAceptacion(PuertoCOM, Pantalla);
    imprimirTrama(Pantalla);
}

void ProtMaestro::aceptarCierreComunicacion(HANDLE &PuertoCOM, HANDLE &Pantalla){
    while (!cerrar){
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
                cerrar = true;
            }
            else{
                if (carR=='2'){
                    tCorrecta=false;
                    enviarTramaAceptacion(PuertoCOM, Pantalla);
                    imprimirTrama(Pantalla);
                    correcto=true;
                }
                else
                    printf("Introduzca un valor correcto\n");
            }
        }
        numTrama++;
    }
}

///METODOS DEL ESCLAVO

ProtEsclavo::ProtEsclavo(){
    Protocolo();
}

void ProtEsclavo::iniciarProtocolo (HANDLE &PuertoCOM, HANDLE &Pantalla){
    printf("Has seleccionado ESCLAVO\n");
    char carR = RecibirCaracter(PuertoCOM);
    tipoOper=carR;
    if(carR=='R')
        seleccion(PuertoCOM, Pantalla);
    else
        sondeo(PuertoCOM, Pantalla);
}

void ProtEsclavo::seleccion(HANDLE &PuertoCOM, HANDLE &Pantalla){ ///SELECCION ESCLAVO
    reiniciarNumTrama();
    ///empieza fase establecimiento
    faseEstablecimiento(PuertoCOM, Pantalla);
    reiniciarNumTrama();
    ///empieza fase transferencia
    faseTransferenciaRecibo(PuertoCOM, Pantalla);
    reiniciarNumTrama();
    ///empieza fase cierre
    esperarRespuesta(04, PuertoCOM, Pantalla); ///esperar trama EOT
    imprimirTrama(Pantalla);
    enviarTramaAceptacion(PuertoCOM, Pantalla); ///envia una trama ACK para cerrar la comunicacion
    imprimirTrama(Pantalla);
}

void ProtEsclavo::sondeo (HANDLE &PuertoCOM, HANDLE &Pantalla){ ///SONDEO ESCLAVO
    reiniciarNumTrama();
    ///empieza fase establecimiento
    faseEstablecimiento(PuertoCOM, Pantalla);
    reiniciarNumTrama();
    ///empieza fase transferencia
    faseTransferenciaEnvio(PuertoCOM, Pantalla);
    reiniciarNumTrama();
    ///empieza fase cierre
    while (!cerrar){
        faseCierre(PuertoCOM, Pantalla);
    }
}

void ProtEsclavo::faseEstablecimiento(HANDLE &PuertoCOM, HANDLE &Pantalla){
    ///poner color establecimiento: azul
    esperarRespuesta(05, PuertoCOM, Pantalla); ///Esperamos una trama ENQ
    imprimirTrama(Pantalla);
    enviarTramaAceptacion(PuertoCOM, Pantalla);
    cambiarNumTrama();
    imprimirTrama(Pantalla);
}
