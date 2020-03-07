#ifndef TRAMA_H
#define TRAMA_H
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "PuertoSerie.h"
using namespace std;


class Trama{
    private:
        unsigned char sincr;
        unsigned char dir;
        unsigned char control;
        unsigned char numTrama;
        unsigned char longit;
        char Datos [255];
        unsigned char BCE;

    public:
        Trama();

        void setSincr (unsigned char sincr);
        void setDir (unsigned char dir);
        void setControl (unsigned char control);
        void setNumTrama (unsigned char numTrama);
        void setLong (unsigned char longit);
        void setBCE (unsigned char bce);
        void setDatos (char cadena []);
        void addChar(char carR, int i);
        unsigned char calcularBce();

        unsigned char getSincr();
        unsigned char getDir();
        unsigned char getControl();
        unsigned char getNumTrama();
        unsigned char getLong();
        unsigned char getBCE();
        char* getDatos();

        void enviarTrama ();
        void imprimirTipoTrama();
        void mostrarTrama ();
};

#endif // TRAMA_H
