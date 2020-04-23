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
    protected:

        bool esMaestro; ///true si se estamos en la estacion maestra, false si es estamos en la esclava
        char tipoOper; ///R:seleccion, T:sondeo
        bool cerrar; ///es false mientras el maestro no quiera cerrar la comunicacion
        char numTrama; ///0 o 1 ciclicamente, se reinicia en cada nueva fase
        FILE *ficheroProt; ///fichero en el que se volcara todo lo relacionado con el protocolo
        Enviar* envio; ///Instancia de envio
        Recibir* recibo; ///Instancia de recibo

    public:

        ///constructor
        Protocolo();

        ///destructor
        ~Protocolo();

        /**
        */
        void iniciarProtocolo(HANDLE &PuertoCOM);
        /**
        */
        void faseEstablecimiento();
        /**
        */
        void faseTransferenciaEnvio();
        /**
        */
        void faseTransferenciaRecibo();
        /**
        */
        void faseCierre();

        /** Reiniciamos numTrama a 0 porque estamos en una nueva fase
        */
        void reiniciarNumTrama();
        /** Cambiamos numTrama:
        *       Si era 0 ahora es 1
        *       Si era 1 ahora es 0
        */
        void cambiarNumTrama();

        /** Imprimimos la trama enviada o recibida
        */
        void imprimirTrama();

        ///METODOS DE ENVIO
        /**
        */
        void enviarTramaEstablecimiento();
        /**
        */
        void enviarTramaAceptación();
        /**
        */
        void enviarTramaCierre();

        ///METODOS DE RECIBO
        /**
        */
        void esperarTramaAceptación();
        /**
        */
        void esperarRespuesta();
        /**
        */
        void recibirTramaDatos();
};

class ProtMaestro: public Protocolo {
    public:
        ///constructor de la subclase ProtMaestro
        ProtMaestro();
        /**
        */
        void iniciarProtocolo(HANDLE &PuertoCOM);
        /**
        */
        void seleccion();
        /**
        */
        void sondeo();
        /**
        */
        void faseEstablecimiento();
        /**
        */
        void aceptarCierreComunicacion();
};

class ProtEsclavo: public Protocolo {
    public:
        ///constructor de la subclase ProtEsclavo
        ProtEsclavo();
        /**
        */
        void iniciarProtocolo(HANDLE &PuertoCOM);
        /**
        */
        void seleccion();
        /**
        */
        void sondeo();
        /**
        */
        void faseEstablecimiento();
};

#endif // PROTOCOLO_H
