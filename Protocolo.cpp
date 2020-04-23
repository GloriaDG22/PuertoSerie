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
        esperarTramaAceptacion();
        imprimirTrama();
//    }
}

void Protocolo::faseTransferenciaRecibo(HANDLE &PuertoCOM, HANDLE &Pantalla){
    while(true){
        recibirTramaDatos();
        imprimirTrama();
        enviarTramaAceptacion();
        cambiarNumTrama();
        imprimirTrama();
    }
}

void Protocolo::faseCierre(HANDLE &PuertoCOM, HANDLE &Pantalla){
    enviarTrama(04);///enviar trama EOT
    cambiarNumTrama();
    imprimirTrama();
    esperarRespuesta(06);
    cerrar = tCorrecta;
    imprimirTrama();
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
    if (tProt!=NULL)
        delete tProt;
}

///MÉTODOS DEL MAESTRO

ProtMaestro::ProtMaestro(){
    Protocolo(PuertoCOM, Pantalla);
}

void ProtMaestro::iniciarProtocolo(HANDLE &PuertoCOM, HANDLE &Pantalla){
    printf("Has seleccionado MAESTRO, seleccione la operacion a realizar: \n 1.Seleccion\n 2.Sondeo\n");
    char sel = getch();
    bool correcto = false;
    while (!correcto){///seleccion
        switch(sel){
        case '1':
            printf("Ha elegido seleccion \n");
            seleccion();
            tipoOper='R';
            EnviarCaracter(PuertoCOM, tipoOper);
            correcto=true;
            break;
        case '2':///sondeo
            printf("Ha elegido sondeo \n");
            sondeo();
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
    faseEstablecimiento();
    reiniciarNumTrama();
    ///empieza fase transferencia
    faseTransferenciaEnvio();
    reiniciarNumTrama();
    ///empieza fase cierre
    faseCierre(); ///solicita cierre de la comunicacion y recibe la respuesta
}

void ProtMaestro::sondeo(HANDLE &PuertoCOM, HANDLE &Pantalla){ ///SONDEO MAESTRO
    reiniciarNumTrama();
    ///empieza fase establecimiento
    faseEstablecimiento();
    reiniciarNumTrama();
    ///empieza fase transferencia
    faseTransferenciaRecibo();
    reiniciarNumTrama();
    ///empieza fase cierre
    aceptarCierreComunicacion(); ///pregunta por pantalla si se desea cerrar o no la comunicacion
}

void ProtMaestro::faseEstablecimiento(HANDLE &PuertoCOM, HANDLE &Pantalla){
    ///poner color establecimiento: azul
    enviarTrama(05); ///enviamos trama ENQ
    cambiarNumTrama();
    imprimirTrama();
    esperarTramaAceptacion();
    imprimirTrama();
}

void ProtMaestro::aceptarCierreComunicacion(HANDLE &PuertoCOM, HANDLE &Pantalla){
    while (!cerrar){
        esperarTrama(04); ///esperar trama EOT
        imprimirTrama();
        printf("El esclavo ha solicitado cerrar la comunicacion, ¿quiere cerrarla?\n 1. Si\n 2. No\n");
        char carR = getch();
        bool correcto=false;
        while(!correcto){
            if(carR=='1'){
                enviarTramaAceptacion();
                imprimirTrama();
                correcto = true;
                cerrar = true;
            }
            else{
                if (carR=='2'){
                    tCorrecta=false;
                    enviarTramaAceptacion();
                    imprimirTrama();
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
    Protocolo(PuertoCOM, Pantalla);
}

void ProtEsclavo::iniciarProtocolo (HANDLE &PuertoCOM, HANDLE &Pantalla){
    printf("Has seleccionado ESCLAVO\n");
    char carR = RecibirCaracter(PuertoCOM);
    tipoOper=carR;
    if(carR=='R')
        seleccion();
    else
        sondeo();
}

void ProtEsclavo::seleccion(HANDLE &PuertoCOM, HANDLE &Pantalla){ ///SELECCION ESCLAVO
    reiniciarNumTrama();
    ///empieza fase establecimiento
    faseEstablecimiento();
    reiniciarNumTrama();
    ///empieza fase transferencia
    faseTransferenciaRecibo();
    reiniciarNumTrama();
    ///empieza fase cierre
    esperarTrama(04); ///esperar trama EOT
    imprimirTrama();
    enviarTramaAceptacion(); ///envia una trama ACK para cerrar la comunicacion
    imprimirTrama();
}

void ProtEsclavo::sondeo (HANDLE &PuertoCOM, HANDLE &Pantalla){ ///SONDEO ESCLAVO
    reiniciarNumTrama();
    ///empieza fase establecimiento
    faseEstablecimiento();
    reiniciarNumTrama();
    ///empieza fase transferencia
    faseTransferenciaEnvio();
    reiniciarNumTrama();
    ///empieza fase cierre
    while (!cerrar){
        faseCierre();
    }
}

void ProtEsclavo::faseEstablecimiento(HANDLE &PuertoCOM, HANDLE &Pantalla){
    ///poner color establecimiento: azul
    esperarRespuesta(05); ///Esperamos una trama ENQ
    imprimirTrama();
    enviarTramaAceptacion();
    cambiarNumTrama();
    imprimirTrama();
}
