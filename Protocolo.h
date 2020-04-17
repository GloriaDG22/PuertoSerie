//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "PuertoSerie.h"
#include "Enviar.h"
#include "Recibir.h"

#define PROTMAESTRO "Prolog-m.txt"
#define PROTESCLAVO "Prolog-e.txt"

class Protocolo
{

    protected:
        char tipoOper; ///R: seleccion, T: sondeo
        FILE *ficheroProt;
        int cont;
        Enviar* envio;
        Recibir* recibo;

    public:
        Protocolo();
        void setEstacion(char _estacion);
        void reiniciarCont();
        void iniciarProtocolo();
        void recibirTrama();
        void confirmacionDeRecibo();
        void seleccion();
        void sondeo();
        void enviarRespuesta();
        void esperarRespuesta();
};

class ProtMaestro: public Protocolo {
    public:
        ProtMaestro();
        void iniciarProtocolo();
        void seleccion();
        void sondeo();
};

class ProtEsclavo: public Protocolo {
    public:
        ProtEsclavo();
        void iniciarProtocolo();
        void seleccion();
        void sondeo();
};

#endif // PROTOCOLO_H
