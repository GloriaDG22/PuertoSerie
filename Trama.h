//============================================================================
// ------------ Gloria M�ndez S�nchez y Gloria D�az Gonz�lez------------------
//============================================================================

#ifndef TRAMA_H
#define TRAMA_H
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "PuertoSerie.h"
#include "Funciones.h"


class Trama{
    private:
        ///Car�cter de sincronizaci�n (22)
        unsigned char sincr;
        ///Car�cter de direcci�n ('T' o 'R')
        unsigned char dir;
        ///Car�cter de control (ENQ, EOT, ACK, NACK)
        unsigned char control;
        ///Car�cter de n�mero de pr�ctica ('0' o '1')
        unsigned char numTrama;
        ///Car�cter de longitud del campo dato
        unsigned char longit;
        ///Cadena de texto a enviar/recibir
        char Datos [255];
        ///Car�cter BCE (convalores entre 1 y 254)
        unsigned char BCE;
        ///
        Funciones* fTrama;

    public:
        ///constructores
        Trama();
        Trama(unsigned char Csincr, unsigned char Cdir, unsigned char Ccontrol,
              unsigned char CnumTrama, unsigned char Clongit, char Cdatos[], unsigned char Cbce);
        ///Setters
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

        ///Hace la operaci�n l�gica XOR dos a dos car�cteres
        unsigned char calcularBce();

        ///Muestra la inforamaci�n que le interesa al usuario
        void mostrarTrama ();

};

#endif // TRAMA_H
