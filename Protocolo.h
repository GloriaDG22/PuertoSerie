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

class Trama;
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
        Trama* tProt; ///Trama en la que va a recibir y a enviar
        bool tCorrecta;
        HANDLE PuertoCOM;
        HANDLE Pantalla;

    public:

        ///constructor
        Protocolo();

        ///destructor
        ~Protocolo();

        /**
        */
        void iniciarProtocolo(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void faseEstablecimiento(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void faseTransferenciaEnvio(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void faseTransferenciaRecibo(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void faseCierre(HANDLE &PuertoCOM, HANDLE &Pantalla);

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
        void imprimirTrama(HANDLE &Pantalla);

        ///METODOS DE ENVIO
        /**
        */
        void enviarTramaAceptacion(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void enviarTrama(int control, HANDLE &PuertoCOM, HANDLE &Pantalla);

        ///METODOS DE RECIBO
        /**
        */
        void esperarTramaAceptacion(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void esperarRespuesta(int control, HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void recibirTramaDatos(HANDLE &PuertoCOM, HANDLE &Pantalla);
};

class ProtMaestro: public Protocolo {
    public:
        ///constructor de la subclase ProtMaestro
        ProtMaestro();
        /**
        */
        void iniciarProtocolo(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void seleccion(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void sondeo(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void faseEstablecimiento(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void aceptarCierreComunicacion(HANDLE &PuertoCOM, HANDLE &Pantalla);
};

class ProtEsclavo: public Protocolo {
    public:
        ///constructor de la subclase ProtEsclavo
        ProtEsclavo();
        /**
        */
        void iniciarProtocolo(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void seleccion(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void sondeo(HANDLE &PuertoCOM, HANDLE &Pantalla);
        /**
        */
        void faseEstablecimiento(HANDLE &PuertoCOM, HANDLE &Pantalla);
};

#endif // PROTOCOLO_H
