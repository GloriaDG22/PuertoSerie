#ifndef TRAMA_H
#define TRAMA_H
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "PuertoSerie.h"


class Trama{
    private:
        unsigned char sincr;
        unsigned char dir;
        unsigned char control;
        unsigned char numTrama;

    public:
        Trama();

        void setSincr (unsigned char sincr);
        void setDir (unsigned char dir);
        void setControl (unsigned char control);
        void setNumTrama (unsigned char numTrama);

        unsigned char getSincr();
        unsigned char getDir();
        unsigned char getControl();
        unsigned char getNumTrama();

        void enviarTrama ();
        void imprimirTipoTrama();
};

#endif // TRAMA_H
