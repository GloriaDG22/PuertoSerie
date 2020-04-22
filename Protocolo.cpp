//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Protocolo.h"

Protocolo::Protocolo(){
    cerrar = false;
    envio = new Enviar();
    recibo = recibo->getInstance();
}

void Protocolo::faseTransferenciaEnvio(){
    while(!finFichero){
        ///troceamos fichero: metodo en enviar que cree una trama con los parametros
        ///dados y la envie e imprima
        numTrama++;
        esperarTramaAceptacion();
        imprimirTrama();
        numTrama++;
    }
}

void Protocolo::faseTransferenciaRecibo(){
    while(){
        recibirTramaDatos();
        imprimirTrama();
        enviarTramaAceptacion();
        numTrama++;
        imprimirTrama();
    }
}

void Protocolo::faseCierre(){
    enviarTramaCierre();
    numTrama++;
    imprimirTrama();
    esperarRespuesta();
    imprimirTrama();
}

void Protocolo::reiniciarNumTrama{
    numTrama=0;
}

///MÉTODOS DEL MAESTRO

ProtMaestro::ProtMaestro(){
    Protocolo();
}

void ProtMaestro::iniciarProtocolo(HANDLE &PuertoCOM){
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

void ProtMaestro::seleccion(){ ///SELECCION MAESTRO
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

void ProtMaestro::sondeo(){ ///SONDEO MAESTRO
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

void ProtMaestro::faseEstablecimiento(){
    ///poner color establecimiento: azul
    enviarTramaEstablecimiento();
    numTrama++;
    imprimirTrama();
    esperarRespuesta();
    imprimirTrama();
}

void ProtMaestro::aceptarCierreComunicacion(){
    while (!cerrar){
        esperarTramaCierre();
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
                    enviarTramaNegacion();
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
    Protocolo();
}

void ProtEsclavo::iniciarProtocolo (HANDLE &PuertoCOM){
    printf("Has seleccionado ESCLAVO\n");
    char carR = RecibirCaracter(PuertoCOM);
    tipoOper=carR;
    if(carR=='R')
        seleccion();
    else
        sondeo();
}

void ProtEsclavo::seleccion(){ ///SELECCION ESCLAVO
    reiniciarNumTrama();
    ///empieza fase establecimiento
    faseEstablecimiento();
    reiniciarNumTrama();
    ///empieza fase transferencia
    faseTransferenciaRecibo();
    reiniciarNumTrama();
    ///empieza fase cierre
    esperarTramaCierre();
    imprimirTrama();
    enviarRespuesta();
    imprimirTrama();
}

void ProtEsclavo::sondeo (){ ///SONDEO ESCLAVO
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
    esperarRespuesta();
    imprimirTrama();
}

void ProtEsclavo::faseEstablecimiento(){
    ///poner color establecimiento: azul
    ///esperar llamada del maestro
    imprimirTrama();
    enviarTramaAceptacion();
    numTrama++;
    imprimirTrama();
}
