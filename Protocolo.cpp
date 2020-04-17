//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Protocolo.h"

Protocolo::Protocolo(){
    cont=0;
    envio = envio->getInstance();
    recibo = recibo->getInstance();
}
///05:ENQ 04:EOT 06:ACK 21:NACK 02:STX
void Protocolo::reiniciarCont(){
    cont=0;
}

/*void Protocolo::iniciarProtocolo(){
}*/

/*void Protocolo::faseEstablecimiento(){
    if(estacion=='M'){
        envio->crearTramaC(PuertoCOM, Pantalla, tipoOper, 4, (unsigned char)cont%2);
        recibirTrama();
    }
    else{
        if(estacion=='E')
            confirmacionDeRecibo();
    }
    faseTransferencia();
}

void Protocolo::faseCierre (){

    recibo.setEsProt(false);
}

void Protocolo::esperarRespuesta (){
    while(recibirTrama()!=06 || recibirTrama()!=21)
}*/

void Protocolo::enviarRespuesta(unsigned char tipoTrama, unsigned char direccion){
    if(recibirTrama()==tipoTrama) ///enviamos una ACK porque hemos recibido correctamente la trama
        envio->crearTramaC(PuertoCOM, Pantalla, direccion, 06, (unsigned char)cont%2);
    else ///enviamos una NACK ya que no se ha recibido correctamente la trama
        envio->crearTramaC(PuertoCOM, Pantalla, direccion, 21, (unsigned char)cont%2);
}

///MÉTODOS DEL MAESTRO

void ProtMaestro::IniciarProtocolo(){
     printf("Has seleccionado MAESTRO, seleccione la operación a realizar: \n 1.Seleccion\n 2.Sondeo\n");
    char sel = getch();
    while (!correcto){///seleccion
        switch(sel){
        case '1':
            seleccion();
            EnviarCaracter(PuertoCOM, 'R');
            break;
        case '2':///sondeo
            sondeo();
            EnviarCaracter(PuertoCOM, 'T');
            break;
        default:
            printf("Ha introducido un valor erroneo, intentelo de nuevo \n");
            break;
        }
    }
}

void ProtMaestro::seleccion(){

}

void ProtMaestro::sondeo (){
}

void ProtMaetro::

///METODOS DEL ESCLAVO

void ProtEsclavo::iniciarProtocolo (){
    printf("Has seleccionado ESCLAVO\n");
    char carR = RecibirCaracter(PuertoCOM);
    ///esperar llamada del maestro
    if(carR=='R')
        seleccion();
    else
        sondeo();
}

void ProtEsclavo::seleccion(){

}
void ProtEsclavo::sondeo (){
}
