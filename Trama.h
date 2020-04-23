//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#ifndef TRAMA_H
#define TRAMA_H

#include "PuertoSerie.h"

class Funciones;

class Trama{
    private:

        unsigned char sincr; ///Carácter de sincronización (22)
        unsigned char dir; ///Carácter de dirección ('T' o 'R')
        unsigned char control; ///Carácter de control (ENQ, EOT, ACK, NACK)
        unsigned char numTrama; ///Carácter de número de práctica ('0' o '1')
        unsigned char longit; ///Carácter de longitud del campo dato
        char Datos [255]; ///Cadena de texto a enviar/recibir
        unsigned char BCE; ///Carácter BCE (convalores entre 1 y 254)
        Funciones* fTrama;

        ///metodos para el Singleton
        static Trama* obj;
        Trama ();
        static void createInstance();

    public:
        static Trama* getInstance();

        ///destructor
        ~Trama();

        ///Setters
        void setAll (unsigned char Csincr, unsigned char Cdir, unsigned char Ccontrol, unsigned char CnumTrama,
             unsigned char Clongit, char Cdatos[], unsigned char Cbce);
        void setSincr (unsigned char sincr);
        void setDir (unsigned char dir);
        void setControl (unsigned char control);
        void setNumTrama (unsigned char numTrama);
        void setLong (unsigned char longit);
        void setDatos (char cadena []);
        void setBCE (unsigned char bce);
        ///Getters
        unsigned char getSincr();
        unsigned char getDir();
        unsigned char getControl();
        unsigned char getNumTrama();
        unsigned char getLong();
        char* getDatos();
        unsigned char getBCE();

        ///Muestra por pantalla el tipo de trama que es
        void imprimirTipoTrama();

        ///Hace la operación lógica XOR dos a dos carácteres
        unsigned char calcularBce();

        ///Muestra la inforamación que le interesa al usuario
        void mostrarTrama ();

};

#endif // TRAMA_H
