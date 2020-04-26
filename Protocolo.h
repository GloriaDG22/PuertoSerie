//============================================================================
// ------------ Gloria M�ndez S�nchez y Gloria D�az Gonz�lez------------------
//============================================================================

#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#include "PuertoSerie.h"


#define PROTMAESTRO "Prolog-m.txt"
#define PROTESCLAVO "Prolog-e.txt"

class Protocolo
{
    private:
        bool esMaestro; ///true si se estamos en la estacion maestra, false si es estamos en la esclava
        char tipoOper; ///R:seleccion, T:sondeo
        bool cerrar; ///es false mientras el maestro no quiera cerrar la comunicacion
        char numTrama; ///0 o 1 ciclicamente, se reinicia en cada nueva fase
        FILE *ficheroProt; ///fichero en el que se volcara todo lo relacionado con el protocolo
        bool tCorrecta;
        bool estacionQueInicia;

        Protocolo();
        static Protocolo* obj;

        static void createInstance();

    public:
        static Protocolo* getInstance();

        /** Reiniciamos numTrama a 0 porque estamos en una nueva fase
        */
        void reiniciarNumTrama();
        /** Cambiamos numTrama:
        *       Si era 0 ahora es 1
        *       Si era 1 ahora es 0
        */
        void cambiarNumTrama();

        void abrirFicherto();
        void cerrarFichero();
        void escribirCadena(string cadena);
        void escribirEntero(int numero);
        void escribirCaracter (char car);
        void iniciarProtMaestro(HANDLE &PuertoCOM, HANDLE &Pantalla);
        void iniciarProtEsclavo(HANDLE &PuertoCOM, HANDLE &Pantalla);
        unsigned char getTipoOper();
        bool getEsMaestro();
        bool getCerrar();
        bool getTCorrecta();
        unsigned char getNumTrama();
        void setCerrar(bool _cerrar);
        void setTCorrecta (bool _tCorrecta);
        void setEstacionQueInicia (bool _estacion);
        string getTipoTrama(unsigned char control);
        void imprimirTrama(unsigned char control, unsigned char bce, string aux);
};

#endif // PROTOCOLO_H
