//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#include "PuertoSerie.h"
#include "Enviar.h"
#include "Recibir.h"

#define PROTMAESTRO "Prolog-m.txt"
#define PROTESCLAVO "Prolog-e.txt"

class Enviar;
class Recibir;

class Protocolo
{
    public:
        char tipoOper; ///R:seleccion, T:sondeo
        bool cerrar; ///es false mientras el maestro no quiera cerrar la comunicacion
        int numTrama;
        FILE *ficheroProt;
        Enviar* envio;
        Recibir* recibo;

        Protocolo();
        void iniciarProtocolo(HANDLE &PuertoCOM);
        void faseEstablecimiento();
        void faseTransferenciaEnvio();
        void faseTransferenciaRecibo();
        void faseCierre();
        void reiniciarNumTrama();
};

class ProtMaestro: public Protocolo {
    public:
        ProtMaestro();
        void iniciarProtocolo(HANDLE &PuertoCOM);
        void seleccion();
        void sondeo();
        void faseEstablecimiento();
        void aceptarCierreComunicacion();
};

class ProtEsclavo: public Protocolo {
    public:
        ProtEsclavo();
        void iniciarProtocolo(HANDLE &PuertoCOM);
        void seleccion();
        void sondeo();
        void faseEstablecimiento();
};

#endif // PROTOCOLO_H
