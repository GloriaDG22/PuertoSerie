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

        Protocolo();
        static Protocolo* obj;

        static void createInstance();

    public:
        static Protocolo* getInstance();


};

#endif // PROTOCOLO_H
